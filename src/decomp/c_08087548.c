#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08087548.
 * sub_08087548 @ 0x08087548
 */

/* Draws every unit in a contiguous run of the gUnknown_02027F74.unk04[]
 * candidate list, skipping the ones whose record is empty.
 *
 * The `subs r0, #0x6c` bias is the same idiom AreAllWarRoomMapsRecorded matched over this
 * table: gUnknown_0200C078 is indexed by (id - 0x6c) with the 0x14 stride, and
 * the bias belongs in the SUBSCRIPT, not in a separate variable.
 *
 * `lsls #0xc; lsrs #0x14` and `ldrh [r1,#2]; lsrs #4` are the two BITFIELD
 * members of struct Unk0200C078Rec (the 8/12/12 word W38-H established), not
 * casts -- unk00_08 at bits 8..19 and unk00_14 at bits 20..31. The record
 * address is CSEd into one pseudo across both reads.
 *
 * The subscript is written INLINE rather than bound to a local: with a local
 * the gUnknown_02027F74 address pseudo is created first and the two pool words
 * come out in the wrong order. Spelled inline, the outer array's base address
 * is expanded before the index, so gUnknown_0200C078 wins the first pool slot,
 * which is what the ROM has.
 */

void DrawWarRoomRanks(int a, int n, int c)
{
    int i;

    for (i = 0; i < n; i++)
        if (gUnknown_0200C078[gUnknown_02027F74.unk04[a + i] - 0x6c].unk00[0].unk00_08 != 0)
            DrawMapListRankIcon(
                GetRankFromScore(gUnknown_0200C078[gUnknown_02027F74.unk04[a + i] - 0x6c].unk00[0].unk00_14),
                i, c);
}
asm(".global sub_08087548\n.thumb_set sub_08087548, DrawWarRoomRanks\n");
