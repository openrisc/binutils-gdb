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

void or1k32bf_insn_before (sim_cpu *current_cpu, SEM_PC vpc)
{
}

SEM_PC or1k32bf_insn_after (sim_cpu *current_cpu, SEM_PC vpc)
{
  USI ppc;
#ifdef WITH_SCACHE
  ppc = vpc->argbuf.addr;
#else
  ppc = vpc;
#endif
  SET_H_SPR (SPR_ADDR(SYS,PPC), ppc);
  
  return vpc;
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

void or1k32bf_mfspr (sim_cpu *current_cpu, USI pc, int rd, USI addr)
{
  SIM_DESC sd = CPU_STATE(current_cpu);
  
  if (!GET_H_SYS_SR_SM () && !GET_H_SYS_SR_SUMRA ()) {
    sim_io_eprintf(sd, "WARNING: l.mfspr in user mode (SR 0x%x)\n", GET_H_SYS_SR());
    return;
  }
  
  if (addr >= NUM_SPR)
    return;
  
  SI val = GET_H_SPR(addr);
  
  switch (addr) {

  case SPR_ADDR(SYS,VR):
  case SPR_ADDR(SYS,UPR):
  case SPR_ADDR(SYS,CPUCFGR):
  case SPR_ADDR(SYS,SR):
  case SPR_ADDR(SYS,FPCSR):
  case SPR_ADDR(SYS,DMMUCFGR):
    SET_H_GPR(rd, val);
    break;
    
  default:
    if (addr >= SPR_ADDR(SYS,GPR0) && addr <= SPR_ADDR(SYS,GPR511)) {
      SET_H_GPR(rd, val);
    } else {
      sim_io_eprintf (sd, "WARNING: l.mfspr with invalid SPR address 0x%x\n", addr);
    }
    break;
    
  }

}

void or1k32bf_mtspr (sim_cpu *current_cpu, USI pc, USI addr, USI val)
{
  SIM_DESC sd = CPU_STATE(current_cpu);
  
  if (!GET_H_SYS_SR_SM () && !GET_H_SYS_SR_SUMRA ()) {
    sim_io_eprintf(sd, "WARNING: l.mtspr in user mode (SR 0x%x)\n", GET_H_SYS_SR());
    return;
  }
  
  if (addr >= NUM_SPR)
    return;
  
  switch (addr) {
    
  case SPR_ADDR(SYS,SR):
    break;

  case SPR_ADDR(SYS,UPR):
    break;
    
  default:
    if (addr >= SPR_ADDR(SYS,GPR0) && addr <= SPR_ADDR(SYS,GPR511)) {
      SET_H_SPR(addr, val);
    }
    break;
    
  }
  
  return;
}

void or1k32bf_exception (sim_cpu *current_cpu, USI pc, USI exnum)
{
  /* TODO */
  abort();
}

void or1k32bf_rfe (sim_cpu *current_cpu, USI pc)
{
  /* TODO */
  abort();
}

USI or1k32bf_make_load_store_addr (sim_cpu *current_cpu, USI base, SI offset, int size)
{
  SIM_DESC sd = CPU_STATE(current_cpu);

  if (!GET_H_SYS_SR_SM ()) {
    sim_io_eprintf(sd, "WARNING: l.mfspr in user mode\n");
    return;
  }
  
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
