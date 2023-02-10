import os
import numpy as np

ns3ScenarioD = 'p2p_scenarioOnBuild'
ns3ScenarioC = 'scratch/p2p_scenarioOnBuild'
ns3Path = '../ns-allinone-3.35/ns-3.35'

ConfigPath = "/scratch/ConfigScenario_stop.json"
Outcomefolder= "OUTCOMES"
Nsim = 1

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
    os.system("mkdir {}/Disconnection2/".format(Outcomefolder))
    time_range = np.round(np.arange(0,3.2,0.2),2)
    print(time_range)
    for timevalue in time_range:
      Sim_name = "{}/Disconnection2/{}stop{}_sims".format(Outcomefolder,Transport_model,timevalue)
      cmd = 'mkdir ./{}/'.format(Sim_name)
      os.system(cmd)
      for sim in range(1, Nsim + 1,1):
        if Sim_name!="":
          Config_Out="--out-folder-path={}/{}/ --stop-time={}".format("../../scripts/" + Sim_name, sim,timevalue) 
        cmd = 'mkdir ./{}/{}/'.format(Sim_name,sim)
        os.system(cmd)
        RunNs3(ns3Path, ns3ScenarioC, ConfigPath, Config_Out, Config_Model)

if __name__ == '__main__':
  main()