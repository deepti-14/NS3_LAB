#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");
int main (int argc, char *argv[])
{
CommandLine cmd;
cmd.Parse (argc, argv);
Time::SetResolution (Time::NS);
bool tracing = true;
cmd.AddValue("tracing", "Enable pcap tracing", tracing);
LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_ALL);
LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_ALL);
NodeContainer nodes;
nodes.Create (5);
PointToPointHelper pointToPoint;
pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("500Mbps"));
pointToPoint.SetChannelAttribute ("Delay", StringValue ("20ms"));
NetDeviceContainer devices1, devices2, devices3, devices4;
NodeContainer connection1 = {nodes.Get(1), nodes.Get(0)};
NodeContainer connection2 = {nodes.Get(2), nodes.Get(0)};
NodeContainer connection3 = {nodes.Get(3), nodes.Get(0)};
NodeContainer connection4 = {nodes.Get(4), nodes.Get(0)};
devices1 = pointToPoint.Install (connection1);
devices2 = pointToPoint.Install (connection2);
devices3 = pointToPoint.Install (connection3);
devices4 = pointToPoint.Install (connection4);
InternetStackHelper stack;
stack.Install (nodes);

Ipv4AddressHelper address1, address2, address3, address4;

address1.SetBase ("10.1.1.0", "255.255.255.0");
address2.SetBase ("10.1.2.0", "255.255.255.0");
address3.SetBase ("10.1.3.0", "255.255.255.0");
address4.SetBase ("10.1.4.0", "255.255.255.0");
Ipv4InterfaceContainer interfaces1 = address1.Assign (devices1);
Ipv4InterfaceContainer interfaces2 = address2.Assign (devices2);
Ipv4InterfaceContainer interfaces3 = address3.Assign (devices3);
Ipv4InterfaceContainer interfaces4 = address4.Assign (devices4);
UdpEchoServerHelper echoServer (9);
ApplicationContainer serverApps = echoServer.Install (nodes.Get (0));
serverApps.Start (Seconds (1.0));
serverApps.Stop (Seconds (10.0));
UdpEchoClientHelper echoClient1 (interfaces1.GetAddress (1), 9);
UdpEchoClientHelper echoClient2 (interfaces2.GetAddress (1), 9);
UdpEchoClientHelper echoClient3 (interfaces3.GetAddress (1), 9);
UdpEchoClientHelper echoClient4 (interfaces4.GetAddress (1), 9);
echoClient1.SetAttribute ("MaxPackets", UintegerValue (1));
echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));
echoClient2.SetAttribute ("MaxPackets", UintegerValue (1));
echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));
echoClient3.SetAttribute ("MaxPackets", UintegerValue (1));
echoClient3.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
echoClient3.SetAttribute ("PacketSize", UintegerValue (1024));
echoClient4.SetAttribute ("MaxPackets", UintegerValue (1));
echoClient4.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
echoClient4.SetAttribute ("PacketSize", UintegerValue (1024));
ApplicationContainer clientApps1 = echoClient1.Install (nodes.Get (1));
ApplicationContainer clientApps2 = echoClient2.Install (nodes.Get (2));
ApplicationContainer clientApps3 = echoClient3.Install (nodes.Get (3));
ApplicationContainer clientApps4 = echoClient4.Install (nodes.Get (4));
clientApps1.Start (Seconds (2.0));
clientApps1.Stop (Seconds (10.0));
clientApps2.Start (Seconds (2.0));
clientApps2.Stop (Seconds (10.0));
clientApps3.Start (Seconds (2.0));
clientApps3.Stop (Seconds (10.0));
clientApps4.Start (Seconds (2.0));
clientApps4.Stop (Seconds (10.0));
AnimationInterface anim("first.xml");

anim.SetConstantPosition(nodes.Get(0), 15.0, 10.0);
anim.SetConstantPosition(nodes.Get(1), 14.0, 20.0);
anim.SetConstantPosition(nodes.Get(2), 10.0, 10.0);
anim.SetConstantPosition(nodes.Get(3), 25.0, 60.0);
anim.SetConstantPosition(nodes.Get(4), 35.0, 40.0);
AsciiTraceHelper ascii;
pointToPoint.EnableAsciiAll(ascii.CreateFileStream("first.tr"));
if(tracing){
pointToPoint.EnablePcapAll("first");
}
Simulator::Run ();
Simulator::Destroy ();
return 0;
}
