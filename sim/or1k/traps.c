#ifndef WANT_OR1K64
#define WANT_CPU or1k32bf
#define WANT_CPU_OR1K32BF
#else
#define WANT_CPU or1k64bf
#define WANT_CPU_OR1K64BF
#endif

#include "sim-main.h"
#include "cgen-ops.h"

SEM_PC
sim_engine_invalid_insn (SIM_CPU *current_cpu, IADDR cia, SEM_PC vpc)
{
  SET_H_SYS_EEAR0(cia);

#ifdef WANT_CPU_OR1K32BF
  or1k32bf_exception (current_cpu, cia, EXCEPT_ILLEGAL);
#endif
  
  return vpc;
}

void or1k32bf_exception (sim_cpu *current_cpu, USI pc, USI exnum)
{
  SIM_DESC sd = CPU_STATE(current_cpu);
  
  SET_H_SYS_ESR0 (GET_H_SYS_SR ());
  
  SET_H_SYS_SR_DSX (current_cpu->delay_slot);
  
  switch (exnum) {
  case EXCEPT_RESET:
    break;

  case EXCEPT_SYSCALL:
    SET_H_SYS_EPCR0 (pc + 4 - (current_cpu->delay_slot ? 4 : 0));
    break;

  case EXCEPT_BUSERR:
  case EXCEPT_ALIGN:
  case EXCEPT_RANGE:
  case EXCEPT_TRAP:
  case EXCEPT_ILLEGAL:
    SET_H_SYS_EPCR0 (pc - (current_cpu->delay_slot ? 4 : 0));
    break;

  default:
    sim_io_error (sd, "unexpected exception 0x%x raised at PC 0x%08x", exnum, pc);
    break;
    
  }
  
  current_cpu->next_delay_slot = 0;
  
  IADDR handler_pc = (GET_H_SYS_SR_EPH() ? 0xf0000000 : 0x00000000) + (exnum << 8);
  
  sim_engine_restart (CPU_STATE (current_cpu),
                      current_cpu,
                      NULL,
                      handler_pc);
}

void or1k32bf_rfe (sim_cpu *current_cpu)
{
  SET_H_SYS_SR (GET_H_SYS_ESR0 ());
  SET_H_SYS_SR_FO (1);

  current_cpu->next_delay_slot = 0;
  
  sim_engine_restart (CPU_STATE (current_cpu),
                      current_cpu,
                      NULL,
                      GET_H_SYS_EPCR0 ());
}

USI or1k32bf_mfspr (sim_cpu *current_cpu, USI addr)
{
  SIM_DESC sd = CPU_STATE(current_cpu);
  
  if (!GET_H_SYS_SR_SM () && !GET_H_SYS_SR_SUMRA ()) {
    sim_io_eprintf(sd, "WARNING: l.mfspr in user mode (SR 0x%x)\n", GET_H_SYS_SR());
    return 0;
  }
  
  if (addr >= NUM_SPR)
    goto bad_address;
  
  SI val = GET_H_SPR(addr);
  
  switch (addr) {
    
  case SPR_ADDR(SYS,VR):
  case SPR_ADDR(SYS,UPR):
  case SPR_ADDR(SYS,CPUCFGR):
  case SPR_ADDR(SYS,SR):
  case SPR_ADDR(SYS,PPC):
  case SPR_ADDR(SYS,FPCSR):
  case SPR_ADDR(SYS,EPCR0):
  case SPR_ADDR(MAC,MACHI):
  case SPR_ADDR(MAC,MACLO):
    break;

  default:
    if (addr < SPR_ADDR(SYS,GPR0) || addr > SPR_ADDR(SYS,GPR511)) {
      goto bad_address;
    }
    break;
    
  }
  
  return val;

 bad_address:
  sim_io_eprintf (sd, "WARNING: l.mfspr with invalid SPR address 0x%x\n", addr);
  return 0;

}

void or1k32bf_mtspr (sim_cpu *current_cpu, USI addr, USI val)
{
  SIM_DESC sd = CPU_STATE(current_cpu);
  
  if (!GET_H_SYS_SR_SM () && !GET_H_SYS_SR_SUMRA ()) {
    sim_io_eprintf(sd, "WARNING: l.mtspr with address 0x%x in user mode (SR 0x%x)\n", addr, GET_H_SYS_SR());
    return;
  }
  
  if (addr >= NUM_SPR)
    goto bad_address;
  
  switch (addr) {
    
  case SPR_ADDR(SYS,FPCSR):
  case SPR_ADDR(SYS,EPCR0):
  case SPR_ADDR(SYS,ESR0):
  case SPR_ADDR(MAC,MACHI):
  case SPR_ADDR(MAC,MACLO):
    SET_H_SPR(addr, val);
    break;
    
  case SPR_ADDR(SYS,SR):
    SET_H_SPR(addr, val);
    SET_H_SYS_SR_FO(1);
    break;

  case SPR_ADDR(SYS,NPC):
    current_cpu->next_delay_slot = 0;
  
    sim_engine_restart (CPU_STATE (current_cpu),
                        current_cpu,
                        NULL,
                        val);
    break;

  case SPR_ADDR(TICK,TTMR):
    /* allow some registers to be silently cleared */
    if (val != 0)
      sim_io_eprintf (sd, "WARNING: l.mtspr to SPR address 0x%x with invalid value 0x%x\n", addr, val);
    break;

  default:
    if (addr >= SPR_ADDR(SYS,GPR0) && addr <= SPR_ADDR(SYS,GPR511)) {
      SET_H_SPR(addr, val);
    } else {
      goto bad_address;
    }
    break;
    
  }
  
  return;

 bad_address:
  sim_io_eprintf (sd, "WARNING: l.mtspr with invalid SPR address 0x%x\n", addr);

}
