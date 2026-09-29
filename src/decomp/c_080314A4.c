#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080314A4.
 * sub_080314A4 @ 0x080314A4
 */

/* Draws the four army rows of the status window: one FillTilemapRect frame per
 * army at y = 6, 8, 10, 12, then either the icon at gUnknown_08090CF4's
 * halfword (when gUnknown_0849B018->unk0a[i] is a small code) or the army's
 * 13-byte name row out of gUnknown_020257E4.
 *
 * The parameter is s16 and the two shift pairs at entry are ONE variable, not
 * two: PROMOTE_MODE zero-extends every sub-word parameter regardless of
 * signedness, so the `lsls #16; lsrs #16` is the entry normalisation and the
 * `lsls #16; asrs #16` right after it is the sign. The result is spilled once
 * and every later test reads the spill.
 *
 * THE y COORDINATE IS NOT A VARIABLE. r8 holds `(2 * i + 6) << 16` -- base
 * 0x60000, step 0x20000 -- and every read is one shift out of it: `lsrs #16`
 * for FillTilemapRect's u16 parameter, `asrs #16` for the int parameters of
 * PutTextTableEntryImmediate and PutTextScriptImmediate. That is a giv of the loop counter with no
 * biv of its own. Spelled as an s16 local it costs a truncation per
 * iteration; spelled as an int local the plain biv survives beside the
 * shifted giv and gcc rebuilds one from the other (`ldr/adds #2/str`) --
 * either way there is a second induction variable the ROM does not have. */

void LinkLobbyDrawPlayerRows(s16 a)
{
    int i;
    u16 buf[5];

    sub_0808B6E8(buf, gUnknown_08090CF4, 10);

    if (a == 2)
        return;

    for (i = 0; i < 4; i++)
    {
        FillTilemapRect(gBG0TilemapBuffer, 0xe, 2 * i + 6, 8, 2, 0);

        if (a == 0 || SioIsPlayerLinked(i))
        {
            if (gUnknown_0849B018->unk0a[i] <= 4)
            {
                PutTextTableEntryImmediate(0xe, (s16)(2 * i + 6), gBG0TilemapBuffer,
                    buf[gUnknown_0849B018->unk0a[i]], 0x8000, 0);
            }
            else
            {
                PutTextScriptImmediate(0xe, (s16)(2 * i + 6), gBG0TilemapBuffer,
                    (u8 *)gUnknown_020257E4[i], 0x8000, 0);

                if (a == 1)
                {
                    if (gUnknown_0300449C[i] != 0)
                        PutTextScriptImmediate(0xe, (s16)(2 * i + 6), gBG0TilemapBuffer,
                            (u8 *)gUnknown_020257E4[i], 0x8000, 0);
                    else
                        PutTextScriptImmediate(0xe, (s16)(2 * i + 6), gBG0TilemapBuffer,
                            (u8 *)gUnknown_020257E4[i], 0x7000, 0);
                }
            }
        }
    }

    BG_EnableSyncBG0();
}
asm(".global sub_080314A4\n.thumb_set sub_080314A4, LinkLobbyDrawPlayerRows\n");
