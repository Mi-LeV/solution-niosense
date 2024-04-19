
#include "calcul_dist.h"

bool compute_comm(long dist){
    float distance_camion_cm = sqrt( (dist * DIST_TO_CM - FEU_LONGITUDINAL_OFFSET)*(dist * DIST_TO_CM - FEU_LONGITUDINAL_OFFSET)
        + FEU_LATERAL_OFFSET*FEU_LATERAL_OFFSET);

    bool comm_simul_possible = false;

    if (distance_camion_cm < RF_SIMUL_RANGE){

        
        int rand_interf = rand() % 100;
        if ( rand_interf >= RF_SIMUL_PROB_INTERF){
            comm_simul_possible = true;
        }
    }

    return comm_simul_possible;

}

void camionAtFeu(long pos_camion){

    if ((long)(FEU_LONGITUDINAL_OFFSET / DIST_TO_CM) - pos_camion < 2){ // on met une distance inf a 2
        //au cas ou le camion skip un step
        String ligne = new_line(CAMION_BY_FEU);
        appendFile(FICHIER_LOG, ligne.c_str());
    }
}