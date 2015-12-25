.section ".text.startup"
.global _start
_start:
/* Set stack pointer */
mov sp,#0x8000
/* Jump to main */
b _cstartup
