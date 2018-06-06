
#ifdef NS3_MODULE_COMPILATION
# error "Do not include ns3 module aggregator headers from other modules; these are meant only for end user scripts."
#endif

#ifndef NS3_MODULE_AUV
    

// Module headers:
#include "auv-glider-helper.h"
#include "auv-remus-helper.h"
#include "auv-remus-helperv2.h"
#include "auv-typhoon-helper.h"
#include "auv-waypoint-mobility-model.h"
#include "glider-constraints.h"
#include "glider-energy-model.h"
#include "kinematic-constraints.h"
#include "remus-constraints.h"
#include "remus-constraintsv2.h"
#include "remus-energy-model.h"
#include "typhoon-constraints.h"
#include "typhoon-energy-model.h"
#endif
