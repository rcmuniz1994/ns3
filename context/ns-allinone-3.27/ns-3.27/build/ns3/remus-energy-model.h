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
#ifndef REMUS_ENERGY_MODEL_H
#define REMUS_ENERGY_MODEL_H

#include "ns3/device-energy-model.h"
#include "ns3/energy-source.h"
#include "ns3/traced-value.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/mobility-model.h"


namespace ns3 {

/**
 *  A REMUS class AUV is an submarine-like device, propelled by an electric motor linked with a propeller.
 *
 * The model is based on the characteristics of the REMUS 100 AUV [1].
 *
 * References:
 * [1] https://www.km.kongsberg.com/ks/web/nokbg0397.nsf/AllWeb/82C0D4915CE64FEAC1257A23002BECC5/$file/REMUS-brochure.pdf?OpenElement
 *
 *The power needed is 100 W (speed = 2,3 m/s). In fact, the robot with a battery capacity of 3,6 MJ can operate for 10 hours at 2,3 m/s.
 *
 *A linear dependency between power and velocity is supposed ----> the power consumption is 45 W if speed = 1 m/s
 /*/

class RemusEnergyModel : public DeviceEnergyModel
{
public:
  // / Callback type for energy depletion handling.
  typedef Callback<void> RemusEnergyDepletionCallback;
  typedef Callback<void> RemusEnergyRechargedCallback;

public:
  static TypeId GetTypeId (void);
  RemusEnergyModel ();
  virtual ~RemusEnergyModel ();

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
  void SetEnergyDepletionCallback (RemusEnergyDepletionCallback callback);

  /**
   * \param callback Callback function.
   *
   * Sets callback for energy recharged handling.
   */
  void SetEnergyRechargedCallback (RemusEnergyRechargedCallback callback);

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
  double m_powerSpeedRatio;

//The capacity of the battery is 4320000 J
  double m_BatteryCapacity;
  Vector m_velocity;
  double m_VelocityModule;
  double m_currentSpeed;
  int m_CountCycle;
  // energy depletion callback
  RemusEnergyDepletionCallback m_energyDepletionCallback;
  RemusEnergyRechargedCallback m_energyRechargedCallback;
};

} // namespace ns3

#endif /* REMUS_ENERGY_MODEL_H */
