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

#ifndef KINEMATIC_CONSTRAINTS_H_
#define KINEMATIC_CONSTRAINTS_H_

#include <list>
#include "ns3/object.h"

namespace ns3 {

class Waypoint;

/**
 *
 * Given a start "point" (3D space point and an absolute time) and a stop "point" (3D space point and an absolute time), using an
 * underline mobility model suitable for AUV's (e.g. remus, glider, typhoon...), the CheckPath function will return a list of feasible intermediate points
 * (x, y, z and Time)
 *
 */
class KinematicConstraints : public Object

{
public:
  static TypeId GetTypeId (void);
  KinematicConstraints ();
  virtual ~KinematicConstraints ();

  /**
   * The CheckPath function will return a list of feasible intermediate points (x, y, z and Time)
   */
  virtual std::list<Waypoint> CheckPath (const Waypoint &start, const Waypoint &stop) = 0;

//Maximum depth and speeds

protected:
  double m_maxDepth;    //!< Maximum AUV depth
  double m_maxSpeed;    //!< Maximum AUV (horizontal) speed [m/s]
  double m_maxSpeedz;    //!< Maximum AUV (vertical) speed [m/s]
  double m_headDirection; //!< Direction of the AUV head [radians w.r.t. the North]
  double m_pitch;         //!< Pitch of the AUV [radians w.r.t. the horizon]
  bool m_first;           //!< True if the AUV was not yet started.
  double m_maxOmegaYaw;
  double m_maxOmegaPitch;
  double m_maxPitch;
  double m_yaw;

};

} // namespace ns3

#endif /* KINEMATIC_CONSTRAINTS_H_ */

