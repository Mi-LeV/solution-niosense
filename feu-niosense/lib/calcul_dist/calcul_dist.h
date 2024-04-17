#ifndef CALCUL_DIST_H
#define CALCUL_DIST_H

#include <math.h>
#include <stdlib.h>
#include "wifi_server.h"


//////////////////////////////////////
////////////////////        CONSTANTES A DEFINIR
/////////////////////

#define FEU_LATERAL_OFFSET (2.0f) // offset lateral des rails en cm 
#define FEU_LONGITUDINAL_OFFSET (10.0f) // offset longitudinal des rails des extermites en cm
#define DIST_TO_CM (0.01f) // constant of conversion of step distance to cm

#define RF_SIMUL_PROB_INTERF (0.01f) // proba entre 0 et 1 d'avoir une interference sur la communication simulee

bool compute_comm(long dist,float rf_simul_reach_cm,float rf_simul_prob_interf);
void camionAtFeu(long dist);

#endif