/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Andrea Sacco
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Andrea Sacco <andrea.sacco85@gmail.com>
 */


/**
 * \file uan-animation.cc
 * \ingroup uan
 * 
 * This example showcases the "CW-MAC" described in System Design Considerations for Undersea Networks
 * article in the IEEE Journal on Selected Areas of Communications 2008 by Nathan Parrish, Leonard Tracy
 * and Sumit Roy.  The MAC protocol is implemented in the class UanMacCw.  CW-MAC is similar in nature
 * to the IEEE 802.11 DCF with a constant backoff window.  It requires two parameters to be set,
 * the slot time and the contention window size.  The contention window size is the backoff window
 * size in slots, and the slot time is the duration of each slot.  These parameters should be set
 * according to the overall network size, internode spacing and the number of nodes in the network.
 * 
 * This example deploys nodes randomly (according to RNG seed of course) in a finite square region with
 * the X and Y coordinates of the nodes distributed uniformly.  The CW parameter is varied throughout
 * the simulation in order to show the variation in throughput with respect to changes in CW.
 */

#include "uan-animation.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include "ns3/applications-module.h"

#include <fstream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("UanCwExample");

NetAnimExperiment::NetAnimExperiment () 
  : m_numNodes (15),
    m_dataRate (80),
    m_depth (70),
    m_boundary (500),
    m_packetSize (32),
    m_bytesTotal (0),
    m_cwMin (10),
    m_cwMax (400),
    m_cwStep (10),
    m_avgs (3),
    m_slotTime (Seconds (0.2)),
    m_simTime (Seconds (1000))
{
}

/*
This method is important to reseting the datas used in the calculation of the throughtput
*/
void
NetAnimExperiment::ResetData ()
{
  NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << "  Resetting data");
  m_throughputs.push_back (m_bytesTotal * 8.0 / m_simTime.GetSeconds ()); // Calculation of avarage throughtput
  m_bytesTotal = 0; // Reset data
}

/*
This method increments tha contention window and calculates the avarage throughtput
*/
void
NetAnimExperiment::IncrementCw (uint32_t cw)
{
  NS_ASSERT (m_throughputs.size () == m_avgs); // m_avgs define how many time the throughtput have to be calculated and storaged, it must have the same size of throughtpur vector

  double avgThroughput = 0.0;
  for (uint32_t i=0; i<m_avgs; i++)
    {
      avgThroughput += m_throughputs[i];
    }
  avgThroughput /= m_avgs; // Calculation of avarage throughtput
  m_throughputs.clear (); // Reset the list of calculated throughtputs

  Config::Set ("/NodeList/*/DeviceList/*/Mac/CW", UintegerValue (cw + m_cwStep)); // Contention Window incrementation

  SeedManager::SetRun (SeedManager::GetRun () + 1);

  NS_LOG_DEBUG ("Average for cw=" << cw << " over " << m_avgs << " runs: " << avgThroughput);
}

/*
This method updates the nodes positions
*/
void
NetAnimExperiment::UpdatePositions (NodeContainer &nodes)
{

  NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Updating positions");
  NodeContainer::Iterator it = nodes.Begin ();
  Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
  uv->SetAttribute ("Min", DoubleValue (0.0));
  uv->SetAttribute ("Max", DoubleValue (m_boundary));
  for (; it != nodes.End (); it++)
    {
      Ptr<MobilityModel> mp = (*it)->GetObject<MobilityModel> ();
      mp->SetPosition (Vector (uv->GetValue (), uv->GetValue (), 70.0));
    }
}

/*
This method acumulates the throughtput to calculte the avarage throughtput 
*/
void
NetAnimExperiment::ReceivePacket (Ptr<Socket> socket)
{
  Ptr<Packet> packet;

  while ((packet = socket->Recv ()))
    {
      m_bytesTotal += packet->GetSize ();
    }
  packet = 0;
}

