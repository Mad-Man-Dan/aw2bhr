#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080604DC.
 * sub_080604DC @ 0x080604DC
 */

#include "proc.h"

/* Family F011: `push {lr}; bl f; ldr r0,=g; movs r1,#N; bl h; pop {r0}; bx r0`.
 * Two statements and NOT a nest -- r0 is overwritten by the pool `ldr` between
 * the two calls, so nothing can survive from the first one. `pop {r0}` is the
 * void epilogue.
 *
 * FOUR OF THE FAMILY'S NINE MEMBERS HAVE Proc_Start AS THE SECOND CALLEE, so
 * F011 is not "a table handed to an installer" -- families.py clusters on
 * shape and erases the callee, and here that spans two unrelated subsystems.
 * `movs r1, #3` is PROC_TREE_3, i.e. a root proc: this wrapper has no
 * parameter of its own for r1 to be forwarded from, which is the case the
 * proc-starter table in docs/agbcc-codegen.md distinguishes by r1 being
 * WRITTEN rather than left alone. Proc_Start returns the proc; `pop {r0}` says
 * this discards it. */
void sub_080604DC(void)
{
    sub_08023348();
    Proc_Start(ProcScr_DesignRoomFadeIn, PROC_TREE_3);
}
