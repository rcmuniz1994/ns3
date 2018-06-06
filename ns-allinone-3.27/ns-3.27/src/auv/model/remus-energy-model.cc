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
#include "auv-waypoint-mobility-model.h"
#include "remus-energy-model.h"


NS_LOG_COMPONENT_DEFINE ("RemusEnergyModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RemusEnergyModel);

TypeId
RemusEnergyModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RemusEnergyModel")
    .SetParent<DeviceEnergyModel> ()
    .SetGroupName ("Auv")
    .AddConstructor<RemusEnergyModel> ()
    .AddAttribute ("PowerSpeedRatio",
                   "Consumed power ratio with respect to motor speed",
                   DoubleValue (83.0), // in W*s/m
                   MakeDoubleAccessor (&RemusEnergyModel::m_powerSpeedRatio),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("TotalEnergyConsumption",
                     "Total energy consumption of the radio device.",
                     MakeTraceSourceAccessor (&RemusEnergyModel::m_totalEnergyConsumption),
                     "ns3::TracedValueCallback::Double")
    .AddAttribute ("BatteryCapacity", "The capacity of the battery, in J.",
                   DoubleValue (5400000),
                   MakeDoubleAccessor (&RemusEnergyModel::m_BatteryCapacity),
                   MakeDoubleChecker<double> ())

  ;
  return tid;


}

RemusEnergyModel::RemusEnergyModel ()
{
  NS_LOG_FUNCTION (this);
  m_currentSpeed = 0.0;
  m_lastUpdateTime = Seconds (0.0);
  m_energyDepletionCallback.Nullify ();
  m_energyRechargedCallback.Nullify ();
  m_node = NULL;
  m_source = NULL;
  m_VelocityModule = 0;
  m_totalEnergyConsumption = 0;
  m_CountCycle = 0;

}

RemusEnergyModel::~RemusEnergyModel ()
{
  m_energyDepletionCallback.Nullify ();
  m_energyRechargedCallback.Nullify ();
  m_node = NULL;
  m_source = NULL;

}
double PowerRemus;
double currentPowerRemus;


void
RemusEnergyModel::NotifyCourseChanged (Ptr<const MobilityModel>model)
{

  //Get the speed of the robot (3D Vector)

  m_velocity = model->GetVelocity ();

  //Get the module of the speed
  m_VelocityModule = std::sqrt (m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y + m_velocity.z * m_velocity.z);
  std::cout << "m_VelocityModule_______ " << m_VelocityModule << std::endl;
  /*/ The power needed [W]:

 - during the vertical motion is 700 W (speed = 0,3 m/s)
 - during the horizontal motion is 300 W (speed = 1 m/s)

 A linear dependency between power and velocity is supposed.

   /*/

  // If m_velocity.z is zero, the robot is moving on the plane, otherwise it's ascending or descending


  PowerRemus =  m_VelocityModule * m_powerSpeedRatio;



  //Initialize duration at zero seconds

  Time duration = Seconds (0);

  /*/Calculate the length of a time slot. A time slot is an interval of time where the speed vector is constant.
   * Remember that the function NotifyCourseChanged is called every time that the speed vector changes.
  /*/

  if ( m_CountCycle != 0)
    {
      duration = Simulator::Now () - m_lastUpdateTime;
    }

  // Check if duration is valid

  NS_ASSERT (duration.GetSeconds () >= 0);

  //Get the supply voltage (V)

  double supplyVoltage = m_source->GetSupplyVoltage ();

  //Calculate the actual current (A)

  m_actualCurrentDrain = currentPowerRemus / supplyVoltage;

  // Notify energy source

  m_source->UpdateEnergySource ();

  // Some debug message
  NS_LOG_DEBUG ("RemusEnergyModel:Total energy consumption at node #" <<
                m_node->GetId () << " is " << m_totalEnergyConsumption << "J");

  //Calculate the energy in every time slot. A time slot is an interval of time where the power is constant (the speed vector is constant)

  double energyToDecrease;
  std::cout << "currentPower " << currentPowerRemus << std::endl;
  std::cout << "duration " << duration << std::endl;

  energyToDecrease = duration.GetSeconds () *  currentPowerRemus;

  // Update the total energy consumption

  m_totalEnergyConsumption += energyToDecrease;

  std::cout << "m_totalEnergyConsumption " << m_totalEnergyConsumption << std::endl;

  // Check if the required energy is less than the capacity of the battery

  NS_ABORT_MSG_IF (m_totalEnergyConsumption > m_BatteryCapacity, "** REQUIRED DENERGY HAS TO BE < BATTERY CAPACITY ");
  //Update the variable "m_lastUpdateTime"

  if ( m_CountCycle != 0)
    {
      m_lastUpdateTime = Simulator::Now ();
    }

  //Update the variable the absorbed power

  m_CountCycle = 1 + m_CountCycle;
  if ( m_CountCycle != 0)
    {
      currentPowerRemus = PowerRemus;
    }
}

void
RemusEnergyModel::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);
  NS_ASSERT (node != NULL);
  m_node = node;
}

Ptr<Node>
RemusEnergyModel::GetNode (void) const
{
  return m_node;
}

void
RemusEnergyModel::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);
  m_source = source;
}

double
RemusEnergyModel::GetTotalEnergyConsumption (void) const
{
  NS_LOG_FUNCTION (this);

  return m_totalEnergyConsumption;
}

void
RemusEnergyModel::ChangeState (int newState)
{
  NS_FATAL_ERROR ("ChangeState not implemented, use ChangeEnergyConsumption instead.");
}

void
RemusEnergyModel::SetEnergyDepletionCallback (RemusEnergyDepletionCallback callback)
{
  NS_LOG_FUNCTION (this);
  if (callback.IsNull ())
    {
      NS_LOG_DEBUG ("RemusEnergyModel:Setting NULL energy depletion callback!");
    }
  m_energyDepletionCallback = callback;
}

void
RemusEnergyModel::SetEnergyRechargedCallback (RemusEnergyRechargedCallback callback)
{
  NS_LOG_FUNCTION (this);
  if (callback.IsNull ())
    {
      NS_LOG_DEBUG ("RemusEnergyModel:Setting NULL energy recharged callback!");
    }
  m_energyRechargedCallback = callback;
}

void
RemusEnergyModel::HandleEnergyDepletion (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("RemusEnergyModel:Energy is depleted at node #" <<
                m_node->GetId ());

  // invoke energy depletion callback, if set.
  if (!m_energyDepletionCallback.IsNull ())
    {
      m_energyDepletionCallback ();
    }
}

void
RemusEnergyModel::HandleEnergyRecharged (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("RemusEnergyModel:Energy is recharged at node #" <<
                m_node->GetId ());

  if (!m_energyRechargedCallback.IsNull ())
    {
      m_energyRechargedCallback ();
    }
}


void
RemusEnergyModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_node = NULL;
  m_source = NULL;
  m_energyDepletionCallback.Nullify ();
}

double
RemusEnergyModel::DoGetCurrentA (void) const
{
  NS_LOG_FUNCTION (this);

  return m_actualCurrentDrain;

}


} // namespace ns3
