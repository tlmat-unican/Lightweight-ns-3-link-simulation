#include <typeinfo>
// State CurrentState_g = State::LoS;
std::ofstream ChannelVarLogFile;
std::ofstream ChannelVarLogFile_stop;
std::vector<State> CurrentStates_g;

std::vector<State2> CurrentDisconnectStates_g;

std::vector<double> timesInLos[2];
std::vector<double> timesInMs[2];
std::vector<double> timesInDs[2];
std::vector<double> timesInWork;
std::vector<double> timesInStop;

State Channelprev;
State2 Channelprev_stop;

// Genera varios, dependiendo de los enlaces que haya
void CreateStateLinksLMS(vector<State>& CurrentStates){ // CurrentStates - to create a log to save each link-sate 
  CurrentStates.push_back(State::LoS);
  //std::cout << "New interface created with total states " <<  CurrentStates.size() << std::endl;
}

void CreateStateLinks(vector<State2>& CurrentStates){ // CurrentStates - to create a log to save each link-sate 
  CurrentStates.push_back(State2::Work);
  //std::cout << "New interface created with total states " <<  CurrentStates.size() << std::endl;
}




void
MakinaEventos(ns3::Ptr<PointToPointNetDevice> nd1, const int& ifaceNumber1, const ProbMat_t& probMat3, const array_t& capFactor1, const double& muLos1, string& index1)
{ 

  //std::cout << "The State Number in t0 is: " << CurrentStates_g.at(ifaceNumber1) << std::endl;
  CurrentStates_g.at(ifaceNumber1) = SwapState(CurrentStates_g.at(ifaceNumber1), probMat3); // actualiza estado
  double t_cambio1 = SwapTime(CurrentStates_g.at(ifaceNumber1), index1);                           // tiempo de permanencia en estado actual
  double bps1 = capFactor1[CurrentStates_g.at(ifaceNumber1) - 1] * muLos1 * pktLen * 8;      // seleccion BW para estado actual

  nd1->SetDataRate(DataRate(bps1));

  if(CurrentStates_g.at(ifaceNumber1)!=Channelprev){
    ChannelVarLogFile << Simulator::Now().GetSeconds() << " " << CurrentStates_g.at(ifaceNumber1)<< std::endl;
  }
  Channelprev = CurrentStates_g.at(ifaceNumber1);
  
  // std::cout << "The State Number in t1 is: " << CurrentStates_g.at(ifaceNumber1) << std::endl;
  // std::cout << typeid(CurrentStates_g.at(ifaceNumber1)).name() << std::endl;
  switch (CurrentStates_g.at(ifaceNumber1)){
    case State::LoS:
      timesInLos[0].push_back(t_cambio1);
      timesInLos[1].push_back(bps1);
      // std::cout << "LoS" << std::endl;
      break;
      
    case State::MS:
      timesInMs[0].push_back(t_cambio1);
      timesInMs[1].push_back(bps1);
      // std::cout << "MS" << std::endl;
      break;
     
    case State::DS:
      timesInDs[0].push_back(t_cambio1);
      timesInDs[1].push_back(bps1);
      // std::cout << "DS" << std::endl;
      break;
     
    default:
      std::cout << "error al registrar tiempo y tasa de estado" << std::endl;
  }



  Simulator::Schedule(Seconds(t_cambio1), &MakinaEventos, nd1, ifaceNumber1, probMat3, capFactor1, muLos1, index1); 
}







