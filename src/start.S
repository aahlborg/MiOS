.section ".text.startup"
.global _start
_start:
  /* This is actually the interrupt vector table.
   * The first instruction runs _reset which moves
   * the table into place and starts our program.
   */
  ldr pc, _reset_vec
  ldr pc, _illegal_instruction_vec
  ldr pc, _software_interrupt_vec
  ldr pc, _prefetch_abort_vec
  ldr pc, _data_abort_vec
  ldr pc, _unused_vec
  ldr pc, _interrupt_vec
  ldr pc, _fast_interrupt_vec

/* Interrupt vector table pointers */
_reset_vec:               .word _reset
_illegal_instruction_vec: .word illegal_instruction_isr
_software_interrupt_vec:  .word software_interrupt_isr
_prefetch_abort_vec:      .word prefetch_abort_isr
_data_abort_vec:          .word _reset
_unused_vec:              .word _reset
_interrupt_vec:           .word interrupt_isr
_fast_interrupt_vec:      .word fast_interrupt_isr

.global _reset
_reset:
  /* Get core number */
  mrc p15, 0, r3, c0, c0, 5
  mov r4, #0x3
  and r3, r4
  /* Calculate and set stack pointer
   * Start at 0x80000 and allocate 0x4000 for each core.
   * sp starts at end of stack and grows downwards.
   */
  add r4, r3, #1
  mov r5, #0x4000
  mul r5, r4, r5
  ldr r4, =_stack
  add sp, r5, r4
  /* Core 1-3 go directly to _core_main */
  cmp r3, #0
  movne r0, r3
  bne _core_main
  /* Save r0-r2 which contain context information
   * set by the bootloader.
   */
  push {r0, r1, r2}
  /* Copy interrupt vector table to 0x0 */
  ldr r0, =_start
  mov r1, #0x0
  ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
  stmia r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
  ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
  stmia r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
  /* Restore r0-r2 */
  pop {r0, r1, r2}
  /* Jump to main */
  b _cstartup
