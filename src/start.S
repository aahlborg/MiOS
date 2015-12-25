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
  /* Set stack pointer */
  mov sp, #0x8000
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
  bl _cstartup
  /* In case main returns */
  b kernel_panic
