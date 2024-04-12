#include "comm_simul.h"

bool compute_comm(c_pos, rf_simu_reach, rf_simu_prob_interf){
    long distance_to_f = sqrt(FEU_OFFSET_RAIL * FEU_OFFSET_RAIL + (c_pos - FEU_POSTION) * (c_pos - FEU_POSTION));

    bool comm_simul_established = true;
    if ( distance_to_f > rf_simu_reach){ // test if distance to feu is higher thn simulated RF reach
        comm_simul_established = false;
    }else{
        uint8_t rand_interf = rand() % 100; // get random number btwn 0 and 100

        //test if the probability of interfrence is below the radom value
        // (interferences are supposed to happen uniformely along the whole travel distance)
        if ( rand_interf > rf_simu_prob_interf){
            comm_simul_established = false;
        }
    }
    return comm_simul_established;
    
}

float dist_to_mm(long dist){
    return dist * DIST_TO_MM;
}

long dist_to_mm(float dist){
    return (long) (dist * MM_TO_DIST);
}