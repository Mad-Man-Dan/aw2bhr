#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BA1C.
 * sub_0803BA1C @ 0x0803BA1C
 */

/* MATCHED byte-for-byte (wave 37, W37-Q2).
 *
 * gUnknown_0200C420.unk38 is a WORD and not part of the old filler_38[0xa8]:
 * `ldr r1,[r0,#0x38]; ands r1,=0xFFF000FF; str r1,[r0,#0x38]` is a whole-word
 * read-modify-write clearing bits 8..19.
 *
 * Wave 43 (W43-E) retyped unk38 as an array of 0x2a 8/12/12 bitfield records
 * (see include/unknown-globals.h), so the literal mask is now the bitfield
 * clear it always was, and this file was re-verified byte-identical after the
 * change. W37-Q2's note that a bitfield clear "narrows to a byte-sized
 * `movs #N; rsbs; ands`" was wrong: that holds only for a field store_bit_field
 * can reach in QImode, and this one spans bytes 1..2, so it goes out in SImode
 * against the same 32-bit pool constant.
 *
 * The loop counter is a signed int, not the u8 the neighbouring functions in
 * this block use: the exit test is `cmp r4,#0x9f; ble` on a plain `adds r4,#1`
 * with no `lsls`/`lsrs` wraparound pair. */

void ClearCampaignFlags60To9F(void)
{
    int i;

    ClearWorldMapMarkers();
    gUnknown_0200C420.unk38[0].unk00_08 = 0;

    for (i = 0x60; i <= 0x9f; i++)
        SetCampaignCompletionFlag(i, 0);
}
asm(".global sub_0803BA1C\n.thumb_set sub_0803BA1C, ClearCampaignFlags60To9F\n");
