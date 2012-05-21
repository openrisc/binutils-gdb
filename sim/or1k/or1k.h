#ifndef OR1K_H
#define OR1K_H

#define NOP_NOP         0x0
#define NOP_EXIT        0x1
#define NOP_REPORT      0x2
#define NOP_PUTC        0x4
#define NOP_CNT_RESET   0x5
#define NOP_GET_TICKS   0x6
#define NOP_GET_PS      0x7
#define NOP_TRACE_ON    0x8
#define NOP_TRACE_OFF   0x9
#define NOP_RANDOM      0xa
#define NOP_OR1KSIM     0xb
#define NOP_EXIT_SILENT 0xc

#define NUM_SPR 0x20000
#define SPR_GROUP_SHIFT 11
#define SPR_GROUP_FIRST(group) (((UWI) SPR_GROUP_##group) << SPR_GROUP_SHIFT)
#define SPR_GROUP_LAST(group) (SPR_GROUP_FIRST | (((UWI) 1 << SPR_GROUP_SHIFT) - 1))
#define SPR_ADDR(group,index) (SPR_GROUP_FIRST(group) | ((UWI) SPR_INDEX_##group##_##index))
#define SPR_FIELD(group,index,field,val) ((SPR_FIELD_MASK_##group##_##index##_##field & (val)) >> SPR_FIELD_LSB_##group##_##index##_##field)

#ifdef WANT_CPU_OR1K32BF
void or1k32bf_cpu_init (SIM_DESC sd, sim_cpu *current_cpu);
void or1k32bf_insn_before (sim_cpu *current_cpu, SEM_PC vpc);
SEM_PC or1k32bf_insn_after (sim_cpu *current_cpu, SEM_PC vpc);
#endif

#endif
