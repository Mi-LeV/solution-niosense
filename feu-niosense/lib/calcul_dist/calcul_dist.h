#ifndef CALCUL_DIST_H
#define CALCUL_DIST_H

#include <math.h>
#include <stdlib.h>
#include "wifi_server.h"


#define FEU_LATERAL_OFFSET (2.0f) // offset lateral des rails en cm 
#define FEU_LONGITUDINAL_OFFSET (15.5f) // offset longitudinal des rails des extermites en cm
#define DIST_TO_CM (0.00103333333f) // constant of conversion of step distance to cm

#define RF_SIMUL_PROB_INTERF (10)
// proba entre 0 et 100 d'avoir une interference sur la communication simulee
#define RF_SIMUL_RANGE (3.0f)
//portee de la RF simulee en CM

bool compute_comm(long dist);
void camionAtFeu(long dist);

#endif