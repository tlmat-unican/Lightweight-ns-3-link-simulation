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

using namespace ns3;

#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define MAGENTA "\x1b[35m"
#define BG_ORANGE "\x1B[48;2;255;128;0m"
#define RESET "\x1b[0m"

enum State
{
  LoS = 1,
  MS,
  DS
};

enum State2
{
  Work = 1,
  Stop
};

Ptr<RandomVariableStream> num_aleatorio;
std::map<std::string, std::vector<Ptr<RandomVariableStream>>> mean_timeDirectory; // store the current mean time value in each link state
using ProbMat_t = std::vector<std::vector<double>>;
using array_t = std::vector<double>;

std::map<std::string, double> time_los, time_ms, time_ds, time_stop, time_work;
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
    std::cout << "error selecting state" << std::endl;
  }
  exit(EXIT_FAILURE);
}

static State SwapState_determ(State currentState, ProbMat_t probs)
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
    return probs[2][0] ? State::LoS : State::MS;
    // std::cout << "DS" << std::endl;
    break;
  default:
    std::cout << "error selecting state" << std::endl;
  }
  exit(EXIT_FAILURE);
}

static double SwapTime(State s1, const std::string index1)
{
  double ret3;
  switch (s1)
  {
  case State::LoS:
    ret3 = mean_timeDirectory[index1].at(0)->GetValue();
    time_los[index1] += ret3;
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
    std::cout << "error in stay times in each state" << std::endl;
  }
  exit(EXIT_FAILURE);
}

static State2 SwapStateDisconnect(State2 currentState, ProbMat_t probs)
{
  switch (currentState)
  {
  case State2::Work:
    return probs[0][1] ? State2::Stop : State2::Work;
    break;
  case State2::Stop:
    return probs[1][0] ? State2::Work : State2::Stop;
    break;
  default:
    std::cout << "error selecting state" << std::endl;
  }
  exit(EXIT_FAILURE);
}

static double SwapTimeDisconnect(State2 s1, const std::string index2)
{
  double ret4;
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
    std::cout << "error in stay times in each state" << std::endl;
  }
  exit(EXIT_FAILURE);
}

#endif /* SCENARIO_HELPER_H */