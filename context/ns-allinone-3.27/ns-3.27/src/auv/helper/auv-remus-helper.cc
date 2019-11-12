/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Tommaso Pecorella
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
 * Authors: Tommaso Pecorella <tommaso.pecorella@unifi.it> Matteo Franchi <teofr89@hotmail.it>
 */

#include "auv-remus-helper.h"
#include "ns3/basic-energy-source-helper.h"
#include "ns3/remus-energy-model.h"
#include "ns3/remus-constraints.h"
#include "ns3/auv-waypoint-mobility-model.h"
#include "ns3/uan-channel.h"
#include "ns3/uan-net-device.h"
#include "ns3/uan-helper.h"
#include "ns3/net-device-container.h"
#include "ns3/net-device-container.h"

namespace ns3 {


AuvRemusHelper::AuvRemusHelper ()
{
}

void
AuvRemusHelper::Install (NodeContainer c) const
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Install (*i);
    }
}

void
AuvRemusHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  Install (node);
}

void
AuvRemusHelper::Install (Ptr<Node> node) const
{

  Ptr<AuvWaypointMobilityModel> wpmm = CreateObject<AuvWaypointMobilityModel> ();
  Ptr<RemusConstraints> remusConstraints = CreateObject<RemusConstraints> ();
  wpmm->SetKinematicConstraints (remusConstraints);

  node->AggregateObject (wpmm);


  // Typhoon energy source
  BasicEnergySourceHelper eh;
  eh.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (3600000.0));
  eh.Install (node);

  Ptr<RemusEnergyModel> rem = CreateObject<RemusEnergyModel> ();
  Ptr<EnergySource> source = node->GetObject<EnergySourceContainer> ()->Get (0);
  NS_ASSERT (source != NULL);
  rem->SetEnergySource (source);
  source->AppendDeviceEnergyModel (rem);
  source->SetNode (node);
  rem->SetNode (node);
  rem->SetEnergyDepletionCallback (MakeCallback (&AuvWaypointMobilityModel::HandleEnergyDepletion, wpmm));
  rem->SetEnergyRechargedCallback (MakeCallback (&AuvWaypointMobilityModel::HandleEnergyRecharged, wpmm));

  Ptr<MobilityModel> mm = node->GetObject<MobilityModel> ();
  NS_ASSERT_MSG (mm != NULL, "Perhaps an AUV should have a mobility model, isn't it ?");

  wpmm->TraceConnectWithoutContext ("CourseChange", MakeCallback (&RemusEnergyModel::NotifyCourseChanged, rem));

}


} // namespace ns3