void
NetAnimExperiment::Run (UanHelper &uan)
{
  uan.SetMac ("ns3::UanMacCw", "CW", UintegerValue (m_cwMin), "SlotTime", TimeValue (m_slotTime)); //Setting the MAC Protocol
  /*------------Nodes Creation---------------*/
  NodeContainer nc = NodeContainer ();
  NodeContainer sink = NodeContainer ();
  nc.Create (m_numNodes);
  sink.Create (1);
  /*------------Nodes Creation---------------*/

  /*????*/
  PacketSocketHelper socketHelper;
  socketHelper.Install (nc);
  socketHelper.Install (sink);
/*Define the propagation model of the channel*/
#ifdef UAN_PROP_BH_INSTALLED
  Ptr<UanPropModelBh> prop = CreateObjectWithAttributes<UanPropModelBh> ("ConfigFile", StringValue ("exbhconfig.cfg"));
#else 
  Ptr<UanPropModelIdeal> prop = CreateObjectWithAttributes<UanPropModelIdeal> ();
#endif //UAN_PROP_BH_INSTALLED
  Ptr<UanChannel> channel = CreateObjectWithAttributes<UanChannel> ("PropagationModel", PointerValue (prop));

  //Create net device and nodes with UanHelper
  NetDeviceContainer devices = uan.Install (nc, channel);
  NetDeviceContainer sinkdev = uan.Install (sink, channel);

  MobilityHelper mobility; //Instantiates an object to handle the mobility of the nodes
  Ptr<ListPositionAllocator> pos = CreateObject<ListPositionAllocator> (); //Create a pointer to an object?????

  {
    /*-------Create a vector with nodes' random positions??????----------*/
    Ptr<UniformRandomVariable> urv = CreateObject<UniformRandomVariable> (); //Create a pointer to an object?????
    urv->SetAttribute ("Min", DoubleValue (0.0));//Set a bottom limit to the minimal range
    urv->SetAttribute ("Max", DoubleValue (m_boundary));//Set a upper limit to the maximal range 
    /*-------Create a vector with nodes' random positions??????----------*/
    pos->Add (Vector (m_boundary / 2.0, m_boundary / 2.0, m_depth)); //Set the initial sink node's position
    double rsum = 0;

    double minr = 2 * m_boundary;
    for (uint32_t i = 0; i < m_numNodes; i++)
      {
        double x = urv->GetValue ();
        double y = urv->GetValue ();
        double newr = std::sqrt ((x - m_boundary / 2.0) * (x - m_boundary / 2.0)
                            + (y - m_boundary / 2.0) * (y - m_boundary / 2.0));//Calculate the new range
        rsum += newr; //Acumulate the range to mean range calculation
        minr = std::min (minr, newr); //Storage a minimal range
        pos->Add (Vector (x, y, m_depth));//Update de sink node's position
      }
    NS_LOG_DEBUG ("Mean range from gateway: " << rsum / m_numNodes
                                              << "    min. range " << minr);

    mobility.SetPositionAllocator (pos);//Set the nodes' position
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");//Set the mobility model of the sink node
    mobility.Install (sink);//Install the mobility model with the set position into the sink node

    NS_LOG_DEBUG ("Position of sink: "
                  << sink.Get (0)->GetObject<MobilityModel> ()->GetPosition ());
    mobility.Install (nc);//Instal the mobility model with the set positions into the others nodes

    PacketSocketAddress socket;
    socket.SetSingleDevice (sinkdev.Get (0)->GetIfIndex ());
    socket.SetPhysicalAddress (sinkdev.Get (0)->GetAddress ());//Set the MAC address of the sink
    socket.SetProtocol (0);

    OnOffHelper app ("ns3::PacketSocketFactory", Address (socket));
    app.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"));
    app.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"));
    app.SetAttribute ("DataRate", DataRateValue (m_dataRate));
    app.SetAttribute ("PacketSize", UintegerValue (m_packetSize));

    ApplicationContainer apps = app.Install (nc);//Install the application on the nodes
    apps.Start (Seconds (0.5));//Set the start of the application
    Time nextEvent = Seconds (0.5);//Set the time step of the events

    /*------------Schedule the simulation events------------*/
    for (uint32_t cw = m_cwMin; cw <= m_cwMax; cw += m_cwStep)
      {

        for (uint32_t an = 0; an < m_avgs; an++)
          {
            nextEvent += m_simTime;
            Simulator::Schedule (nextEvent, &NetAnimExperiment::ResetData, this);
            Simulator::Schedule (nextEvent, &NetAnimExperiment::UpdatePositions, this, nc);
          }
        Simulator::Schedule (nextEvent, &NetAnimExperiment::IncrementCw, this, cw);
      }
    /*------------Schedule the simulation events------------*/

    apps.Stop (nextEvent + m_simTime);//Set the stop of the application

    Ptr<Node> sinkNode = sink.Get (0);
    TypeId psfid = TypeId::LookupByName ("ns3::PacketSocketFactory");
    if (sinkNode->GetObject<SocketFactory> (psfid) == 0)
      {
        Ptr<PacketSocketFactory> psf = CreateObject<PacketSocketFactory> ();
        sinkNode->AggregateObject (psf);
      }
    Ptr<Socket> sinkSocket = Socket::CreateSocket (sinkNode, psfid);
    sinkSocket->Bind (socket);
    sinkSocket->SetRecvCallback (MakeCallback (&NetAnimExperiment::ReceivePacket, this));

    m_bytesTotal = 0;

    std::string traceFileName = "uan-animation.xml";
    AnimationInterface anim(traceFileName.c_str ());

    Simulator::Run ();
    sinkNode = 0;
    sinkSocket = 0;
    pos = 0;
    channel = 0;
    prop = 0;
    for (uint32_t i=0; i < nc.GetN (); i++)
      {
        nc.Get (i) = 0;
      }
    for (uint32_t i=0; i < sink.GetN (); i++)
      {
        sink.Get (i) = 0;
      }

    for (uint32_t i=0; i < devices.GetN (); i++)
      {
        devices.Get (i) = 0;
      }
    for (uint32_t i=0; i < sinkdev.GetN (); i++)
      {
        sinkdev.Get (i) = 0;
      }

    Simulator::Destroy ();//Destroy the simulation
  }
}

