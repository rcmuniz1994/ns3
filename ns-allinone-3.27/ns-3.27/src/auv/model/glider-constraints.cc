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

#include "glider-constraints.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/nstime.h"
#include "ns3/assert.h"
#include "ns3/abort.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("GliderConstraints");

NS_OBJECT_ENSURE_REGISTERED (GliderConstraints);

TypeId
GliderConstraints::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::GliderConstraints")
    .SetParent (KinematicConstraints::GetTypeId ())
    .SetGroupName ("Auv")
    .AddConstructor<GliderConstraints> ()
    .AddAttribute ("MaxDepth", "The maximum operational depth, in m",
                   DoubleValue (1000.0),
                   MakeDoubleAccessor (&GliderConstraints::m_maxDepth),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxSpeed", "The maximum speed of the vehicle, in m/s.",
                   DoubleValue (0.5),
                   MakeDoubleAccessor (&GliderConstraints::m_maxSpeed),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxOmegaYaw", "The maximum omega (along z-axis) of the vehicle, in radians/s.",
                   DoubleValue (0.3),
                   MakeDoubleAccessor (&GliderConstraints::m_maxOmegaYaw),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxPitch", "The maximum pitch angle of the vehicle (absolute value = 60°) , in radians.",
                   DoubleValue (1.045),
                   MakeDoubleAccessor (&GliderConstraints::m_maxPitch),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MinPitch", "The minimum pitch angle of the vehicle (absolute value = 14°) , in radians.",
                   DoubleValue (0.24),
                   MakeDoubleAccessor (&GliderConstraints::m_minPitch),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("MaxBuoyancy", "The maximum buoyancy supported by the glider buoyancy control system, in grammes",
                   DoubleValue (200), // derived from the paper
                   MakeDoubleAccessor (&GliderConstraints::m_maxBuoyancy),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("aBuoyancy", "First param for buoyancy calculus",
                   DoubleValue (0.0022436), // derived from the paper
                   MakeDoubleAccessor (&GliderConstraints::m_aBuoyancy),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("bBuoyancy", "Second param for buoyancy calculus",
                   DoubleValue (0.01249), // derived from the paper
                   MakeDoubleAccessor (&GliderConstraints::m_bBuoyancy),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("cBuoyancy", "Third param for buoyancy calculus",
                   DoubleValue (9.8016e-6), // derived from the paper
                   MakeDoubleAccessor (&GliderConstraints::m_cBuoyancy),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("rho", "Water density kg/m^3",
                   DoubleValue (1023),
                   MakeDoubleAccessor (&GliderConstraints::m_rho),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("l", "seaglider hull length, in m",
                   DoubleValue (1.8),
                   MakeDoubleAccessor (&GliderConstraints::m_l),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

/*/Initialize m_first (support variable) at true. We suppose that, during the first step,
the robot is aligned with the next waypoint, therefore we don't want to have any rotation during the first run.
/*/
GliderConstraints::GliderConstraints ()
  : m_first (0)
{
}

GliderConstraints::~GliderConstraints ()
{
}


double requiredYaw;
Time intermediateStepTime = Seconds (0);

double GliderConstraints::GetBuoyancy (double U, double W) const
{
  // here we get the bouyancy needed to maintain
  // the given velocity at the given glide angle
  double a = m_aBuoyancy;
  double b = m_bBuoyancy;
  double c = m_cBuoyancy;
  // v^2=U^2+W^2
  double v = std::sqrt (U * U + W * W);

  // dynamic pressure
  double q = 0.5 * m_rho * (v * v);

  double tgteta = W / U;
  double teta = std::atan (tgteta);

  double lambda = (a * a) / (b * (1 / std::pow (q, 0.25)) * c);

  double temp = (q * (m_l * m_l) * (a * a) * std::sin (teta)) /
    (2 * c * (std::cos (teta) * std::cos (teta)));
  // positive sqrt solution for Buoyancy
  // double Bp = temp*(1 + std::sqrt(1 - 4/(lambda*(tgteta * tgteta))));
  // negative sqrt solution for Buoyancy
  double Bn = temp * (1 - std::sqrt (1 + 4 / (lambda * (tgteta * tgteta))));
  std::cout << "Bn* 100 " << Bn * 100 << std::endl;
  // the negative solution is returned, as the more efficient, as said into the article
  // TODO discrepancies with paper values
  // The value is in grammes [g]
  return Bn * 100;
}


std::list<Waypoint>
GliderConstraints::CheckPath (const Waypoint &start, const Waypoint &stop)
{
  std::list<Waypoint> ret;

  //Check if the required depth is negative
  NS_ABORT_MSG_IF (stop.position.z < 0, "** THE DEPTH HAS TO  NON-NEGATIVE");
  //Check if the required depth is minor than the the maximum operational depth
  NS_ABORT_MSG_IF (stop.position.z > m_maxDepth, "** REQUIRED DEPTH HAS TO BE < MAXIMUM OPERATIONAL DEPTH");
/*/Check the required yaw angle. Since in the first run "m_first" is zero, any rotation occurs during the first run
the robot is aligned with the next waypoint, therefore we don't want to have any rotation during the first run.
/*/
  requiredYaw = std::atan2 (stop.position.y - start.position.y, stop.position.x - start.position.x);
  //std::cout << "REQUIRED YAW " << requiredYaw << std::endl;
  int case1 = 0;
  int case2 = 0;
  int case3 = 0;
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
          rotationyaw.time = Seconds (std::abs ((std::abs (requiredYaw) - std::abs (m_Yaw))) / (m_maxOmegaYaw));
          //std::cout << "ROTATION YAW TIME " << rotationyaw.time << std::endl;
          //Check rotation time for the yaw angle
          NS_ABORT_MSG_IF (stop.time < rotationyaw.time, "** STOP TIME HAS TO BE > ROTATION YAW TIME");
          ret.push_back (rotationyaw);
          m_Yaw = requiredYaw;
        }
      intermediateStepTime = rotationyaw.time;
    }

/*/Four subcases arise:
1 start point and stop point at the same depth
2 start point and stop point in the same vertical line
3 not case 1 or 2 and stop point is reachable through a straight line
4 not case 1,2 or 3
/*/

//Case 1: Pitch set as the minimum

  if (start.position.z == stop.position.z)

    {
      double module = std::sqrt ((stop.position.x - start.position.x) * (stop.position.x - start.position.x) + (stop.position.y - start.position.y) * (stop.position.y - start.position.y) + (stop.position.z - start.position.z) * (stop.position.z - start.position.z));
      //std::cout << "module " << module << std::endl;
      Waypoint down;
      down.position.x = start.position.x + 0.5 * module * cos (requiredYaw);
      down.position.y = start.position.y + 0.5 * module * sin (requiredYaw);
      down.position.z = start.position.z + 0.5 * module * tan (m_minPitch);
      down.time = (start.time + (stop.time - (start.time + intermediateStepTime)) / 2);
      ret.push_back (down);
      //std::cout << "DOWN1 " << down << std::endl;
      const double duration = (stop.time - (start.time + intermediateStepTime)).ToDouble (ns3::Time::S);
      double module2 = 2 * std::sqrt ((down.position.x - start.position.x) * (down.position.x - start.position.x) + (down.position.y - start.position.y) * (down.position.y - start.position.y) + (down.position.z - start.position.z) * (down.position.z - start.position.z));
      //std::cout << "module2 " << module2 << std::endl;
      double required_speed = module2 / (duration);
      //std::cout << "required_speed " << required_speed << std::endl;
      NS_ABORT_MSG_IF (required_speed > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < m_maxSpeed");
      double buoyancy = GetBuoyancy (required_speed * cos (m_minPitch), required_speed * sin (m_minPitch));
      //std::cout << "required_speed*cos(m_minPitch) " << required_speed * cos (m_minPitch) << std::endl;
      //std::cout << "required_speed*sin(m_minPitch) " << required_speed * sin (m_minPitch) << std::endl;
      //std::cout << "buoyancy 1 " << buoyancy << std::endl;
      NS_ABORT_MSG_IF (buoyancy > m_maxBuoyancy, "** REQUIRED BUOYANCY HAS TO BE < m_maxBuoyancy");
      case1 = 1;
      m_Yaw = requiredYaw;
    }

//Case 2: Pitch set as the maximum
  if (start.position.x == stop.position.x && start.position.y == stop.position.y )
    {
      double module = std::sqrt ((stop.position.x - start.position.x) * (stop.position.x - start.position.x) + (stop.position.y - start.position.y) * (stop.position.y - start.position.y) + (stop.position.z - start.position.z) * (stop.position.z - start.position.z));

      if (stop.position.z > start.position.z)
        {
          Waypoint down2;
          down2.position.x = start.position.x + 0.5 * module * cos (requiredYaw) * 1 / (tan (m_maxPitch));
          down2.position.y = start.position.y + 0.5 * module * sin (requiredYaw) * 1 / (tan (m_maxPitch));
          down2.position.z = start.position.z + module * 0.5;
          down2.time = (start.time + (stop.time - (start.time + intermediateStepTime)) / 2);
          ret.push_back (down2);
          Waypoint down3 = down2;
          //std::cout << "DOWN2 " << down2 << std::endl;
          down3.time = down2.time + Seconds (3.14 / m_maxOmegaYaw);
          ret.push_back (down3);
          //std::cout << "DOWN3 " << down3 << std::endl;
          const double duration = (down2.time - (start.time + intermediateStepTime)).ToDouble (ns3::Time::S);
          double module2 = std::sqrt ((down2.position.x - start.position.x) * (down2.position.x - start.position.x) + (down2.position.y - start.position.y) * (down2.position.y - start.position.y) + (down2.position.z - start.position.z) * (down2.position.z - start.position.z));
          //std::cout << "module2 " << module2 << std::endl;
          double required_speed = module2 / (duration);
          //std::cout << "required_speed " << required_speed << std::endl;
          //std::cout << "required_speed*cos(m_maxPitch) " << required_speed * cos (m_maxPitch) << std::endl;
          //std::cout << "required_speed*sin(m_maxPitch) " << required_speed * sin (m_maxPitch) << std::endl;
          NS_ABORT_MSG_IF (required_speed > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < m_maxSpeed");
          double buoyancy = GetBuoyancy (required_speed * cos (m_maxPitch), required_speed * sin (m_maxPitch));
          const double duration2 = (stop.time - (down3.time)).ToDouble (ns3::Time::S);
          NS_ABORT_MSG_IF (duration2 < 0, "** STOP TIME HAS TO BE > REQUIRED TIME");
          //std::cout << "module2 " << module2 << std::endl;
          required_speed = module2 / (duration2);
          NS_ABORT_MSG_IF (required_speed > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < m_maxSpeed");
          //std::cout << "required_speed*cos(m_maxPitch) " << required_speed * cos (m_maxPitch) << std::endl;
          //std::cout << "required_speed*sin(m_maxPitch) " << required_speed * sin (m_maxPitch) << std::endl;
          //std::cout << "buoyancy 2 " << buoyancy << std::endl;
          NS_ABORT_MSG_IF (buoyancy > m_maxBuoyancy, "** REQUIRED BUOYANCY HAS TO BE < m_maxBuoyancy");
          m_Yaw = requiredYaw + 3.14;
        }
      else
        {
          Waypoint down2;
          down2.position.x = start.position.x + 0.5 * module * cos (requiredYaw) * 1 / (tan (m_maxPitch));
          down2.position.y = start.position.y + 0.5 * module * sin (requiredYaw) * 1 / (tan (m_maxPitch));
          down2.position.z = start.position.z - module * 0.5;
          down2.time = (start.time + (stop.time - (start.time + intermediateStepTime)) / 2);
          ret.push_back (down2);
          Waypoint down3 = down2;
          //std::cout << "DOWN2 " << down2 << std::endl;
          down3.time = down2.time + Seconds (3.14 / m_maxOmegaYaw);
          ret.push_back (down3);
          //std::cout << "DOWN3 " << down3 << std::endl;
          const double duration = (down2.time - (start.time + intermediateStepTime)).ToDouble (ns3::Time::S);
          double module2 = std::sqrt ((down2.position.x - start.position.x) * (down2.position.x - start.position.x) + (down2.position.y - start.position.y) * (down2.position.y - start.position.y) + (down2.position.z - start.position.z) * (down2.position.z - start.position.z));
          //std::cout << "module2 " << module2 << std::endl;
          double required_speed = module2 / (duration);
          //std::cout << "required_speed " << required_speed << std::endl;
          //std::cout << "required_speed*cos(m_maxPitch) " << required_speed * cos (m_maxPitch) << std::endl;
          //std::cout << "required_speed*sin(m_maxPitch) " << required_speed * sin (m_maxPitch) << std::endl;
          NS_ABORT_MSG_IF (required_speed > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < m_maxSpeed");
          double buoyancy = GetBuoyancy (required_speed * cos (m_maxPitch), required_speed * sin (m_maxPitch));
          const double duration2 = (stop.time - (down3.time)).ToDouble (ns3::Time::S);
          NS_ABORT_MSG_IF (duration2 < 0, "** STOP TIME HAS TO BE > REQUIRED TIME");
          //std::cout << "module2 " << module2 << std::endl;
          required_speed = module2 / (duration2);
          NS_ABORT_MSG_IF (required_speed > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < m_maxSpeed");
          //std::cout << "required_speed*cos(m_maxPitch) " << required_speed * cos (m_maxPitch) << std::endl;
          //std::cout << "required_speed*sin(m_maxPitch) " << required_speed * sin (m_maxPitch) << std::endl;
          //std::cout << "buoyancy 2 " << buoyancy << std::endl;
          NS_ABORT_MSG_IF (buoyancy > m_maxBuoyancy, "** REQUIRED BUOYANCY HAS TO BE < m_maxBuoyancy");
          m_Yaw = requiredYaw + 3.14;
        }
      case2 = 1;
    }

//Case 3

  double requiredPitch = std::atan2 (stop.position.z - start.position.z, stop.position.x - start.position.x);
  //std::cout << "REQUIRED PITCH " << requiredPitch << std::endl;
  //std::cout << "case1 " << case1 << std::endl;
  //std::cout << "case2 " << case2 << std::endl;
//Check if the required pitch is acceptable given MaxPitch and MinPitch. If it is, the robot goes straight toward the stop point
  if (case1 != 1 && case2 != 1)
    {
      std::cout << "QUIIIIII3 " << std::endl;
      if (((requiredPitch > m_minPitch) && (requiredPitch < m_maxPitch)) || ((requiredPitch < -m_minPitch) && (requiredPitch > -m_maxPitch)) || ((requiredPitch < 3.14 - m_minPitch) && (requiredPitch > 3.14 -  m_maxPitch)) || ((requiredPitch > 3.14 + m_minPitch) && (requiredPitch < 3.14 + m_maxPitch)))
        {
          double module = std::sqrt ((stop.position.x - start.position.x) * (stop.position.x - start.position.x) + (stop.position.y - start.position.y) * (stop.position.y - start.position.y) + (stop.position.z - start.position.z) * (stop.position.z - start.position.z));
          //std::cout << "QUIIIIII3bis " << std::endl;
          const double duration = (stop.time - (start.time + intermediateStepTime)).ToDouble (ns3::Time::S);
          double required_speed = module / (duration);
          NS_ABORT_MSG_IF (required_speed > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < m_maxSpeed");
          double buoyancy = GetBuoyancy (required_speed * cos (requiredPitch), required_speed * sin (requiredPitch));
          //std::cout << "buoyancy 3 " << buoyancy << std::endl;
          NS_ABORT_MSG_IF (buoyancy > m_maxBuoyancy, "** REQUIRED BUOYANCY HAS TO BE < m_maxBuoyancy");
          case3 = 1;
        }
    }

//Case 4
  if (case1 != 1 && case2 != 1 && case3 != 1 )
    {

      if (((requiredPitch > -m_minPitch) && (requiredPitch < m_minPitch)) || ((requiredPitch > m_maxPitch) && (requiredPitch < 3.14 - m_maxPitch)) || ((requiredPitch > 3.14 - m_minPitch) && (requiredPitch > 3.14 +  m_minPitch)) || ((requiredPitch > 3.14 + m_maxPitch) && (requiredPitch < 3.14 + 2 * m_maxPitch)))

        {
//module bis is the distance in the x-y plane between start point and stop point
          double module_bis = std::sqrt ((stop.position.x - start.position.x) * (stop.position.x - start.position.x) + (stop.position.y - start.position.y) * (stop.position.y - start.position.y));

          if (stop.position.z > start.position.z)
            {
              Vector3D temp;
              temp.x = start.position.x;
              temp.y = start.position.y;
              temp.z = stop.position.z + module_bis * tan (3.14 / 2 - m_maxPitch);
              double module = std::sqrt ((temp.x - start.position.x) * (temp.x - start.position.x) + (temp.y - start.position.y) * (temp.y - start.position.y) + (temp.z - start.position.z) * (temp.z - start.position.z));

//distance between start point and down point
              double distance = module * sin (m_maxPitch);
//distance2 is the distance between down point and the z-axis
              double distance2 = distance * cos (m_maxPitch);
//Find the intermediate waypoint ("down")
/*/
The point is on the intersection between:
- the line the starts from "start point" with slope m_maxPitch (dawnwards)
- the line that passes on "stop point" and it is perpendicular respect to the previous one

The "down time" is supposed to be in the middle between "start time" and "stop time" (if necessary the time to set the yaw angle is considered)
/*/
              Waypoint down;
              down.position.x = start.position.x + distance2 * cos (requiredYaw);
              down.position.y = start.position.y + distance2 * sin (requiredYaw);
              down.position.z = start.position.z + distance * sin (m_maxPitch);
              down.time = (start.time + (stop.time - (start.time + intermediateStepTime)) / 2);
              ret.push_back (down);
              //std::cout << "DOWN " << down << std::endl;
              Waypoint downyaw = down;
//Rotate the robot on the x-y plane of 180° (yaw angle)
              downyaw.time = down.time + Seconds (3.14 / m_maxOmegaYaw);
              ret.push_back (downyaw);
              //std::cout << "DOWNYAW " << downyaw << std::endl;
//Find the time to reach "down point"
              const double duration = (down.time - (start.time + intermediateStepTime)).ToDouble (ns3::Time::S);
// module2 is the distance between "down point" and "start point"
              double module2 = std::sqrt ((down.position.x - start.position.x) * (down.position.x - start.position.x) + (down.position.y - start.position.y) * (down.position.y - start.position.y) + (down.position.z - start.position.z) * (down.position.z - start.position.z));
              //std::cout << "module2 " << module2 << std::endl;
//Check the required speed
              double required_speed = module2 / (duration);
              //std::cout << "required_speed " << required_speed << std::endl;
              //std::cout << "required_speed*cos(m_maxPitch) " << required_speed * cos (m_maxPitch) << std::endl;
              //std::cout << "required_speed*sin(m_maxPitch) " << required_speed * sin (m_maxPitch) << std::endl;
              NS_ABORT_MSG_IF (required_speed > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < m_maxSpeed");
              double buoyancy = GetBuoyancy (required_speed * cos (m_maxPitch), required_speed * sin (m_maxPitch));
//Find the time to reach "stop point"
              const double duration2 = (stop.time - (downyaw.time)).ToDouble (ns3::Time::S);
              NS_ABORT_MSG_IF (duration2 < 0, "** STOP TIME HAS TO BE > REQUIRED TIME");
// module3 here is the distance between down point and stop point
              double module3 = std::sqrt ((down.position.x - stop.position.x) * (down.position.x - stop.position.x) + (down.position.y - stop.position.y) * (down.position.y - stop.position.y) + (down.position.z - stop.position.z) * (down.position.z - stop.position.z));
              //std::cout << "module3 " << module3 << std::endl;
              double required_speed2 = module3 / (duration2);
              NS_ABORT_MSG_IF (required_speed2 > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < m_maxSpeed");
              //std::cout << "required_speed2 " << required_speed2 << std::endl;
              //std::cout << "buoyancy 2 " << buoyancy << std::endl;
              NS_ABORT_MSG_IF (buoyancy > m_maxBuoyancy, "** REQUIRED BUOYANCY HAS TO BE < m_maxBuoyancy");
              m_Yaw = requiredYaw + 3.14;
            }
          else
            {
              Vector3D temp;
              temp.x = start.position.x;
              temp.y = start.position.y;
              temp.z = stop.position.z - module_bis * tan (3.14 / 2 - m_maxPitch);
              double module = std::sqrt ((temp.x - start.position.x) * (temp.x - start.position.x) + (temp.y - start.position.y) * (temp.y - start.position.y) + (temp.z - start.position.z) * (temp.z - start.position.z));

//distance between start point and down point
              double distance = module * sin (m_maxPitch);
//distance2 is the distance between down point and the z-axis
              double distance2 = distance * cos (m_maxPitch);
//Find the intermediate waypoint ("down")
/*/
The point is on the intersection between:
- the line the starts from "start point" with slope m_maxPitch (upwards)
- the line that passes on "stop point" and it is perpendicular respect to the previous one

The "down time" is supposed to be in the middle between "start time" and "stop time" (if necessary the time to set the yaw angle is considered)
/*/
              Waypoint down;
              down.position.x = start.position.x + distance2 * cos (requiredYaw);
              down.position.y = start.position.y + distance2 * sin (requiredYaw);
              down.position.z = start.position.z - distance * sin (m_maxPitch);
              down.time = (start.time + (stop.time - (start.time + intermediateStepTime)) / 2);
              ret.push_back (down);
              //std::cout << "DOWN " << down << std::endl;
              Waypoint downyaw = down;
//Rotate the robot on the x-y plane of 180° (yaw angle)
              downyaw.time = down.time + Seconds (3.14 / m_maxOmegaYaw);
              ret.push_back (downyaw);
              //std::cout << "DOWNYAW " << downyaw << std::endl;
//Find the time to reach "down point"
              const double duration = (down.time - (start.time + intermediateStepTime)).ToDouble (ns3::Time::S);
// module2 is the distance between "down point" and "start point"
              double module2 = std::sqrt ((down.position.x - start.position.x) * (down.position.x - start.position.x) + (down.position.y - start.position.y) * (down.position.y - start.position.y) + (down.position.z - start.position.z) * (down.position.z - start.position.z));
              //std::cout << "module2 " << module2 << std::endl;
//Check the required speed
              double required_speed = module2 / (duration);
              //std::cout << "required_speed " << required_speed << std::endl;
              //std::cout << "required_speed*cos(m_maxPitch) " << required_speed * cos (m_maxPitch) << std::endl;
              //std::cout << "required_speed*sin(m_maxPitch) " << required_speed * sin (m_maxPitch) << std::endl;
              NS_ABORT_MSG_IF (required_speed > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < m_maxSpeed");
              double buoyancy = GetBuoyancy (required_speed * cos (m_maxPitch), required_speed * sin (m_maxPitch));
//Find the time to reach "stop point"
              const double duration2 = (stop.time - (downyaw.time)).ToDouble (ns3::Time::S);
              NS_ABORT_MSG_IF (duration2 < 0, "** STOP TIME HAS TO BE > REQUIRED TIME");
// module2 here is the distance between down point and stop point
              double module3 = std::sqrt ((down.position.x - stop.position.x) * (down.position.x - stop.position.x) + (down.position.y - stop.position.y) * (down.position.y - stop.position.y) + (down.position.z - stop.position.z) * (down.position.z - stop.position.z));
              //std::cout << "module3 " << module3 << std::endl;
              double required_speed2 = module3 / (duration2);
              NS_ABORT_MSG_IF (required_speed2 > m_maxSpeed, "** REQUIRED SPEED HAS TO BE < m_maxSpeed");
              //std::cout << "required_speed*cos(m_maxPitch) " << required_speed * cos (m_maxPitch) << std::endl;
              //std::cout << "required_speed*sin(m_maxPitch) " << required_speed * sin (m_maxPitch) << std::endl;
              //std::cout << "buoyancy 2 " << buoyancy << std::endl;
              NS_ABORT_MSG_IF (buoyancy > m_maxBuoyancy, "** REQUIRED BUOYANCY HAS TO BE < m_maxBuoyancy");
              m_Yaw = requiredYaw + 3.14;
            }
        }


    }

  ret.push_back (stop);

//  Variable "m_first" is set at true . Therefore, apart from the first run, the program has to check whether or not the robot is aligned with the next waypoint.

  m_first = 1;
  m_Yaw = requiredYaw;
//Set case1, case2, case3  at zero value
  case1 = 0;
  case2 = 0;
  case3 = 0;
  return ret;


}




} // namespace ns3
