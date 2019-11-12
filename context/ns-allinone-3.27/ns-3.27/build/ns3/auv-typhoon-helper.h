/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 Universita' di Firenze, Italy
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
 * Authors: Matteo Franchi <teofr89@hotmail.it>
 *          Tommaso Pecorella <tommaso.pecorella@unifi.it>
 */

#ifndef AUV_TYPHOON_HELPER_H
#define AUV_TYPHOON_HELPER_H

#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/names.h"
#include "ns3/auv-waypoint-mobility-model.h"

namespace ns3 {

/**
 * Install into a node (or set of nodes) the TYPHOON features:
 * - Typhoon energy model
 * - Typhoon energy source
 *
 * Typhoon uses 5 rechargeable batteries LiPo (1.2 kWh) in operating conditions.
 *
 * Moreover,"TraceConnectWithoutContext" calls "NotifyCourseChanged" (in typhoon-energy-model) every time
 * a change in the speed vector occurs.
 */
class AuvTyphoonHelper
{
public:
  AuvTyphoonHelper ();


  /**
   * Install the Typhoon features into a set of nodes
   *
   * \param c NodeContainer where to install the features
   */
  void Install (NodeContainer c) const;

  /**
   * Install the Typhoon features into a single node
   *
   * \param nodeName Name of the node where to install the features
   */
  void Install (std::string nodeName) const;

  /**
   * Install the Typhoon features into a single node
   *
   * \param node Pointer of the node where to install the features
   */
  void Install (Ptr<Node> node) const;



};



} // namespace ns3

#endif /* AUV_TYPHOON_HELPER_H */
