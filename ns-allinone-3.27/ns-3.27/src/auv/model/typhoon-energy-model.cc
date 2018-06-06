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

#include "auv-waypoint-mobility-model.h"
#include "typhoon-energy-model.h"


NS_LOG_COMPONENT_DEFINE ("TyphoonEnergyModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TyphoonEnergyModel);

TypeId
TyphoonEnergyModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TyphoonEnergyModel")
    .SetParent<DeviceEnergyModel> ()
    .SetGroupName ("Auv")
    .AddConstructor<TyphoonEnergyModel> ()
    .AddAttribute ("PowerSpeedRatioLongitudinal",
                   "Consumed power ratio with respect to motor speed (the motion is horizontal)",
                   DoubleValue (110.0),                      // in W*(s/m)^3
                   MakeDoubleAccessor (&TyphoonEnergyModel::m_powerSpeedRatioHorizontal),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("PowerSpeedRatioVertical",
                   "Consumed power ratio with respect to motor speed",
                   DoubleValue (1200),                       // in W*(s/m)^3
                   MakeDoubleAccessor (&TyphoonEnergyModel::m_powerSpeedRatioVertical),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("BatteryCapacity", "The capacity of the battery, in J.",
                   DoubleValue (4320000),
                   MakeDoubleAccessor (&TyphoonEnergyModel::m_BatteryCapacity),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("TotalEnergyConsumption",
                     "Total energy consumption of the radio device.",
                     MakeTraceSourceAccessor (&TyphoonEnergyModel::m_totalEnergyConsumption),
                     "ns3::TracedValueCallback::Double")

  ;
  return tid;
}

TyphoonEnergyModel::TyphoonEnergyModel ()
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
  m_firstUpdate = true;
}

TyphoonEnergyModel::~TyphoonEnergyModel ()
{
  m_energyDepletionCallback.Nullify ();
  m_energyRechargedCallback.Nullify ();
  m_node = NULL;
  m_source = NULL;

}

double Power;
double currentPower;


void
TyphoonEnergyModel::NotifyCourseChanged (Ptr<const MobilityModel>model)
{

  m_velocity = model->GetVelocity ();
  m_VelocityModule = std::sqrt (m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y + m_velocity.z * m_velocity.z);

  //  The power needed [W]:
  // - during the vertical motion is 700 W (speed = 0,3 m/s)
  // - during the horizontal motion is 300 W (speed = 1 m/s)
  //
  // A dependency between power and speed^3 is supposed.

  if (m_velocity.z == 0 )
    {
      Power =  m_powerSpeedRatioHorizontal * pow(m_VelocityModule, 3.0);
    }
  else
    {
      Power =  m_powerSpeedRatioVertical * pow(m_VelocityModule, 3.0);
    }

  Time duration = Seconds (0);

  //  Calculate the length of a time slot. A time slot is an interval of time where the speed vector is constant.
  //  Remember that the function NotifyCourseChanged is called every time that the speed vector changes.
  if (m_firstUpdate == false)
    {
      duration = Simulator::Now () - m_lastUpdateTime;
    }

  // Check if duration is valid

  NS_ASSERT (duration.GetSeconds () >= 0);

  double supplyVoltage = m_source->GetSupplyVoltage ();
  m_actualCurrentDrain = currentPower / supplyVoltage;

  m_source->UpdateEnergySource ();

  NS_LOG_DEBUG ("TyphoonEnergyModel:Total energy consumption at node #" <<
                m_node->GetId () << " is " << m_totalEnergyConsumption << "J");

  //Calculate the energy in every time slot. A time slot is an interval of time where the power is constant (the speed vector is constant)

  double energyToDecrease;
  std::cout << "currentPower " << currentPower << std::endl;
  std::cout << "duration " << duration << std::endl;

  energyToDecrease = duration.GetSeconds () *  currentPower;

  // Update the total energy consumption
  m_totalEnergyConsumption += energyToDecrease;

  std::cout << "m_totalEnergyConsumption " << m_totalEnergyConsumption << std::endl;

  // Check if the required energy is less than the capacity of the battery
  NS_ABORT_MSG_IF (m_totalEnergyConsumption > m_BatteryCapacity, "** REQUIRED DENERGY HAS TO BE < BATTERY CAPACITY ");

  if (m_firstUpdate == false)
    {
      m_lastUpdateTime = Simulator::Now ();
    }

  //Update the variable the absorbed power
  m_firstUpdate = false;
  currentPower = Power;
}

void
TyphoonEnergyModel::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);
  NS_ASSERT (node != NULL);
  m_node = node;
}

Ptr<Node>
TyphoonEnergyModel::GetNode (void) const
{
  return m_node;
}

void
TyphoonEnergyModel::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);
  m_source = source;
}

double
TyphoonEnergyModel::GetTotalEnergyConsumption (void) const
{
  NS_LOG_FUNCTION (this);

  return m_totalEnergyConsumption;
}

void
TyphoonEnergyModel::ChangeState (int newState)
{
  NS_FATAL_ERROR ("ChangeState not implemented, use ChangeEnergyConsumption instead.");
}

void
TyphoonEnergyModel::SetEnergyDepletionCallback (TyphoonEnergyDepletionCallback callback)
{
  NS_LOG_FUNCTION (this);
  if (callback.IsNull ())
    {
      NS_LOG_DEBUG ("TyphoonEnergyModel:Setting NULL energy depletion callback!");
    }
  m_energyDepletionCallback = callback;
}

void
TyphoonEnergyModel::SetEnergyRechargedCallback (TyphoonEnergyRechargedCallback callback)
{
  NS_LOG_FUNCTION (this);
  if (callback.IsNull ())
    {
      NS_LOG_DEBUG ("TyphoonEnergyModel:Setting NULL energy recharged callback!");
    }
  m_energyRechargedCallback = callback;
}

void
TyphoonEnergyModel::HandleEnergyDepletion (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("TyphoonEnergyModel:Energy is depleted at node #" <<
                m_node->GetId ());

  // invoke energy depletion callback, if set.
  if (!m_energyDepletionCallback.IsNull ())
    {
      m_energyDepletionCallback ();
    }
}

void
TyphoonEnergyModel::HandleEnergyRecharged (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("TyphoonEnergyModel:Energy is recharged at node #" <<
                m_node->GetId ());

  if (!m_energyRechargedCallback.IsNull ())
    {
      m_energyRechargedCallback ();
    }
}


void
TyphoonEnergyModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_node = NULL;
  m_source = NULL;
  m_energyDepletionCallback.Nullify ();
}

double
TyphoonEnergyModel::DoGetCurrentA (void) const
{
  NS_LOG_FUNCTION (this);

  return m_actualCurrentDrain;

}


} // namespace ns3
