#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08048850.
 * sub_08048850 @ 0x08048850
 */

/* Wave 43, W43-L. MATCHED (144/144; the one reloc difference is the pool word
 * spelled against a different symbol, which trymatch reports as resolving to
 * the same address).
 *
 * NEEDS ITS POOL WORD PLACED. The data/promoted.json entry must carry
 *   "rodata": ["0x0812A144"]
 * then re-run tools/split_rodata.py and tools/gen_lds.py. That word holds
 * &gUnknown_084C30F8 -- it is agbcc's own -fforce-addr address constant, which
 * is why the splitter invented a gUnknown_0812A144 symbol for it. Naming
 * gUnknown_084C30F8 directly (the wave-18 honest spelling) is what produced it.
 *
 * Redraws the record list: clears the tilemap scratch at
 * gUnknown_084C30F8->unk032, then draws one row per entry of the
 * 0xff-terminated u8 table gUnknown_02028E1C starting at a2, stopping at
 * either the terminator or a1 + 3 rows.
 *
 * `i * 2 + 2` and NOT `(i + 1) * 2` -- this was the whole of attempt 2 and is
 * the transferable finding. The two are the same value, but `(i + 1) * 2` lets
 * CSE share the `i + 1` with the loop's own increment, so agbcc folds the +1
 * inside the shift and then reuses the temp for `i++`; that is 4 bytes SHORT of
 * the ROM. Written `i * 2 + 2`, agbcc instead distributes the constant through
 * the QImode shorten and emits the ROM's `lsls r1, r5, #0x19` /
 * `movs r0, #0x80; lsls r0, r0, #0x12` / `adds` / `lsrs #0x18`, and computes
 * `i + 1` separately for the increment. Read that shift pair as the (u8)
 * truncation of a MULTIPLY, not as a mask plus a shift (the wave-16 error).
 *
 * Both parameters are u16 and the function is void. The declaration now in
 * unknown-functions.h is taken from this definition, not from a call site,
 * which is why the widths are firm. */

void RenderShopListBuffer(u16 a1, u16 a2)
{
    u16 i;

    InitTextTileCache(0);
    FillTilemapRect(gUnknown_084C30F8->unk032, 0, 0, 0x14, 0xa, 0);

    for (i = 0; i < a1 + 3 && gUnknown_02028E1C[a2 + i] != 0xff; i++)
        DrawShopItemRow(0, i * 2 + 2, gUnknown_084C30F8->unk032,
            gUnknown_02028E1C[a2 + i], 3);
}
asm(".global sub_08048850\n.thumb_set sub_08048850, RenderShopListBuffer\n");
