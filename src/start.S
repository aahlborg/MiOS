/* Startup code
 * This takes over after the bootloader.
 * It initializes stack pointers, etc.
 * and prepares hand over to the C start code.
 */

/* CPSR register options */
.equ PSR_MODE_MASK,      0x0000001f
.equ PSR_MODE_USER,      0x00000010
.equ PSR_MODE_FIQ,       0x00000011
.equ PSR_MODE_IRQ,       0x00000012
.equ PSR_MODE_SVC,       0x00000013
.equ PSR_MODE_ABORT,     0x00000017
.equ PSR_MODE_HYP,       0x0000001a
.equ PSR_MODE_UNDEFINED, 0x0000001b
.equ PSR_MODE_SYSTEM,    0x0000001f
.equ PSR_T_BIT,          0x00000020
.equ PSR_F_BIT,          0x00000040
.equ PSR_I_BIT,          0x00000080
.equ PSR_A_BIT,          0x00000100

/* Macro for switching to SVC mode at boot
 * Newer bootloaders start the ARM core in HYP
 * mode and some extra steps are neccessary
 * to exit HYP.
 */
.macro safe_svcmode reg:req
  /* Read CPSR and check for HYP mode */
  mrs \reg , cpsr
  eor \reg, \reg, #PSR_MODE_HYP
  tst \reg, #PSR_MODE_MASK
  /* Set wanted CPSR value */
  bic \reg , \reg , #PSR_MODE_MASK
  orr \reg , \reg , #PSR_I_BIT | PSR_F_BIT | PSR_MODE_SVC
  /* Write CPSR directly if not in HYP */
  msrne cpsr_c, \reg
  bne resume$
  /* Prepare to exit HYP(erspace) */
  orr \reg, \reg, #PSR_A_BIT
  adr lr, resume$
  msr spsr_cxsf, \reg
  msr ELR_hyp, lr
  eret
resume$:
.endm


/* Kernel entry point
 * The bootloader will load this program
 * to 0x8000 and start execution here.
 */
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
  /* Make sure we boot up in SVC mode */
  safe_svcmode r9

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

  /* Configure IRQ stack */
  mov r4, #0x100000
  msr SP_irq, r4

  /* Core 1-3 go directly to _core_main */
  cmp r3, #0
  movne r0, r3
  bne _core_main

  /* Copy interrupt vector table to 0x0 */
  ldr r3, =_start
  mov r4, #0x0
  ldmia r3!, {r5, r6, r7, r8, r9, r10, r11, r12}
  stmia r4!, {r5, r6, r7, r8, r9, r10, r11, r12}
  ldmia r3!, {r5, r6, r7, r8, r9, r10, r11, r12}
  stmia r4!, {r5, r6, r7, r8, r9, r10, r11, r12}

  /* Jump to C main */
  b _cstartup
