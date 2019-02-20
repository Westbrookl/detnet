# Detnet
NS-3 implementation of deterministic networks/time sensitive networks

## Bandwidth Reservation
Bandwidth Reservation is a very important requirement for Deterministic Networks. The flows marked as detnet flows ask for bandwidth reservation and the routers need to explicitly reserve required bandwidth for the detnet flows. For this scenario we create a queue disc `BwResv` bandwidth reservation queue disc for ns-3 nodes. This is implemented at traffic-control layer of ns-3.

Different components of BwResv queue disc are

```
Flow Table
Active flows list for detnet flows
Active flows list for other flows
Priority for detnet flows to accept packets upto their bandwidth reservation limit
Calculate remaining bandwidth after serving all detnet flows and then serve other flows fairly.

```

### FlowTable
* TABLESIZE macro i.e number of rows. (we consider 1024)
* At each packet arrival flow hash is used to index the flow entry in flowtable by hash%TABLESIZE
* Each row in flowtable is a vector in which new elements are added when there are collisions. A liner search finds out if an entry already exists in a row when a flow arrives.
* Each entry consists of `FlowHash`, `Bw Requirement`, `FlowQueuelength`, `FlowType`

### Activelists
* Separate activlist is maintained for each type.
* Activelist contains only pointers to flowtable entries.

### Algorithm-Stage-1 (Decrement of queues)
* At every packet arrival, for all the flows in the detnet activelist, queues are decremented with `BwReq*delta` delta is the time difference between packet arrivals. The sum of all the decrements (`detnet_consumption`) is calculated
* If `delta*link_rate` is greater than `detnet_consumption` which means that, the output link still has some capacity to serve after detnet flows were served. In this case, the other flows queues are decremented equally with the remaining capacity available.

### Algorithm-Stage-2 (Increment of queues)
* At every packet arrival, packet is classified and its flowtable entry is obtained.
* It's queue is checked against a `THRESHOLD`
* Packet is enqued in only if flow queue is less that `THRESHOLD` . If flowqueue is zero, its is added to flow type activelist.
* If the packet is accepted, its queue is incremented with its `PktSize`

## NS-3 Experiment

### OnOff Application
We modified ns-3 onoff application to send user data in the packet payload. We reserved the first 12 bytes for sending `BwResev` data. The rest of the payload can be anything.

Format of first 12 bytes in payload (String data)
```
XXXXYbps T

XXXX is a 4 digit number
Y is  K/k/M/m/G/g etc. for representing kilo,mega,giga etc.
T is either "D" or "O". D is for detnet flows and O for Other flows.

```

We add an attribute "Buffer" in onoff application to send these 12 bytes.
Usage:
```
onoff.SetAttribute("Buffer",StringValue("1000Kbps D")); //12 bytes reserved
```

## BwResv queue disc configuration
Configuration of `BwResv` Queue :
```
TrafficControlHelper tchBottleneck;
tchBottleneck.SetRootQueueDisc ("ns3::BwResvQueueDisc","RateOther",DataRateValue(DataRate("1000kbps")),"RateDetnet",DataRateValue(DataRate("10000kbps")));
tchBottleneck.Install(d2d3); // d2d3 is a device container
```
## Files
* `ns-3-dev/scratch/detnet_2.cc` - ns-3 experiment file
* `ns-3-dev/src/traffic-control/bw_resv.cc` - BwResv QueueDisc implementation
* `scripts` folder contains bash scripts for collecting ns-3 throughput output data and gnuplot files.
