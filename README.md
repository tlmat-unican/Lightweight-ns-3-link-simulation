# Lightweight modeling of dynamic channels: application to NTN scenarios

This repository contains the implementation over ns-3 of thoretical wireless link models based on Markov Chains. This modeling is intended to carry out performance evalaution of upper layers (e.g. transport) over dynamic channels without the complexity of detailed implementation of wireless technologies. 

## Dependencies

It has been tested in `Ubuntu 22.04.1 LTS` _(jammy)_. It has the following dependencies:
- Dependencies of [ns-3](https://www.nsnam.org/wiki/Installation)
```
apt install g++ python3 cmake ninja-build git
```

- _python3_ and _pip3_ to automatize the evaluation
```
apt install python3
apt install python3-pip
```

- [Jupyter](https://jupyter.org/install) optional to generate plots.

## Project Structure
- The folder [_ns-allinone-3.35_](./ns-allinone-3.35) contains a copy of _ns-3_. The [_scratch_](./ns-allinone-3.35/ns-3.35/scratch/) folder contains a generic scenario ([_p2p_scenarioOnBuild.cc_](./ns-allinone-3.35/ns-3.35/scratch/p2p_scenarioOnBuild.cc)) to generate evaluation topologies. A second scenario [_p2p_scenarioReusable_taps.cc_](./ns-allinone-3.35/ns-3.35/scratch/p2p_scenarioReusable_taps.cc)) has been defined to use the link models with real applications using ns-3 TAP. 

- The [_scripts_](./scripts/) folder contains scripts needed to run different topologies. It also contains a Jupyter notebook to represent the outcomes.

## Building ns-3

Configure and compile ns-3, we disable not used functionalities
```
cd ns-allinone-3.35/ns-3.35
./waf configure --disable-werror --disable-python --disable-tests --disable-examples --build-profile=optimized
./waf
```

## Scenarios

Below are the scripts that have been used to prepare the results of the paper. The scripts automatically generate the folder tree where the results are stored, and by keeping this structure, a representation of the results can be obtained like in the paper. All results will be generated under the folder _./scripts/RESULTS_

In addition, the files used for the simulation of each of the scenarios using Python3 are described below, all of them are collected in [scripts](./scripts/). 
In all cases the evaluation embraces a pair ns-3 client and server sending a 300 MB datafile with a rate of 40 Mbpos, that equals the average capacity of the first link from the sender (access link). 

In all cases, the execution of a scenario generates the following logs: 
- Congestion window: _logCWND.log_ [time (seconds) | size (B)]
- Received traffic: _RXFile.log_ [pkt number | time (seconds) | pkt size (B)]
- Transmission traffic: _TXFile.log_  [pkt number | time (seconds) | pkt size(B)]
- Access buffer occupancy: _logBuffer.log_ [time (seconds) | size buffer (B)]
- ChannelVar.log: [time (seconds)| State] where State = {LoS=1,Ms=2,Ds=3}
- (Optional) ChannelVar_stop.log: [time (seconds)| State] where State = {Work=1,Stop}

### Synthetic LMS link (figure 3)

This script deploys a Land-Mobile-System (LMS) link modeled as a 3-state Markov Chain with constant stay times of 5 seconds. It runs the scenario once. 

- Run python script [_sim_ns3_lms_toy.py_](./scripts/lms_toy.json)
```
cd scripts
python3 sim_ns3_lms_toy.py
```
- The topology configuration is in file [./ns-allinone-3.35/ns-3.35/scratch/lms_toy.json](./ns-allinone-3.35/ns-3.35/scratch/lms_toy.json)
- The output is saved in the folder [./scripts/RESULTS/Cubic_SyntheticLMS/](./scripts/RESULTS/Cubic_SyntheticLMS/).

### Realistic LMS link (figure 4)

This script deploys a Land-Mobile-System (LMS) link modeles as a 3-state Markov Chain with exponentially distributed stay times. It runs the scenario once.

- Run python script [_sim_ns3_lms_toy.py_](./scripts/lms_toy.json)
```
cd scripts
python3 sim_ns3_lms.py 
```
- The topology configuration is in file [./ns-allinone-3.35/ns-3.35/scratch/lms.json](./ns-allinone-3.35/ns-3.35/scratch/lms.json)
- The output is saved in the folder [./scripts/RESULTS/Cubic_RealisticLMS/](./scripts/RESULTS/Cubic_RealisticLMS/).

### End-to-end with disconnections traffic: LMS - ISL(disconnections)-LMS (figure 5)

This script deploys three links, UP/DOWN LMS links and an inter-satellite-link (ISL) connecting them, with disconnections. The script sweeps the average disconnection time from 0 to 3 seconds with step 0.2 seconds. For each value it runs *Nsim* independent executions. By default *NSim* is set to 5, increase it to obtain statistically significant results.

- Run python script [_sim_ns3_lms_toy.py_](./scripts/ConfigScenario_disconnect.json)
```
cd scripts
python3 sim_ns3_lms.py 
```
- The topology configuration is in file [./ns-allinone-3.35/ns-3.35/scratch/ConfigScenario_stop.json](./ns-allinone-3.35/ns-3.35/scratch/ConfigScenario.json)
- The output is saved in the folder [./scripts/RESULTS/endToEndDisconnect/](./scripts/RESULTS/endToEndDisconnect/) where one folder is created for each value of the background traffic, and simulation.  

### End-to-end with background traffic: LMS - ISL(background traffic)-LMS (figure 6)

This script deploys three links, UP/DOWN LMS links and an inter-satellite-link (ISL) connecting them, with background traffic. The script sweeps the background traffic from 5 to 30 Mbps with step 1 Mbps. For each value it runs *Nsim* independent executions. By default *NSim* is set to 5, increase it to obtain statistically significant results.

- Run python script [_sim_ns3_lms_toy.py_](./scripts/lms_toy.json)
```
cd scripts
python3 sim_ns3_lms.py 
```
- The topology configuration is in file [./ns-allinone-3.35/ns-3.35/scratch/ConfigScenario.json](./ns-allinone-3.35/ns-3.35/scratch/ConfigScenario.json)
- The output is saved in the folder [./scripts/RESULTS/endToEndBackground/](./scripts/RESULTS/endToEndBackground/) where one folder is created for each value of the background traffic, and simulation.  

### Applications with ns-3 TAP
This script runs the scenario using ns-3 TAP to use real applications instead of ns-3 client/server.
The script uses the commands _tunctl_ and _ip_. Install them as follows
```
apt install uml-utilities
apt install iproute2
```

## Jupyter NoteBook
[Analysis.ipynb](./scripts/Analysis.ipynb) contains code snippets to reproduce the results shown in the paper. Figures generated by [Analysis.ipynb](./scripts/Analysis.ipynb) are saved in [figures](./figures/) folder. 

__Disclaimer__: figures in manuscrip are generated with Tikz, points in temporal plots are reduced for better visibility and boxplots are generated with 100 executions.

## Contact 

* [Fátima Khan Blanco (khanf@unican.es)](mailto:khanf@unican.es)
* [Luis Diez (diezlf@unican.es)](mailto:diezlf@unican.es)
* [Ramón Agüero (agueroc@unican.es)](mailto:agueroc@unican.es)
