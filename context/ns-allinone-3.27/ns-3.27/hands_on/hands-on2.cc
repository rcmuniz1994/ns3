#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SecondScriptExample");

int
main (int argc, char *argv[])
{
bool verbose = true;
uint32_t nCsma = 3;

CommandLine cmd;
cmd.AddValue ("nCsma","Number of \"extra\" CSMA nodes/devices",nCsma);
cmd.AddValue ("verbose","Tell echo applications to log if true",verbose);

cmd.Parse(argc,argv);


Time::SetResolution (Time::NS);
LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

NodeContainer p2pNodes;
p2pNodes.Create (2); // Point-to-point nodes criation

NodeContainer csmaNodes;
csmaNodes.Add (p2pNodes.Get (1)); // Adds node 01 to CSMA network
csmaNodes.Create (nCsma); // CSMA nodes criation

PointToPointHelper pointToPoint;
pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

NetDeviceContainer p2pDevices;
p2pDevices = pointToPoint.Install (p2pNodes); // Install point-to-point nodes' NetDevices

CsmaHelper csma;
csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

NetDeviceContainer csmaDevices;
csmaDevices = csma.Install (csmaNodes); // Install Csma nodes' NetDevices

InternetStackHelper stack;
stack.Install (p2pNodes.Get (0)); // Install internet stack in point-to-point nodes
stack.Install (csmaNodes); // Install internet stack in CSMA nodes

Ipv4AddressHelper address;
address.SetBase ("10.1.1.0","255.255.255.0"); // Set ip base to p2p network
Ipv4InterfaceContainer p2pInterfaces; // Interface creation
p2pInterfaces = address.Assign (p2pDevices); // IP attribuition

address.SetBase ("10.1.2.0","255.255.255.0");
Ipv4InterfaceContainer csmaInterfaces;
csmaInterfaces = address.Assign (csmaDevices);

UdpEchoServerHelper echoServer (9); // Set application port

ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get(nCsma)); // Install the application into third csmaNode
serverApps.Start (Seconds (1.0)); // Start application time
serverApps.Stop (Seconds (10.0)); // Stop application time

UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), 9); // Set port 9 and get server's interfaces to client send the packets
echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

ApplicationContainer clientApps = echoClient.Install (p2pNodes.Get (0)); // Install the application into the first p2pNode
clientApps.Start (Seconds (2.0));
clientApps.Stop (Seconds (10.0));

Ipv4GlobalRoutingHelper::PopulateRoutingTables();

pointToPoint.EnablePcapAll ("second"); //Enable pcap to point-to-point network
csma.EnablePcap ("second",csmaDevices.Get (1),true); // Enable pcap to csma network. The third argument means "promiscuous mode enable".

Simulator::Run ();
Simulator::Destroy ();

return 0;

}
