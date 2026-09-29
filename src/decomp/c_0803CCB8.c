#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CCB8.
 * sub_0803CCB8 @ 0x0803CCB8
 */

/* `u8 i = id;` and not `gUnknown_020280C0[(u8)id]` at each use: the cast form
 * makes agbcc materialise &gUnknown_020280C0 before narrowing the index, which
 * lengthens the base's live range and swaps the register roles -- the ROM
 * keeps `dst` in the callee-saved r4 and the table base in the scratch r3, and
 * the cast form gets that backwards. Same instructions, different allocation.
 *
 * unk13 == 0xff is the empty-slot test struct Unk020280C0 already documents. */
bool8 LoadDesignRoomName(int id, u8 *dst)
{
    u8 i = id;

    if (gUnknown_020280C0[i].unk13 == 0xff)
        return 0;
    CopyString(dst, gUnknown_020280C0[i].unk02);
    return 1;
}
asm(".global sub_0803CCB8\n.thumb_set sub_0803CCB8, LoadDesignRoomName\n");
