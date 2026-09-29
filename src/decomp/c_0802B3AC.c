#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802B3AC.
 * sub_0802B3AC @ 0x0802B3AC
 */

/* MATCHED, wave 77 (W77-L). 296/296 bytes, relocs match, trymatch exit 0.
 *
 * Parked since wave 65 and reduced to four bytes at +0xD8 in wave 71:
 *   ROM        lsls index,#1; adds table,#2; adds index,table
 *   candidate  adds index,#1; lsls index,#1; adds index,table
 * i.e. the second table element's +2 had to ride on the BASE, applied to the
 * -fforce-addr pseudo AFTER the index is scaled, not on the index before it.
 *
 * THE LEVER WAS ALREADY IN THE TREE. src/decomp/c_0802AA14.c -- a promoted,
 * byte-matched file reading THIS SAME TABLE -- carries the derivation and the
 * spelling in its own header comment, and it names the two failure modes this
 * entry had recorded as dead ends without recognising them:
 *   `gUnknown_0849A2A6[a4 * 3 + 1]` folds the element into the index, and
 *   `gUnknown_0849A2A6 + 1` folds it into the relocation.
 * Only reaching the array THROUGH a struct member gives the ROM's add on the
 * loaded base. So arm 2 is
 *   ((struct Tbl49A2A6 *)gUnknown_0849A2A6)->unk02[c * 3]
 * while arm 1 stays flat `gUnknown_0849A2A6[c * 3]` -- the ROM has two pool
 * words for the same clean symbol and only the second one gets `adds r2,#2`.
 * The struct is a leading `s16 unk00;` plus `s16 unk02[0x100];`. The extent
 * matters and a struct of three s16 does NOT work: ARM's
 * STRUCTURE_SIZE_BOUNDARY is 32, so a 6-byte struct rounds to 8 and the index
 * would scale by 8. Wave 71's "local struct with a leading halfword" negative
 * was this lever with the wrong member shape.
 *
 * A 2D row view -- `((const s16 (*)[3])gUnknown_0849A2A6)[c][1]` -- is NOT a
 * substitute: it regresses to 84 differing bytes from +0x28 (71.6%).
 *
 * THE THREE asm CONSTRUCTS BELOW ARE ALL LOAD-BEARING; each was removed and
 * re-measured against this matching body:
 *   drop `asm("" : "+r" (tmp))` and the u8 temp   -> -4 bytes, 22.6%
 *   drop the two `register ... asm("rN")` pins    -> +4 bytes, 39.9%
 *   drop both                                     -> size-exact, 56.4%
 * The empty read/write constraint is the wave-75 lever (see the chapter in
 * docs/agbcc-codegen.md; it closed SoundScope_Loop). The register pins are a
 * measured COUNTEREXAMPLE to wave 77's "pinning is catastrophic" claim: here
 * they are worth the match. None of the three emits an instruction, so the
 * recorded residual was never fake in the W77-A sense -- but this body is not
 * plausible original source, and promotion should be a coordinator decision.
 *
 * Keep the clean-symbol BattleUnit member access and the `||` guard shape;
 * both remain required. */
#include "proc.h"
struct Tbl49A2A6
{
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02[0x100];
};

void DrawCursorDamagePreview(s16 a, s16 b, s16 c)
{
    u8 tmp;
    register int d asm("r8");
    int idx;
    register int x asm("r0");

    tmp = GetClockPhase();
    asm("" : "+r" (tmp));
    d = tmp;

    if (Proc_Find(gUnknown_0849A02C) == NULL)
        return;

    if (FindInventionAt(gUnknown_030033E4.unk00, gUnknown_030033E4.unk02) != NULL
        || gUnknown_020288B4[idx =
               gMap->rowOffset[gUnknown_030033E4.unk02]
               + gUnknown_030033E4.unk00] != 0)
    {
        CalcBattleDamageVsStructure(gUnknown_03003F38);
        x = (u16)gUnknown_0849A2A6[c * 3] + a + 0x18;
        PutRightAlignedNumberSpritesPlusSprite(x, b - 0x14 - (s8)d,
                     gBattleAttacker->displayDamage, 0x1a6);
        return;
    }

    CalcAttackOutcome(gUnknown_03003F38,
                 gMap->unit[idx],
                 &gUnknown_03003100.pos);
    x = (u16)((struct Tbl49A2A6 *)gUnknown_0849A2A6)->unk02[c * 3] + a + 0x1b;
    PutRightAlignedNumberSpritesPlusSprite(x, b - 0x14 - (s8)d,
                 gBattleAttacker->displayDamage, 0x1a6);
}
asm(".global sub_0802B3AC\n.thumb_set sub_0802B3AC, DrawCursorDamagePreview\n");
