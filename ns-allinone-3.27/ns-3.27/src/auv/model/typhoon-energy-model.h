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

#ifndef TYPHOON_ENERGY_MODEL_H
#define TYPHOON_ENERGY_MODEL_H

#include "ns3/device-energy-model.h"
#include "ns3/energy-source.h"
#include "ns3/traced-value.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/mobility-model.h"


namespace ns3 {

/**
 * A TYPHOON class AUV is an submarine-like device, propelled by some electric motors.
 *
 * The model is based on the characteristics of the TYPHOON AUV (DIEF University of Florence, MDM Lab) . The robot has 5 DOF.
  The power needed [W]:

- during the vertical motion is 700 W (speed = 0,3 m/s)
- during the horizontal motion is 300 W (speed = 1 m/s)

A linear dependency between power and velocity is supposed.

 /*/

class TyphoonEnergyModel : public DeviceEnergyModel
{
public:
  // / Callback type for energy depletion handling.
  typedef Callback<void> TyphoonEnergyDepletionCallback;
  typedef Callback<void> TyphoonEnergyRechargedCallback;

public:
  static TypeId GetTypeId (void);
  TyphoonEnergyModel ();
  virtual ~TyphoonEnergyModel ();

  /**
   * \brief Sets pointer to node.
   *
   * \param node Pointer to node.
   *
   * Implements DeviceEnergyModel::SetNode.
   */
  virtual void SetNode (Ptr<Node> node);

  /**
   * \brief Gets pointer to node.
   *
   * \returns Pointer to node.
   *
   * Implements DeviceEnergyModel::GetNode.
   */
  virtual Ptr<Node> GetNode (void) const;


  /**
   * \brief Sets pointer to EnergySouce installed on node.
   *
   * \param source Pointer to EnergySource installed on node.
   *
   * Implements DeviceEnergyModel::SetEnergySource.
   */
  virtual void SetEnergySource (Ptr<EnergySource> source);

  /**
   * \returns Total energy consumption of the vehicle.
   *
   * Implements DeviceEnergyModel::GetTotalEnergyConsumption.
   */
  virtual double GetTotalEnergyConsumption (void) const;

  /**
   * \param newState New state the device is in.
   *
   * DeviceEnergyModel is a state based model. This function is implemented by
   * all child of DeviceEnergyModel to change the model's state. States are to
   * be defined by each child using an enum (int).
   */
  virtual void ChangeState (int newState);


  /**
   * \param callback Callback function.
   *
   * Sets callback for energy depletion handling.
   */
  void SetEnergyDepletionCallback (TyphoonEnergyDepletionCallback callback);

  /**
   * \param callback Callback function.
   *
   * Sets callback for energy recharged handling.
   */
  void SetEnergyRechargedCallback (TyphoonEnergyRechargedCallback callback);

  /**
   * \brief Handles energy depletion.
   *
   * Implements DeviceEnergyModel::HandleEnergyDepletion
   */
  virtual void HandleEnergyDepletion (void);

  /**
   * \brief Handles energy recharge.
   *
   * Implements DeviceEnergyModel::HandleEnergyRecharged
   */
  virtual void HandleEnergyRecharged (void);

  /**
   * \brief Notify the model that the node mobility model changed its params
   *
   * \param model the mobility model.
   *
   */
  void NotifyCourseChanged (Ptr<const MobilityModel>);

private:
  void DoDispose (void);

  /**
   * \returns Current draw of device, at current state.
   *
   * Implements DeviceEnergyModel::GetCurrentA.
   */
  virtual double DoGetCurrentA (void) const;


private:
  Ptr<Node> m_node;
  Ptr<EnergySource> m_source;


  // This variable keeps track of the total energy consumed by this particular model.
  TracedValue<double> m_totalEnergyConsumption;


  // actual current drain
  double m_actualCurrentDrain;
  // current navigation speed in m/s

  // time stamp of previous energy update
  Time m_lastUpdateTime;

  /*/ The constants are found as follow.

 The power needed [W]:

- during the vertical motion is 700 W (speed = 0,3 m/s)
- during the horizontal motion is 300 W (speed = 1 m/s)

A linear dependency between power and velocity is supposed.

Therefore, m_powerSpeedRatioHorizontal is 300 [Ws/m], m_powerSpeedRatioVertical is 700/0.3 [Ws/m]
   /*/
  double m_powerSpeedRatioHorizontal;
  double m_powerSpeedRatioVertical;
//The capacity of the battery is 4320000 J
  double m_BatteryCapacity;
  Vector m_velocity;
  double m_VelocityModule;
  double m_currentSpeed;
  bool m_firstUpdate;
  // energy depletion callback
  TyphoonEnergyDepletionCallback m_energyDepletionCallback;
  TyphoonEnergyRechargedCallback m_energyRechargedCallback;
};

} // namespace ns3

#endif /* TYPHOON_ENERGY_MODEL_H */
