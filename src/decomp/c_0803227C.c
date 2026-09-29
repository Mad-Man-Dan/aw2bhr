#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803227C.
 * sub_0803227C @ 0x0803227C
 */

#include "proc.h"
#include "hardware.h"

/* Steps the army-slot cursor left/right over gUnknown_0300449C, skipping empty
 * slots, and bumps unk0a when the slot actually changed.
 *
 * `SioCountLinkedPlayers` returns s8 (src/decomp/c_0802F504.c) yet the call site narrows
 * `lsls #0x18; lsrs #0x18` -- that is PROMOTE_MODE holding the s8 pseudo
 * zero-extended, and every READ of it re-extends signed (`lsls #0x18; asrs`).
 * It is not a u8 return.
 *
 * The two `if`s are independent, not an else-chain: `gpKeySt->pressed` is reloaded
 * for the second test. `i` is s16, so each step runs in the HIGH half
 * (`lsls #0x10; adds <-0x10000>; lsrs #0x10`) and the wrap test reads the sign
 * of that word for free. The `-0x10000` pool word and the LICM-hoisted
 * `((s8)n << 16) - 0x10000` preheader are the loop optimiser's, not source. */

void LinkPlayerCursor_HandleInput(void)
{
    s16 i;
    s8 n;

    i = gUnknown_0849B060->unk04;
    n = SioCountLinkedPlayers();

    if (gpKeySt->pressed & DPAD_LEFT)
    {
        do
        {
            i--;

            if (i < 0)
                i = n - 1;
        } while (gUnknown_0300449C[i] == 0);
    }

    if (gpKeySt->pressed & DPAD_RIGHT)
    {
        do
        {
            i++;

            if (i >= n)
                i = 0;
        } while (gUnknown_0300449C[i] == 0);
    }

    gUnknown_0849B060->unk06 = gUnknown_0849B060->unk04;
    gUnknown_0849B060->unk04 = i;

    if (gUnknown_0849B060->unk06 != i)
        PlayMusicOrSfx2(0x67);

    if (gUnknown_0849B060->unk04 != gUnknown_0849B060->unk06)
        gUnknown_0849B060->unk0a = (gUnknown_0849B060->unk0a + 1) & 1;
}
asm(".global sub_0803227C\n.thumb_set sub_0803227C, LinkPlayerCursor_HandleInput\n");
