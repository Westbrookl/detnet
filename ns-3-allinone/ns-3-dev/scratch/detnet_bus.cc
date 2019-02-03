/*
 * detnet.cc
 *
 *  Created on: Dec 4, 2018
 *      Author: vamsi
 */

#include <iostream>
#include <fstream>
#include <string>

#include "ns3/animation-interface.h"
#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/csma-module.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("detnet");

int
main (int argc, char *argv[])
{
	Time::SetResolution(Time::NS);
	LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_ALL);
	LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_ALL);
	//NS_LOG_INFO ("Creating Topology");
	NodeContainer p2pnodes; // similar to machines/pc/switch/routers

	NodeContainer csmanodes;
	csmanodes.Create(11);

	NodeContainer csmanodes1;
	csmanodes1.Create(11);

	p2pnodes.Add(csmanodes.Get(10));
	p2pnodes.Add(csmanodes1.Get(10));

	CsmaHelper csmachannel;
	csmachannel.SetChannelAttribute("DataRate", StringValue("10Mbps"));
	csmachannel.SetChannelAttribute("Delay",TimeValue(NanoSeconds(2000)));

	CsmaHelper csmachannel1;
	csmachannel1.SetChannelAttribute("DataRate", StringValue("10Mbps"));
	csmachannel1.SetChannelAttribute("Delay",TimeValue(NanoSeconds(2000)));


	PointToPointHelper p2pchannel; //physical channel
	p2pchannel.SetDeviceAttribute("DataRate",StringValue("10Mbps"));
	p2pchannel.SetChannelAttribute("Delay",StringValue("2ms"));

	NetDeviceContainer devices; // hardware devices like nic
	devices = p2pchannel.Install(p2pnodes);

	NetDeviceContainer csmadevices;
	csmadevices = csmachannel.Install(csmanodes);

	NetDeviceContainer csmadevices1;
	csmadevices1 = csmachannel1.Install(csmanodes1);

	InternetStackHelper stack;
	//stack.Install(p2pnodes.Get(0)); // to install ip stack i.e ip,tcp/udp on net devices
	stack.Install(csmanodes);
	stack.Install(csmanodes1);



	Ipv4AddressHelper addr;
	addr.SetBase("10.1.1.0","255.255.255.0"); //ip base and subnet mask
	Ipv4InterfaceContainer p2pinterfaces = addr.Assign(devices);
	addr.SetBase("10.1.2.0","255.255.255.0");
	Ipv4InterfaceContainer csmainterfaces = addr.Assign(csmadevices);
	addr.SetBase("10.1.3.0","255.255.255.0");
	Ipv4InterfaceContainer csmainterfaces1 = addr.Assign(csmadevices1);

	UdpEchoServerHelper echoserver(9); //port number in the brackets
	// udp port 9 is a discard server

	ApplicationContainer serverapps = echoserver.Install(csmanodes.Get(9));
	serverapps.Start(Seconds(1.0));
	serverapps.Stop(Seconds(10.0));

	UdpEchoClientHelper echoclient(csmainterfaces.GetAddress(9),9);
	echoclient.SetAttribute("MaxPackets",UintegerValue(20));
	echoclient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
	echoclient.SetAttribute("PacketSize",UintegerValue(10240));

	ApplicationContainer clientapps = echoclient.Install(csmanodes1.Get(9));
	clientapps.Start(Seconds(2.0));
	clientapps.Stop(Seconds(10.0));

	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

	/*Capture*/
	p2pchannel.EnablePcapAll ("second");
	csmachannel.EnablePcap ("second", csmadevices.Get (5), true);
	csmachannel.EnablePcap ("second", csmadevices1.Get (5), true);

  AnimationInterface anim ("detnet_bus.xml");
  anim.SetConstantPosition(p2pnodes.Get(1),45,45);
  anim.SetConstantPosition(p2pnodes.Get(0),45,70);

  for (uint32_t i=0;i<10;i++){
	  anim.SetConstantPosition(csmanodes.Get(i),50+5*i,70+3*i);
  }
  for (uint32_t i=0;i<10;i++){
	  anim.SetConstantPosition(csmanodes1.Get(i),50+5*i,45+3*i);
  }
  anim.EnableIpv4RouteTracking("detnet_bus_rout.xml",NanoSeconds(2000),NanoSeconds(11000),Seconds(1));
  //Simulator::Stop (Seconds (11.0));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}



