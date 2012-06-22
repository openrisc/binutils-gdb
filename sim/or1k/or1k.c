#ifndef WANT_OR1K64
#define WANT_CPU or1k32bf
#define WANT_CPU_OR1K32BF
#else
#define WANT_CPU or1k64bf
#define WANT_CPU_OR1K64BF
#endif

#include "sim-main.h"
#include "symcat.h"
#include "cgen-ops.h"

#include <string.h>

/* not sure what the point of these is */
int XCONCAT2(WANT_CPU,_fetch_register) (sim_cpu *current_cpu, int rn, unsigned char *buf, int len)
{
  return -1;
}

int XCONCAT2(WANT_CPU,_store_register) (sim_cpu *current_cpu, int rn, unsigned char *buf, int len)
{
  return -1;
}

#ifdef WANT_CPU_OR1K32BF
int or1k32bf_model_or1200_u_exec (sim_cpu * UNUSED current_cpu, const IDESC * UNUSED idesc, int unit_num, int referenced)
{
}

int or1k32bf_model_or1200nd_u_exec (sim_cpu * UNUSED current_cpu, const IDESC * UNUSED idesc, int unit_num, int referenced)
{
}

void or1k32bf_model_insn_before (sim_cpu * UNUSED current_cpu, int UNUSED first_p)
{
}

void or1k32bf_model_insn_after (sim_cpu * UNUSED current_cpu, int UNUSED last_p, int UNUSED cycles)
{
}

USI or1k32bf_h_spr_get_raw (sim_cpu *current_cpu, USI addr)
{
  SIM_DESC sd = CPU_STATE(current_cpu);
  SIM_ASSERT(addr < NUM_SPR);
  return current_cpu->spr[addr];
}

void or1k32bf_h_spr_set_raw (sim_cpu *current_cpu, USI addr, USI val)
{
  SIM_DESC sd = CPU_STATE(current_cpu);
  SIM_ASSERT(addr < NUM_SPR);
  current_cpu->spr[addr] = val;
}

USI or1k32bf_h_spr_field_get_raw (sim_cpu *current_cpu, USI addr, int msb, int lsb)
{
  SIM_DESC sd = CPU_STATE(current_cpu);
  SIM_ASSERT(addr < NUM_SPR);
  return LSEXTRACTED(current_cpu->spr[addr], msb, lsb);
}

void or1k32bf_h_spr_field_set_raw (sim_cpu *current_cpu, USI addr, int msb, int lsb, USI val)
{
  current_cpu->spr[addr] &= ~LSMASK32(msb, lsb);
  current_cpu->spr[addr] |= LSINSERTED(val, msb, lsb);
}

