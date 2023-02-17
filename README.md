# Lightweight modeling of dynamic channels: application to NTN scenarios
## Usage requirements

- Intsall [Python3](https://www.python.org/downloads/) 
- Install [ns-3](https://www.nsnam.org/wiki/Installation)
- Intsall [Jupyter](https://jupyter.org/install) 
## Project Structure
- Two scenarios are included in folder [_scratch_](./ns-allinone-3.35/ns-3.35/scratch/), it has been tested for _ns-3.35_.  One of them includes tap-bridges.
- Folder [_scripts_](./scripts/) contains every script needed to run each topology, shown in the paper. It also contains Jupyter notebooks to represent the outcomes.
## Dependecies 
It has been tested in `Ubuntu 22.04.1 LTS` _(jammy)_. It has the following dependencies:

- _python3_, _pip3_,
```
apt install python3
apt install python3-pip
```

- Running NS3 optimized
```
# first we clean all the compile options
./waf distclean
# we configure the compile options as optimized, disable examples, tests, python integration and static.
./waf configure --disable-python --disable-tests --disable-examples --build-profile=optimized
# we recompile (this might take some time)
./waf
```

## Event - Driver - Simulator - Utilities
As mentioned above, [_scratch_](./ns-allinone-3.35/ns-3.35/scratch/) folder contains the scenarios. Besides, it includes the needed utilities to deploy a topology and the skills to mimic a variant channel. 

## Scenarios
Below are the scripts that have been used to prepare the results of [PAPER], also, as mentioned, a version is included where the necessary Tap Briges are added to be able to mimic real hosts.

In addition, the files used for the simulation of each of the scenarios using Python3 are described below, all of them are collected in [scripts](./scripts/). 

### Simple model: LMS TOY
This script deploys a Land - Mobile - System, with 5 s stay time in each state.
- Use python script _sim_ns3_lms_toy.py_
    -   Requires:
        - lms_toy.json [Location: Scratch folder inside ns3-35 folder]
    -   Interest Output:
        - Outcome folder contains:
            - logCWND.log: [time | size (MSS)]
            - RXFile.log: [num pkt | time | size pkt (B)]
            - TXFile.log: [num pkt | time | size pkt (B)]
            - logBuffer.log: [time | size buffer (B)]
            - ChannelVar.log: [time | State] where State = {LoS=1,Ms,Ds}
            - ChannelVar_stop.log: [time | State] where State = {Work=1,Stop}
    - Mode of use: This script is able to run once the scenario, by excuting the _python3_ command. It creates a tree folder where the results are collected. You can customise this parameters, as well as the scenario described in the aforementioned JSON file. After running this script, you will get data files which are handled by the Jupyter notebook to show the performance.
### LEO based scenarios: LMS 
This script deploys a Land - Mobile - System, with exponentially distributed stay times.
- Use python script _sim_ns3_lms.py_
    -   Requires:
        - lms.json [Location: Scratch folder inside ns3-35 folder]
    -   Interest Output:
        - Outcome folder:
            - logCWND.log: [time | size (MSS)]
            - RXFile.log: [num pkt | time | size pkt (B)]
            - TXFile.log: [num pkt | time | size pkt (B)]
            - logBuffer.log: [time | size buffer (B)]
            - ChannelVar.log: [time | State] where State = {LoS=1,Ms,Ds}
    - Mode of use: This script is able to run once the scenario, by excuting the _python3_ command. It creates a tree folder where the results are collected. You can customise this parameters, as well as the scenario described in the aforementioned JSON file. After running this script, you will get data files which are handled by the Jupyter notebook to show the performance.
           
### LEO based scenarios: LMS - Background traffic - LMS
This script deploys three links, UP/DOWN links are modeling a Land - Mobile - System and the ISL link presents background traffic. This last feature is swept.
- Use python script _sim_ns3_sims_stop.py_
    -   Requires:
        - ConfigScenario_back_template.json [Location: Scratch folder inside ns3-35 folder]
    -   Interest Output:
        - Outcome folder - sweep configuration contains:
            - SIM [#] folder:
                - logCWND.log: [time | size (MSS)]
                - RXFile.log: [num pkt | time | size pkt (B)]
                - TXFile.log: [num pkt | time | size pkt (B)]
                - logBuffer.log: [time | size buffer (B)]
                - ChannelVar.log: [time | State] where State = {LoS=1,Ms,Ds}
    - Mode of use: This script is able to run the scenario, by excuting the _python3_ command. It creates a tree folder where the results are collected. You can customise this parameters, as well as the scenario described in the aforementioned JSON file. After running this script, you will get data files which are handled by the Jupyter notebook to show the performance. MonteCarlo simulations [Nsim] and sweep range of the background traffic can be adjusted. <br />
    In order to mimic the paper outcomes - the setup values are:
        - Nsim  = 100
        - Background traffic = [ 5 to 30 ] Mbps with step = 1 Mbps. 
               

### LEO based scenarios: LMS - Interrupted Channel - LMS
This script deploys three links, UP/DOWN links are modeling a Land - Mobile - System and the ISL link presents disconnections. This last feature is swept.
- Use python script _sim_ns3_sims_background_traffic.py_
    -   Requires:
        - ConfigScenario_back_template.json [Location: Scratch folder inside ns3-35 folder]
    -   Interest Output:
        - Outcome folder - sweep configuration contains:
            - SIM [#] folder:
                - logCWND.log: [time | size (MSS)]
                - RXFile.log: [num pkt | time | size pkt (B)]
                - TXFile.log: [num pkt | time | size pkt (B)]
                - logBuffer.log: [time | size buffer (B)]
                - ChannelVar.log: [time | State] where State = {LoS=1,Ms,Ds}
    - Mode of use: This script is able to run the scenario, by excuting the _python3_ command. It creates a tree folder where the results are collected. You can customise this parameters, as well as the scenario described in the aforementioned JSON file. After running this script, you will get data files which are handled by the Jupyter notebook to show the performance. MonteCarlo simulations [Nsim] and sweep range of the stop mean time can be adjusted. <br />
    In order to mimic the paper outcomes - the setup values are:
        - Nsim  = 100
        - Stop mean time = [0 to 3] s with step = 0.2 s. 
              
### Applications with ns-3 TAP
- Use python script _sim_ns3_taps.py_
    -   Requires:
        - lms_toy.json [Location: Scratch folder inside ns3-35 folder]
    -   Interest Output:
        - Outcome folder contains:
            - logCWND.log: [time | size (MSS)]
            - RXFile.log: [num pkt | time | size pkt (B)]
            - TXFile.log: [num pkt | time | size pkt (B)]
            - logBuffer.log: [time | size buffer (B)]
            - ChannelVar.log: [time | State] where State = {LoS=1,Ms,Ds}

### Extras: Running with real hosts
- _tunctl_, _ip_ (should be there, just in case of using real hosts). You need to install the following packages for easy bridge management.
```
apt install uml-utilities
apt install iproute2
```

## Usage
To run a network topology the _<--name-->.py_ file, where _name_ changes according the details explained above.
```shell
python3 <--name-->.py
```

## Jupyter NoteBook
[Analysis.ipynb](./scripts/Analysis.ipynb) contains code snippets to reproduce the results shown in the paper. According to the folder tree created by running the scripts, this notebook is able to show the performance of the results obtained. It also includes several figures as examples of what can be obtained by running the code sections. The code snippets are organised according to the scenarios simulated.


## Contact 

* [Fátima Khan Blanco (khanf@unican.es)](mailto:khanf@unican.es)
* [Luis Diez (diezlf@unican.es)](mailto:diezlf@unican.es)
* [Ramón Agüero (agueroc@unican.es)](mailto:agueroc@unican.es)
