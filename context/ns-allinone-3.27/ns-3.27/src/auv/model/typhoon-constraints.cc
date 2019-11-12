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
#include "typhoon-constraints.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/nstime.h"
#include "ns3/assert.h"
#include "ns3/abort.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TyphoonConstraints");

NS_OBJECT_ENSURE_REGISTERED (TyphoonConstraints);

TypeId
TyphoonConstraints::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TyphoonConstraints")
    .SetParent (KinematicConstraints::GetTypeId ())
    .SetGroupName ("Auv")
    .AddConstructor<TyphoonConstraints> ()
    .AddAttribute ("MaxDepth", "The maximum operational depth, in m",
                   DoubleValue (300.0),
                   MakeDoubleAccessor (&TyphoonConstraints::m_maxDepth),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxSpeed", "The maximum (horizontal) speed of the vehicle, in m/s.",
                   DoubleValue (2.5),
                   MakeDoubleAccessor (&TyphoonConstraints::m_maxSpeed),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxSpeedz", "The maximum (vertical) speed of the vehicle, in m/s.",
                   DoubleValue (0.26),
                   MakeDoubleAccessor (&TyphoonConstraints::m_maxSpeedz),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxOmega", "The maximum omega (along z-axis) of the vehicle, in rad/s.",
                   DoubleValue (0.3),
                   MakeDoubleAccessor (&TyphoonConstraints::m_maxOmega),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}
/*/Initialize m_first (support variable) at "zero". We suppose that, during the first step,
the robot is aligned with the next waypoint, therefore we don't want to have any rotation during the first run.
/*/
TyphoonConstraints::TyphoonConstraints ()
  : m_headDirection (0), m_first (0)
{
}

TyphoonConstraints::~TyphoonConstraints ()
{
}

