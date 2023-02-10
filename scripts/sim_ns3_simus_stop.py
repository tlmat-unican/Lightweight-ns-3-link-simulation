

import os
import numpy as np
import time 
import json
import subprocess






ns3ScenarioD = 'p2p_scenarioOnBuild_stop'
ns3ScenarioC = 'scratch/p2p_scenarioOnBuild_stop'
# ns3ScenarioC = 'scratch/tap-lms'
ns3Path = '../ns-allinone-3.35/ns-3.35'

ConfigPath = "/scratch/ConfigScenario_stop.json"

Nsim = 100
sim_rate = 0
Transport_model = "Cubic"







if ConfigPath!="":
    ConfigPathStr="--json-path={}".format(ConfigPath)


if Transport_model!="":
    Config_Model="--tcp-model={}".format(Transport_model) 






def RunNs3 (ns3Path, scenario, ConfigPath,folder_name,Config_model_value):
  cmd = 'cd {} && sudo ./waf'.format(ns3Path)
  os.system(cmd)
  if ConfigPath!="":
    ConfigPathStr=" --json-path=.{}".format(ConfigPath)
    cmd = 'cd {} && sudo ./waf --run "{}{} {} {}"'.format(ns3Path,scenario,ConfigPathStr,folder_name,Config_model_value)
  else:
    cmd = 'cd {} && sudo ./waf --run "{}{}"'.format(ns3Path, scenario,folder_name)
  #print(cmd + "\n")
  os.system(cmd) 
  # pr.wait() 
  # return pr

def StopNs3 (scenario) :
  '''
  Step the ns-3 scenario
  '''
  cmd = 'sudo killall {}'.format(scenario)
  print(cmd)
  os.system (cmd)



def main():
    print("Start sim")

    
    os.system("mkdir RESULTADOS/Disconnection/")
    time_range = np.round(np.arange(0,3.2,0.2),2)
    print(time_range)
    for timevalue in time_range:
      Sim_name = "{}_finito_1BDPConfigScenario_stop{}_sims".format(Transport_model,timevalue)
      cmd = 'mkdir ./RESULTADOS/Disconnection/{}/'.format(Sim_name)
      os.system(cmd)
      if Sim_name!="":
        Config_Out="--out-folder-path={} --stop-time={}".format("../../ns3_sim/" + Sim_name+ "/",timevalue) 

      for sim in range(1, Nsim + 1,1):
        cmd = 'mkdir ./RESULTADOS/Disconnection/{}/{}/'.format(Sim_name,sim)
        os.system(cmd)
        if Sim_name!="":
          Config_Out="--out-folder-path={}".format("../../ns3_sim/RESULTADOS/Disconnection/" + Sim_name + "/"+ str(sim) + "/") 
        RunNs3(ns3Path, ns3ScenarioC, ConfigPath, Config_Out, Config_Model)
    
      # StopNs3(ns3ScenarioC)

if __name__ == '__main__':
  main()