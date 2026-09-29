#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805F0EC.
 * sub_0805F0EC @ 0x0805F0EC
 */

/* Scores every passable cell by the gUnknown_0202DAD8 influence record for its
 * 4x4 block, keeps the best, and hands the winning cell to AiAdvanceToward -- the
 * same 4-byte (u16, u16) buffer contract the rest of the 0x08059/0x0805F block
 * uses. AiProtectHq is its twin; c_08058BB4.c is the vocabulary exemplar.
 *
 * MATCHED in Wave 63 after splitting the packed store into three statements.
 * The configured matcher accepts both literal-pool relocations below as
 * equivalent and reports byte-identical linked output.
 *
 *  1. ONE BYTE is the pool word at +0x14c. The ROM has
 *     `.word gUnknown_085D584C` (addend 0); this emits
 *     `.word gUnknown_085D583C + 0x10`. 0x085D584C IS 0x085D583C + 0x10 --
 *     it is &gUnknown_085D583C[0].unk10, the same 0x14-stride terrain table
 *     c_08058BB4.c reads, and gen_lds.py invented the second name for the
 *     mid-array address agbcc's own address-constant pool produced. This is the
 *     "different symbols that resolve to the same address" case; it differs
 *     pre-link only because the addend lives in the word. DO NOT declare
 *     gUnknown_085D584C to make it go away -- it is not an object.
 *
 *  2. TWO BYTES are one transposition in the packed store. The ROM interleaves
 *     the (u16)x truncation with the y shift:
 *         lsls r0, r5, #16 / lsls r1, r6, #16 / lsrs r0, r0, #16 / orrs r0, r1
 *     Wave 63 reached that order by making the two shifted values real
 *     temporaries before the store: `xp = x << 16; yp = y << 16;` followed by
 *     `(xp >> 16) | yp`. This is the same scheduling lever independently found
 *     on AiMoveToNearestNonTeamCell. Earlier one-expression, comma-anchor, split-store and
 *     540-second permuter probes did not reach the interleaving.
 *
 *  3. WHAT THE PERMUTER FOUND, and it is a general result: `p` must be assigned
 *     AFTER the loop nest, not before it and not inside it. The ROM binds &v in
 *     the loop-guard block, between `ldr r2,[r0]` and `ldrh r3,[r2,#2]`.
 *     Assigned before the `for` it lands ahead of the map load; assigned in
 *     either loop body LICM hoists it past the guard branch; assigned after the
 *     loop it lands exactly where the ROM has it. That one move took this from
 *     95.8% to 98.9% and fixed six register assignments at once.
 *
 * TWO MORE THINGS MEASURED HERE, both worth reusing:
 *  - `cx`/`cy` MUST be bound before the gUnknown_0202DAD8 subscript. Written
 *    inline as gUnknown_0202DAD8[y / 4][x / 4], agbcc interleaves the second
 *    division with the first multiply and needs an extra `mov r2, r1` to get
 *    x / 4 into the multiply's destination (+2 bytes). Bound, both divisions
 *    are emitted first, exactly as the ROM does.
 *  - That same change FLIPPED which address constant wins sl. Inline,
 *    gUnknown_085D5ABC took the callee-saved register and
 *    gUnknown_085D583C+0x10 was rematerialised; bound, it is the other way
 *    round, which is what the ROM has. So the "pseudo created first wins the
 *    callee-saved register" tie-break was NOT the lever -- the lever was a
 *    pressure change two statements earlier. Chasing the pool order directly
 *    would have missed it.
 *
 * gUnknown_0816DAA4 is NOT an object: the ROM word holds 0x030040D8, verified
 * in baserom.gba, so it is agbcc's own -fforce-addr address constant for
 * gUnknown_030040D8. Naming gUnknown_030040D8 honestly is correct and the
 * promotion carries "rodata": ["0x0816DAA4"] -- same reading as W48-J's
 * gUnknown_0816D93C -> gUnknown_08499590. */
void AiMoveToHighInfluence(void)
{
    union Unk802C57CBuf v;
    u8 cost;
    u8 best2;
    u32 best;
    u32 xp;
    u32 yp;
    u16 *p;
    int x;
    int y;
    int cx;
    int cy;
    int score;
    int sum;

    best = 1;
    best2 = 0;
    AiGetReachBudget(&cost);
    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                      gUnknown_030040D8->unk00, cost, best2);
    v.raw = (v.raw & 0xFFFF0000) | 0x270F;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] < 0)
                continue;

            cx = x / 4;
            cy = y / 4;

            sum = gUnknown_0202DAD8[cy][cx].unk00[5]
                + gUnknown_0202DAD8[cy][cx].unk00[6]
                + gUnknown_0202DAD8[cy][cx].unk00[7];

            if (gUnknown_030046B8 & 1)
                sum += gUnknown_0202DAD8[cy][cx].unk00[8];
            if (gUnknown_030046B8 & 2)
                sum += gUnknown_0202DAD8[cy][cx].unk00[9];

            if (best <= sum)
            {
                best = sum;

                if (gUnknown_030040D8->unk00 == 0
                 || gUnknown_085D5ABC[gUnknown_030040D8->unk00].deployLocation != 0x10)
                    score = gUnknown_085D583C[
                        gMap->terrain[
                            gMap->rowOffset[y]
                            + x] & 0x1f].defense * 10;
                else
                    score = 0;

                if (best2 < score)
                {
                    best2 = score;
                    xp = (u32)x << 16;
                    yp = (u32)y << 16;
                    v.raw = (xp >> 16) | yp;
                }
            }
        }
    }

    p = (u16 *)&v;

    if (p[0] == 0x270F)
        AiEmbarkOrFallback();
    AiAdvanceToward(p);
    AiFallbackMove();
}
asm(".global sub_0805F0EC\n.thumb_set sub_0805F0EC, AiMoveToHighInfluence\n");
