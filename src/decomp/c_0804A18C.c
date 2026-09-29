#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804A18C.
 * sub_0804A18C @ 0x0804A18C, sub_0804A1E4 @ 0x0804A1E4
 */

/* Four parallel ROM tables selected by a pair of flags. All four arms compute
 * only an ADDRESS and share one `ldrb` at the tail -- gcc cross-jumping the
 * four identical loads, not four separate returns.
 *
 * The INNER test of the unk5c == 0 arm is written `!= 0` even though `== 0`
 * with the arms swapped is the same predicate: the ROM branches `bne` to the
 * gUnknown_084C3C01 arm, and only the `!= 0` spelling puts gUnknown_084C3BA6
 * in the fallthrough. Semantically identical, one byte apart. */
u8 GetNameEntryGridChar(u8 a1)
{
    if (gUnknown_030044E0->unk5c != 0)
    {
        if (gUnknown_030044E0->unk66 != 0)
            return gUnknown_084C3CB7[a1];
        else
            return gUnknown_084C3C5C[a1];
    }
    else
    {
        if (gUnknown_030044E0->unk66 != 0)
            return gUnknown_084C3C01[a1];
        else
            return gUnknown_084C3BA6[a1];
    }
}
asm(".global sub_0804A18C\n.thumb_set sub_0804A18C, GetNameEntryGridChar\n");

/* GetStringPixelWidthSimple returns int, NOT u8, and this function is the proof: the ROM
 * stores the result with a bare `strb` and only THEN narrows it, `lsls r0,#24`
 * with no `lsrs`, as a truth test. Declared u8, agbcc re-narrows the result at
 * the call site (`lsls #24; lsrs #24`) BEFORE the store, which is two extra
 * instructions in the wrong place. The `(u8)` cast on the test is what
 * produces the ROM's lone `lsls`.
 *
 * FillTilemapRect's third and fifth arguments are BOTH 2 -- the ROM materialises
 * the constant once in r2 and both passes it and stores it to the stack slot,
 * which reads like one argument until the arity is checked. */
void RedrawNameEntryText(u8 a1)
{
    int v;

    ResetTextTileCache(0xb0);
    FillTilemapRect(gBG0TilemapBuffer, gUnknown_030044E0->unk61, 2,
                 gUnknown_030044E0->unk5e + 1, 2, 0);
    PutTextScriptImmediate(gUnknown_030044E0->unk61, 2, gBG0TilemapBuffer,
                 gUnknown_030044E0->unk2c, 0x8000, 0);

    v = GetStringPixelWidthSimple(gUnknown_030044E0->unk2c);
    gUnknown_030044E0->unk65 = v;

    if ((u8)v != 0)
        gUnknown_030044E0->unk65 -= a1;

    BG_EnableSyncBG0();
}
asm(".global sub_0804A1E4\n.thumb_set sub_0804A1E4, RedrawNameEntryText\n");
