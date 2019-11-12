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

#include "kinematic-constraints.h"
#include "ns3/type-id.h"

namespace ns3 {

TypeId
KinematicConstraints::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::KinematicConstraints")
    .SetParent (Object::GetTypeId ())
    .SetGroupName ("Auv")
  ;
  return tid;
}

KinematicConstraints::KinematicConstraints ()
{
}


KinematicConstraints::~KinematicConstraints ()
{
}

}
// namespace ns3
