/**
 *
 *
 *
 * */

#ifndef SCENARIO_HELPER_H
#define SCENARIO_HELPER_H

#include <cassert>
#include <string>
#include <fstream>
#include <map>
#include <tuple>
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <iomanip>
#include <exception>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/nstime.h"
#include <map>
#include "ns3/random-variable-stream.h"
#include "ns3/tag.h"
#include "ns3/poisson-tag.h"

//#include <string_view>

using namespace ns3;


enum State{LoS =1, MS, DS};

enum State2{Work=1, Stop};

Ptr<RandomVariableStream> num_aleatorio;
std::map<std::string,std::vector<Ptr<RandomVariableStream>>> mean_timeDirectory; // store the current mean time value in each link state
using ProbMat_t = std::vector<std::vector<double>>;
using array_t = std::vector<double>;

std::map<std::string,double> time_los, time_ms, time_ds, time_stop, time_work;

/*
PsnTag tagCopy;*/
std::map<int, std::unordered_map<std::string, double>> timeLog; // mapa con tiempos

static Ptr<RandomVariableStream> CreaDistribucionExponencial(double Mean)
{
    auto ret2 = CreateObject<ExponentialRandomVariable>();
    ret2->SetAttribute("Mean", DoubleValue(Mean));
    return ret2;
}

static State SwapState(State currentState, ProbMat_t probs)
{
    auto num_aleatorio = CreateObject<UniformRandomVariable>();
    num_aleatorio->SetAttribute("Min", DoubleValue(0));
    num_aleatorio->SetAttribute("Max", DoubleValue(1.0));

    switch (currentState)
    {
        case State::LoS:
            return num_aleatorio->GetValue() < probs[0][1] ? State::MS : State::DS;
            break;
        case State::MS:
            return num_aleatorio->GetValue() < probs[1][0] ? State::LoS : State::DS;
            break;
        case State::DS:
            return num_aleatorio->GetValue() < probs[2][0] ? State::LoS : State::MS;
            break;
        default:
            std::cout << "error al elegir estado" << std::endl;
    }
    exit(EXIT_FAILURE);


}


static State SwapState_truqui(State currentState, ProbMat_t probs)
{
    

    switch (currentState)
    {
        case State::LoS:
            return probs[0][1] ? State::MS : State::DS;
            // std::cout << "LoS" << std::endl;
            break;
        case State::MS:
            return probs[1][0] ? State::LoS : State::DS;
            // std::cout << "MS" << std::endl;
            break;
        case State::DS:
            return  probs[2][0] ? State::LoS : State::MS;
            // std::cout << "DS" << std::endl;
            break;
        default:
            std::cout << "error al elegir estado" << std::endl;
    }
    exit(EXIT_FAILURE);


}






static double SwapTime(State s1, const std::string index1) // obtiene nuevo tiempo de permanencia en el estado dado
{
    double ret3;
    // std::cout << "Printing link id from a LMS link " << index1 << "\n";
   
    switch (s1)
    {
        case State::LoS:
            ret3 = mean_timeDirectory[index1].at(0)->GetValue();
            time_los[index1] += ret3; // Se guarda el tiempo acumulado por estado a través de la etiqute de enlace lms y su valor 
            return ret3;
            break;
        case State::MS:
            ret3 = mean_timeDirectory[index1].at(0)->GetValue();
            time_ms[index1] += ret3;
            return ret3;
            break;
        case State::DS:
            ret3 = mean_timeDirectory[index1].at(0)->GetValue();
            time_ds[index1] += ret3;
            return ret3;
            break;
        default:
            std::cout << "Error en tiempos de permanencia en cada estado" << std::endl;
    }
    exit(EXIT_FAILURE);
}



static State2 SwapStateDisconnect(State2 currentState, ProbMat_t probs)
{
    // auto num_aleatorio = CreateObject<UniformRandomVariable>();
    // num_aleatorio->SetAttribute("Min", DoubleValue(0));
    // num_aleatorio->SetAttribute("Max", DoubleValue(1.0));
    switch (currentState)
    {
    case State2::Work:
        return  probs[0][1] ? State2::Stop : State2::Work;
        break;
    case State2::Stop:
        return  probs[1][0] ? State2::Work : State2::Stop;
        break;
    default:
        std::cout << "error al elegir estado" << std::endl;
    }
    exit(EXIT_FAILURE);
}

static double SwapTimeDisconnect(State2 s1, const std::string index2) // obtiene nuevo tiempo de permanencia en el estado dado
{
    double ret4;
    // std::cout << "Printing link id from a dropped link " << index2 << "\n";
    switch (s1)
    {
    case State2::Work:
        ret4 = mean_timeDirectory[index2].at(0)->GetValue();
        time_work[index2] += ret4;
        return ret4;
        break;
    case State2::Stop:
        ret4 = mean_timeDirectory[index2].at(1)->GetValue();
        time_stop[index2] += ret4;
        return ret4;
        break;
    default:
        std::cout << "error en tiempos de permanencia en cada estado" << std::endl;
    }
    exit(EXIT_FAILURE);
}




