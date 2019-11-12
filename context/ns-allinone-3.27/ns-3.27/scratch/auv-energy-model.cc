/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Andrea Sacco
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
 * Author: Andrea Sacco <andrea.sacco85@gmail.com>
 */

#include "ns3/simulator.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/glider-energy-model.h"
#include "ns3/energy-source-container.h"

using namespace ns3;

/**
 * In this example we show the basic usage of an AUV energy model.
 * Specifically, we show how to create a generic node, adding to it a
 * basic energy source and consuming energy from the energy source.
 *
 * The Seaglider AUV power consumption depends on buoyancy and vertical
 * speed values, so we simulate a 20 seconds movement at 0.3 m/s of vertical
 * speed and 138g of buoyancy. Then a 20 seconds movement at 0.2 m/s
 * of vertical speed and 138g of buoyancy and then a stop of 5 seconds.
 *
 * The required energy will be drained by the model basing on the
 * given buoyancy/speed values, from the energy source installed onto
 * the node.
 *
 * We register a callback to the TotalEnergyConsumption traced value.
 */

void
DoubleTrace (double oldValue, double newValue)
{
  std::cout << "Consumed energy changed from " << oldValue << "J to " << newValue << "J" << std::endl;
}

int
main (int argc, char **argv)
{
  // uncomment to see energy consumption details
  // LogComponentEnable ("GliderEnergyModel", LOG_LEVEL_ALL);

  // create node
  Ptr<Node> node = CreateObject<Node> ();

  // create energy source
  ObjectFactory m_energySource;
  m_energySource.SetTypeId ("ns3::BasicEnergySource");
  m_energySource.Set ("BasicEnergySourceInitialEnergyJ",
                      DoubleValue (1000));
  Ptr<EnergySource> source = m_energySource.Create<EnergySource> ();
  source->SetNode (node);
  Ptr<EnergySourceContainer> sourceCont = CreateObject<EnergySourceContainer> ();
  sourceCont->Add (source);
  // aggregate energy source to node
  node->AggregateObject (sourceCont);

  // create device energy model
  Ptr<GliderEnergyModel> model = CreateObject<GliderEnergyModel> ();
  // set energy source pointer
  model->SetEnergySource (source);
  model->SetNode (node);
  // add device energy model to model list in energy source
  source->AppendDeviceEnergyModel (model);
  // register a callback to the total consumption value
  model->TraceConnectWithoutContext ("TotalEnergyConsumption", MakeCallback (&DoubleTrace));

  // retrieve device energy model from energy source
  DeviceEnergyModelContainer modelCont =
    source->FindDeviceEnergyModels ("ns3::GliderEnergyModel");
  NS_ASSERT (modelCont.GetN () != 0);
  // get pointer
  Ptr<GliderEnergyModel> devModel = DynamicCast<GliderEnergyModel> (modelCont.Get (0));

  // simulate 20 seconds of movement with 138g buoyancy and 0.3 m/s of W
  devModel->ChangeEnergyConsumption (138, 0.3);

  // simulate 20 seconds of movement with 138g buoyancy and 0.2 m/s of W
  Simulator::Schedule (Seconds (20),
                       &GliderEnergyModel::ChangeEnergyConsumption,
                       devModel,
                       138,
                       0.2);

  // simulate a stop of the vehicle for the remaining 5 seconds
  Simulator::Schedule (Seconds (40),
                       &GliderEnergyModel::ChangeEnergyConsumption,
                       devModel,
                       0,
                       0);

  // run simulation
  Simulator::Stop (Seconds (45));
  Simulator::Run ();
  Simulator::Destroy ();

  std::cout << "Remaining energy: " << source->GetRemainingEnergy () << std::endl;

  return 0;
}
