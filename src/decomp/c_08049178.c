#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049178.
 * sub_08049178 @ 0x08049178, sub_08049264 @ 0x08049264
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
/* Wave 43, W43-L. MATCHED, byte-for-byte, relocs match (236/236).
 *
 * Scrolls the record list up one row per frame for ten frames. Frame 0 kicks
 * the redraw off; frames 2..9 copy rows 2..limit of the tilemap scratch
 * gUnknown_084C30F8->unk032 into gBG0TilemapBuffer, sliding the destination
 * row down by `13 - limit`. Frame 10 ends the proc.
 *
 * unk64 is s16 and NOT the u16 that src/decomp/c_08049170.c's own file-local
 * struct calls it. The evidence is the entry `movs r4, #0; ldrsh r2, [r0, r4]`
 * -- per the wave-37 refutation, sign_extend of a HImode MEM always folds to
 * ldrsh, so a u16 member cannot produce it -- corroborated by the tail, where
 * the incremented value is re-widened `lsls #0x10; asrs #0x10` before `cmp #9`
 * (a u16 would use lsrs). c_08049170.c only STORES 0 to the member, which is
 * byte-identical either way, so it is not evidence and I have left it alone.
 *
 * A `switch`, not an if-chain: the literal low bound `cmp r2, #2; blt` survives
 * only because expand_case does not go through fold. Every `if` spelling of
 * `t >= 2 && t <= 9` folds the low bound away into an unsigned range test.
 *
 * THE ONE HARD-WON LINE is the destination index, and it is worth reading
 * before touching anything here. `(row + (13 - limit))` is byte-exact;
 * `(row + 13 - limit)` -- the same expression without the inner parentheses --
 * costs TWELVE bytes. fold rewrites the unparenthesised form as
 * `row - (limit - 13)`, which manufactures a genuinely outer-loop-invariant
 * subexpression `limit - 13`; LICM then hoists it into the preheader, consumes
 * sl for it, and spills the proc pointer to the stack (`sub sp, #4` /
 * `str r0, [sp]` where the ROM has `mov sl, r0`). The parenthesised form
 * survives fold intact, leaves nothing invariant to hoist, and emits the ROM's
 * three-instruction `adds r0, r1, #0` / `adds r0, #0xd` / `subs r0, r0, r7`.
 * This is the inverse of the usual reading: writing the invariant EXPLICITLY
 * is what stops the loop optimiser from inventing one. */

struct Unk8049178
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
};
/* Wave 43, W43-L. MATCHED, byte-for-byte, relocs match (184/184).
 *
 * The downward twin of ShopList_RiseStep: while unk1e + 2 is still within 9 it
 * blanks a row and copies rows 0..(5 - unk1e) of the tilemap scratch
 * gUnknown_084C30F8->unk032 into gBG0TilemapBuffer at a fixed halfword offset
 * of 0x100; past that it hands off to ClearSlotScriptCallback instead. unk1e advances
 * every frame either way.
 *
 * unk1e is s16, from the three `movs rI, #0x1e; ldrsh` reads (THUMB ldrsh has
 * no immediate-offset form, hence the register-offset spelling). The `ldrh`
 * beside them is the `+ 7` argument and the tail increment, where only the low
 * 16 bits matter, so it does not contradict the sign.
 *
 * Both loops reload unk1e from memory on every iteration because the `strh`
 * into gBG0TilemapBuffer may alias the proc; that is the ROM's behaviour and
 * falls out of writing the member access in place rather than caching it.
 *
 * THE ONE HARD-WON LINE is the destination index. `0x100 + proc->unk1e * 32 +
 * col` is byte-exact; `proc->unk1e * 32 + col + 0x100` and `col +
 * proc->unk1e * 32 + 0x100` are both 8 bytes wrong in the same way, and the
 * difference is ONLY where the `ldr r0, [gBG0TilemapBuffer]` lands. Leading
 * with the constant makes agbcc expand the base pointer load between the
 * `ldrsh` of unk1e and the `lsls #5` that scales it -- the ROM's order. Lead
 * with the index instead and the whole index is computed first and the base
 * loaded after. Same instructions, same count, two of them displaced; the
 * candidate scored 95.7% and size-exact, which is exactly the positional
 * near-miss the brief warns is worth reading as a diff and not as a score. */

struct Unk8049264
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
};

void ShopList_RiseStep(struct Unk8049178 *proc)
{
    s8 limit;
    u8 row;
    u8 col;

    if (gUnknown_084C30F8->unk836 == 0 || gUnknown_084C30F8->unk83c != 0)
    {
        Proc_Break(proc);
        return;
    }

    switch (proc->unk64)
    {
    case 0:
        gUnknown_084C30F8->unk838++;
        RenderShopListBuffer(0, 0);
        break;

    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        limit = proc->unk64 - 1;

        for (row = 2; row < limit; row++)
            for (col = 0; col < 0x14; col++)
                gBG0TilemapBuffer[(row + (13 - limit)) * 32 + col] =
                    gUnknown_084C30F8->unk032[row * 32 + col];
        break;
    }

    BG_EnableSyncBG0();

    proc->unk64++;

    if (proc->unk64 > 9)
        Proc_Break(proc);
}

void ShopList_CollapseStep(struct Unk8049264 *proc)
{
    u8 row;
    u8 col;

    if (proc->unk1e + 2 <= 9)
    {
        FillTilemapRect(gBG0TilemapBuffer, 0, proc->unk1e + 7, 0x12, 1, 0);

        for (row = 0; row < 5 - proc->unk1e; row++)
            for (col = 0; col < 0x14; col++)
                gBG0TilemapBuffer[0x100 + proc->unk1e * 32 + col] =
                    gUnknown_084C30F8->unk032[row * 32 + col];
    }
    else
    {
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }

    BG_EnableSyncBG0();

    proc->unk1e++;
}
asm(".global sub_08049264\n.thumb_set sub_08049264, ShopList_CollapseStep\n");

asm(".global sub_08049178\n.thumb_set sub_08049178, ShopList_RiseStep\n");
