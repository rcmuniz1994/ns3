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

#include "auv-typhoon-helper.h"
#include "ns3/basic-energy-source-helper.h"
#include "ns3/typhoon-energy-model.h"
#include "ns3/typhoon-constraints.h"
#include "ns3/auv-waypoint-mobility-model.h"
#include "ns3/uan-channel.h"
#include "ns3/uan-net-device.h"
#include "ns3/uan-helper.h"
#include "ns3/net-device-container.h"
#include "ns3/net-device-container.h"

namespace ns3 {


AuvTyphoonHelper::AuvTyphoonHelper ()
{
}

void
AuvTyphoonHelper::Install (NodeContainer c) const
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Install (*i);
    }
}

void
AuvTyphoonHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  Install (node);
}

void
AuvTyphoonHelper::Install (Ptr<Node> node) const
{

  Ptr<AuvWaypointMobilityModel> wpmm = CreateObject<AuvWaypointMobilityModel> ();
  Ptr<TyphoonConstraints> typhoonConstraints = CreateObject<TyphoonConstraints> ();
  wpmm->SetKinematicConstraints (typhoonConstraints);

  node->AggregateObject (wpmm);


  // Typhoon energy source
  BasicEnergySourceHelper eh;
  eh.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (4320000.0));
  eh.Install (node);

  Ptr<TyphoonEnergyModel> typ = CreateObject<TyphoonEnergyModel> ();
  Ptr<EnergySource> source = node->GetObject<EnergySourceContainer> ()->Get (0);
  NS_ASSERT (source != NULL);
  typ->SetEnergySource (source);
  source->AppendDeviceEnergyModel (typ);
  source->SetNode (node);
  typ->SetNode (node);
  typ->SetEnergyDepletionCallback (MakeCallback (&AuvWaypointMobilityModel::HandleEnergyDepletion, wpmm));
  typ->SetEnergyRechargedCallback (MakeCallback (&AuvWaypointMobilityModel::HandleEnergyRecharged, wpmm));

  Ptr<MobilityModel> mm = node->GetObject<MobilityModel> ();
  NS_ASSERT_MSG (mm != NULL, "Perhaps an AUV should have a mobility model, isn't it ?");

  wpmm->TraceConnectWithoutContext ("CourseChange", MakeCallback (&TyphoonEnergyModel::NotifyCourseChanged, typ));

}


} // namespace ns3
