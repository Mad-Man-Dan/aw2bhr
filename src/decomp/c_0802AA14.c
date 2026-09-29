#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802AA14.
 * sub_0802AA14 @ 0x0802AA14
 */

/* The +2 must ride on the BASE, not the index. `gUnknown_0849A2A6[a4 * 3 + 1]`
 * folds the element into the index (`adds r0, #1` before the scale) and
 * `gUnknown_0849A2A6 + 1` folds it into the relocation
 * (`.word gUnknown_0849A2A6+0x2`); only wrapping the table in a struct and
 * reaching the array THROUGH a member gives the ROM's `adds r1, #2` on the
 * loaded base. This is the third `.rodata` spelling docs/agbcc-codegen.md
 * describes for a hoisted member offset, used here on ordinary ROM data.
 *
 * A struct of three s16 does NOT work as the element type: ARM's
 * STRUCTURE_SIZE_BOUNDARY is 32, so a 6-byte struct rounds up to 8 and the
 * index scales by `lsls #3` instead of the ROM's `* 6`. */
struct Tbl49A2A6
{
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02[0x100];
};

void DrawMapCursorInfoUnitName(int a1, int a2, int a3, int a4)
{
    RegisterDataMove((void *)GetUnitNameGraphic(a1), (void *)0x06013A40, 0x100);
    PutOamHi((a2 + ((struct Tbl49A2A6 *)gUnknown_0849A2A6)->unk02[a4 * 3]
                     + gUnknown_0849A284[2]) & 0x1ff,
                 (a3 + gUnknown_0849A284[3]) | 0x400,
                 gUnknown_0849A248, 0x1000);
}
asm(".global sub_0802AA14\n.thumb_set sub_0802AA14, DrawMapCursorInfoUnitName\n");
