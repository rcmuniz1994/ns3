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

#ifndef AUV_MOBILITY_HELPER_H
#define AUV_MOBILITY_HELPER_H


#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/position-allocator.h"
#include "ns3/names.h"
#include "ns3/auv-waypoint-mobility-model.h"

namespace ns3 {

/**
 * \ingroup auv
 * \brief Install into a set of nodes an AuvMobilityModel
 *
 * Install into a set of nodes (or single node) the specified AuvMobilityModel.
 * The specific AuvMobilityModel can be specified with the SetType method.
 *
 * The helper also uses a PositionAllocator to set the nodes positions.
 *
 * By default the mobility model is the RemusMobilityModel and the position
 * allocator is a RandomBoxPositionAllocator with X, Y and Z variables set to
 * constant zero.
 */
class AuvMobilityHelper
{
public:
  /**
   * Set the default mobility model (RemusMobilityModel) and the
   * default position allocator (RandomBoxPositionAllocator with
   * constant position in 0,0,0)
   */
  AuvMobilityHelper ();

  /**
   * \param type the AUV mobility model TypeId
   */
  void SetType (std::string type);

  /**
   * Set an attribute of the specified auv mobility model
   *
   * \param name name of the parameter
   * \param value value of the parameter
   */
  void SetAttribute (std::string name, const AttributeValue &value);
  /**
   * Set the position allocator to be used during the install process
   * to give each node an initial position
   *
   * \param allocator the position allocator to be set
   */
  void SetPositionAllocator (Ptr<PositionAllocator> allocator);
  /**
   * Set the position allocator to be used during the install process
   * to give each node an initial position
   *
   * \param type the position allocator TypeId
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   * \param n2 the name of the attribute to set
   * \param v2 the value of the attribute to set
   * \param n3 the name of the attribute to set
   * \param v3 the value of the attribute to set
   * \param n4 the name of the attribute to set
   * \param v4 the value of the attribute to set
   * \param n5 the name of the attribute to set
   * \param v5 the value of the attribute to set
   * \param n6 the name of the attribute to set
   * \param v6 the value of the attribute to set
   * \param n7 the name of the attribute to set
   * \param v7 the value of the attribute to set
   * \param n8 the name of the attribute to set
   * \param v8 the value of the attribute to set
   * \param n9 the name of the attribute to set
   * \param v9 the value of the attribute to set
   */
  void SetPositionAllocator (std::string type,
                             std::string n1, const AttributeValue &v1,
                             std::string n2, const AttributeValue &v2,
                             std::string n3, const AttributeValue &v3,
                             std::string n4, const AttributeValue &v4,
                             std::string n5, const AttributeValue &v5,
                             std::string n6, const AttributeValue &v6,
                             std::string n7, const AttributeValue &v7,
                             std::string n8, const AttributeValue &v8,
                             std::string n9, const AttributeValue &v9);

  /**
   * For each of the input nodes a new mobility model of the specified type
   * is created and attached to the node.
   *
   * \param c a set of nodes
   */
  void Install (NodeContainer c) const;

  /**
   * Install the specified auv mobility model into the node with the specified
   * name.
   *
   * \param nodeName name of the node
   */
  void Install (std::string nodeName) const;

  /**
   * Install the specified auv mobility model into the specified node
   *
   * \param node Pointer of the node where to install the mobility model
   */
  void Install (Ptr<Node> node) const;

  ObjectFactory m_factory;
  Ptr<PositionAllocator> m_allocator;
};

} // namespace ns3

#endif /* AUV_MOBILITY_HELPER_H */
