#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08048FD8.
 * sub_08048FD8 @ 0x08048FD8
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

/* Wave 43, W43-L. MATCHED (228/228; the one reloc difference is the pool word
 * spelled against a different symbol, resolving to the same address).
 *
 * NEEDS ITS POOL WORD PLACED. The data/promoted.json entry must carry
 *   "rodata": ["0x0812A158"]
 * then re-run tools/split_rodata.py and tools/gen_lds.py. The word holds
 * &gUnknown_084C30F8, exactly as RenderShopListBuffer's 0x0812A144 does.
 *
 * Picks the next script to run. unk839 starts as gUnknown_0200C420.unk0f; if
 * that is above 1 it either forces 2 (when unk836 is clear) or walks the
 * sixteen predicates at gUnknown_084C24A4[3..0x12], compacts the passing
 * indices into unk83d, and picks one of them at random.
 *
 * `n` deliberately does double duty -- it is both the compaction counter and
 * the "something was chosen" flag. It is 1 on entry and is only reset to 0 on
 * the path that recomputes it, which is why the tail's `if (n != 0)` guard
 * reads a variable that otherwise looks like a pure count. Do not split it
 * into two locals; the ROM keeps one register (r5) live across both roles.
 *
 * `(u32)gGameClock` is required, not cosmetic: the global is declared
 * s32 but the ROM calls __umodsi3, and unknown-globals.h already records other
 * readers treating it unsigned.
 *
 * The store and the increment are SEPARATE statements, and that was the whole
 * of attempt 1's +4 bytes. `unk83d[n++] = i` binds the old n to its own temp
 * and increments BEFORE computing the address; splitting them lets agbcc
 * compute the address with n, store, and only then narrow n + 1, which is the
 * ROM's order.
 *
 * gUnknown_084C24A0 and gUnknown_084C24A4 are two declared views of ONE
 * 8-byte-stride ROM table -- see the note in unknown-globals.h for why both
 * exist and why a single struct array at ...A0 would relocate the second use
 * wrongly. */

void ShopScreen_PickGreeting(ProcPtr proc)
{
    u8 n;
    u8 i;

    n = 1;

    if (ShopScreen_StepOffsetToZero() != 0)
        return;

    gUnknown_084C30F8->unk839 = gUnknown_0200C420.unk0f;

    if (gUnknown_084C30F8->unk839 > 1)
    {
        if (gUnknown_084C30F8->unk836 != 0)
        {
            n = 0;

            for (i = 0; i <= 0xf; i++)
            {
                if (gUnknown_084C24A4[i + 3].unk00() == 1)
                {
                    gUnknown_084C30F8->unk83d[n] = i;
                    n++;
                }
            }

            gUnknown_084C30F8->unk839 =
                gUnknown_084C30F8->unk83d[(u32)gGameClock % n] + 3;
        }
        else
        {
            gUnknown_084C30F8->unk839 = 2;
        }
    }

    if (n != 0)
        ShopScreen_StartMessage(gUnknown_084C24A0[gUnknown_084C30F8->unk839].unk00);

    Proc_Break(proc);
}

asm(".global sub_08048FD8\n.thumb_set sub_08048FD8, ShopScreen_PickGreeting\n");
