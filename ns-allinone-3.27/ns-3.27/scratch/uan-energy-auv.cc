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

#include "ns3/uan-net-device.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "auv-glider-helper.h"
#include "ns3/uan-helper.h"
#include "ns3/basic-energy-source-helper.h"
#include "ns3/acoustic-modem-energy-model.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/log.h"
#include "ns3/uan-channel.h"
#include "ns3/uan-noise-model-default.h"
#include "ns3/uan-prop-model-ideal.h"
#include "uan-energy-auv.h"

  //------EDITEI AQUI - Rodrigo ------//
#include "ns3/energy-source.h"
#include "ns3/device-energy-model.h"
#include "ns3/glider-energy-model.h"
#include "ns3/glider-constraints.h"
#include "ns3/auv-waypoint-mobility-model.h"
  //------EDITEI AQUI - Rodrigo ------//

using namespace ns3;

UanEnergyAuv::UanEnergyAuv ()
  : m_bytesRx (0),
    m_sentPackets (0)
{
}

UanEnergyAuv::~UanEnergyAuv ()
{
  m_auv = 0;
  m_gateway = 0;
}

void
UanEnergyAuv::SendOnePacket (Ptr<Node> node)
{
  // create an empty 17 bytes packet
  Ptr<Packet> pkt = Create<Packet> (17);
  // send the packet in broadcast
  Ptr<UanNetDevice> dev = node->GetDevice (0)->GetObject<UanNetDevice> ();
  dev->Send (pkt, dev->GetBroadcast (), 0);
  // increase the sent packets number
  ++m_sentPackets;

  std::cout << "PACKET SEND" << std::endl;

  Simulator::Schedule (Seconds (10.0),
                       &UanEnergyAuv::SendOnePacket,
                       this,
                       node);
}

bool
UanEnergyAuv::RxPacket (Ptr<NetDevice> dev, Ptr<const Packet> pkt, uint16_t mode, const Address &sender)
{
  // increase the total bytes received
  m_bytesRx += pkt->GetSize ();
  std::cout << "PACKET RECEIVED" << std::endl;

  return true;
}
bool
UanEnergyAuv::Run ()
{
//------------- EDITEI AQUI - RODRIGO ---------------//
  Ptr<DeviceEnergyModel> source1 = 0;
  Ptr<DeviceEnergyModel> source2 = 0;
  Time StopTime = Seconds (20000);
//------------- EDITEI AQUI - RODRIGO ---------------//

  // create a generic node
  Ptr<Node> m_auv = CreateObject<Node> ();

  // create a default underwater channel
  Ptr<UanChannel> channel = CreateObject<UanChannel> ();
  Ptr<UanNoiseModelDefault> noise = CreateObject<UanNoiseModelDefault> ();
  channel->SetPropagationModel (CreateObject<UanPropModelIdeal> ());
  channel->SetNoiseModel (noise);

  // install the underwater communication stack
  UanHelper uan;
  uan.Install (m_auv, channel);

  // install the glider components
  AuvGliderHelper gh;
  gh.Install (m_auv);

  //------EDITEI AQUI - RODRIGO ------//
  source1 = m_auv->GetObject<EnergySourceContainer> ()->Get (0)->FindDeviceEnergyModels ("ns3::GliderEnergyModel").Get(0);
  source2 = m_auv->GetObject<EnergySourceContainer> ()->Get (1)->FindDeviceEnergyModels ("ns3::AcousticModemEnergyModel").Get(0);
  // move the vehicle somewhere
  Ptr<AuvWaypointMobilityModel> mob = m_auv->GetObject <AuvWaypointMobilityModel> ();
  mob->AddWaypoint (Waypoint (Seconds (0), Vector (0,0,0)));
  mob->AddWaypoint (Waypoint (StopTime/2, Vector (45,0,1000)));
  mob->AddWaypoint (Waypoint (StopTime, Vector (90,0,0)));
  //------EDITEI AQUI - RODRIGO ------//

  // Schedule a packet every 10 seconds
  Simulator::ScheduleNow (&UanEnergyAuv::SendOnePacket,
                          this,
                          m_auv);

  // create a gateway node
  m_gateway = CreateObject<Node> ();

  // install the underwater communication stack
  uan.Install (m_gateway, channel);

  // energy source
  BasicEnergySourceHelper eh;
  eh.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (10000000.0));
  eh.Install (m_gateway);

  // mobility model
  Ptr<ConstantPositionMobilityModel> mobility = CreateObject<ConstantPositionMobilityModel> ();
  mobility->SetPosition (Vector (1000,0,0));
  m_gateway->AggregateObject (mobility);

  // micro modem energy model
  Ptr<AcousticModemEnergyModel> umem = CreateObject<AcousticModemEnergyModel> ();
  Ptr<EnergySource> source = m_gateway->GetObject<EnergySourceContainer> ()->Get (0);
  NS_ASSERT (source != 0);
  umem->SetEnergySource (source);
  source->AppendDeviceEnergyModel (umem);

  // set the receive callback
  Ptr<NetDevice> dev = m_gateway->GetDevice (0);
  dev->SetReceiveCallback (MakeCallback (&UanEnergyAuv::RxPacket,
                                         this));

  // run the simulation
  Simulator::Stop (StopTime + Seconds (0.5)); //MUDEI AQUI - RODRIGO
  Simulator::Run ();
  Simulator::Destroy ();

  // print the simulation's statistics

  std::cout << "AUV: sent " << m_sentPackets << " packets" << std::endl;

  std::cout << "Gateway: received " << m_bytesRx << " bytes" << std::endl;

  Ptr<EnergySource> src1 = m_gateway->GetObject<EnergySourceContainer> ()->Get (0);
  std::cout << "Gateway energy consumption: " << src1->GetInitialEnergy () - src1->GetRemainingEnergy () << " J" << std::endl;

//----------- EDITEI AQUI - RODRIGO ------------------//  
  DoubleValue auvBatteryCapacity;
  source1->GetAttribute ("BatteryCapacity", auvBatteryCapacity);

  std::cout << "AUV energy consumption (nav): " << source1->GetTotalEnergyConsumption () << " J" << std::endl;
//----------- EDITEI AQUI - RODRIGO ------------------//

  // COMENTEI AQUI -- PONTEIROS QUE ESTAVAM GERANDO O ERRO -- RODRIGO MUNIZ
  DoubleValue commBatteryCapacity;
  source2->GetAttribute ("TxPowerW", commBatteryCapacity);
  std::cout << "AUV energy consumption (comms): " << source2->GetTotalEnergyConsumption () << " J" << std::endl;

  return false;
}

int
main (int argc, char **argv)
{
  // uncomment to see models log
  // LogComponentEnable("GliderEnergyModel", LOG_LEVEL_ALL);
  // LogComponentEnable("BasicEnergySource", LOG_LEVEL_ALL);
  // LogComponentEnable("AcousticModemEnergyModel", LOG_LEVEL_ALL);

  UanEnergyAuv uan;
  uan.Run ();
 
  return 0;
}