int
main (int argc, char **argv)
{

  LogComponentEnable ("UanCwExample", LOG_LEVEL_ALL);
  //LogComponentEnable ("AnimationInterface", LOG_LEVEL_ALL);

  NetAnimExperiment exp;

  std::string perModel = "ns3::UanPhyPerGenDefault";//Define the PER model
  std::string sinrModel = "ns3::UanPhyCalcSinrDefault";//Define the SINR model
  /*--------------------Define the simulation parameters------------------------*/
  CommandLine cmd;
  cmd.AddValue ("NumNodes", "Number of transmitting nodes", exp.m_numNodes);
  cmd.AddValue ("Depth", "Depth of transmitting and sink nodes", exp.m_depth);
  cmd.AddValue ("RegionSize", "Size of boundary in meters", exp.m_boundary);
  cmd.AddValue ("PacketSize", "Generated packet size in bytes", exp.m_packetSize);
  cmd.AddValue ("DataRate", "DataRate in bps", exp.m_dataRate);
  cmd.AddValue ("CwMin", "Min CW to simulate", exp.m_cwMin);
  cmd.AddValue ("CwMax", "Max CW to simulate", exp.m_cwMax);
  cmd.AddValue ("SlotTime", "Slot time duration", exp.m_slotTime);
  cmd.AddValue ("Averages", "Number of topologies to test for each cw point", exp.m_avgs);
  cmd.AddValue ("PerModel", "PER model name", perModel);
  cmd.AddValue ("SinrModel", "SINR model name", sinrModel);
  cmd.Parse (argc, argv);//Parse the simulation parameters to the NS3
 /*--------------------Define the simulation parameters------------------------*/
  ObjectFactory obf;
  obf.SetTypeId (perModel);//Set the PER model
  Ptr<UanPhyPer> per = obf.Create<UanPhyPer> ();
  obf.SetTypeId (sinrModel);//Set the SINR model
  Ptr<UanPhyCalcSinr> sinr = obf.Create<UanPhyCalcSinr> ();

  /*-------------------Define the transmition mode------------------*/
  UanHelper uan;
  UanTxMode mode;
  mode = UanTxModeFactory::CreateMode (UanTxMode::FSK, exp.m_dataRate,
                                       exp.m_dataRate, 12000,
                                       exp.m_dataRate, 2,
                                       "Default mode");
  /*-------------------Define the transmition mode------------------*/

  UanModesList myModes;//Instantiates an object to the Uan Transmition Modes List
  myModes.AppendMode (mode);//Add the defined transmition mode on the Uan Transmition Mode List

  /*-----------------Set the PHY Layer-----------------------*/
  uan.SetPhy ("ns3::UanPhyGen",
              "PerModel", PointerValue (per),
              "SinrModel", PointerValue (sinr),
              "SupportedModes", UanModesListValue (myModes));
  /*-----------------Set the PHY Layer-----------------------*/

  exp.Run (uan);//Run the simulation

  per = 0;//Clean the PER value
  sinr = 0;//Clean the SINR value

}

