#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");

int
main (int argc, char *argv[])
{
bool verbose = true;
uint32_t nCsma = 5;
uint32_t nWifi = 5;

CommandLine cmd;
cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

cmd.Parse (argc,argv);

if (verbose)
{
LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
}

NodeContainer p2pNodes;
p2pNodes.Create (2);

PointToPointHelper pointToPoint;
pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

NetDeviceContainer p2pDevices;
p2pDevices = pointToPoint.Install (p2pNodes);

NodeContainer csmaNodes;
csmaNodes.Add (p2pNodes.Get (1));
csmaNodes.Create (nCsma);

CsmaHelper csma;
csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

NetDeviceContainer csmaDevices;
csmaDevices = csma.Install (csmaNodes);

NodeContainer wifiStaNodes;
wifiStaNodes.Create (nWifi); // Creates wifi nodes
NodeContainer wifiApNode = p2pNodes.Get (0); // Set p2pNodes 0 as WiFi`s AP

YansWifiChannelHelper channel = YansWifiChannelHelper::Default (); // Path Loss 46.677 dB/m
YansWifiPhyHelper phy = YansWifiPhyHelper::Default (); // PHY Layer implementation

phy.SetChannel (channel.Create ());

WifiHelper wifi = WifiHelper::Default ();
wifi.SetRemoteStationManager ("ns3::AarfWifiManager"); // Set AARF as the Rate Control Algorithm

NqosWifiMacHelper mac = NqosWifiMacHelper::Default (); // Create MAC Layer Assistent as default. It sets as MAC Non-QoS

Ssid ssid = Ssid ("ns-3-ssid"); // Set SSID (Set Service ID). Identifies network`s name
mac.SetType ("ns3::StaWifiMac", "Ssid", SsidValue (ssid),
  "ActiveProbing", BooleanValue (false));
// Set the type of MAC layer
// MACs won't send polling requests

NetDeviceContainer staDevices;
staDevices = wifi.Install (phy, mac, wifiStaNodes); // Creates Sta NetDevices

mac.SetType ("ns3::ApWifiMac", "Ssid", SsidValue (ssid));
// Set AP's SSID
// Set type of AP's MAC layer

NetDeviceContainer apDevices;
apDevices = wifi.Install (phy, mac, wifiApNode); // Creates AP NetDevice. It has the same PHY configuration of STAs NetDevices

MobilityHelper mobility;
mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
  "MinX", DoubleValue (0.0),
  "MinY", DoubleValue (0.0),
  "DeltaX", DoubleValue (5.0),
  "DeltaY", DoubleValue (10.0)
  ); // Build the grid that the STAs will be

mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
    "Bounds", RectangleValue (Rectangle (-50,50,-50,50))); // Set STAs mobility

mobility.Install (wifiStaNodes);

mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
mobility.Install (wifiApNode); // Set a static mobility model to AP

InternetStackHelper stack;
stack.Install (csmaNodes);
stack.Install (wifiApNode);
stack.Install (wifiStaNodes);

Ipv4AddressHelper address;

address.SetBase ("10.1.1.0", "255.255.255.0");
Ipv4InterfaceContainer p2pInterfaces;
p2pInterfaces = address.Assign (p2pDevices);

address.SetBase ("10.1.2.0","255.255.255.0");
Ipv4InterfaceContainer csmaInterfaces;
csmaInterfaces = address.Assign (csmaDevices);

address.SetBase ("192.168.1.0", "255.255.255.0");
address.Assign (staDevices);
address.Assign (apDevices);

UdpEchoServerHelper echoServer (7);

ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma));
serverApps.Start (Seconds (1.0));
serverApps.Stop (Seconds (26.0));

UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), 7);
UdpEchoClientHelper echoClient2 (csmaInterfaces.GetAddress (nCsma), 7);
UdpEchoClientHelper echoClient3 (csmaInterfaces.GetAddress (nCsma), 7);

echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
echoClient2.SetAttribute ("MaxPackets", UintegerValue (1));
echoClient3.SetAttribute ("MaxPackets", UintegerValue (1));

echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
echoClient3.SetAttribute ("Interval", TimeValue (Seconds (1.0)));

echoClient.SetAttribute ("PacketSize", UintegerValue (512));
echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));
echoClient3.SetAttribute ("PacketSize", UintegerValue (2048));

ApplicationContainer clientApps = echoClient.Install (wifiStaNodes.Get (nWifi -1));
ApplicationContainer clientApps2 = echoClient2.Install (wifiStaNodes.Get (nWifi -2));
ApplicationContainer clientApps3 = echoClient3.Install (wifiStaNodes.Get (nWifi -3));

clientApps.Start (Seconds (5.0));
clientApps.Stop (Seconds (26.0));

clientApps2.Start (Seconds (10.0));
clientApps2.Stop (Seconds (26.0));

clientApps3.Start (Seconds (15.0));
clientApps3.Stop (Seconds (26.0));

Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

Simulator::Stop (Seconds (26.0));

pointToPoint.EnablePcapAll ("challenge_3_p2p");
phy.EnablePcap ("challenge_3_wifi", apDevices.Get (0));
csma.EnablePcap ("challenge_3_csma",csmaDevices.Get (0), true);

Simulator::Run ();
Simulator::Destroy ();

return 0;
}
