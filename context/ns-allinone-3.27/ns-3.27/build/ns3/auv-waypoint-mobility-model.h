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

#ifndef AUV_WAYPOINT_MOBILITY_MODEL_H
#define AUV_WAYPOINT_MOBILITY_MODEL_H

#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/simulator.h"
#include "ns3/pointer.h"
#include "ns3/waypoint-mobility-model.h"

namespace ns3 {

class KinematicConstraints;

/**
 * \ingroup auv
 *
 */


class AuvWaypointMobilityModel : public WaypointMobilityModel
{
public:
  typedef Callback<void> WaypointReachedCallback;

  /**
   * Register this type with the TypeId system.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  AuvWaypointMobilityModel ();

  virtual void AddWaypoint (const Waypoint &waypoint);

  void HandleEnergyDepletion ();
  void HandleEnergyRecharged ();

  void SetKinematicConstraints (Ptr<KinematicConstraints> kinematicConstraints);
  Ptr<KinematicConstraints> GetKinematicConstraints (void);


private:
  virtual void Update (void) const;
  virtual void DoDispose (void);
  bool m_first;
  bool m_energyDepleted;
  Waypoint m_lastWaypointAdded;
  Ptr<KinematicConstraints> m_kinematicConstraints;
};

} // namespace ns3

#endif /* AUV_WAYPOINT_MOBILITY_MODEL_H */

