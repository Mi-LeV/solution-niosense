
#include "calcul_dist.h"


extern bool flag_demande;

uint8_t compute_comm(long dist){
    float distance_camion_cm = sqrt( (dist * DIST_TO_CM - FEU_LONGITUDINAL_OFFSET)*(dist * DIST_TO_CM - FEU_LONGITUDINAL_OFFSET)
        + FEU_LATERAL_OFFSET*FEU_LATERAL_OFFSET);

    uint8_t comm_simul_possible = false;

    if (distance_camion_cm < RF_SIMUL_RANGE){

        
        int rand_interf = rand() % 100;
        if ( rand_interf >= RF_SIMUL_PROB_INTERF){
            flag_demande = true;
            comm_simul_possible = true;
        }
    }

    return comm_simul_possible;

}