void
MakinaEventos_toy(ns3::Ptr<PointToPointNetDevice> nd1, const int& ifaceNumber1,const band_features_t& Band_pass, string& index1)
{ 

  //std::cout << "The State Number in t0 is: " << CurrentStates_g.at(ifaceNumber1) << std::endl;
  CurrentStates_g.at(ifaceNumber1) = SwapState_truqui(CurrentStates_g.at(ifaceNumber1), Band_pass.probMat); // actualiza estado
  double t_cambio1 = Band_pass.mean_time.at(0);                       // tiempo de permanencia en estado actual
  double bps1 = Band_pass.capFactor[CurrentStates_g.at(ifaceNumber1) - 1] * Band_pass.muLos * pktLen * 8;      // seleccion BW para estado actual

  nd1->SetDataRate(DataRate(bps1));
  // std::cout << Simulator::Now().GetSeconds() << " --> " << bps1 << " bps (" << Band_pass.muLos * pktLen * 8 << " bps)"  << std::endl;
  //std::cout << "The State Number in t1 is: " << CurrentStates_g.at(ifaceNumber1) << std::endl;
  //std::cout << typeid(CurrentStates_g.at(ifaceNumber1)).name() << std::endl;

  if(CurrentStates_g.at(ifaceNumber1)!=Channelprev){
    ChannelVarLogFile << Simulator::Now().GetSeconds() << " " << CurrentStates_g.at(ifaceNumber1)<< std::endl;
  }
  Channelprev = CurrentStates_g.at(ifaceNumber1);
  // switch (CurrentStates_g.at(ifaceNumber1)){
  //   case State::LoS:
  //     timesInLos[0].push_back(t_cambio1);
  //     timesInLos[1].push_back(bps1);
  //     // std::cout << "LoS" << std::endl;
  //     break;
  //   case State::MS:
  //     timesInMs[0].push_back(t_cambio1);
  //     timesInMs[1].push_back(bps1);
  //     // std::cout << "MS" << std::endl;
  //     break;
  
  //   case State::DS:
  //     timesInDs[0].push_back(t_cambio1);
  //     timesInDs[1].push_back(bps1);
  //     // std::cout << "DS" << std::endl;
  //     break;

  //   default:
  //     std::cout << "error al registrar tiempo y tasa de estado" << std::endl;
  // }
     Simulator::Schedule(Seconds(t_cambio1), &MakinaEventos_toy, nd1, ifaceNumber1, Band_pass, index1); 
}







void
DisconnectMachine(ns3::Ptr<PointToPointNetDevice> nd2,const int& ifaceNumber2, const ProbMat_t& probMat2, const array_t& capFactor2, const double& muLos2, string& index2)
{
  
  CurrentDisconnectStates_g.at(ifaceNumber2) = SwapStateDisconnect(CurrentDisconnectStates_g.at(ifaceNumber2), probMat2); // actualiza estado
  double t_cambio2 = SwapTimeDisconnect(CurrentDisconnectStates_g.at(ifaceNumber2),index2);                           // tiempo de permanencia en estado actual
  double bps2 = capFactor2[CurrentDisconnectStates_g.at(ifaceNumber2) - 1] * muLos2 * pktLen * 8;      // seleccion BW para estado actual

  nd2->SetDataRate(DataRate(bps2));
  if(CurrentDisconnectStates_g.at(ifaceNumber2)!=Channelprev_stop){
    ChannelVarLogFile_stop << Simulator::Now().GetSeconds() << " " << CurrentDisconnectStates_g.at(ifaceNumber2)<< std::endl;
  }
  Channelprev_stop = CurrentDisconnectStates_g.at(ifaceNumber2);


  Simulator::Schedule(Seconds(t_cambio2), &DisconnectMachine, nd2, ifaceNumber2, probMat2, capFactor2, muLos2, index2); 
}


void
DisconnectMachine_toy(ns3::Ptr<PointToPointNetDevice> nd1, const int& ifaceNumber2,const band_features_t& Band_pass, string& index1)
{ 

  //std::cout << "The State Number in t0 is: " << CurrentStates_g.at(ifaceNumber1) << std::endl;
  CurrentDisconnectStates_g.at(ifaceNumber2) = SwapStateDisconnect(CurrentDisconnectStates_g.at(ifaceNumber2), Band_pass.probMat); // actualiza estado
  double t_cambio1 = Band_pass.mean_time.at(CurrentDisconnectStates_g.at(ifaceNumber2) - 1);                       // tiempo de permanencia fijado
  double bps1 = Band_pass.capFactor[CurrentDisconnectStates_g.at(ifaceNumber2) - 1] * Band_pass.muLos * pktLen * 8;      // seleccion BW para estado actual

  nd1->SetDataRate(DataRate(bps1));
  // std::cout << Simulator::Now().GetSeconds() << " --> " << bps1 << " bps (" << Band_pass.muLos * pktLen * 8 << " bps)"  << std::endl;
  //std::cout << "The State Number in t1 is: " << CurrentStates_g.at(ifaceNumber1) << std::endl;
  //std::cout << typeid(CurrentStates_g.at(ifaceNumber1)).name() << std::endl;
  if(CurrentDisconnectStates_g.at(ifaceNumber2)!=Channelprev_stop){
    ChannelVarLogFile_stop << Simulator::Now().GetSeconds() << " " << CurrentDisconnectStates_g.at(ifaceNumber2)<< std::endl;
  }
  Channelprev_stop = CurrentDisconnectStates_g.at(ifaceNumber2);


  Simulator::Schedule(Seconds(t_cambio1), &DisconnectMachine_toy, nd1, ifaceNumber2, Band_pass, index1); 
}


