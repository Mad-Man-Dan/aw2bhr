#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803ACB8.
 * sub_0803ACB8 @ 0x0803ACB8
 */

/* Family F011: `push {lr}; bl f; ldr r0,=g; movs r1,#N; bl h; pop {r0}; bx r0`.
 * Two statements and NOT a nest -- r0 is overwritten by the pool `ldr` between
 * the two calls, so nothing can survive from the first one. `pop {r0}` is the
 * void epilogue.
 *
 * gUnknown_0849E600 is a gUnknown_03001470 script blob -- IsDebugFlagControlActive next
 * door is its matching `FindSlotScript(script) != -1` liveness predicate. */
void StartDebugFlagControl(void)
{
    sub_080116E8();
    sub_080152EC(gUnknown_0849E600, 0);
}
asm(".global sub_0803ACB8\n.thumb_set sub_0803ACB8, StartDebugFlagControl\n");
