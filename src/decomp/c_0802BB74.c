#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802BB74.
 * sub_0802BB74 @ 0x0802BB74, sub_0802BB98 @ 0x0802BB98
 */

/* Both parameters are u16. The first argument's `lsls #0x17; lsrs #0x17` is
 * NOT a standalone mask -- it is PROMOTE_MODE's `lsls #0x10; lsrs #0x10` fused
 * with the `& 0x1ff`. Spelling the parameter `int` costs a pool word for
 * 0x1ff and an `ands` instead. */
void sub_0802BB74(u16 a1, u16 a2)
{
    PutOamHi(a1 & 0x1ff, a2 | 0x400, gUnknown_0849A3B8, 0x1c);
}

/* gUnknown_08090BC0 and gUnknown_08090BC4 are agbcc address words, not globals
 * -- the ROM holds 0x030033E4 and 0x03003130 at those two addresses, and naming
 * the objects directly reproduces the three-level read exactly. See the note in
 * include/unknown-globals.h. */
void InitCursorInfoPanelPosition(void)
{
    if (gUnknown_030033E4.unk00 <= 7)
    {
        gUnknown_03003130.unk00 = 1;
        gUnknown_03003130.unk04 = 0xad;
        gUnknown_03003130.unk0c = 0x94;
    }
    else
    {
        gUnknown_03003130.unk08 = 0;
        gUnknown_03003130.unk04 = 3;
        gUnknown_03003130.unk0c = 3;
    }
    gUnknown_03003130.unk10 = gUnknown_030033E4.unk00;
    gUnknown_03003130.unk11 = gUnknown_030033E4.unk02;
}
asm(".global sub_0802BB98\n.thumb_set sub_0802BB98, InitCursorInfoPanelPosition\n");
