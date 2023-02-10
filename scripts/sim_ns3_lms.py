import os
import time 
import subprocess

## Global Params

ns3ScenarioC = 'scratch/p2p_scenarioOnBuild'
# ns3ScenarioC = 'scratch/tap-lms'
ns3Path = '../ns-allinone-3.35/ns-3.35'

ConfigPath = "/scratch/lms.json"
OutcomeFolder = "OUTCOMES/"
os.system('mkdir ./{}'.format(OutcomeFolder))

Transport_model = "Cubic"
Sim_name = "{}{}_finito_MedioBDPConfigScenario_LMS".format(OutcomeFolder,Transport_model)

if ConfigPath!="":
    ConfigPathStr="--json-path={}".format(ConfigPath)

if Sim_name!="":
    Config_Out="--out-folder-path={}".format("../../ns3_sim/" + Sim_name+ "/") 

if Transport_model!="":
    Config_Model="--tcp-model={}".format(Transport_model) 

cmd = 'mkdir ./{}/'.format(Sim_name)

os.system(cmd)







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

def StopNs3 (scenario) :
  '''
  Step the ns-3 scenario
  '''
  cmd = 'sudo killall {}'.format(scenario)
  print(cmd)
  os.system (cmd)



def main():
  
    # print("Start sim")
    RunNs3(ns3Path, ns3ScenarioC, ConfigPath, Config_Out, Config_Model)

    # StopNs3(ns3ScenarioC)

if __name__ == '__main__':
  main()