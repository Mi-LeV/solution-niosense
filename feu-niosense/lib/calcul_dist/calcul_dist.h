#ifndef CALCUL_DIST_H
#define CALCUL_DIST_H

#include <math.h>
#include <stdlib.h>
#include "wifi_server.h"

#define AXE_LENGHT (21.7f)//leght of the rail in cm
#define FEU_LATERAL_OFFSET (17.5f) // offset lateral des rails en cm 
#define FEU_LONGITUDINAL_OFFSET (10.85f) // offset longitudinal des rails des extermites en cm
#define DIST_TO_CM ((float)(AXE_LENGHT/32000)) // constant of conversion of step distance to cm

#define RF_SIMUL_PROB_INTERF (10)
// proba entre 0 et 100 d'avoir une interference sur la communication simulee
#define RF_SIMUL_RANGE (20.0f)
//portee de la RF simulee en CM

bool compute_comm(long dist);
void camionAtFeu(long dist);

#endif