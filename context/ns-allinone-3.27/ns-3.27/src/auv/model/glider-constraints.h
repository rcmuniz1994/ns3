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

#ifndef GLIDER_CONSTRAINTS_H_
#define GLIDER_CONSTRAINTS_H_


#include "kinematic-constraints.h"
#include "ns3/event-id.h"
#include "ns3/waypoint.h"


namespace ns3 {

/**
 *
 *
 * A TYPHOON class AUV is an submarine-like device, propelled by some electric motors.
 *
 * The model is based on the characteristics of the TYPHOON AUV (DIEF University of Florence, MDM Lab) . The robot has 5 DOF.
 *
 * - maximum operating depth, 100 m
 * - maximum speed (on a plane) 1.5 m/s
 * - maximum speed (ascending or descending) 0.25 m/s
 * - maximum rotational speed (along z-axis) 18°/s
 * - the robot cannot move with a pitch angle different from zero.
 *
 * The function CheckPath gives a list of feasible intermediate waypoints between a start ("start") and an end point ("stop").
 *
 * Considering the cinematic constraints of the robot, given these two points, the situation goes as follow:
 *
 * - firstly (if necessary), the robot ascends or descends at the depth of the "stop" point;
 * - then (if necessary), it rotates along the z-axis in order to align itself towards the "stop" point;
 * - then (if necessary), it goes straight towards the "stop" point
 */

class GliderConstraints : public KinematicConstraints
{
public:
  static TypeId GetTypeId (void);
  GliderConstraints ();
  ~GliderConstraints ();

  virtual std::list<Waypoint> CheckPath (const Waypoint &start, const Waypoint &stop);
  double GetBuoyancy (double U, double W) const;


private:
  double m_maxDepth;
  double m_maxSpeed;
  // double m_headDirection;
  double m_maxOmegaYaw;
  double m_maxPitch;
  double m_minPitch;
  double m_maxBuoyancy;
  double m_aBuoyancy;
  double m_bBuoyancy;
  double m_cBuoyancy;
  double m_rho;
  double m_l;
  int m_first;
  double m_Yaw;

};

}  // namespace ns3

#endif /* GLIDER_CONSTRAINTS_H_ */
