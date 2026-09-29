#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026040.
 * sub_08026040 @ 0x08026040
 */

/* Deals the four army rosters out of the pristine bank in gUnknown_02022684:
 * one 0xC00-byte scratch block, 0x100 records copied into it, then one
 * CopyRosterToArmy per quarter (64 records each). The `1, 2, 3, 4` early-out is a
 * debug guard and is reproduced as written.
 *
 * The copy loop is a STRUCT ASSIGNMENT of the 0x0c-byte struct Unit,
 * which is where the `ldm r0!, {r5,r6,r7}` / `stm r1!, {r5,r6,r7}` pair comes
 * from -- not a memcpy and not word-at-a-time. The element type is fixed by
 * CopyRosterToArmy's promoted definition, which takes `struct Unit *`.
 *
 * `subs r3,#1; cmp r3,#0; bge` at the bottom is check_dbra_loop's rewrite of
 * the ASCENDING `i < 0x100` counter, not a descending source loop. The four
 * arguments living in sl / [sp] / r8 / sb is plain register allocation; they
 * are four ordinary ints. */
void RemapArmyRosters(int a1, int a2, int a3, int a4)
{
    struct Unit *buf;
    int i;

    if (a1 == 1 && a2 == 2 && a3 == 3 && a4 == 4)
        return;

    buf = HeapMalloc(0xc00);

    for (i = 0; i < 0x100; i++)
        buf[i] = gUnknown_02022684[i];

    CopyRosterToArmy(&buf[0], a1);
    CopyRosterToArmy(&buf[0x40], a2);
    CopyRosterToArmy(&buf[0x80], a3);
    CopyRosterToArmy(&buf[0xc0], a4);
    HeapFree(buf);
}
asm(".global sub_08026040\n.thumb_set sub_08026040, RemapArmyRosters\n");
