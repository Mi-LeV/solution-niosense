
#include "calcul_dist.h"

bool compute_comm(long dist,float rf_simul_reach_cm,float rf_simul_prob_interf){
    float distance_camion_cm = sqrt( (dist * DIST_TO_CM - FEU_LONGITUDINAL_OFFSET)*(dist * DIST_TO_CM - FEU_LONGITUDINAL_OFFSET)
        + FEU_LATERAL_OFFSET*FEU_LATERAL_OFFSET);

    bool comm_simul_possible = false;

    if (distance_camion_cm > rf_simul_reach_cm){

        
        float rand_interf = rand() / (RAND_MAX + 1.);
        if ( rand_interf < rf_simul_prob_interf){
            comm_simul_possible = true;
        }
    }

    return comm_simul_possible;

}

void camionAtFeu(long dist){

    if ((long)(FEU_LONGITUDINAL_OFFSET / DIST_TO_CM) - dist == 0){
        String ligne = new_line(CAMION_BY_FEU);
        appendFile(FICHIER_LOG, ligne.c_str());
    }
}