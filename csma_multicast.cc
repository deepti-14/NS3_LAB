    #include <iostream>
    #include <fstream>
    
    #include "ns3/core-module.h"
    #include "ns3/network-module.h"
    #include "ns3/csma-module.h"
    #include "ns3/applications-module.h"
    #include "ns3/internet-module.h"
    #include "ns3/netanim-module.h"
    using namespace ns3;
    
    NS_LOG_COMPONENT_DEFINE ("CsmaMulticastExample");
    
    int 
    main (int argc, char *argv[])
    {
      Config::SetDefault ("ns3::CsmaNetDevice::EncapsulationMode", StringValue ("Dix"));
      // run-time, via command-line arguments
      CommandLine cmd;
      cmd.Parse (argc, argv);
    
      NS_LOG_INFO ("Create nodes.");
      NodeContainer c;
      c.Create (5);
      // We will later want two subcontainers of these nodes, for the two LANs
      NodeContainer c0 = NodeContainer (c.Get (0), c.Get (1), c.Get (2));
      NodeContainer c1 = NodeContainer (c.Get (2), c.Get (3), c.Get (4));
    
      NS_LOG_INFO ("Build Topology.");
      CsmaHelper csma;
      csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate (5000000)));
      csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
     
      // We will use these NetDevice containers later, for IP addressing
      NetDeviceContainer nd0 = csma.Install (c0);  // First LAN
      NetDeviceContainer nd1 = csma.Install (c1);  // Second LAN
    
      NS_LOG_INFO ("Add IP Stack.");
      InternetStackHelper internet;
      internet.Install (c);
    
      NS_LOG_INFO ("Assign IP Addresses.");
      Ipv4AddressHelper ipv4Addr;
      ipv4Addr.SetBase ("10.1.1.0", "255.255.255.0");
      ipv4Addr.Assign (nd0);
      ipv4Addr.SetBase ("10.1.2.0", "255.255.255.0");
     ipv4Addr.Assign (nd1);
    
      NS_LOG_INFO ("Configure multicasting.");
   
     Ipv4Address multicastSource ("10.1.1.1");
     Ipv4Address multicastGroup ("225.1.2.4");
  
    Ipv4StaticRoutingHelper multicast;
   
     // 1) Configure a (static) multicast route on node n2 (multicastRouter)
     Ptr<Node> multicastRouter = c.Get (2);  // The node in question
     Ptr<NetDevice> inputIf = nd0.Get (2);  // The input NetDevice
     NetDeviceContainer outputDevices;  // A container of output NetDevices
     outputDevices.Add (nd1.Get (0));  // (we only need one NetDevice here)
   
     multicast.AddMulticastRoute (multicastRouter, multicastSource, 
                                  multicastGroup, inputIf, outputDevices);
   
     Ptr<Node> sender = c.Get (0);
     Ptr<NetDevice> senderIf = nd0.Get (0);
     multicast.SetDefaultMulticastRoute (sender, senderIf);
 
     NS_LOG_INFO ("Create Applications.");
   
     uint16_t multicastPort = 9;   // Discard port (RFC 863)
   
     // Configure a multicast packet generator that generates a packet
     // every few seconds
     OnOffHelper onoff ("ns3::UdpSocketFactory", 
                        Address (InetSocketAddress (multicastGroup, multicastPort)));
     onoff.SetConstantRate (DataRate ("255b/s"));
     onoff.SetAttribute ("PacketSize", UintegerValue (128));
     ApplicationContainer srcC = onoff.Install (c0.Get (0));
   
     //
     // Tell the application when to start and stop.
     //
     srcC.Start (Seconds (1.));
     srcC.Stop (Seconds (10.));
   
     // Create an optional packet sink to receive these packets
     PacketSinkHelper sink ("ns3::UdpSocketFactory",
                            InetSocketAddress (Ipv4Address::GetAny (), multicastPort));
   
     ApplicationContainer sinkC = sink.Install (c1.Get (2)); // Node n4 
     // Start the sink
     sinkC.Start (Seconds (1.0));
     sinkC.Stop (Seconds (10.0));
   
     NS_LOG_INFO ("Configure Tracing.");
     AnimationInterface anim("csma-multicast.xml");
     AsciiTraceHelper ascii;
     csma.EnableAsciiAll (ascii.CreateFileStream ("csma-multicast.tr"));
   
     // Also configure some tcpdump traces; each interface will be traced.
     // The output files will be named:
     //     csma-multicast-<nodeId>-<interfaceId>.pcap
     // and can be read by the "tcpdump -r" command (use "-tt" option to
     // display timestamps correctly)
     csma.EnablePcapAll ("csma-multicast", false);
   
     //
     // Now, do the actual simulation.
     //
     NS_LOG_INFO ("Run Simulation.");
     Simulator::Run ();
     Simulator::Destroy ();
     NS_LOG_INFO ("Done.");
   }
