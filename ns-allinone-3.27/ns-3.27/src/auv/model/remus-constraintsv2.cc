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

#include "remus-constraintsv2.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/nstime.h"
#include "ns3/assert.h"
#include "ns3/abort.h"



namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RemusConstraintsv2");

NS_OBJECT_ENSURE_REGISTERED (RemusConstraintsv2);

TypeId
RemusConstraintsv2::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RemusConstraintsv2")
    .SetParent (KinematicConstraints::GetTypeId ())
    .SetGroupName ("Auv")
    .AddConstructor<RemusConstraintsv2> ()
    .AddAttribute ("MaxDepth", "The maximum operational depth, in m",
                   DoubleValue (100.0),
                   MakeDoubleAccessor (&RemusConstraintsv2::m_maxDepth),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxSpeed", "The maximum speed of the vehicle, in m/s.",
                   DoubleValue (2.3),
                   MakeDoubleAccessor (&RemusConstraintsv2::m_maxSpeed),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("MaxOmegaPitch", "The maximum omega (along x-axis) of the vehicle, in radians/s.",
                   DoubleValue (0.3),
                   MakeDoubleAccessor (&RemusConstraintsv2::m_maxOmegaPitch),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxOmegaYaw", "The maximum omega (along z-axis) of the vehicle, in radians/s.",
                   DoubleValue (0.3),
                   MakeDoubleAccessor (&RemusConstraintsv2::m_maxOmegaYaw),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxPitch", "The maximum pitch angle of the vehicle (absolute value = 60°) , in radians/s.",
                   DoubleValue (1.05),
                   MakeDoubleAccessor (&RemusConstraintsv2::m_maxPitch),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

RemusConstraintsv2::RemusConstraintsv2 ()
  : m_first (0)
{
}

RemusConstraintsv2::~RemusConstraintsv2 ()
{
}

/*/Initialize m_first (support variable) at zero. We suppose that, during the first step,
the robot is aligned with the next waypoint, therefore we don't want to have any rotation during the first run.
/*/


std::list<Waypoint>
RemusConstraintsv2::CheckPath (const Waypoint &start, const Waypoint &stop)
{
  std::list<Waypoint> ret;

  Time intermediateStepTime = Seconds (0);
  Time intermediateStepTime2 = Seconds (0);


  //Check if the required depth is negative
  NS_ABORT_MSG_IF (stop.position.z < 0, "** THE DEPTH HAS TO  NON-NEGATIVE");
  //Check if the required depth is minor than the the maximum operational depth
  NS_ABORT_MSG_IF (stop.position.z > m_maxDepth, "** REQUIRED DEPTH HAS TO BE < MAXIMUM OPERATIONAL DEPTH");

  double requiredPitch = std::atan2 (stop.position.z - start.position.z, stop.position.x - start.position.x);

  double requiredYaw;
  double requiredYaw2;
  std::cout << "REQUIRED PITCH 2222 " << requiredPitch << std::endl;
/*/Check if next position is within a cone of amplitude 120° centered in the actual position
  (therefore, if the next position is reachable considering the limitation for the yaw angle).
  SUBCASE: OUT OF THE CONE
/*/

  if ( std::abs (requiredPitch) >= m_maxPitch && std::abs (requiredPitch) <= 2 * m_maxPitch  )
    {
      std::cout << "OUT OF THE CONE " << std::endl;
      std::cout << "m_first" << m_first << std::endl;

      // Find the required yaw angle. The angle returns in radians.

      requiredYaw = std::atan2 (stop.position.y - start.position.y, stop.position.x - start.position.x);
      std::cout << "REQUIRED YAW " << requiredYaw << std::endl;
      std::cout << "bhopoooooooo " << m_Yaw << std::endl;
      /*/Align the robot on the x-y plane. This operation doesn't occur: during the first run (look at variable "m_first")
      and if the required angle ( "requiredYaw" ) is the same as the actual one ( "m_Yaw" )
      /*/
      if (m_first != 0 && m_Yaw != requiredYaw )
        {
          Waypoint rotationyaw = start;
          /*/If I am in a singular point ( when "atan2" has an indeterminate form ) don't change anything.
              In other words, the robot is maintained in the same attitude
              /*/
          if (stop.position.x == start.position.x && stop.position.y == start.position.y )
            {
              requiredYaw = m_Yaw;
            }
          else
            {
              rotationyaw.time = start.time + Seconds (std::abs ((std::abs (requiredYaw) - std::abs (m_Yaw))) / (m_maxOmegaYaw));
              std::cout << "ROTATION YAW TIME " << rotationyaw.time << std::endl;
              //Check rotation time for the yaw angle
              NS_ABORT_MSG_IF (stop.time < rotationyaw.time, "** STOP TIME HAS TO BE > ROTATION YAW TIME");
              ret.push_back (rotationyaw);
              m_Yaw = requiredYaw;
            }
          std::cout << "bhopoooooooo222222222222212 " << requiredYaw << std::endl;
          intermediateStepTime = rotationyaw.time;
        }

      /*/ Set the maximum possible pitch angle. Two subcases:
       if "stop.position.z"-"start.position.z" > 0 the robot has to go down, so the pitch angle is negative.
       The other case is trivial.
       SUBCASE: "stop.position.z"-"start.position.z" > 0
       /*/

      if ( stop.position.z - start.position.z > 0)
        {
          Waypoint rotationpitch = start;
          rotationpitch.time = intermediateStepTime + Seconds (m_maxPitch / (m_maxOmegaPitch));
          std::cout << "ROTATION YAW + PITCH TIME " << rotationpitch.time << std::endl;
          //Check rotation time for the pitch angle
          NS_ABORT_MSG_IF (stop.time < rotationpitch.time, "** STOP TIME HAS TO BE > ROTATION YAW + PITCH TIME ");
          ret.push_back (rotationpitch);
          const double intermediateStepTime2 = (rotationpitch.time).ToDouble (ns3::Time::S);
          const double StopTime = (stop.time).ToDouble (ns3::Time::S);

          /*/ Immerses along the line with slop = -tan(m_maxPitch) and centered in "start point" and find the intersection point
                          with the line centered in "stop point" with slop = +tan(m_maxPitch)
           /*/

          double intersection_xp = (stop.position.z + tan (m_maxPitch) * stop.position.x) * (1 / tan (m_maxPitch) * 1 / 2);
          std::cout << "intersection_xp" << intersection_xp << std::endl;
          double intersection_zp = (stop.position.z + tan (m_maxPitch) * stop.position.x) * 1 / 2;
          std::cout << "intersection_zp" << intersection_zp << std::endl;
          double SpacetoTravel = std::sqrt (intersection_xp * intersection_xp + intersection_zp * intersection_zp);
          double TimetoTravel = SpacetoTravel / m_maxSpeed + intermediateStepTime2;

          Waypoint travel;
          travel.position.x = SpacetoTravel * cos (m_maxPitch) * cos (requiredYaw) + start.position.x;
          travel.position.y = SpacetoTravel * cos (m_maxPitch) * sin (requiredYaw) + start.position.y;
          travel.position.z = SpacetoTravel * sin (m_maxPitch) + start.position.z;
          travel.time = Seconds (TimetoTravel);
          std::cout << "travel.position.x " << travel.position.x << std::endl;
          std::cout << "travel.position.y " << travel.position.y << std::endl;
          std::cout << "travel.position.z " << travel.position.z << std::endl;
          ret.push_back (travel);

          std::cout << "SPACE TO TRAVEL " << SpacetoTravel << std::endl;
          std::cout << "TIME TO TRAVEL " << TimetoTravel << std::endl;

          //Check time to travel
          NS_ABORT_MSG_IF (TimetoTravel > StopTime, "** TIME TO TRAVEL HAS TO BE < STOP TIME");
          std::cout << "AAAAAAAA " << requiredYaw << std::endl;
          //Rotate of 180° and immerses with the same pitch
          requiredYaw2 = std::atan2 (stop.position.y - travel.position.y, stop.position.x - travel.position.x);
          std::cout << "REQUIRED YAW2  " << requiredYaw2 << std::endl;
          Waypoint rotationyaw2 = travel;
          rotationyaw2.time = travel.time + Seconds (3.14 / m_maxOmegaYaw);
          std::cout << "ROTATION YAW2 TIME " << rotationyaw2.time << std::endl;
          //Check rotation time for the yaw angle
          NS_ABORT_MSG_IF (stop.time < rotationyaw2.time, "** STOP TIME HAS TO BE > ROTATION YAW2 TIME");
          ret.push_back (rotationyaw2);
          m_Yaw = requiredYaw2;
          requiredYaw = requiredYaw2;


          /*/ Find that speed that permits you to reach the stop point at the stop time.
           First of all, calculate the module.
           /*/

          double distance = std::sqrt ((stop.position.x - rotationyaw2.position.x) * (stop.position.x - rotationyaw2.position.x) + (stop.position.y - rotationyaw2.position.y) * (stop.position.y - rotationyaw2.position.y) + (stop.position.z - rotationyaw2.position.z) * (stop.position.z - rotationyaw2.position.z));
          std::cout << "DISTANCE " << distance << std::endl;
          /*/Add the time to set the pitch at zero at the end of the cycle. This time reduce "lastStepTime", increasing "required_speed"
          /*/
          const double lastStepTime = (stop.time - rotationyaw2.time - Seconds (m_maxPitch / m_maxOmegaPitch)).ToDouble (ns3::Time::S);
          double required_speed = (distance) / (lastStepTime);
          std::cout << "REQUIRED SPEED " << required_speed << std::endl;

          //Check required speed
          NS_ABORT_MSG_IF (required_speed > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < MAX SPEED");
          ret.push_back (stop);
        }

      /*/ Set the maximum possible pitch angle. Two subcases:
       if "stop.position.z"-"start.position.z" > 0 the robot has to go down, so the pitch angle is negative.
       The other case is trivial.
       SUBCASE: "stop.position.z"-"start.position.z" < 0
       /*/

      else

        {
          Waypoint rotationpitch = start;
          rotationpitch.time = intermediateStepTime + Seconds (m_maxPitch / (m_maxOmegaPitch));
          std::cout << "ROTATION YAW + PITCH TIME " << rotationpitch.time << std::endl;
          //Check rotation time for the pitch angle
          NS_ABORT_MSG_IF (stop.time < rotationpitch.time, "** STOP TIME HAS TO BE > ROTATION YAW + PITCH TIME ");
          ret.push_back (rotationpitch);
          const double intermediateStepTime2 = (rotationpitch.time).ToDouble (ns3::Time::S);
          const double StopTime = (stop.time).ToDouble (ns3::Time::S);

          /*/ Immerses along the line with slop = -tan(m_maxPitch) and centered in "start point" and find the intersection point
                          with the line centered in "stop point" with slop = +tan(m_maxPitch)
           /*/
          double intersection_xp = (stop.position.z + tan (m_maxPitch) * stop.position.x) * (1 / tan (m_maxPitch) * 1 / 2);
          std::cout << "intersection_xp" << intersection_xp << std::endl;
          double intersection_zp = (stop.position.z + tan (m_maxPitch) * stop.position.x) * 1 / 2;
          std::cout << "intersection_zp" << intersection_zp << std::endl;
          double SpacetoTravel = std::sqrt (intersection_xp * intersection_xp + intersection_zp * intersection_zp);
          double TimetoTravel = SpacetoTravel / m_maxSpeed + intermediateStepTime2;

          Waypoint travel;
          travel.position.x = SpacetoTravel * cos (m_maxPitch) * cos (requiredYaw) + start.position.x;
          travel.position.y = SpacetoTravel * cos (m_maxPitch) * sin (requiredYaw) + start.position.y;
          travel.position.z = SpacetoTravel * sin (m_maxPitch) + start.position.z;
          travel.time = Seconds (TimetoTravel);
          std::cout << "travel.position.x " << travel.position.x << std::endl;
          std::cout << "travel.position.y " << travel.position.y << std::endl;
          std::cout << "travel.position.z " << travel.position.z << std::endl;
          ret.push_back (travel);

          std::cout << "SPACE TO TRAVEL " << SpacetoTravel << std::endl;
          std::cout << "TIME TO TRAVEL " << TimetoTravel << std::endl;
          //Check time to travel
          NS_ABORT_MSG_IF (TimetoTravel > StopTime, "** TIME TO TRAVEL HAS TO BE < STOP TIME");
          std::cout << "AAAAAAAA " << requiredYaw << std::endl;
          //Find the coordinates of the point after the travel
          requiredYaw2 = std::atan2 (stop.position.y - travel.position.y, stop.position.x - travel.position.x);
          std::cout << "REQUIRED YAW2  " << requiredYaw2 << std::endl;
          Waypoint rotationyaw2 = travel;
          rotationyaw2.time = travel.time + Seconds (3.14 / m_maxOmegaYaw);
          std::cout << "ROTATION YAW2 TIME " << rotationyaw2.time << std::endl;
          //Check rotation time for the yaw angle
          NS_ABORT_MSG_IF (stop.time < rotationyaw2.time, "** STOP TIME HAS TO BE > ROTATION YAW2 TIME");
          ret.push_back (rotationyaw2);
          m_Yaw = requiredYaw2;
          requiredYaw = requiredYaw2;


          /*/ Find that speed that permits you to reach the stop point at the stop time.
           First of all, calculate the module.
           /*/

          double distance = std::sqrt ((stop.position.x - rotationyaw2.position.x) * (stop.position.x - rotationyaw2.position.x) + (stop.position.y - rotationyaw2.position.y) * (stop.position.y - rotationyaw2.position.y) + (stop.position.z - rotationyaw2.position.z) * (stop.position.z - rotationyaw2.position.z));
          std::cout << "DISTANCE " << distance << std::endl;
          /*/Add the time to set the pitch at zero at the end of the cycle. This time reduce "lastStepTime", increasing "required_speed"
          /*/
          const double lastStepTime = (stop.time - rotationyaw2.time - Seconds (m_maxPitch / m_maxOmegaPitch)).ToDouble (ns3::Time::S);
          double required_speed = (distance) / (lastStepTime);
          std::cout << "REQUIRED SPEED " << required_speed << std::endl;

          //Check required speed
          NS_ABORT_MSG_IF (required_speed > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < MAX SPEED");
          ret.push_back (stop);
        }
    }

  /*/Check if next position is within a cone of amplitude 120° centered in the actual position
    (therefore, if the next position is reachable considering the limitation for the yaw angle).
    SUBCASE: NOT OUT OF THE CONE
  /*/

  else
    {
      // Find the required yaw angle. The angle returns in radians.
      requiredYaw = std::atan2 (stop.position.y - start.position.y, stop.position.x - start.position.x);
      std::cout << "REQUIRED YAW " << requiredYaw << std::endl;
      //Align the robot on the x-y plane
      if (m_first != 0 && m_Yaw != requiredYaw )
        {
          Waypoint rotationyaw = start;
          std::cout << "m_Yaw1111" << m_Yaw << std::endl;
          rotationyaw.time = start.time + Seconds (std::abs ((std::abs (requiredYaw) - std::abs (m_Yaw))) / (m_maxOmegaYaw));
          std::cout << "ROTATION YAW TIME " << rotationyaw.time << std::endl;
          //Check rotation time for the yaw angle
          NS_ABORT_MSG_IF (stop.time < rotationyaw.time, "** STOP TIME HAS TO BE > ROTATION YAW TIME");
          ret.push_back (rotationyaw);
          intermediateStepTime = rotationyaw.time;

        }
      if ( requiredPitch != 0 )
        {
          //Since the robot is already aligned towards the next point, the required pitch is always ranged between ( -60°; 60° )
          requiredPitch = std::min (std::abs (requiredPitch),std::abs (requiredPitch - 3.14));
          //Find the necessary time for the pitch angle
          Waypoint rotationpitch = start;
          /*/Add the time to set newly the pitch as zero as well. In theory, it should be add at the start of the new cycle.
           On the other hand, as first approach, we can consider two times this value at this point. (Note that this is an assumption).
           /*/
          rotationpitch.time = intermediateStepTime + Seconds (std::abs (requiredPitch) / (m_maxOmegaPitch));
          intermediateStepTime = rotationpitch.time;
          std::cout << "ROTATION YAW + PITCH TIME " << rotationpitch.time << std::endl;
          //Check rotation time for the pitch angle
          NS_ABORT_MSG_IF (stop.time < rotationpitch.time, "** STOP TIME HAS TO BE > ROTATION YAW + PITCH TIME ");
          ret.push_back (rotationpitch);

        }
      // Find that speed that permits you to reach the stop point at the stop time. First of all, calculate the module
      double distance = std::sqrt ((stop.position.x - start.position.x) * (stop.position.x - start.position.x) + (stop.position.y - start.position.y) * (stop.position.y - start.position.y) + (stop.position.z - start.position.z) * (stop.position.z - start.position.z));
      std::cout << "DISTANCE " << distance << std::endl;
      const double lastStepTime = (stop.time - intermediateStepTime - Seconds (std::abs (requiredPitch) / (m_maxOmegaPitch))).ToDouble (ns3::Time::S);
      std::cout << "lastStepTime " << lastStepTime << std::endl;
      double required_speed = (distance) / (lastStepTime);
      std::cout << "REQUIRED SPEED " << required_speed << std::endl;
      m_Yaw = requiredYaw;
      //Check required speed
      NS_ABORT_MSG_IF (required_speed > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < MAX SPEED");
      ret.push_back (stop);

    }

/*/  Variable "m_first" is set at true .
 Therefore, apart from the first run, the program has to check whether or not the robot is aligned with the next waypoint.
/*/

  m_first = 1;
  std::cout << "m_Yaw !!!!!" << m_Yaw << std::endl;
  return ret;

}


} // namespace ns3
