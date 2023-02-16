'''
Script to analyse the variation in background traffic state. Modify Nsim to adjust the number of simulations.
'''

import os
import time 
import json
import subprocess


# Global Params
ns3ScenarioC = 'scratch/p2p_scenarioOnBuild'
ns3Path = '../ns-allinone-3.35/ns-3.35'
ConfigPath = "/scratch/ConfigScenario.json"
Nsim = 1
sim_rate = 0
Transport_model = "Cubic"
OutcomeFolder = "OUTCOMES"
Min_rate = 5
Max_rate = 30
Step = 1
background_traff_swept = range(Min_rate,Max_rate,Step)




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
  print(cmd + "\n")
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

    os.system("mkdir {}/BackTrafficv2/".format(OutcomeFolder))
    for sim_rate in background_traff_swept:
      Sim_name = "{}_finito_1BDPConfigScenario{}_sims".format(Transport_model,sim_rate)
      cmd = 'mkdir ./{}/BackTrafficv2/{}/'.format(OutcomeFolder,Sim_name)
      os.system(cmd)

      if Sim_name!="":
        Config_Out="--out-folder-path={}".format("../../ns3_sim/" + Sim_name+ "/") 
      with open("../ns-allinone-3.35/ns-3.35/scratch/ConfigScenario_template.json", "r") as jsonFileReceiver:
        data = json.load(jsonFileReceiver)
      print(data["Links"][1]["BackFeatures"]["DataRate"] )
      data["Links"][1]["BackFeatures"]["DataRate"] = sim_rate

      with open("../ns-allinone-3.35/ns-3.35/scratch/ConfigScenario.json", "w") as jsonFileReceiver:
                  json.dump(data, jsonFileReceiver)
      for sim in range(1, Nsim + 1,1):
        cmd = 'mkdir ./{}/BackTrafficv2/{}/{}/'.format(OutcomeFolder,Sim_name,sim)
        os.system(cmd)
        if Sim_name!="":
          Config_Out="--out-folder-path={}".format("../../scripts/{}/BackTrafficv2/" + Sim_name + "/"+ str(sim) + "/".format(OutcomeFolder)) 
        RunNs3(ns3Path, ns3ScenarioC, ConfigPath, Config_Out, Config_Model)

      # StopNs3(ns3ScenarioC)

if __name__ == '__main__':
  main()