void or1k32bf_cpu_init (SIM_DESC sd, sim_cpu *current_cpu)
{
#define CHECK_SPR_FIELD(GROUP, INDEX, FIELD, test)                      \
  do {                                                                  \
    USI field = GET_H_##SYS##_##INDEX##_##FIELD ();                     \
    if (!(test)) {                                                      \
      sim_io_eprintf(sd, "WARNING: unsupported %s field in %s register: 0x%x\n", \
                     #FIELD, #INDEX, field);                            \
    }                                                                   \
  } while (0)
  
  current_cpu->next_delay_slot = 0;
  current_cpu->delay_slot = 0;

  CHECK_SPR_FIELD(SYS,UPR,UP,   field == 1);
  CHECK_SPR_FIELD(SYS,UPR,DCP,  field == 0);
  CHECK_SPR_FIELD(SYS,UPR,ICP,  field == 0);
  CHECK_SPR_FIELD(SYS,UPR,DMP,  field == 0);
  CHECK_SPR_FIELD(SYS,UPR,MP,   field == 0);
  CHECK_SPR_FIELD(SYS,UPR,IMP,  field == 0);
  CHECK_SPR_FIELD(SYS,UPR,DUP,  field == 0);
  CHECK_SPR_FIELD(SYS,UPR,PCUP, field == 0);
  CHECK_SPR_FIELD(SYS,UPR,PICP, field == 0);
  CHECK_SPR_FIELD(SYS,UPR,PMP,  field == 0);
  CHECK_SPR_FIELD(SYS,UPR,TTP,  field == 0);
  CHECK_SPR_FIELD(SYS,UPR,CUP,  field == 0);
  
  CHECK_SPR_FIELD(SYS,CPUCFGR,NSGR,  field == 0);
  CHECK_SPR_FIELD(SYS,CPUCFGR,CGF,   field == 0);
  CHECK_SPR_FIELD(SYS,CPUCFGR,OB32S, field == 1);
  CHECK_SPR_FIELD(SYS,CPUCFGR,OB64S, field == 0);
  CHECK_SPR_FIELD(SYS,CPUCFGR,OF64S, field == 0);
  CHECK_SPR_FIELD(SYS,CPUCFGR,OV64S, field == 0);
  
#undef CHECK_SPR_FIELD
  
  SET_H_SYS_UPR_UP(1);
  
  SET_H_SYS_SR(SPR_FIELD_MASK_SYS_SR_SM |
               SPR_FIELD_MASK_SYS_SR_FO
               );
  
  SET_H_SYS_FPCSR(0);
}

void or1k32bf_insn_before (sim_cpu *current_cpu, SEM_PC vpc, IDESC *idesc)
{
  SIM_DESC sd = CPU_STATE(current_cpu);

  current_cpu->delay_slot = current_cpu->next_delay_slot;
  current_cpu->next_delay_slot = 0;

  if (current_cpu->delay_slot &&
      CGEN_ATTR_BOOLS (CGEN_INSN_ATTRS ((idesc)->idata)) & CGEN_ATTR_MASK (CGEN_INSN_NOT_IN_DELAY_SLOT)) {
    USI pc;
#ifdef WITH_SCACHE
    pc = vpc->argbuf.addr;
#else
    pc = vpc;
#endif
    sim_io_error (sd, "invalid instruction in a delay slot at PC 0x%08x", pc);
  }
  
}

void or1k32bf_insn_after (sim_cpu *current_cpu, SEM_PC vpc, IDESC *idesc)
{
  SIM_DESC sd = CPU_STATE(current_cpu);
  USI ppc;
  
#ifdef WITH_SCACHE
  ppc = vpc->argbuf.addr;
#else
  ppc = vpc;
#endif

  SET_H_SYS_PPC (ppc);

  if (!GET_H_SYS_CPUCFGR_ND () &&
      CGEN_ATTR_BOOLS (CGEN_INSN_ATTRS ((idesc)->idata)) & CGEN_ATTR_MASK (CGEN_INSN_DELAYED_CTI)) {
    SIM_ASSERT (!current_cpu->delay_slot);
    current_cpu->next_delay_slot = 1;
  }
}

void or1k32bf_nop (sim_cpu *current_cpu, USI uimm16)
{
  SIM_DESC sd = CPU_STATE(current_cpu);
  
  switch (uimm16) {

  case NOP_NOP:
    break;
    
  case NOP_EXIT:
    sim_io_printf (CPU_STATE(current_cpu), "exit(%d)\n", GET_H_GPR(3));
    /* fall through */
  case NOP_EXIT_SILENT:
    sim_engine_halt (sd, current_cpu, NULL, CPU_PC_GET (current_cpu), sim_exited, GET_H_GPR (3));
    break;

  case NOP_REPORT:
    sim_io_printf (CPU_STATE(current_cpu), "report(0x%08x);\n", GET_H_GPR(3));
    break;
    
  case NOP_PUTC:
    sim_io_printf (CPU_STATE(current_cpu), "%c", (char)(GET_H_GPR(3) & 0xff));
    break;

  default:
    sim_io_eprintf(sd, "WARNING: l.nop with unsupported code 0x%08x\n", uimm16);
    break;
  }
  
}

USI or1k32bf_make_load_store_addr (sim_cpu *current_cpu, USI base, SI offset, int size)
{
  SIM_DESC sd = CPU_STATE(current_cpu);

  USI addr = base + offset;
  
  if (GET_H_SYS_SR_LEE ()) {
    switch (size) {

    case 4:
      break;
    
    case 2:
      addr ^= 0x2;
    
    case 1:
      addr ^= 0x3;

    default:
      SIM_ASSERT (0);
      return 0;
    }
  }
  
  return addr;
}

USI or1k32bf_ff1 (sim_cpu *current_cpu, USI val)
{
  USI bit;
  USI ret;
  for (bit = 1, ret = 1; bit; bit <<= 1, ret++) {
    if (val & bit)
      return ret;
  }
  return 0;
}

USI or1k32bf_fl1 (sim_cpu *current_cpu, USI val)
{
  USI bit;
  USI ret;
  for (bit = 1, ret = 1; bit; bit <<= 1, ret++) {
    if (!(val & bit))
      return ret;
  }
  return 0;
}
#endif
