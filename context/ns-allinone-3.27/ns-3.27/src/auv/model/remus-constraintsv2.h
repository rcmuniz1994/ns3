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

#ifndef REMUS_CONSTRAINTSV2_H_
#define REMUS_CONSTRAINTSV2_H_


#include "kinematic-constraints.h"
#include "ns3/event-id.h"
#include "ns3/waypoint.h"


namespace ns3 {

/**
 *
 * A REMUS class AUV is an submarine-like device, propelled by an electric motor linked with a propeller.
 *
 * The model is based on the characteristics of the REMUS 100 AUV [1]:
 *
 * - maximum operating depth, 100 m
 * - maximum speed, 2.3 m/s
 * - pitch range 60°
 *
 * References:
 * [1] https://www.km.kongsberg.com/ks/web/nokbg0397.nsf/AllWeb/82C0D4915CE64FEAC1257A23002BECC5/$file/REMUS-brochure.pdf?OpenElement
 *
 *
 * The function CheckPath gives a list of feasible intermediate waypoints between a start ("start") and an end point ("stop").
 *
 * Considering the cinematic constraints of the robot, given these two points, the situation goes as follow:
 *
 * - firstly (if necessary), the robot ascends or descends at the depth of the "stop" point (remember that the maximum operating pitch range is 60°);
 * - then (if necessary), it rotates along the z-axis in order to align itself towards the "stop" point;
 * - then (if necessary), it goes straight towards the "stop" point
 *
 * The difference between "remus-constraints" e "remus-constraintsv2" takes place during the immersion/emersion step. Here the shortest path is considered.
 /*/

class RemusConstraintsv2 : public KinematicConstraints
{
public:
  static TypeId GetTypeId (void);
  RemusConstraintsv2 ();
  ~RemusConstraintsv2 ();

  virtual std::list<Waypoint> CheckPath (const Waypoint &start, const Waypoint &stop);


private:
  double m_maxDepth;
  double m_maxSpeed;
  double m_maxOmegaYaw;
  double m_maxOmegaPitch;
  double m_maxPitch;
  double m_Yaw;
  int m_first;


};

}  // namespace ns3

#endif /* REMUS_CONSTRAINTSV2_H_ */
