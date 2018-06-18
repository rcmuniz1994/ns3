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

#include "auv-glider-helper.h"
#include "ns3/basic-energy-source-helper.h"
#include "ns3/glider-energy-model.h"
#include "ns3/glider-constraints.h"
#include "ns3/auv-waypoint-mobility-model.h"
//-------------------EDITEI AQUI - RODRIGO ------------------//
#include "ns3/acoustic-modem-energy-model-helper.h"

#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/names.h"
#include "ns3/glider-energy-model.h"
#include "ns3/acoustic-modem-energy-model-helper.h"
#include "ns3/uan-helper.h"
#include "ns3/waypoint-mobility-model.h"
#include "ns3/energy-source-container.h"
//-------------------EDITEI AQUI - RODRIGO ------------------//

namespace ns3 {


AuvGliderHelper::AuvGliderHelper ()
{
}

void
AuvGliderHelper::Install (NodeContainer c) const
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Install (*i);
    }
}

void
AuvGliderHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  Install (node);
}

void
AuvGliderHelper::Install (Ptr<Node> node) const
{

  Ptr<AuvWaypointMobilityModel> wpmm = CreateObject<AuvWaypointMobilityModel> ();
  Ptr<GliderConstraints> gliderConstraints = CreateObject<GliderConstraints> ();
  wpmm->SetKinematicConstraints (gliderConstraints);

  node->AggregateObject (wpmm);

  // Glider energy source
  BasicEnergySourceHelper eh;
  eh.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (4320000.0));
  eh.Install (node);

  Ptr<GliderEnergyModel> typ = CreateObject<GliderEnergyModel> ();
  Ptr<EnergySource> source = node->GetObject<EnergySourceContainer> ()->Get (0);
  NS_ASSERT (source != NULL);
  typ->SetEnergySource (source);
  source->AppendDeviceEnergyModel (typ);
  source->SetNode (node);
  typ->SetNode (node);
  //typ->SetEnergyDepletionCallback (MakeCallback (&AuvWaypointMobilityModel::HandleEnergyDepletion, wpmm));
  //typ->SetEnergyRechargedCallback (MakeCallback (&AuvWaypointMobilityModel::HandleEnergyRecharged, wpmm));
}

} // namespace ns3
