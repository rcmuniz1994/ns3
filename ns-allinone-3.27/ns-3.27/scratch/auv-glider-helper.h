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

#ifndef AUV_GLIDER_HELPER_H
#define AUV_GLIDER_HELPER_H

#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/names.h"
#include "ns3/auv-waypoint-mobility-model.h"

namespace ns3 {

/**
 * Install into a node (or set of nodes) the SeaGlider features:
 * - SeaGlider energy model
 * - SeaGlider energy source
 *
 * Moreover,"TraceConnectWithoutContext" calls "NotifyCourseChanged" (in glider-energy-model) every time
 * a change in the speed vector occurs.
 */
class AuvGliderHelper
{
public:
  AuvGliderHelper ();


  /**
   * Install the SeaGlider features into a set of nodes
   *
   * \param c NodeContainer where to install the features
   */
  void Install (NodeContainer c) const;

  /**
   * Install the SeaGlider features into a single node
   *
   * \param nodeName Name of the node where to install the features
   */
  void Install (std::string nodeName) const;

  /**
   * Install the SeaGlider features into a single node
   *
   * \param node Pointer of the node where to install the features
   */
  void Install (Ptr<Node> node) const;

};

} // namespace ns3

#endif /* AUV_GLIDER_HELPER_H */
