# Tittle : [Name of the Paper]
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

In addition, the files used for the simulation of each of the scenarios using Python3 are described.

### Simple model: LMS TOY
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
### LEO based scenarios: LMS 
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
           
### LEO based scenarios: LMS - Background traffic - LMS
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
               

### LEO based scenarios: LMS - Interrupted Channel - LMS
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
## Outcomes - display
In folder [_scratch_](./ns-allinone-3.35/ns-3.35/scratch/), there is a Jupyter notebook, drawing the perfomance evolution from the result folders.
## Usage

To run a network topology the _<--name-->.py_ file:

```shell
python3 <--name-->.py
```
           
## Contact 

* [Fátima Khan Blanco (khanf@unican.es)](mailto:khanf@unican.es)
* [Luis Diez (diezlf@unican.es)](mailto:diezlf@unican.es)
* [Ramón Agüero (agueroc@unican.es)](mailto:agueroc@unican.es)
