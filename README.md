# Tittle : [Name of the Paper]
## Usage requirements

- Intsall Python3: https://www.python.org/downloads/
- Install ns3: https://www.nsnam.org/wiki/Installation
- Intsall Jupyter: https://jupyter.org/install 

## Scenarios

### Simple model: LMS TOY
- Use python script sim_ns3_lms_toy.py
    -   Requires:
        - lms_toy.json [Location: Scratch folder inside ns3-35 folder]
    -   Interest Output:
        - Outcome folder contains:
            - logCWND.log: [time | size (MSS)]
            - RXFile.log: [num pkt | time | size pkt(B)]
            - TXFile.log: [num pkt | time | size pkt(B)]
            - logBuffer.log: [time | size buffer(B)]
            - ChannelVar.log: [time | State] where State = {LoS=1,Ms,Ds}
            - ChannelVar_stop.log: [time | State] where State = {Work=1,Stop}
### LEO based scenarios: LMS 
- Use python script sim_ns3_lms.py
    -   Requires:
        - lms.json [Location: Scratch folder inside ns3-35 folder]
    -   Interest Output:
        - Outcome folder:
            - logCWND.log: [time | size (MSS)]
            - RXFile.log: [num pkt | time | size pkt(B)]
            - TXFile.log: [num pkt | time | size pkt(B)]
            - logBuffer.log: [time | size buffer(B)]
            - ChannelVar.log: [time | State] where State = {LoS=1,Ms,Ds}
            - ChannelVar_stop.log: [time | State] where State = {Work=1,Stop}
### LEO based scenarios: LMS - Background traffic - LMS
- Use python script sim_ns3_sims_stop.py
    -   Requires:
        - ConfigScenario_back_template.json [Location: Scratch folder inside ns3-35 folder]
    -   Interest Output:
        - Outcome folder - sweep configuration contains:
            - SIM [#] folder:
                - logCWND.log: [time | size (MSS)]
                - RXFile.log: [num pkt | time | size pkt(B)]
                - TXFile.log: [num pkt | time | size pkt(B)]
                - logBuffer.log: [time | size buffer(B)]
                - ChannelVar.log: [time | State] where State = {LoS=1,Ms,Ds}
                - ChannelVar_stop.log: [time | State] where State = {Work=1,Stop}


### LEO based scenarios: LMS - Interrupted Channel - LMS
- Use python script sim_ns3_sims_back.py
    -   Requires:
        - ConfigScenario_back_template.json [Location: Scratch folder inside ns3-35 folder]
    -   Interest Output:
        - Outcome folder - sweep configuration contains:
            - SIM [#] folder:
                - logCWND.log: [time | size (MSS)]
                - RXFile.log: [num pkt | time | size pkt(B)]
                - TXFile.log: [num pkt | time | size pkt(B)]
                - logBuffer.log: [time | size buffer(B)]
                - ChannelVar.log: [time | State] where State = {LoS=1,Ms,Ds}
                - ChannelVar_stop.log: [time | State] where State = {Work=1,Stop}

### Applciations with ns-3 TAP
- Use python script sim_ns3_taps.py
    -   Requires:
        - lms_toy.json [Location: Scratch folder inside ns3-35 folder]
    -   Interest Output:
        - Outcome folder contains:
            - logCWND.log: [time | size (MSS)]
            - RXFile.log: [num pkt | time | size pkt(B)]
            - TXFile.log: [num pkt | time | size pkt(B)]
            - logBuffer.log: [time | size buffer(B)]
            - ChannelVar.log: [time | State] where State = {LoS=1,Ms,Ds}
            - ChannelVar_stop.log: [time | State] where State = {Work=1,Stop}

## Contact 

* [Fátima Khan Blanco (khanf@unican.es)](mailto:khanf@unican.es)
* [Luis Diez (diezlf@unican.es)](mailto:diezlf@unican.es)
* [Ramón Agüero (agueroc@unican.es)](mailto:agueroc@unican.es)
