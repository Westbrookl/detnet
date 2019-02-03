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
	Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (210));
	Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("448kb/s"));


	NS_LOG_INFO ("Create nodes.");
	NodeContainer c;
	c.Create(6);
	NodeContainer n0n2 = NodeContainer(c.Get(0),c.Get(2));
	NodeContainer n1n2 = NodeContainer(c.Get(1),c.Get(2));
	NodeContainer n2n3 = NodeContainer(c.Get(2),c.Get(3));
	NodeContainer n4n3 = NodeContainer(c.Get(4),c.Get(3));
	NodeContainer n5n3 = NodeContainer(c.Get(5),c.Get(3));

	InternetStackHelper internet;
	internet.Install(c);
	Ipv4AddressHelper ipv4;

	//n0n2 n1n2
	PointToPointHelper n0n2ch,n1n2ch;
	n0n2ch.SetDeviceAttribute("DataRate",StringValue("10Mbps"));
	n0n2ch.SetChannelAttribute("Delay",StringValue("2ms"));
	NetDeviceContainer d0d2 = n0n2ch.Install(n0n2);
	ipv4.SetBase("10.1.1.0","255.255.255.0");
	Ipv4InterfaceContainer i0i2 = ipv4.Assign(d0d2);

	n1n2ch.SetDeviceAttribute("DataRate",StringValue("10Mbps"));
	n1n2ch.SetChannelAttribute("Delay",StringValue("2ms"));
	NetDeviceContainer d1d2 = n1n2ch.Install(n1n2);
	ipv4.SetBase("10.1.2.0","255.255.255.0");
	Ipv4InterfaceContainer i1i2 = ipv4.Assign(d1d2);

	//n4n3 n5n3
	PointToPointHelper n4n3ch,n5n3ch;
	n4n3ch.SetDeviceAttribute("DataRate",StringValue("10Mbps"));
	n4n3ch.SetChannelAttribute("Delay",StringValue("2ms"));
	NetDeviceContainer d4d3 = n4n3ch.Install(n4n3);
	ipv4.SetBase("10.1.3.0","255.255.255.0");
	Ipv4InterfaceContainer i4i3 = ipv4.Assign(d4d3);

	n5n3ch.SetDeviceAttribute("DataRate",StringValue("10Mbps"));
	n5n3ch.SetChannelAttribute("Delay",StringValue("2ms"));
	NetDeviceContainer d5d3 = n5n3ch.Install(n5n3);
	ipv4.SetBase("10.1.4.0","255.255.255.0");
	Ipv4InterfaceContainer i5i3 = ipv4.Assign(d5d3);

	//n2n3
	PointToPointHelper n2n3ch;
	n2n3ch.SetDeviceAttribute("DataRate",StringValue("15Mbps"));
	n2n3ch.SetChannelAttribute("Delay",StringValue("2ms"));
	NetDeviceContainer d2d3 = n2n3ch.Install(n2n3);
	ipv4.SetBase("10.1.5.0","255.255.255.0");
	Ipv4InterfaceContainer i2i3 = ipv4.Assign(d2d3);

	 Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

	 // Create the OnOff application to send UDP datagrams of size
	   // 210 bytes at a rate of 448 Kb/s
	   NS_LOG_INFO ("Create Applications.");
	   uint16_t port = 9;   // Discard port (RFC 863)
	   OnOffHelper onoff ("ns3::UdpSocketFactory",
	                      Address (InetSocketAddress (i4i3.GetAddress (0), port)));
	   onoff.SetConstantRate (DataRate ("10Mbps"));
	   ApplicationContainer apps = onoff.Install (c.Get (0));
	   apps.Start (Seconds (1.0));
	   apps.Stop (Seconds (10.0));

	   // Create a packet sink to receive these packets
	   PacketSinkHelper sink ("ns3::UdpSocketFactory",
	                          Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
	   apps = sink.Install (c.Get (4));
	   apps.Start (Seconds (1.0));
	   apps.Stop (Seconds (10.0));

	   // Create a similar flow from n4 to n1, starting at time 1.1 seconds
	   onoff.SetAttribute ("Remote",
	                       AddressValue (InetSocketAddress (i4i3.GetAddress (0), port)));
	   apps = onoff.Install (c.Get (1));
	   apps.Start (Seconds (1.0));
	   apps.Stop (Seconds (10.0));

//	   // Create a packet sink to receive these packets
//	   apps = sink.Install (c.Get (4));
//	   apps.Start (Seconds (1.0));
//	   apps.Stop (Seconds (10.0));

	   AnimationInterface anim ("detnet_1.xml");
	   anim.SetConstantPosition(c.Get(0),10,25);
	   anim.SetConstantPosition(c.Get(1),10,75);
	   anim.SetConstantPosition(c.Get(2),30,50);
	   anim.SetConstantPosition(c.Get(3),50,50);
	   anim.SetConstantPosition(c.Get(4),70,25);
	   anim.SetConstantPosition(c.Get(5),70,75);

	   //anim.EnableIpv4RouteTracking("rout.xml",NanoSeconds(2000),NanoSeconds(11000),Seconds(1));
	   Simulator::Stop (Seconds (11));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}



