import os
import json
'''
Script to analyse the variation in background traffic state. Modify Nsim to adjust the number of simulations.
'''
# Global Params
ns3ScenarioC = 'scratch/p2p_scenarioOnBuild'
ns3Path = '../ns-allinone-3.35/ns-3.35'
ConfigPath = "/scratch/ConfigScenario.json"
sim_rate = 0
Transport_model = "Cubic"
OutcomeFolder = "RESULTS"
## To set up Background traffic swept
Min_rate = 5
Max_rate = 20
Step = 1
Nsim = 5 ## To set the number of simulations from each scenario
background_traff_swept = range(Min_rate,Max_rate+Step,Step) ## To create sweep range

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

def StopNs3 (scenario) :
  '''
  Step the ns-3 scenario
  '''
  cmd = 'sudo killall {}'.format(scenario)
  print(cmd)
  os.system (cmd)

def main():
    print("Start sim")
    os.system("mkdir -p {}/endToEndBackground/".format(OutcomeFolder))
    for sim_rate in background_traff_swept:
      Sim_name = "{}_background_{}_mbps".format(Transport_model,sim_rate)
      cmd = 'mkdir -p ./{}/endToEndBackground/{}/'.format(OutcomeFolder,Sim_name)
      os.system(cmd)

      Config_Out="--out-folder-path={}".format("../../ns3_sim/" + Sim_name+ "/") 
      with open("../ns-allinone-3.35/ns-3.35/scratch/ConfigScenario_template.json", "r") as jsonFileReceiver:
        data = json.load(jsonFileReceiver)
      print(data["Links"][1]["BackFeatures"]["DataRate"] )
      data["Links"][1]["BackFeatures"]["DataRate"] = sim_rate

      with open("../ns-allinone-3.35/ns-3.35/scratch/ConfigScenario.json", "w") as jsonFileReceiver:
                  json.dump(data, jsonFileReceiver)
      for sim in range(1, Nsim + 1,1):
        outFolder = '{}/endToEndBackground/{}/{}/'.format(OutcomeFolder,Sim_name,sim)
        os.system('mkdir -p ./{}'.format(outFolder))
        Config_Out="--out-folder-path={}".format("../../scripts/{}".format(outFolder)) 
        print(Config_Out)
        print(f'Running simulation {sim}/{Nsim} with background traffic {sim_rate}Mbps ')  
        RunNs3(ns3Path, ns3ScenarioC, ConfigPath, Config_Out, Config_Model)

if __name__ == '__main__':
  main()