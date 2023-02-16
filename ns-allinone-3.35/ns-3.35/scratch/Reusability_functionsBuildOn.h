/*
This header just contains the functions which will be used in p2p_scenarioReusable.cc
*/

#define MAX 10000000

bool ChekingLinkDelay(json data){
    int cont = 0;
    for (int i = 0; i < (int) data.at("NumLinks"); i++){
        if(data.at("Links")[i]["Link-type"] == "Ideal")
            if (data.at("Links")[i]["Delay"] == 0 && data.at("Links")[i]["Background"] == false) {
                cont ++;
            }
    }
    if (cont == (int) data.at("NumLinks")){
        return true;
    }else{
        return false;
    }
}



void CreateMiddleLinks(PointToPointHelper pointToPoint, vector<NetDeviceContainer>& Devs, json data, NodeContainer nodes){
    for (int i = 0; i < (int)  data.at("NumLinks"); i++){
        pointToPoint.SetDeviceAttribute("DataRate", StringValue(to_string(data.at("Links")[i]["DataRate"]) + "Mbps")); // BW
        pointToPoint.SetChannelAttribute("Delay", StringValue(to_string(data.at("Links")[i]["Delay"]) + "ms"));      // delay
        pointToPoint.SetDeviceAttribute("Mtu", UintegerValue(data.at("Links")[i]["MTU"]));
        // pointToPoint.SetQueue("ns3::DropTailQueue", "MaxSize", StringValue(to_string(data.at("Links")[i]["DropTailQueue"]) + "p")); // FIXME: 1000000000000p, 15p, 7p
        pointToPoint.SetDeviceAttribute("Mode", StringValue(data.at("Links")[i]["Mode"])); 
        pointToPoint.SetQueue("ns3::DropTailQueue", "MaxSize", ns3::QueueSizeValue(QueueSize(QueueSizeUnit::BYTES, (data.at("Links")[i]["DropTailQueue"]))));
        
        NetDeviceContainer p2pDev = pointToPoint.Install(NodeContainer{nodes.Get(i), nodes.Get(i+1)});
        Devs.push_back(p2pDev);

        if(data.at("Links")[i]["ErrorRate"]!=0){
            Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
            em->SetAttribute("ErrorRate", DoubleValue(data.at("Links")[i]["ErrorRate"]));
            em->SetAttribute("ErrorUnit", EnumValue(2)); // ERROR_UNIT_PACKET
            Devs.at(i).Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));
        }
        // cout << "The number of PDPdev "<< i+1 <<" created is: " << Devs.at(i).GetN() << "\n";
    }
    std::cout << MAGENTA << "INFO: " << RESET <<"Middle Links created" << std::endl;

}



int Sumvalues(json data, string Parameter){
    int cont = 0;
    for(int i = 0; i < (int)  data.at("NumLinks"); i++){
        cont += (int) data.at("Links")[i][Parameter];
    }
    return cont;
}


int SearchMinLink(json data){
    int min = MAX, Link=0;
    for(int i = 0; i < (int)  data.at("NumLinks"); i++){
        if(data.at("Links")[i]["DataRate"] < min){
            min = (int) data.at("Links")[i]["DataRate"]; // Select features from the worst link.
            Link = i;
        }
    }
    return Link;
}




void  SettingupMiddleNetworks(vector<Ipv4InterfaceContainer>& ifacesP2p, vector<NetDeviceContainer> Devs, json data){
    for (int i = 0; i < (int)  data.at("NumLinks") ; i++){
        Ipv4AddressHelper addressesP2p;
        string str = "11." + std::to_string(i+1) + ".0.0";
        const char* adr = str.c_str();
        std::cout << MAGENTA << "INFO: " << RESET <<"Creating P2P Network "<< i+1 <<": " << str << "\n";
        addressesP2p.SetBase(adr,"255.255.255.0");
        Ipv4InterfaceContainer ifacP2p = addressesP2p.Assign(Devs.at(i)); // Assign fuction returns a Ipv4InterfaceConatiner 
        ifacesP2p.push_back(ifacP2p);
        
        std::cout << MAGENTA << "INFO: " << RESET << "Ifaces P2P are " << ifacesP2p.at(i).GetN() << "\n";  

    }

    //NS_LOG_UNCOND ("P2P Networks created");
}





