#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065710.
 * sub_08065710 @ 0x08065710
 */

/* The `(s32)` cast on FindSlotScript's argument is the spelling every other
 * FindSlotScript caller in src/decomp already uses (c_080670D8.c reaches the
 * SAME script). `lsls #0x18; asrs #0x18` after the `bl` is agbcc re-narrowing
 * that callee's `s8` return before the compare against -1. */
void sub_08065710(void)
{
    DrawOamObject(0x68, 0xAC, 4, 0, 1);

    if (FindSlotScript((s32)gUnknown_08580DD8) == -1)
        ClearSlotScriptCallback(gUnknown_03001FBC);
}
