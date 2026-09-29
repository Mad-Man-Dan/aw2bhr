#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08024330.
 * sub_08024330 @ 0x08024330
 */

/* A SWITCH, not an if/else chain, and the layout says so: the ROM tests both
 * constants up front and then falls into an unconditional `b` to the exit, with
 * the two bodies sitting past the literal pool. An `if/else if` puts the first
 * body in the fall-through and the second compare after it -- measured, and it
 * is a different instruction order, not just different labels.
 *
 * `__umodsi3` and not `__modsi3`, so the dividend is UNSIGNED. gGameClock
 * is declared `s32` in unknown-globals.h (inherited from src/title-screen.c,
 * where nothing settles it), hence the explicit cast rather than a retype --
 * every other access is `ldr` plus a mask and cannot tell the two apart.
 *
 * 0x050001C0 is PLTT + 0x1c0, i.e. BG palette 14. It is too large for a
 * `movs`/`lsls` pair, so the pool word is forced by the value. */

void sub_08024330(void)
{
    switch ((u32)gGameClock % 40)
    {
    case 0:
        CpuCopyAuto(gUnknown_0810E6E0, (void *)(PLTT + 0x1C0), 0x20);
        break;

    case 4:
        CpuCopyAuto(gUnknown_0810E720, (void *)(PLTT + 0x1C0), 0x20);
        break;
    }
}