std::list<Waypoint>
TyphoonConstraints::CheckPath (const Waypoint &start, const Waypoint &stop)
{
  std::list<Waypoint> ret;
  //support variable for times calculation (used later)
  Time intermediateStepTime = Seconds (0);
  //the required head direction (yaw) of the robot
  double requiredHeadDirection = std::atan2 (stop.position.y - start.position.y, stop.position.x - start.position.x);

  //Check if the required depth is negative
  NS_ABORT_MSG_IF (stop.position.z < 0, "** THE DEPTH HAS TO BE NON-NEGATIVE");
  //Check if the required depth is minor than the the maximum operational depth
  NS_ABORT_MSG_IF (stop.position.z > m_maxDepth, "** REQUIRED DEPTH HAS TO BE < MAXIMUM OPERATIONAL DEPTH");

  /*/Two subcases arise:
                                          - SUBCASE 1) "start point" and "stop point" are NOT at the same depth
                                          - SUBCASE 2) "start point" and "stop point" are  at the same depth
 /*/

  //SUBCASE 1) "start point" and "stop point" are NOT at the same depth
  //Check if "start point" and "stop point" and move along z-axis
  if (stop.position.z != start.position.z)
    {
      //Go down
      Waypoint down;
      down.position = start.position;
      down.position.z = stop.position.z;
      std::cout << "DOWN POSITION " << down.position.z << std::endl;
      /*/Two subcases (within SUBCASE 1) arise:
                                          - SUBCASE 1a) I just have to go down
                                          - SUBCASE 1b) I DO NOT just have to go down ("start point" and "stop point" have different x/y coordinates)
/*/

      //SUBCASE 1a): I just have to go down. Therefore, in this situation, I have to find the speed along z-axis (remember stop time is fixed)
      if ( stop.position.x == start.position.x && stop.position.y == start.position.y )
        {
          const double stopTime = (stop.time).ToDouble (ns3::Time::S);
          const double startTime = (start.time).ToDouble (ns3::Time::S);
          //I have to find the speed along z-axis (remember stop time is fixed)
          double requiredSpeedz = std::abs (stop.position.z - start.position.z) / (stopTime - startTime);
          std::cout << "REQUIRED Z SPEED " << requiredSpeedz << std::endl;
          //Check speed along z
          NS_ABORT_MSG_IF (requiredSpeedz > m_maxSpeedz, "** REQUIRED Z SPEED HAS TO BE < MAXIMUM OPERATIONAL Z SPEED ");
          down.time = stop.time;
        }
      //SUBCASE 1b): I DO NOT just have to go down.
      else
        {
          //I have to set the speed along z-axis at m_maxSpeedz
          down.time = Seconds (std::abs (stop.position.z - start.position.z) / (m_maxSpeedz)) + start.time;
          std::cout << "DOWN TIME " << down.time << std::endl;
          //Check down time
          NS_ABORT_MSG_IF (stop.time < down.time, "** STOP TIME HAS TO BE > DOWN TIME");
          ret.push_back (down);
        }
    }
  // The continue of SUBCASE 1) "start point" and "stop point" are NOT at the same depth
  // Check the next and actual x/y position. This check is made to avoid to find me newly in SUBCASE 1a)
  // This if-cycle and the one of SUBCASE 1a) are separated,
  // so if SUBCASE 2) arises ("start point" and "stop point" are  at the same depth) the run of SUBCASE 2) starts here
  if (stop.position.x != start.position.x || stop.position.y != start.position.y)
    {
      //Find the required yaw angle. The angle returns in radians.
      std::cout << "REQUIRED HEAD DIRECTION " << requiredHeadDirection << std::endl;
      std::cout << "HEAD DIRECTION " << m_headDirection << std::endl;
      /*/Check if variable "m_first" is zero. This occurs only during the first run, in fact at the end of the CheckPath function, variable avoidFirstRotation is set at 1.
   If so, any rotation is made.
   Moreover, any rotation has to be made if "headDirection" (actual yaw angle) and "required_headDirection" (required yaw angle) have the same value.
/*/
      if (m_first != 0 && m_headDirection != requiredHeadDirection )
        {
          //The waypoint "rotation" is centered on the previously defined waypoint "down" in SUBCASE 1a) (with the required time for the yaw rotation)
          //In other words, the waypoint "rotation" is centered on "start" (but at the depth of "down") and with the required time for the yaw rotation)
          Waypoint rotation = start;
          rotation.position.z = stop.position.z;
          rotation.time = start.time + Seconds (std::abs ((std::abs (requiredHeadDirection) - std::abs (m_headDirection))) / (m_maxOmega)) + Seconds (std::abs (stop.position.z - start.position.z) / (m_maxSpeedz));
          std::cout << "ROTATION TIME " << rotation.time << std::endl;
          //Check rotation time
          NS_ABORT_MSG_IF (stop.time < rotation.time, "** STOP TIME HAS TO BE > ROTATION TIME");
          ret.push_back (rotation);
          std::cout << "QUIIII " << std::endl;
          std::cout << "rotation.time " << rotation.time << std::endl;
          intermediateStepTime = rotation.time;
          //Set the head direction "m_headDirection" at the new value "required_headDirection"
          m_headDirection = requiredHeadDirection;
        }
    }
  //Find that speed that permits you to reach the "stop point" at the "stop time". First of all, calculate the module (at this point we are on a plane).
  intermediateStepTime = start.time + Seconds (std::abs ((std::abs (requiredHeadDirection) - std::abs (m_headDirection))) / (m_maxOmega)) + Seconds (std::abs (stop.position.z - start.position.z) / (m_maxSpeedz));
  double distance = std::sqrt ((stop.position.x - start.position.x) * (stop.position.x - start.position.x) + (stop.position.y - start.position.y) * (stop.position.y - start.position.y));
  std::cout << "DISTANCE " << distance << std::endl;
  std::cout << "INTERMEDIATE STEP TIME " << intermediateStepTime << std::endl;
  //The difference between the "stop time" and the time of the last waypoint (namely "rotation")
  const double lastStepTime = (stop.time - intermediateStepTime).ToDouble (ns3::Time::S);
  double required_speed = (distance) / (lastStepTime);
  std::cout << "REQUIRED SPEED " << required_speed << std::endl;
  //Check required speed
  NS_ABORT_MSG_IF (required_speed > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < MAX SPEED");
  ret.push_back (stop);
  /*/Variable "m_first" is set at true . Therefore, apart from the first run, the program has to check whether or not the robot is aligned with the next waypoint.
/*/
  m_headDirection = requiredHeadDirection;
  m_first = 1;
  return ret;
}


} // namespace ns3
