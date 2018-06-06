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

#ifndef UAN_ENERGY_AUV_H_
#define UAN_ENERGY_AUV_H_

/**
 * This is a comprehensive example where all the project's components are used.
 * We setup two nodes, one fixed surface gateway and a moving Seaglider AUV,
 * both equipped with an acoustic modem.
 *
 * Using the waypoint mobility model with an underlying GliderMobilityModel,
 * we make the glider descend to -1000 meters and then emerge to the water surface.
 *
 * The AUV sends a generic 17-bytes packet every 10 seconds during the navigation
 * process.
 * The gateway receives the packets and stores the total bytes amount.
 *
 * During the simulation the AUV consumes energy for navigation and packets sending.
 * The energy drained is subtracted from the main energy source.
 * The surface gateway instead consumes energy only receiving packets.
 *
 * At the end of the simulation are printed out the energy consumptions of the two
 * nodes and the networking stats.
 */

using namespace ns3;

class UanEnergyAuv
{
public:
  UanEnergyAuv ();
  ~UanEnergyAuv ();

  bool RxPacket (Ptr<NetDevice> dev, Ptr<const Packet> pkt, uint16_t mode, const Address &sender);
  void SendOnePacket (Ptr<Node> node);

  void PrintStats ();

  bool Run (void);

  uint32_t m_bytesRx;
  uint32_t m_sentPackets;
  Ptr<Node> m_auv;
  Ptr<Node> m_gateway;
};

#endif /* UAN_ENERGY_AUV_H_ */
