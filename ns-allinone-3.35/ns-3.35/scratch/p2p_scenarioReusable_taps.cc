/*
This file just connect the containers through differente kind of links which can be set scenario up by a JSON file.

For testing delay has been added to CSMA and point-to-point links.

*/

#include <iostream>
#include <string>
#include <fstream>
#include <numeric>
#include <iomanip>
#include <csignal>
#include <stdbool.h>
#include <map>
#include <vector>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/tap-bridge-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/on-off-helper.h"
#include "ns3/onoff-application.h"
#include "ns3/traffic-control-module.h"
#include "json.hpp"
#include "Scenario-helper.h"

#define NS_LOG_APPEND_CONTEXT

using namespace ns3;
using namespace std;
using json = nlohmann::json;

NS_LOG_COMPONENT_DEFINE("TapCsmaNS3_Scenario");

#include "LMS-Bands.h"
#include "LMS-skills.h"
#include "Reusability_functions_taps.h"

// Global variables
std::uint64_t currentBuffer = 0u;
std::ofstream bufferLogFile;
std::ofstream TxFile;
std::ofstream RxFile;
std::ofstream CwndFile;

static uint32_t contTX = 0;
static uint32_t contRX = 0;
uint32_t segmentSize = 1460;

static void
CwndTracer(uint32_t oldCwnd, uint32_t newCwnd)
{
  CwndFile << Simulator::Now().GetSeconds() << " \t" << newCwnd / (double)segmentSize << std::endl;
}

void TxTracer(Ptr<const Packet> pkt)
{

  TxFile << contTX << " " << Simulator::Now().GetSeconds() << " " << pkt->GetSize() << std::endl;
  contTX++;
}

void RxTracer(Ptr<const Packet> pkt, const Address &)
{
  auto rxBytes = pkt->GetSize();
  if (rxBytes < 1460)
  {
    std::cout << MAGENTA << "Pkt with size: " << rxBytes << RESET << std::endl;
  }
  while (rxBytes)
  {
    RxFile << contRX << " " << Simulator::Now().GetSeconds() << " " << pkt->GetSize() << std::endl;
    rxBytes -= segmentSize;
    contRX++;
  }
}

static void
PktIn(ns3::Ptr<const Packet> pkt)
{
  currentBuffer += pkt->GetSize();
  bufferLogFile << Simulator::Now().GetSeconds() << " " << currentBuffer << "\n";
}

static void
PktOut(ns3::Ptr<const Packet> pkt)
{
  currentBuffer -= pkt->GetSize();
  bufferLogFile << Simulator::Now().GetSeconds() << " " << currentBuffer << "\n";
}

