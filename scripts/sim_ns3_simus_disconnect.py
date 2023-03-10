import os
import numpy as np
''' 
Script to analyse the variation in stop state. Modify Nsim to adjust the number of simulations.
'''
## Global Params
ns3ScenarioC = 'scratch/p2p_scenarioOnBuild'
ns3Path = '../ns-allinone-3.35/ns-3.35'
ConfigPath = "/scratch/ConfigScenario_stop.json"
Outcomefolder= "RESULTS"
## To set up Stop time swept
Time_min_value = 0
Time_max_value = 3
Step = 0.2
Nsim = 5 ## To set the number of simulations from each scenario
time_range = np.round(np.arange(Time_min_value,Time_max_value+Step,Step),2) ## To create sweep range
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
  print(cmd)
  os.system(cmd) 

def StopNs3 (scenario) :
  '''
  Step the ns-3 scenario
  '''
  cmd = 'sudo killall {}'.format(scenario)
  print(cmd)
  os.system (cmd)

def main():
    print("Start sim")
    os.system("mkdir -p {}/endToEndDisconnect/".format(Outcomefolder))
    for timevalue in time_range:
      Sim_name = "{}/endToEndDisconnect/{}stop{}_sims".format(Outcomefolder,Transport_model,timevalue)
      cmd = 'mkdir -p ./{}/'.format(Sim_name)
      os.system(cmd)
      for sim in range(1, Nsim + 1,1):
        Config_Out="--out-folder-path={}/{}/ --stop-time={}".format("../../scripts/" + Sim_name, sim,timevalue) 
        cmd = 'mkdir -p ./{}/{}/'.format(Sim_name,sim)
        os.system(cmd)
        print(f'Running simulation {sim}/{Nsim} with avg. disconnect time {timevalue}s ')
        RunNs3(ns3Path, ns3ScenarioC, ConfigPath, Config_Out, Config_Model)

if __name__ == '__main__':
  main()