void ManageMiddleLinks(json data, PointToPointHelper pointToPoint, vector<NetDeviceContainer>& Devs, NodeContainer& nodes, int& auxnodes){
    
    
    if(ChekingLinkDelay(data)){
        cout << "Simulation with only 1 link due to the same features!!\n";
        auxnodes = 1 + 1;
        nodes.Create(auxnodes);
        // NS_LOG_UNCOND ("Nodes Created");
        std::cout << MAGENTA << "INFO: " << RESET <<"Nodes created"<< std::endl;
        int Link = SearchMinLink(data);
        std::cout << MAGENTA << "INFO: " << RESET <<"Numero del enlace: " << Link << std::endl;
        pointToPoint.SetDeviceAttribute("DataRate", StringValue(to_string(data.at("Links")[Link]["DataRate"]) + "Mbps")); // BW
        pointToPoint.SetChannelAttribute("Delay", StringValue(to_string(data.at("Links")[Link]["Delay"]) + "ms"));      // delay
        pointToPoint.SetDeviceAttribute("Mtu", UintegerValue(data.at("Links")[Link]["MTU"]));
        // pointToPoint.SetQueue("ns3::DropTailQueue", "MaxSize", StringValue(to_string(data.at("Links")[Link]["DropTailQueue"]) + "p")); // FIXME: 1000000000000p, 15p, 7p
        pointToPoint.SetQueue("ns3::DropTailQueue", "MaxSize", ns3::QueueSizeValue(QueueSize(QueueSizeUnit::BYTES, (data.at("Links")[Link]["DropTailQueue"]))));

        NetDeviceContainer p2pDev = pointToPoint.Install(NodeContainer{nodes.Get(0), nodes.Get(1)});
        Devs.push_back(p2pDev);
        std::cout << MAGENTA << "INFO: " << RESET <<"Devices created"<< std::endl;
        Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
        em->SetAttribute("ErrorRate", DoubleValue(data.at("Links")[Link]["ErrorRate"]));
        em->SetAttribute("ErrorUnit", EnumValue(2)); // ERROR_UNIT_PACKET
        Devs.at(0).Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));
        //cout << "The number of PDPdev "<< 1 <<" created is: " << Devs.at(0).GetN() << "\n";

        data.at("NumLinks")=1;
    }else{
        auxnodes = 1 + (int) data.at("NumLinks");
        nodes.Create(auxnodes);
       // NS_LOG_UNCOND ("Nodes Created");
        CreateMiddleLinks(pointToPoint,Devs,data,nodes);
    }
    std::cout << MAGENTA << "INFO: " << RESET << "Manage Midlle Links finished" << std::endl;
}






void 
BackgroundTrafficSetup(ApplicationContainer& app, ApplicationContainer& sinkApps, json data, 
    vector<Ipv4InterfaceContainer>& ifacesP2p, NodeContainer nodes){

    double start_time = 0;
    uint16_t sinkport = 8080;
      for (int i = 0; i < (int) data.at("NumLinks") ; i++){
        if (data.at("Links")[i]["Background"]){
            Address sinkAddress(InetSocketAddress(ifacesP2p.at(i).GetAddress(1,0), sinkport));
            PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory",Address(sinkAddress));
            sinkApps.Add(packetSinkHelper.Install(nodes.Get(i+1))); // Node 3: Destination node
            sinkApps.Start(Seconds(start_time));
                
            
            OnOffHelper Sender("ns3::TcpSocketFactory", Address(sinkAddress));
            Sender.SetAttribute("DataRate",StringValue(to_string( data.at("Links")[i]["BackFeatures"].at("DataRate") ) + "Mbps"));
            Sender.SetAttribute("PacketSize",UintegerValue(data.at("Links")[i]["BackFeatures"].at("PktSize")));
            // Link.SetAttribute("Remote", AddressValue(sinkAddress));
            Sender.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant="+ 
                to_string(data.at("Links")[i]["BackFeatures"].at("OnTime")) +"]"));
            Sender.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant="+ 
                to_string(data.at("Links")[i]["BackFeatures"].at("OffTime"))+ "]"));
            Sender.SetAttribute("MaxBytes", UintegerValue(data.at("Links")[i]["BackFeatures"].at("Max_bytes"))); // max_packets
            app.Add(Sender.Install(NodeContainer{nodes.Get(i)}));
            app.Start(Seconds(start_time));
            
            std::cout << MAGENTA << "INFO: " << RESET << "Ideal Link: " << i + 1 << " with background traffic" << std::endl; 
            sinkport +=1;    
        }
    }
      //NS_LOG_UNCOND ("BackTrafficCreated");
}

