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

#include "remus-constraints.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/nstime.h"
#include "ns3/assert.h"
#include "ns3/abort.h"



namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RemusConstraints");

NS_OBJECT_ENSURE_REGISTERED (RemusConstraints);

TypeId
RemusConstraints::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RemusConstraints")
    .SetParent (KinematicConstraints::GetTypeId ())
    .SetGroupName ("Auv")
    .AddConstructor<RemusConstraints> ()
    .AddAttribute ("MaxDepth", "The maximum operational depth, in m",
                   DoubleValue (100.0),
                   MakeDoubleAccessor (&RemusConstraints::m_maxDepth),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxSpeed", "The maximum speed of the vehicle, in m/s.",
                   DoubleValue (2.3),
                   MakeDoubleAccessor (&RemusConstraints::m_maxSpeed),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxOmegaPitch", "The maximum omega (along x-axis) of the vehicle, in radians/s.",
                   DoubleValue (0.3),
                   MakeDoubleAccessor (&RemusConstraints::m_maxOmegaPitch),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxOmegaYaw", "The maximum omega (along z-axis) of the vehicle, in radians/s.",
                   DoubleValue (0.3),
                   MakeDoubleAccessor (&RemusConstraints::m_maxOmegaYaw),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxPitch", "The maximum pitch angle of the vehicle (absolute value = 60°) , in radians/s.",
                   DoubleValue (1.05),
                   MakeDoubleAccessor (&RemusConstraints::m_maxPitch),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

RemusConstraints::RemusConstraints ()
  : m_first (0)
{
}

RemusConstraints::~RemusConstraints ()
{
}

/*/Initialize m_first (support variable) at zero. We suppose that, during the first step,
the robot is aligned with the next waypoint, therefore we don't want to have any rotation during the first run.
/*/


std::list<Waypoint>
RemusConstraints::CheckPath (const Waypoint &start, const Waypoint &stop)
{
  std::list<Waypoint> ret;
//support variables for times calculation (used later)
  Time intermediateStepTime = Seconds (0);
  Time intermediateStepTime2 = Seconds (0);

//Check if the required depth is negative
  NS_ABORT_MSG_IF (stop.position.z < 0, "** THE DEPTH HAS TO  NON-NEGATIVE");
//Check if the required depth is minor than the the maximum operational depth
  NS_ABORT_MSG_IF (stop.position.z > m_maxDepth, "** REQUIRED DEPTH HAS TO BE < MAXIMUM OPERATIONAL DEPTH");
//Calculate the required pitch angle
  double requiredPitch = std::atan2 (stop.position.z - start.position.z, stop.position.x - start.position.x);
  std::cout << "REQUIRED PITCH " << requiredPitch << std::endl;
//the required head direction (yaw) of the robot
  double requiredYaw;
  double requiredYaw2;
/*/Check if next position is within a cone of amplitude 2*m_maxPitch (namely 120°) centered in the actual position
  (therefore, if the next position is reachable considering the limitation for the pitch angle).
  It is worth to note that two cones, horizzontally disposed, are present (both with the top centered on the "start point")
  Two subcases arise:
                                          - SUBCASE 1) OUT OF THE CONE
                                          - SUBCASE 2) WITHIN THE CONE

/*/
//SUBCASE 1) OUT OF THE CONE
  if ( std::abs (requiredPitch) >= m_maxPitch && std::abs (requiredPitch) < 2 * m_maxPitch  )
    {
      std::cout << "OUT OF THE CONE " << std::endl;
// Find the required yaw angle. The angle returns in radians.
      requiredYaw = std::atan2 (stop.position.y - start.position.y, stop.position.x - start.position.x);
      std::cout << "REQUIRED YAW " << requiredYaw << std::endl;
      std::cout << "ACTUAL YAW (BEFORE YAW ROTATION)" << m_Yaw << std::endl;
/*/Align the robot on the x-y plane. This operation doesn't occur: during the first run (look at variable "m_first")
and if the required angle ( "requiredYaw" ) is the same as the actual one ( "m_Yaw" )
/*/
      if (m_first != 0 && m_Yaw != requiredYaw )
        {
//Define a new waypoint (centered on start and with the time required to reach "requiredYaw"
          Waypoint rotationyaw = start;
/*/If I am in a singular point ( when "atan2" has an indeterminate form ) don't change anything.
In other words, the robot is maintained in the same attitude
/*/
          if (stop.position.x == start.position.x && stop.position.y == start.position.y )
            {
//The robot is maintained in the same attitude
              requiredYaw = m_Yaw;
            }
          else
            {
//Find the time to reach the required yaw angle
              rotationyaw.time = start.time + Seconds (std::abs ((std::abs (requiredYaw) - std::abs (m_Yaw))) / (m_maxOmegaYaw));
              std::cout << "ROTATION YAW TIME " << rotationyaw.time << std::endl;
//Check rotation time for the yaw angle
              NS_ABORT_MSG_IF (stop.time < rotationyaw.time, "** STOP TIME HAS TO BE > ROTATION YAW TIME");
              ret.push_back (rotationyaw);
//The actual yaw angle ("m_Yaw") becomes "requiredYaw"
              m_Yaw = requiredYaw;
            }
          std::cout << "ACTUAL YAW (AFTER YAW ROTATION)" << m_Yaw << std::endl;
//support variable for times calculation
          intermediateStepTime = rotationyaw.time;
        }
/*/ SUBCASE 1) OUT OF THE CONE two other subcases arise:
                         - SUBCASE 1a) If "stop.position.z"-"start.position.z" > 0 the robot has to go down, so the pitch angle is negative.
                         - SUBCASE 1b) if "stop.position.z"-"start.position.z" < 0 the robot has to go up, so the pitch angle is positive.
 The pitch angle is set at the maximum possible value.
/*/

/*/SUBCASE 1a): "stop.position.z"-"start.position.z" > 0
                                                The robot submerges unless it reaches the required depth
/*/
      if ( stop.position.z - start.position.z > 0)
        {
//Define a new waypoint centered on "rotationyaw" and with the time required to reach "|m_maxPitch|"
          Waypoint rotationpitch = start;
          rotationpitch.time = intermediateStepTime + Seconds (m_maxPitch / (m_maxOmegaPitch));
          std::cout << "ROTATION PITCH " << rotationpitch << std::endl;
//Check rotation time for the pitch angle
          NS_ABORT_MSG_IF (stop.time < rotationpitch.time, "** STOP TIME HAS TO BE > ROTATION YAW + PITCH TIME ");
          ret.push_back (rotationpitch);
//support variable for times calculation
          const double intermediateStepTime2 = (rotationpitch.time).ToDouble (ns3::Time::S);
          const double StopTime = (stop.time).ToDouble (ns3::Time::S);
//Set the speed as the maximum and find the time that is necessary to reach the z-coordinate of the stop point.
          double SpacetoTravel = std::abs (stop.position.z - start.position.z) / sin (m_maxPitch);
          double TimetoTravel = SpacetoTravel / m_maxSpeed + intermediateStepTime2;
          std::cout << "SPACE TO TRAVEL " << SpacetoTravel << std::endl;
          std::cout << "TIME TO TRAVEL " << TimetoTravel << std::endl;
//Check time to travel
          NS_ABORT_MSG_IF (TimetoTravel > StopTime, "** TIME TO TRAVEL HAS TO BE < STOP TIME");
//Find the coordinates of the point after the travel
//Define a new waypoint at the required depth after the dive at |m_maxPitch| angle
          Waypoint travel;
          travel.position.x = std::abs (stop.position.z - start.position.z) * (1 / tan (m_maxPitch)) * cos (requiredYaw) + start.position.x;
          travel.position.y = std::abs (stop.position.z - start.position.z) * (1 / tan (m_maxPitch)) * sin (requiredYaw) + start.position.y;
          travel.position.z = stop.position.z;
          travel.time = Seconds (TimetoTravel);
          std::cout << "TRAVEL " << travel << std::endl;
          ret.push_back (travel);
//Add the time to set newly the pitch as zero as well
//The waypoint "travel pitch" is centred on "travel" plus the time to set newly the pitch at zero
          Waypoint travelpitch;
          travelpitch = travel;
          travelpitch.time = travel.time + Seconds (m_maxPitch / (m_maxOmegaPitch));
//Check rotation time for the waypoint "travel pitch"
          NS_ABORT_MSG_IF (stop.time < travelpitch.time, "** STOP TIME HAS TO BE > TRAVEL PITCH");
          ret.push_back (travelpitch);
          std::cout << "TRAVEL PITCH " << travelpitch << std::endl;
//Rotate of 3.14 rad the yaw again
          requiredYaw2 = std::atan2 (stop.position.y - travel.position.y, stop.position.x - travel.position.x);
//The waypoint "rotationyaw2" is centred on "travelpitch" plus the time to rotate the yaw angle of 3.14
          Waypoint rotationyaw2 = travel;
          rotationyaw2.time = travelpitch.time + Seconds (std::abs ((std::abs (requiredYaw2) - std::abs (m_Yaw))) / (m_maxOmegaYaw));
//Check rotation time for the yaw angle
          NS_ABORT_MSG_IF (stop.time < rotationyaw2.time, "** STOP TIME HAS TO BE > ROTATION YAW2 TIME");
          ret.push_back (rotationyaw2);
          std::cout << "ROTATION YAW " << rotationyaw2 << std::endl;
//Update the yaw angle
          m_Yaw = requiredYaw2;
          requiredYaw = requiredYaw2;
/*/Find that speed that permits you to reach the stop point at the stop time.
   First of all, calculate the module  (at this point we are on a plane).
/*/
          double distance = std::sqrt ((stop.position.x - rotationyaw2.position.x) * (stop.position.x - rotationyaw2.position.x) + (stop.position.y - rotationyaw2.position.y) * (stop.position.y - rotationyaw2.position.y));
          std::cout << "DISTANCE " << distance << std::endl;
          const double lastStepTime = (stop.time - rotationyaw2.time).ToDouble (ns3::Time::S);
          double required_speed = (distance) / (lastStepTime);
          std::cout << "REQUIRED SPEED " << required_speed << std::endl;
          //Check required speed
          NS_ABORT_MSG_IF (required_speed > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < MAX SPEED");
          ret.push_back (stop);
        }
      /*/SUBCASE 1b): "stop.position.z"-"start.position.z" < 0
                                                The robot emerges unless it reaches the required depth
      /*/
      else
        {
//Define a new waypoint centered on "rotationyaw" and with the time required to reach "|m_maxPitch|"
          Waypoint rotationpitch = start;
          rotationpitch.time = intermediateStepTime + Seconds (m_maxPitch / (m_maxOmegaPitch));
          std::cout << "ROTATION PITCH " << rotationpitch << std::endl;
//Check rotation time for the pitch angle
          NS_ABORT_MSG_IF (stop.time < rotationpitch.time, "** STOP TIME HAS TO BE > ROTATION YAW + PITCH TIME ");
          ret.push_back (rotationpitch);
//support variable for times calculation
          const double intermediateStepTime2 = (rotationpitch.time).ToDouble (ns3::Time::S);
          const double StopTime = (stop.time).ToDouble (ns3::Time::S);
//Set the speed as the maximum and find the time that is necessary to reach the z-coordinate of the stop point.
          double SpacetoTravel = std::abs (stop.position.z - start.position.z) / sin (m_maxPitch);
          double TimetoTravel = SpacetoTravel / m_maxSpeed + intermediateStepTime2;
          std::cout << "SPACE TO TRAVEL " << SpacetoTravel << std::endl;
          std::cout << "TIME TO TRAVEL " << TimetoTravel << std::endl;
//Check time to travel
          NS_ABORT_MSG_IF (TimetoTravel > StopTime, "** TIME TO TRAVEL HAS TO BE < STOP TIME");
//Find the coordinates of the point after the travel
//Define a new waypoint at the required depth after the dive at |m_maxPitch| angle
          Waypoint travel;
          travel.position.x = std::abs (stop.position.z - start.position.z) * (1 / tan (m_maxPitch)) * cos (requiredYaw) + start.position.x;
          travel.position.y = std::abs (stop.position.z - start.position.z) * (1 / tan (m_maxPitch)) * sin (requiredYaw) + start.position.y;
          travel.position.z = stop.position.z;
//Add the time to set newly the pitch as zero as well
          travel.time = Seconds (TimetoTravel);
          ret.push_back (travel);
          std::cout << "TRAVEL " << travel << std::endl;
//Add the time to set newly the pitch as zero as well
//The waypoint "travel pitch" is centred on "travel" plus the time to set newly the pitch at zero
          Waypoint travelpitch;
          travelpitch = travel;
          travelpitch.time = travel.time + Seconds (m_maxPitch / (m_maxOmegaPitch));
//Check rotation time for the waypoint "travel pitch"
          NS_ABORT_MSG_IF (stop.time < travelpitch.time, "** STOP TIME HAS TO BE > TRAVEL PITCH");
          ret.push_back (travelpitch);
          std::cout << "TRAVEL PITCH " << travelpitch << std::endl;
          ret.push_back (travelpitch);
//Rotate of 3.14 rad the yaw again
          requiredYaw2 = std::atan2 (stop.position.y - travel.position.y, stop.position.x - travel.position.x);
//The waypoint "rotationyaw2" is centred on "travelpitch" plus the time to rotate the yaw angle of 3.14
          Waypoint rotationyaw2 = travel;
          rotationyaw2.time = travelpitch.time + Seconds (std::abs ((std::abs (requiredYaw2) - std::abs (m_Yaw))) / (m_maxOmegaYaw));
//Check rotation time for the yaw angle
          NS_ABORT_MSG_IF (stop.time < rotationyaw2.time, "** STOP TIME HAS TO BE > ROTATION YAW2 TIME");
          ret.push_back (rotationyaw2);
          std::cout << "ROTATION yaw2 " << rotationyaw2 << std::endl;
          m_Yaw = requiredYaw2;
          requiredYaw = requiredYaw2;
/*/ Find that speed that permits you to reach the stop point at the stop time.
    First of all, calculate the module  (at this point we are on a plane).
/*/
          double distance = std::sqrt ((stop.position.x - rotationyaw2.position.x) * (stop.position.x - rotationyaw2.position.x) + (stop.position.y - rotationyaw2.position.y) * (stop.position.y - rotationyaw2.position.y));
          std::cout << "DISTANCE " << distance << std::endl;
          const double lastStepTime = (stop.time - rotationyaw2.time).ToDouble (ns3::Time::S);
          double required_speed = (distance) / (lastStepTime);
          std::cout << "REQUIRED SPEED " << required_speed << std::endl;
          //Check required speed
          NS_ABORT_MSG_IF (required_speed > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < MAX SPEED");
          ret.push_back (stop);
        }
    }

  /*/Check if next position is within a cone of amplitude 120° centered in the actual position
    (therefore, if the next position is reachable considering the limitation for the yaw angle).
    SUBCASE 2): NOT OUT OF THE CONE
  /*/
  else
    {
// Find the required yaw angle.
      requiredYaw = std::atan2 (stop.position.y - start.position.y, stop.position.x - start.position.x);
      std::cout << "REQUIRED YAW (WITHIN THE CONE)" << requiredYaw << std::endl;
/*/Check if variable "m_first" is zero. This occurs only during the first run, in fact at the end of the CheckPath function, variable avoidFirstRotation is set at 1.
   If so, any rotation is made.
   Moreover, any rotation has to be made if "headDirection" (actual yaw angle) and "required_headDirection" (required yaw angle) have the same value.
/*/
      if (m_first != 0 && m_Yaw != requiredYaw )
        {
//The waypoint "rotationyaw" is centered on the defined waypoint "start" (with the required time for the yaw rotation)
          Waypoint rotationyaw = start;
          rotationyaw.time = start.time + Seconds (std::abs ((std::abs (requiredYaw) - std::abs (m_Yaw))) / (m_maxOmegaYaw));
          std::cout << "ROTATION YAW TIME (WITHIN THE CONE) " << rotationyaw.time << std::endl;
//Check rotation time for the yaw angle
          NS_ABORT_MSG_IF (stop.time < rotationyaw.time, "** STOP TIME HAS TO BE > ROTATION YAW TIME");
          ret.push_back (rotationyaw);
          intermediateStepTime = rotationyaw.time;
          m_Yaw = requiredYaw;
        }
      if ( requiredPitch != 0 )
        {
//Since the robot is already aligned towards the next point, the required pitch is always ranged between ( -60°; 60° )
          requiredPitch = std::min (std::abs (requiredPitch),std::abs (requiredPitch - 3.14));
//Define a new waypoint centered on "rotationyaw" and with the time required to reach "|m_maxPitch|"
          Waypoint rotationpitch = start;
/*/Add the time to set newly the pitch as zero as well. In theory, it should be add at the start of the new cycle.
   On the other hand, as first approach, we can consider two times this value at this point. (Note that this is an assumption).
/*/
          rotationpitch.time = intermediateStepTime + Seconds (std::abs (2 * requiredPitch) / (m_maxOmegaPitch));
          intermediateStepTime = rotationpitch.time;
          std::cout << "ROTATION PITCH (WITHIN THE CONE) " << rotationpitch << std::endl;
          //Check rotation time for the pitch angle
          NS_ABORT_MSG_IF (stop.time < rotationpitch.time, "** STOP TIME HAS TO BE > ROTATION YAW + PITCH TIME ");
          ret.push_back (rotationpitch);
        }
// Find that speed that permits you to reach the stop point at the stop time. First of all, calculate the module
      double distance = std::sqrt ((stop.position.x - start.position.x) * (stop.position.x - start.position.x) + (stop.position.y - start.position.y) * (stop.position.y - start.position.y) + (stop.position.z - start.position.z) * (stop.position.z - start.position.z));
      std::cout << "DISTANCE (WITHIN THE CONE) " << distance << std::endl;
      const double lastStepTime = (stop.time - intermediateStepTime).ToDouble (ns3::Time::S);
      std::cout << "lastStepTime (WITHIN THE CONE) " << lastStepTime << std::endl;
      double required_speed = (distance) / (lastStepTime);
      std::cout << "REQUIRED SPEED (WITHIN THE CONE) " << required_speed << std::endl;
      //Check required speed
      NS_ABORT_MSG_IF (required_speed > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < MAX SPEED");
      ret.push_back (stop);
    }
/*/  Variable "m_first" is set at true .
 Therefore, apart from the first run, the program has to check whether or not the robot is aligned with the next waypoint.
/*/
  m_first = 1;
  return ret;
}


} // namespace ns3
