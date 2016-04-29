#ifndef SIM_MAIN_H
#define SIM_MAIN_H

#include "sim-basics.h"
#include "sim-base.h"
#include "or32sim.h"

struct _sim_cpu {

  sim_cpu_base base;
};

struct sim_state {

  sim_cpu *cpu[MAX_NR_PROCESSORS];

  or1ksim_state or1ksim;

  sim_state_base base;
};


#endif
