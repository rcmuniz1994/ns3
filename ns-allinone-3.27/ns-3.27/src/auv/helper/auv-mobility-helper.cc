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

#include "auv-mobility-helper.h"
#include "ns3/string.h"

namespace ns3 {

AuvMobilityHelper::AuvMobilityHelper ()
{
  m_factory.SetTypeId ("ns3::RemusMobilityModel");

  m_allocator = CreateObjectWithAttributes<RandomBoxPositionAllocator>
      ("X", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"),
      "Y", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"),
      "Z", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"));
}

void
AuvMobilityHelper::SetType (std::string type)
{
  m_factory.SetTypeId (type);
}

void AuvMobilityHelper::SetAttribute (std::string name,
                                      const AttributeValue &value)
{
  m_factory.Set (name,
                 value);
}

void
AuvMobilityHelper::SetPositionAllocator (Ptr<PositionAllocator> allocator)
{
  m_allocator = allocator;
}

void
AuvMobilityHelper::SetPositionAllocator (std::string type,
                                         std::string n1, const AttributeValue &v1,
                                         std::string n2, const AttributeValue &v2,
                                         std::string n3, const AttributeValue &v3,
                                         std::string n4, const AttributeValue &v4,
                                         std::string n5, const AttributeValue &v5,
                                         std::string n6, const AttributeValue &v6,
                                         std::string n7, const AttributeValue &v7,
                                         std::string n8, const AttributeValue &v8,
                                         std::string n9, const AttributeValue &v9)
{
  ObjectFactory pos;
  pos.SetTypeId (type);
  pos.Set (n1, v1);
  pos.Set (n2, v2);
  pos.Set (n3, v3);
  pos.Set (n4, v4);
  pos.Set (n5, v5);
  pos.Set (n6, v6);
  pos.Set (n7, v7);
  pos.Set (n8, v8);
  pos.Set (n9, v9);
  m_allocator = pos.Create ()->GetObject<PositionAllocator> ();
}

void
AuvMobilityHelper::Install (NodeContainer c) const
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Install (*i);
    }
}

void
AuvMobilityHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  Install (node);
}

void
AuvMobilityHelper::Install (Ptr<Node> node) const
{
  Ptr<AuvMobilityModel> model = m_factory.Create ()->GetObject<AuvMobilityModel> ();

  NS_ASSERT_MSG (model != NULL,
                 "The requested mobility model is not an auv mobility model: \"" <<
                 m_factory.GetTypeId ().GetName () << "\"");

  Vector pos = m_allocator->GetNext ();
  model->SetPosition (pos);
  model->SetNode (node);
  node->AggregateObject (model);
}

} // namespace ns3
