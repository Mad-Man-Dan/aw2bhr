#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803DE14.
 * sub_0803DE14 @ 0x0803DE14
 */

/* Clears all sixteen gUnknown_02028360 records (16 = the extent fixed by
 * gUnknown_020283E0 sitting exactly 0x80 bytes later).
 *
 * The three read-modify-write groups are struct Unk02028360's bitfields and
 * each one's ACCESS WIDTH is what names it: `ldrb`/`strb` at +2 with ~0x38
 * then ~0x07 for unk02_3 and unk02_0 (two separate 3-bit inserts, not one
 * 6-bit field -- a single field would clear with ~0x3f once), `ldrh`/`strh`
 * at +2 with 0xFFFFFC3F for unk02_6 because bits 6..9 straddle the byte
 * boundary, and `ldrb`/`strb` at +3 with ~0x3c for unk02_a. Exactly the
 * boundaries the type's own comment already records from AddInventionRecord and
 * InventionTurn_ReloadCountdowns, reached here from the clearing side.
 *
 * Only ONE `strb` comes out of the first pair: agbcc CSEs the +2 byte across
 * the two inserts and then drops the first store as dead. */
void ClearInventionRecords(void)
{
    int i;

    for (i = 0; i < 16; i++) {
        gUnknown_02028360[i].unk02_3 = 0;
        gUnknown_02028360[i].unk02_0 = 0;
        gUnknown_02028360[i].unk01 = 0;
        gUnknown_02028360[i].unk00 = 0;
        gUnknown_02028360[i].unk02_6 = 0;
        gUnknown_02028360[i].unk04 = 0;
        gUnknown_02028360[i].filler_05[0] = 0;
        gUnknown_02028360[i].unk02_a = 0;
        gUnknown_02028360[i].unk06 = 0;
        gUnknown_02028360[i].unk07 = 0;
    }
}
asm(".global sub_0803DE14\n.thumb_set sub_0803DE14, ClearInventionRecords\n");
