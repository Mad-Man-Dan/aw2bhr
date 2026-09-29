#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018018.
 * sub_08018018 @ 0x08018018
 */

/* The guarded twin of DrawCoPortrait: the same 6x6 fill of the gUnknown_08499588
 * tilemap, but cells whose `col + a` runs past 5 are blanked instead of drawn,
 * so `a` slides a six-wide window along the row.
 *
 * TWO things follow from the guard and both are needed for the match:
 *   - the store value must be bound to a `u16` LOCAL, not written inline as
 *     `map[...] = cond ? ... : 0`. The ROM evaluates the value first and the
 *     destination address second, and it truncates the true arm with an
 *     explicit `lsls #0x10; lsrs #0x10`. Written inline the address is
 *     computed first and the truncation folds into the `strh`. Binding the u16
 *     temp also flips the two LICM hoists (`row * 32` ahead of `row * 6`) into
 *     the ROM's order -- one fact, not two.
 *   - `pal << 12` stays inside the loop here, where DrawCoPortrait hoists it
 *     clear of both, because the guard puts it on a conditional path.
 *
 * BOTH counters are u8 (`lsls #0x18; lsrs #0x18` re-narrowing and unsigned
 * `bls`), where DrawCoPortrait's are u16. Read the width off each function's own
 * narrowing shifts; the two siblings genuinely differ. The `col + a <= 5` test
 * is signed (`bgt`) -- both operands promote to int.
 *
 * The `v = 9` preset with a conditional override to 1 is this block's house
 * idiom, shared with LoadPortraitFace and DrawCoPortrait. */
void DrawCoPortraitClipped(u8 a)
{
    u8 row;
    u8 col;
    int pal;
    u16 v;

    pal = 9;
    if (IsAnyWorldMapProcRunning())
        pal = 1;

    for (row = 0; row <= 5; row++)
    {
        for (col = 0; col <= 5; col++)
        {
            v = (col + a <= 5) ? (col + 0x20B + a + row * 6 + (pal << 12)) : 0;
            gUnknown_08499588[row * 32 + col] = v;
        }
    }

    UploadEventTilemap();
}
asm(".global sub_08018018\n.thumb_set sub_08018018, DrawCoPortraitClipped\n");
