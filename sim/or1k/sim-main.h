#ifndef SIM_MAIN_H
#define SIM_MAIN_H

#include "ansidecl.h"
#include "or1k-desc.h"
#include "sim-basics.h"
#include "cgen-types.h"
#include "arch.h"
typedef IAI sim_cia;
#define CIA_GET(cpu) CPU_PC_GET (cpu)
#define CIA_SET(cpu,val) CPU_PC_SET ((cpu),(val))
#include "sim-base.h"

typedef struct _sim_cpu SIM_CPU;
#include "or1k-opc.h"
#include "cgen-sim.h"
#include "or1k-sim.h"
#include "cpuall.h"
#include "or1k.h"

#ifdef OR1K_LINUX
#define OR1K_DEFAULT_MEM_SIZE 0x2000000 /* 32M */
#else
#define OR1K_DEFAULT_MEM_SIZE 0x800000 /* 8M */
#endif

/* The _sim_cpu struct.  */
struct _sim_cpu {
  /* sim/common cpu base.  */
  sim_cpu_base base;

  /* Static parts of cgen.  */
  CGEN_CPU cgen_cpu;

  OR1K_MISC_PROFILE or1k_misc_profile;
#define CPU_OR1K_MISC_PROFILE(cpu) (& (cpu)->or1k_misc_profile)

  /* CPU specific parts go here.
     Note that in files that don't need to access these pieces WANT_CPU_FOO
     won't be defined and thus these parts won't appear.  This is ok in the
     sense that things work.  It is a source of bugs though.
     One has to of course be careful to not take the size of this
     struct and no structure members accessed in non-cpu specific files can
     go after here.  Oh for a better language.  */
  UWI spr[NUM_SPR];

#ifdef WANT_CPU_OR1K32BF
  OR1K32BF_CPU_DATA cpu_data;
#endif
#ifdef WANT_CPU_OR1K64BF
  OR1K64BF_CPU_DATA cpu_data;
#endif

};

/* The sim_state struct.  */

struct sim_state {
  sim_cpu *cpu[MAX_NR_PROCESSORS];
#if (WITH_SMP)
#define STATE_CPU(sd, n) ((sd)->cpu[n])
#else
#define STATE_CPU(sd, n) ((sd)->cpu[0])
#endif

  CGEN_STATE cgen_state;

  sim_state_base base;
};

#endif
