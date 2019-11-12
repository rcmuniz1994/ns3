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

#include "kinematic-constraints.h"
#include "auv-waypoint-mobility-model.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("AuvWaypointMobilityModel");

NS_OBJECT_ENSURE_REGISTERED (AuvWaypointMobilityModel);


TypeId
AuvWaypointMobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::AuvWaypointMobilityModel")
    .SetParent<WaypointMobilityModel> ()
    .SetGroupName ("Auv")
    .AddConstructor<AuvWaypointMobilityModel> ()

  ;
  return tid;
}


AuvWaypointMobilityModel::AuvWaypointMobilityModel ()
  : WaypointMobilityModel (),  m_first (true), m_energyDepleted (false)
{
}

void
AuvWaypointMobilityModel::DoDispose (void)
{
  m_kinematicConstraints = 0;
  MobilityModel::DoDispose ();
}

void
AuvWaypointMobilityModel::Update (void) const
{
  if ( m_energyDepleted )
    {
      return;
    }
  WaypointMobilityModel::Update ();
}

void
AuvWaypointMobilityModel::AddWaypoint (const Waypoint &waypoint)
{
  if (m_first)
    {
      m_first = false;
      WaypointMobilityModel::AddWaypoint (waypoint);
      m_lastWaypointAdded = waypoint;
      return;
    }

  NS_LOG_LOGIC ("Adding points between " << m_lastWaypointAdded << " and " << waypoint);
  std::list<Waypoint> points = m_kinematicConstraints->CheckPath (m_lastWaypointAdded, waypoint);
  std::list<Waypoint>::iterator iter;
  for (iter = points.begin (); iter != points.end (); iter++)
    {
      NS_LOG_LOGIC ("Adding middle point " << *iter);
      std::cout << "*** Adding point " << *iter << std::endl;
      WaypointMobilityModel::AddWaypoint (*iter);
    }
  m_lastWaypointAdded = waypoint;
}

void
AuvWaypointMobilityModel::HandleEnergyDepletion ()
{
  m_energyDepleted = true;
  EndMobility ();
  NotifyCourseChange ();
}

void
AuvWaypointMobilityModel::HandleEnergyRecharged ()
{
  m_energyDepleted = false;
}

void
AuvWaypointMobilityModel::SetKinematicConstraints (Ptr<KinematicConstraints> kinematicConstraints)
{
  m_kinematicConstraints = kinematicConstraints;
}

Ptr<KinematicConstraints> AuvWaypointMobilityModel::GetKinematicConstraints (void)
{
  return m_kinematicConstraints;
}

} // namespace ns3