void 
SettingUpModelLinks(json data, const vector<NetDeviceContainer>& Devs, float timevalue){
    int aux1 = 0, aux2 = 0;
    string indexDisconnect, indexLMS;
    for (int i = 0; i < (int)  data.at("NumLinks") ; i++){
        if (data.at("Links")[i]["Link-type"] == "Disconnection"){
                indexDisconnect = "DroppedConnection" + to_string(i);
                mean_timeDirectory[indexDisconnect].push_back(CreaDistribucionExponencial(Bands[data.at("Links")[i]["Band"]].mean_time[0]));
                if(timevalue != 0){
                    mean_timeDirectory[indexDisconnect].push_back(CreaDistribucionExponencial(timevalue));
                }else{
                    mean_timeDirectory[indexDisconnect].push_back(CreaDistribucionExponencial(Bands[data.at("Links")[i]["Band"]].mean_time[1]));
                }
                
                CreateStateLinks(CurrentDisconnectStates_g);
                if(Bands[data.at("Links")[i]["Band"]].type=="FixedTime"){
                    Simulator::Schedule(Seconds(0), &DisconnectMachine_toy,
                    DynamicCast<ns3::PointToPointNetDevice>(Devs.at(i).Get(0)), aux1, Bands[data.at("Links")[i]["Band"]], indexDisconnect);
                   
                    aux1 = aux1 + 1;
                    CreateStateLinks(CurrentDisconnectStates_g);
        
                    Simulator::Schedule(Seconds(0), &DisconnectMachine_toy,
                    DynamicCast<ns3::PointToPointNetDevice>(Devs.at(i).Get(1)), aux1, Bands[data.at("Links")[i]["Band"]], indexDisconnect);
                    // ChannelVarLogFile << Simulator::Now().GetSeconds() << " " << 1 << std::endl;
                    std::cout << MAGENTA << "INFO: " << RESET <<"Disconnection Link " << i+1 << " has been created" << std::endl;
                    aux1 = aux1 + 1;

                }else{
                    Simulator::Schedule(Seconds(0), &DisconnectMachine,
                    DynamicCast<ns3::PointToPointNetDevice>(Devs.at(i).Get(0)), aux1, Bands[data.at("Links")[i]["Band"]].probMat, 
                    Bands[data.at("Links")[i]["Band"]].capFactor, Bands[data.at("Links")[i]["Band"]].muLos, indexDisconnect);
                    aux1 = aux1 + 1;
                    CreateStateLinks(CurrentDisconnectStates_g);
                    Simulator::Schedule(Seconds(0), &DisconnectMachine,
                    DynamicCast<ns3::PointToPointNetDevice>(Devs.at(i).Get(1)), aux1, Bands[data.at("Links")[i]["Band"]].probMat, 
                    Bands[data.at("Links")[i]["Band"]].capFactor, Bands[data.at("Links")[i]["Band"]].muLos, indexDisconnect);
                    //cout << "A dropped connection " << i << " has been created\n";
                    aux1 = aux1 + 1;
                    std::cout << MAGENTA << "INFO: " << RESET <<"Disconnection Link " << i+1 << " has been created" << std::endl;

                }
                

        }else if (data.at("Links")[i]["Link-type"] == "LMS"){
                indexLMS = "LMS" + to_string(i);
                mean_timeDirectory[indexLMS].push_back(CreaDistribucionExponencial(Bands[data.at("Links")[i]["Band"]].mean_time[0]));
                mean_timeDirectory[indexLMS].push_back(CreaDistribucionExponencial(Bands[data.at("Links")[i]["Band"]].mean_time[1]));
                mean_timeDirectory[indexLMS].push_back( CreaDistribucionExponencial(Bands[data.at("Links")[i]["Band"]].mean_time[2]));
                CreateStateLinksLMS(CurrentStates_g);
                if(Bands[data.at("Links")[i]["Band"]].type=="FixedTime"){
                  
                    Simulator::Schedule(Seconds(0), &MakinaEventos_toy,
                    DynamicCast<ns3::PointToPointNetDevice>(Devs.at(i).Get(0)), aux2, Bands[data.at("Links")[i]["Band"]], indexLMS);
                   
                    aux2 = aux2 + 1;
                    CreateStateLinksLMS(CurrentStates_g);
                   
                    Simulator::Schedule(Seconds(0), &MakinaEventos_toy,
                    DynamicCast<ns3::PointToPointNetDevice>(Devs.at(i).Get(1)), aux2, Bands[data.at("Links")[i]["Band"]], indexLMS);
                 
                    std::cout << MAGENTA << "INFO: " << RESET <<"LMS Link " << i + 1 << " has been created" << std::endl;
                    aux2 = aux2 + 1;

                }else{
                    Simulator::Schedule(Seconds(0), &MakinaEventos,
                    DynamicCast<ns3::PointToPointNetDevice>(Devs.at(i).Get(0)), aux2, Bands[data.at("Links")[i]["Band"]].probMat, 
                    Bands[data.at("Links")[i]["Band"]].capFactor, Bands[data.at("Links")[i]["Band"]].muLos, indexLMS);
                    aux2 = aux2 + 1;
                    CreateStateLinksLMS(CurrentStates_g);
                    Simulator::Schedule(Seconds(0), &MakinaEventos,
                    DynamicCast<ns3::PointToPointNetDevice>(Devs.at(i).Get(1)), aux2, Bands[data.at("Links")[i]["Band"]].probMat, 
                    Bands[data.at("Links")[i]["Band"]].capFactor, Bands[data.at("Links")[i]["Band"]].muLos, indexLMS);
                    std::cout << MAGENTA << "INFO: " << RESET << "LMS Link " << i + 1 << " has been created" << std::endl;
                    aux2 = aux2 + 1;
                }
                
        }
        
        
    }

   

}