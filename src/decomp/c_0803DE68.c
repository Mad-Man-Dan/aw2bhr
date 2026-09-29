#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803DE68.
 * sub_0803DE68 @ 0x0803DE68
 */

/* Clears the same 4-bit kind field out of both entry-0 headers. The shared
 * `0xFFFFFC3F` pool word is the 32-bit ~(0xF << 6) a bitfield store produces --
 * see the note on struct Unk02028360 in include/unknown-globals.h -- and CSE
 * keeps it in r1 across both writes. */
void ResetInventionRecords(void)
{
    gUnknown_02028360[0].unk02_6 = 0;
    gUnknown_020283E0[0].unk02_6 = 0;
    ClearInventionRecords();
}
asm(".global sub_0803DE68\n.thumb_set sub_0803DE68, ResetInventionRecords\n");