/* 
static void Rx(Ptr<const Packet> packet) // trace Rx lo vuelca a map timelog
{
    bool ret = packet->PeekPacketTag(tagCopy);
    assert(ret && "Tag not found!!");
    try
    {
        timeLog[tagCopy.Get()]["App_Rx"] = Simulator::Now().GetSeconds();
    }
    catch (std::exception& e)
    {
        std::cout << "App rx exception " << std::endl;
        std::exit(-1);
    }
}

static void Tx(Ptr<const Packet> packet) // trace Tx y lo vuelca a map timelog
{
    bool ret = packet->PeekPacketTag(tagCopy);
    assert(ret && "Tag not found!!");
    if (timeLog[tagCopy.Get()].empty())
    {
        timeLog[tagCopy.Get()]["Dev_Enqueue"] = -1;
        timeLog[tagCopy.Get()]["Dev_Dequeue"] = -1;
        timeLog[tagCopy.Get()]["Dev_Out"] = -1;
        timeLog[tagCopy.Get()]["App_Rx"] = -1;
    }
    timeLog[tagCopy.Get()]["App_Tx"] = Simulator::Now().GetSeconds();
    timeLog[tagCopy.Get()]["Pkt_Size"] = packet->GetSize(); // Tamaño en Bytes
}

static void Colas(Ptr<const Packet> packet) // trace Entra en cola y lo vuelca a map timelog
{
    bool ret = packet->PeekPacketTag(tagCopy);
    assert(ret && "Tag not found!!");
    try
    {
        if (timeLog[tagCopy.Get()].empty())
        {
            timeLog[tagCopy.Get()]["Pkt_Size"] = packet->GetSize(); // Tamaño en Bytes
            timeLog[tagCopy.Get()]["App_Tx"] = -1;
            timeLog[tagCopy.Get()]["Dev_Dequeue"] = -1;
            timeLog[tagCopy.Get()]["Dev_Out"] = -1;
            timeLog[tagCopy.Get()]["App_Rx"] = -1;
        }
        timeLog[tagCopy.Get()]["Dev_Enqueue"] = Simulator::Now().GetSeconds();
    }
    catch (std::exception& e)
    {
        std::cout << "Dev enqueue exception " << std::endl;
        std::exit(-1);
    }
}

static void FinCola(Ptr<const Packet> packet)
{
    bool ret = packet->PeekPacketTag(tagCopy);
    assert(ret && "Tag not found!!");
    try
    {
        timeLog[tagCopy.Get()]["Dev_Dequeue"] = Simulator::Now().GetSeconds();
    }
    catch (std::exception& e)
    {
        std::cout << "Fin cola exception " << std::endl;
        std::exit(-1);
    }
}

static void OutDevice(Ptr<const Packet> packet)
{
    bool ret = packet->PeekPacketTag(tagCopy);
    assert(ret && "Tag not found!!");
    try
    {
        timeLog[tagCopy.Get()]["Dev_Out"] = Simulator::Now().GetSeconds();
    }
    catch (std::exception& e)
    {
        std::cout << "Out device exception " << std::endl;
        std::exit(-1);
    }
}

static void DropPacket(Ptr<const Packet>)
{
    std::cout << "Packet dropped!!!" << std::endl;
    std::exit(-1);
}

static void PintaMapa()
{
    for (auto const &ent1 : timeLog)
    {
        std::cout << "pkt con tag: " << std::setw(5) << ent1.first;
        for (auto const &ent2 : ent1.second)
        {
            std::cout << std::setw(10) << ent2.first << std::setw(10) << ent2.second << "\t";
        }
        std::cout << std::endl;
    }
}

static void PintaMapa2()
{
    for (auto const &ent1 : timeLog)
    {
        std::cout << "pkt con tag: " << ent1.first << std::setw(15) << "App_Tx: " << std::setw(10) << ent1.second.at("App_Tx")
                  << std::setw(15) << "Dev_Enqueue: " << std::setw(10) << ent1.second.at("Dev_Enqueue")
                  << std::setw(15) << "Dev_Dequeue: " << std::setw(10) << ent1.second.at("Dev_Dequeue")
                  << std::setw(15) << "Dev_Out: " << std::setw(10) << ent1.second.at("Dev_Out")
                  << std::setw(15) << "App_Rx: " << std::setw(10) << ent1.second.at("App_Rx") << std::endl;
    }
}

static void SalidaMapa()
{
    std::ofstream fich("results.txt");
    fich.precision(5);
    for (auto const &ent1 : timeLog)
    {
        fich << ent1.first << " " << ent1.second.at("Pkt_Size")
             << " " << std::fixed << ent1.second.at("App_Tx")
             << " " << std::fixed << ent1.second.at("Dev_Enqueue")
             << " " << std::fixed << ent1.second.at("Dev_Dequeue")
             << " " << std::fixed << ent1.second.at("Dev_Out")
             << " " << std::fixed << ent1.second.at("App_Rx") << std::endl;
    }
    fich.close();
}

static void TiemposModelo(void)
{
    int n_elementos = 0;
    double t_servicio = 0;
    double t_transmision = 0;
    double t_cola = 0;
    for (auto const &ent1 : timeLog)
    {
        if (ent1.second.size() == 5)
        {
            t_transmision += ent1.second.at("Dev_Out") - ent1.second.at("App_Tx");
            t_servicio += ent1.second.at("Dev_Out") - ent1.second.at("Dev_Dequeue");
            t_cola += ent1.second.at("Dev_Dequeue") - ent1.second.at("App_Tx");
            n_elementos++;
        }
    }
    t_transmision /= n_elementos;
    t_servicio /= n_elementos;
    t_cola /= n_elementos;
    std::cout << "tiempo de transmision device (medio) = " << t_transmision << std::endl;
}

static void
Tput(Ptr<UdpServer> algo, uint32_t p_size)
{
    static uint32_t old_received = 0;
    std::cout << Simulator::Now().GetSeconds() << " |Throughout en ultimo intervalo:  " << (double)(algo->GetReceived() - old_received) * p_size * 8 / 1000 << " kbps " << std::endl;
    old_received = algo->GetReceived();
} */

#endif /* SCENARIO_HELPER_H */