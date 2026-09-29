#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B18C.
 * sub_0803B18C @ 0x0803B18C, sub_0803B198 @ 0x0803B198
 */

#include "hardware.h"

/* Twelve bytes: one call, then a literal 0 in r0 and `pop {r1}; bx r1`. The
 * `pop {r1}` epilogue is the one that PRESERVES r0, so unlike the `pop {r0}`
 * forwarders all around it this one returns a value, and the value is the
 * literal rather than GetNextRandomNumber's -- the `movs r0, #0` sits after the `bl`
 * and overwrites it. The declared `bool8` return in
 * include/unknown-functions.h is kept; `int` compiles identically here, since
 * a returned literal 0 needs no narrowing either way. */

bool8 sub_0803B18C(void)
{
    GetNextRandomNumber();

    return FALSE;
}

/* A/START (9 = 1 | 8) on gpKeySt->pressed plays sound id 0x71 and stops the
 * gUnknown_03001FBC entry; either way the tail runs. Both tail calls are
 * unconditional and neither result is used, so they are two statements and not
 * a nesting -- sequential `bl`s carry no nesting signal (docs/agbcc-codegen.md)
 * and DebugScreenNoOp is a bare `bx lr` that reads nothing.
 * `ldrb` on the `s16` gUnknown_03001FBC is ClearSlotScriptCallback's `u8` parameter
 * conversion folded into the load, exactly as in DebugEntry_Loop above.
 * `pop {r0}; bx r0` -> void. */

void sub_0803B198(void)
{
    if (gpKeySt->pressed & 9)
    {
        PlayMusicOrSfx2(0x71);
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }

    GetNextRandomNumber();
    DebugScreenNoOp();
}
