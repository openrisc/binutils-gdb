/* Simulator model support for or1k64bf.

THIS FILE IS MACHINE GENERATED WITH CGEN.

Copyright 1996-2010 Free Software Foundation, Inc.

This file is part of the GNU simulators.

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.

*/

#define WANT_CPU or1k64bf
#define WANT_CPU_OR1K64BF

#include "sim-main.h"

/* The profiling data is recorded here, but is accessed via the profiling
   mechanism.  After all, this is information for profiling.  */

#if WITH_PROFILE_MODEL_P

/* Model handlers for each insn.  */

/* We assume UNIT_NONE == 0 because the tables don't always terminate
   entries with it.  */

#endif /* WITH_PROFILE_MODEL_P */

#if WITH_PROFILE_MODEL_P
#define TIMING_DATA(td) td
#else
#define TIMING_DATA(td) 0
#endif

static const MODEL or64_models[] =
{
  { 0 }
};

static const MODEL or64nd_models[] =
{
  { 0 }
};

/* The properties of this cpu's implementation.  */

static const MACH_IMP_PROPERTIES or1k64bf_imp_properties =
{
  sizeof (SIM_CPU),
#if WITH_SCACHE
  sizeof (SCACHE)
#else
  0
#endif
};


static void
or1k64bf_prepare_run (SIM_CPU *cpu)
{
  if (CPU_IDESC (cpu) == NULL)
    or1k64bf_init_idesc_table (cpu);
}

static const CGEN_INSN *
or1k64bf_get_idata (SIM_CPU *cpu, int inum)
{
  return CPU_IDESC (cpu) [inum].idata;
}

static void
or64_init_cpu (SIM_CPU *cpu)
{
  CPU_REG_FETCH (cpu) = or1k64bf_fetch_register;
  CPU_REG_STORE (cpu) = or1k64bf_store_register;
  CPU_PC_FETCH (cpu) = or1k64bf_h_pc_get;
  CPU_PC_STORE (cpu) = or1k64bf_h_pc_set;
  CPU_GET_IDATA (cpu) = or1k64bf_get_idata;
  CPU_MAX_INSNS (cpu) = OR1K64BF_INSN__MAX;
  CPU_INSN_NAME (cpu) = cgen_insn_name;
  CPU_FULL_ENGINE_FN (cpu) = or1k64bf_engine_run_full;
#if WITH_FAST
  CPU_FAST_ENGINE_FN (cpu) = or1k64bf_engine_run_fast;
#else
  CPU_FAST_ENGINE_FN (cpu) = or1k64bf_engine_run_full;
#endif
}

const MACH or64_mach =
{
  "or64", "or1k64", MACH_OR64,
  64, 64, & or64_models[0], & or1k64bf_imp_properties,
  or64_init_cpu,
  or1k64bf_prepare_run
};

static void
or64nd_init_cpu (SIM_CPU *cpu)
{
  CPU_REG_FETCH (cpu) = or1k64bf_fetch_register;
  CPU_REG_STORE (cpu) = or1k64bf_store_register;
  CPU_PC_FETCH (cpu) = or1k64bf_h_pc_get;
  CPU_PC_STORE (cpu) = or1k64bf_h_pc_set;
  CPU_GET_IDATA (cpu) = or1k64bf_get_idata;
  CPU_MAX_INSNS (cpu) = OR1K64BF_INSN__MAX;
  CPU_INSN_NAME (cpu) = cgen_insn_name;
  CPU_FULL_ENGINE_FN (cpu) = or1k64bf_engine_run_full;
#if WITH_FAST
  CPU_FAST_ENGINE_FN (cpu) = or1k64bf_engine_run_fast;
#else
  CPU_FAST_ENGINE_FN (cpu) = or1k64bf_engine_run_full;
#endif
}

const MACH or64nd_mach =
{
  "or64nd", "or1k64nd", MACH_OR64ND,
  64, 64, & or64nd_models[0], & or1k64bf_imp_properties,
  or64nd_init_cpu,
  or1k64bf_prepare_run
};

