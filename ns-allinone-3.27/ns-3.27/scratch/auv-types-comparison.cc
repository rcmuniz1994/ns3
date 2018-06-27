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

#include "ns3/auv-typhoon-helper.h"
#include "ns3/auv-remus-helper.h"
#include "ns3/auv-glider-helper.h"
#include "ns3/auv-waypoint-mobility-model.h"
#include "ns3/node.h"
#include "ns3/energy-source.h"
#include "ns3/basic-energy-source.h"
#include "ns3/device-energy-model.h"
#include "ns3/log.h"
#include "ns3/command-line.h"
#include <list>
#include <iostream>

using namespace ns3;


NS_LOG_COMPONENT_DEFINE ("AuvTypesComparison");

/**
 * \brief Traces periodically the position of a mobile node.
 * \param mob AUV mobility model.
 * \param delta Trace period.
 */
static void
TracePos (Ptr<const AuvWaypointMobilityModel> mob, Time delta)
{
  Vector curPos = mob->GetPosition ();

  //std::cout << "At " << Simulator::Now ().GetSeconds () << " seconds: x= " <<
  //    curPos.x << " y= " << curPos.y << " z= " << curPos.z << std::endl;
  std::cout << curPos.x << " " << curPos.z << std::endl;

  Simulator::Schedule (delta, &TracePos, mob, delta);
}

int
main (int argc, char **argv)
{
  std::string auvType = "Typhoon";
  //std::string auvType = "Remus";
  //std::string auvType = "SeaGlider";
  Ptr<DeviceEnergyModel> energyModel = 0;
  Time StopTime = Seconds (418);
  Time delta = Seconds (0.1);

  CommandLine cmd;
  cmd.AddValue ("AuvType", "Type of AUV (Typhoon, Remus, [Sea]Glider) - default is Typhoon", auvType);
  cmd.Parse (argc, argv);

  // create node
  Ptr<Node> node = CreateObject<Node> ();

  // Select AUV type
  if (auvType.compare ("Typhoon") == 0)
    {
      AuvTyphoonHelper re;
      re.Install(node);
      energyModel = node->GetObject<EnergySourceContainer> ()->Get (0)->FindDeviceEnergyModels ("ns3::TyphoonEnergyModel").Get (0);
      //StopTime = Seconds (1200);
      //Time delta = Seconds (1);
    }
  else if (auvType.compare ("Remus") == 0)
    {
      AuvRemusHelper re;
      re.Install(node);
      energyModel = node->GetObject<EnergySourceContainer> ()->Get (0)->FindDeviceEnergyModels ("ns3::RemusEnergyModel").Get (0);
    }
  else if (auvType.compare ("SeaGlider") == 0 || auvType.compare ("Glider") == 0)
    {
      AuvGliderHelper re;
      re.Install(node);
      energyModel = node->GetObject<EnergySourceContainer> ()->Get (0)->FindDeviceEnergyModels ("ns3::GliderEnergyModel").Get (0);
      StopTime = Seconds (2000); // Glider is much slower than the other two.
      delta = Seconds (1);
    }
  else
    {
     NS_LOG_DEBUG ("Invalid AUV type");
      exit (1);
    }

  Ptr<AuvWaypointMobilityModel> mm = node->GetObject <AuvWaypointMobilityModel> ();

  mm->AddWaypoint (Waypoint (Seconds (0), Vector (0, 0, 0)));
  //mm->AddWaypoint (Waypoint (StopTime, Vector (50,0,1000)));
  mm->AddWaypoint (Waypoint (StopTime, Vector (50, 0, 100)));

  // Trace the position
  TracePos (mm, delta);

  // Run the simulation
  DoubleValue auvBatteryCapacity;
  energyModel->GetAttribute ("BatteryCapacity", auvBatteryCapacity);

  Simulator::Stop (StopTime + Seconds (1));
  Simulator::Run ();
  std::cout << "Total Energy Consumption " << energyModel->GetTotalEnergyConsumption ()<<std::endl;
  std::cout << "Remained Energy " << auvBatteryCapacity.Get () - energyModel->GetTotalEnergyConsumption ()<<std::endl;
  Simulator::Destroy ();


  return 0;
}
