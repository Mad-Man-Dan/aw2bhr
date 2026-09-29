#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806F000.
 * sub_0806F000 @ 0x0806F000
 */

/* One `s16` LOCAL whose live range spans the FillTilemapRect call, and that is the
 * whole type argument: `lsls #0x10; lsrs #0x10` produces the u16 the callee
 * wants while LEAVING the shifted value in r4, then `asrs r4,#0x10` after the
 * `bl` re-signs the same register for StartTextBoxViaRecord's `int`. Two spellings of
 * one variable, not a u16 and a cast.
 *
 * `v = 0` sits ABOVE the `if`, not inside it -- the ROM's `movs r4,#0` is
 * ahead of `cmp r6,#0`.
 *
 * `movs r0,#0xc0; lsls r0,#6` is the constant 0x3000 written as a literal;
 * agbcc rebuilds the two instructions because it does not fit an imm8. */
void SoundRoomDrawTrackTitle(int a1, int a2)
{
    s16 v;

    v = 0;

    if (a1 != 0)
    {
        if ((a1 & 1) == 0)
            v = 0x10;

        FillTilemapRect(gBG0TilemapBuffer, v, 0x10, 0x10, 2, 0);
        StartTextBoxViaRecord(v, 0x10, gBG0TilemapBuffer, gUnknown_08582764[a1].unk00, 0x3000, a2);
        BG_EnableSyncBG0();
    }
}
asm(".global sub_0806F000\n.thumb_set sub_0806F000, SoundRoomDrawTrackTitle\n");
