/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
#include "glider-energy-model.h"
#include "glider-constraints.h"


NS_LOG_COMPONENT_DEFINE ("GliderEnergyModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (GliderEnergyModel);

TypeId
GliderEnergyModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::GliderEnergyModel")
    .SetParent<DeviceEnergyModel> ()
    .SetGroupName ("Auv")
    .AddConstructor<GliderEnergyModel> ()
    .AddAttribute ("BatteryCapacity", "The capacity of the battery, in J.",
                   DoubleValue (4320000),
                   MakeDoubleAccessor (&GliderEnergyModel::m_BatteryCapacity),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("TotalEnergyConsumption",
                     "Total energy consumption of the radio device.",
                     MakeTraceSourceAccessor (&GliderEnergyModel::m_totalEnergyConsumption),
                     "ns3::TracedValueCallback::Double")
  ;
  return tid;


}

GliderEnergyModel::GliderEnergyModel ()
{
  NS_LOG_FUNCTION (this);
  m_currentSpeed = 0.0;
  m_currentBuoyancy = 0.0;
  m_currentW = 0.0;
  m_actualCurrentDrain = 0.0;
  m_lastUpdateTime = Seconds (0.0);
  m_energyDepletionCallback.Nullify ();
  m_energyRechargedCallback.Nullify ();
  m_node = 0;
  m_source = 0;
  m_VelocityModule = 0;
  m_totalEnergyConsumption = 0;
  m_CountCycle = 0;

}

GliderEnergyModel::~GliderEnergyModel ()
{
  m_energyDepletionCallback.Nullify ();
  m_energyRechargedCallback.Nullify ();
  m_node = NULL;
  m_source = NULL;

}
double Powerglider;
double currentPowerglider;


void
GliderEnergyModel::NotifyCourseChanged (Ptr<const MobilityModel>model)
{
  Ptr<GliderConstraints> gc = CreateObject<GliderConstraints> ();
  //Get the speed of the robot (3D Vector)
  m_velocity = model->GetVelocity ();

  //Get the module of the speed
  m_VelocityModule = std::sqrt (m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y + m_velocity.z * m_velocity.z);
  std::cout << "m_VelocityModule " << m_VelocityModule << std::endl;
  // The power needed [W] is the product of buoyancy (in Newton) and the vertical speed.
//Remember that GetBuoyancy returns a value in grammes
  std::cout << "m_velocity.z " << m_velocity.z << std::endl;
  std::cout << "gc->GetBuoyancy (m_velocity.x,m_velocity.z) " << gc->GetBuoyancy (m_velocity.x,m_velocity.z) << std::endl;
  if (m_velocity.x != 0 && m_velocity.z != 0)
    {
      Powerglider =  0.01 * std::abs ((m_velocity.z) * gc->GetBuoyancy (m_velocity.x,m_velocity.z));

      std::cout << "Powerglider " << Powerglider << std::endl;
    }

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

  m_actualCurrentDrain = currentPowerglider / supplyVoltage;

  // Notify energy source

  m_source->UpdateEnergySource ();

  // Some debug message
  NS_LOG_DEBUG ("GliderEnergyModel:Total energy consumption at node #" <<
                m_node->GetId () << " is " << m_totalEnergyConsumption << "J");

  //Calculate the energy in every time slot. A time slot is an interval of time where the power is constant (the speed vector is constant)

  double energyToDecrease;
  std::cout << "currentPower " << currentPowerglider << std::endl;
  std::cout << "duration " << duration << std::endl;

  energyToDecrease = duration.GetSeconds () *  currentPowerglider;

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
      currentPowerglider = Powerglider;
    }
}

void
GliderEnergyModel::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);
  NS_ASSERT (node != NULL);
  m_node = node;
}

Ptr<Node>
GliderEnergyModel::GetNode (void) const
{
  return m_node;
}

void
GliderEnergyModel::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);
  m_source = source;
}

double
GliderEnergyModel::GetTotalEnergyConsumption (void) const
{
  NS_LOG_FUNCTION (this);

  return m_totalEnergyConsumption;
}

void
GliderEnergyModel::ChangeState (int newState)
{
  NS_FATAL_ERROR ("ChangeState not implemented, use ChangeEnergyConsumption instead.");
}


void
GliderEnergyModel::ChangeEnergyConsumption (const double buoyancy, const double W)
{
  NS_LOG_FUNCTION (this << buoyancy << W);
 
  Time duration = Simulator::Now () - m_lastUpdateTime;
  NS_ASSERT (duration.GetNanoSeconds () >= 0); // check if duration is valid

  // update remaining energy accordingly
  double power = GetPower(buoyancy, W);
  double energyToDecrease = duration.GetSeconds () * power;

  // update total energy consumption
  m_totalEnergyConsumption += energyToDecrease;
 
  // update current buoyancy
  m_currentBuoyancy = buoyancy;
  // update current vertical speed
  m_currentW = W;
  // update current drain
  double supplyVoltage = m_source->GetSupplyVoltage ();
  m_actualCurrentDrain = power / supplyVoltage;
 
  // update last update time stamp
  m_lastUpdateTime = Simulator::Now ();
 
  // notify energy source
  m_source->UpdateEnergySource (); 
  // some debug message
  NS_LOG_DEBUG ("GliderEnergyModel:Total energy consumption at node #"
      << m_node->GetId () << " is " << m_totalEnergyConsumption << "J");
}



void
GliderEnergyModel::SetEnergyDepletionCallback (GliderEnergyDepletionCallback callback)
{
  NS_LOG_FUNCTION (this);
  if (callback.IsNull ())
    {
      NS_LOG_DEBUG ("GliderEnergyModel:Setting NULL energy depletion callback!");
    }
  m_energyDepletionCallback = callback;
}

void
GliderEnergyModel::SetEnergyRechargedCallback (GliderEnergyRechargedCallback callback)
{
  NS_LOG_FUNCTION (this);
  if (callback.IsNull ())
    {
      NS_LOG_DEBUG ("GliderEnergyModel:Setting NULL energy recharged callback!");
    }
  m_energyRechargedCallback = callback;
}

void
GliderEnergyModel::HandleEnergyDepletion (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("GliderEnergyModel:Energy is depleted at node #" <<
                m_node->GetId ());

  // invoke energy depletion callback, if set.
  if (!m_energyDepletionCallback.IsNull ())
    {
      m_energyDepletionCallback ();
    }
}

void
GliderEnergyModel::HandleEnergyRecharged (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("GliderEnergyModel:Energy is recharged at node #" <<
                m_node->GetId ());

  if (!m_energyRechargedCallback.IsNull ())
    {
      m_energyRechargedCallback ();
    }
}


void
GliderEnergyModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_node = NULL;
  m_source = NULL;
  m_energyDepletionCallback.Nullify ();
}

double
GliderEnergyModel::DoGetCurrentA (void) const
{
  NS_LOG_FUNCTION (this);

  return m_actualCurrentDrain;

}

double
GliderEnergyModel::GetPower (double buoyancy, double W) const
{
 // power needed to get the buoyancy, in watts
 double power = buoyancy / 100 * W;
 
 return power;
}

} // namespace ns3