int main(int argc, char *argv[])
{

  RngSeedManager::SetSeed(time(NULL));
  string JSONpath = "";
  double timevalue = 0;
  double duration = 60;
  double app_rate = 40;
  double file_size = 300e6;
  string name = "";
  string cwnd_value = "Cubic";
  string outcome_folder = "";
  int run = 1;

  CommandLine cmd(__FILE__);
  cmd.AddValue("json-path", "Configuration file name", JSONpath);
  cmd.AddValue("tracing-name", "Tracing file name id", name);
  cmd.AddValue("tcp-model", "To select Reno model", cwnd_value);
  cmd.AddValue("stop-time", "Select stop time", timevalue);
  cmd.AddValue("out-folder-path", "Location of the outcome folder", outcome_folder);
  cmd.AddValue("app-rate", "Select app-rate", app_rate);
  cmd.AddValue("file-size-bytes", "Select file-size", file_size);
  cmd.AddValue("time", "Simulation Time", duration);
  cmd.AddValue("run", "", run);
  cmd.Parse(argc, argv);

  bufferLogFile.open(outcome_folder + "logBuffer" + name + ".log", std::fstream::out); // ofstream
  CwndFile.open(outcome_folder + "logCwnd" + name + ".log", std::fstream::out);        // ofstream
  RxFile.open(outcome_folder + "RxFile" + name + ".log", std::fstream::out);           // ofstream
  TxFile.open(outcome_folder + "TxFile" + name + ".log", std::fstream::out);           // ofstream
  ChannelVarLogFile.open(outcome_folder + "ChannelVar" + name + ".log", std::fstream::out);
  std::cout << YELLOW << "Configuration File: " << JSONpath << " was selected" << RESET << std::endl;
  RngSeedManager::SetRun(run);
  std::ifstream f(JSONpath);
  json data = json::parse(f);

  std::string mode = "ConfigureLocal";
    NS_LOG_UNCOND("SIM - START: Hello :)");
  std::cout << MAGENTA << BG_ORANGE << "Simulator: Hello :)" << RESET << std::endl;
  Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(segmentSize));

  if (cwnd_value != "Cubic")
  {
    Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpNewReno"));
  }

  NodeContainer nodes;
  int auxnodes;
  PointToPointHelper pointToPoint;
  vector<NetDeviceContainer> Devs;
  ManageMiddleLinks(data, pointToPoint, Devs, nodes, auxnodes); // To handle middle links and create them

  CsmaHelper csma;
  csma.SetChannelAttribute("DataRate", StringValue("100Gbps"));

  NetDeviceContainer csmaDevIn = csma.Install(NodeContainer{nodes.Get(0), nodes.Get(1)});
  NetDeviceContainer csmaDevOut = csma.Install(NodeContainer{nodes.Get(auxnodes - 1), nodes.Get(auxnodes - 2)});

  InternetStackHelper stack;
  for (int i = 1; i <= auxnodes - 2; i++)
  {
    stack.Install(nodes.Get(i));
  }

  // NS_LOG_UNCOND ("Nodes Created");
  // CREATING - NETWORKS ----------------------------------------------------------------

  Ipv4AddressHelper addressesCsmaIn;
  addressesCsmaIn.SetBase("10.1.0.0", "255.255.255.0", "0.0.0.3");
  Ipv4InterfaceContainer ifacesCsmaIn = addressesCsmaIn.Assign({csmaDevIn.Get(1)});

  // Creating middle networks - Links
  vector<Ipv4InterfaceContainer> ifacesP2p;
  SettingupMiddleNetworks(ifacesP2p, Devs, data);

  Ipv4AddressHelper addressesCsmaOut;
  addressesCsmaOut.SetBase("10.2.0.0", "255.255.255.0", "0.0.0.3");
  Ipv4InterfaceContainer ifacesCsmaOut = addressesCsmaOut.Assign({csmaDevOut.Get(1)});

  // ------------------------------------------------------------------------------------

  // SETTING - UP TAPS -------------------------------------------------------------------
  {
    TapBridgeHelper tapBridge;
    tapBridge.SetAttribute("Mode", StringValue("UseBridge"));
    tapBridge.SetAttribute("DeviceName", StringValue("tap-left"));
    tapBridge.Install(nodes.Get(0), csmaDevIn.Get(0));
  }
  {
    TapBridgeHelper tapBridge;
    tapBridge.SetAttribute("Mode", StringValue("UseBridge"));
    tapBridge.SetAttribute("DeviceName", StringValue("tap-right"));
    tapBridge.Install(nodes.Get(auxnodes - 1), csmaDevOut.Get(0));
  }

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();
  // NS_LOG_UNCOND ("Routing Tables Populated");

  //-----------------------------------------------------------------------------------------

  ApplicationContainer app, sinkApps;
  BackgroundTrafficSetup(app, sinkApps, data, ifacesP2p, nodes); // To generate background traffic

  SettingUpModelLinks(data, Devs, timevalue); // To set up the link model: Land-Mobile-System or Disconnections
  NS_LOG_UNCOND("Configuration finished");
  std::cout << "Configuration finished" << std::endl;

  std::cout << GREEN << "++++++++++++++++++ NET-Configuration-finished ++++++++++++++++++" << RESET << std::endl;

  /*********************************************
   *  Install applications on the end devices  *
   *********************************************/
  ApplicationContainer Client_app, Server_app;
  // Create a packet sink on the end of the chain
  Address Server_Address(InetSocketAddress(ifacesP2p.at(int(data.at("NumLinks")) - 1).GetAddress(1, 0), 50000));
  PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory", Address(Server_Address));
  Server_app.Add(packetSinkHelper.Install(nodes.Get(auxnodes - 2))); // Last node server
  Server_app.Start(Seconds(0));

  // Create the OnOff APP to send TCP to the server
  OnOffHelper clientHelper("ns3::TcpSocketFactory", Address(Server_Address));
  clientHelper.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
  clientHelper.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  clientHelper.SetAttribute("DataRate", StringValue(to_string(app_rate) + "Mbps"));
  clientHelper.SetAttribute("MaxBytes", UintegerValue(file_size));
  clientHelper.SetAttribute("PacketSize", UintegerValue(1460));

  Client_app.Add(clientHelper.Install(NodeContainer{nodes.Get(1)}));
  Client_app.Start(Seconds(0));

  /*********************************************
   *              TRACING - STUDY              *
   *********************************************/
  Simulator::Schedule(Seconds(1.00001), []()
                      { Config::ConnectWithoutContext("/NodeList/1/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow", MakeCallback(&CwndTracer)); });
  Config::ConnectWithoutContext("/NodeList/1/ApplicationList/*/$ns3::OnOffApplication/Tx", MakeCallback(&TxTracer));
  Config::ConnectWithoutContext("/NodeList/" + to_string(auxnodes - 2) + "/ApplicationList/*/$ns3::PacketSink/Rx", MakeCallback(&RxTracer));
  DynamicCast<ns3::PointToPointNetDevice>(Devs.at(0).Get(0))->TraceConnectWithoutContext("Inqueue", MakeCallback(&PktIn));
  DynamicCast<ns3::PointToPointNetDevice>(Devs.at(0).Get(0))->TraceConnectWithoutContext("Outqueue", MakeCallback(&PktOut));
  Ptr<PacketSink> Server_trace = StaticCast<PacketSink>(Server_app.Get(0));

  Simulator::Stop(Seconds(duration));
  Simulator::Run();
  Simulator::Destroy();

  std::cout << "Sink: Total RX - " << Server_trace->GetTotalRx() << " bytes" << std::endl;
  return 0;
}