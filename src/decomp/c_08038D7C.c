#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08038D7C.
 * sub_08038D7C @ 0x08038D7C
 */

/* MATCHED wave 87 (W87-A), 744/744 bytes, exit 0 under the `configured`
 * profile, after 27 waves parked.  NOTE FOR PROMOTION: this match needs its
 * four -fforce-addr .rodata pool words PLACED -- add to data/promoted.json
 *   "rodata": ["0x08090F1C", "0x08090F20", "0x08090F24", "0x08090F28"]
 * then re-run tools/split_rodata.py and tools/gen_lds.py.  The reloc lines
 * trymatch prints for those four offsets are the disassembly artefact, not a
 * difference.
 *
 * WHAT CLOSED IT -- both of wave 60's named spots, from ONE change of two
 * characters plus one statement:
 *
 *     s8 n;
 *     s8 m;          <- ADDED
 *     n = FindMovePathStep(unk00, unk02);
 *     m = n;         <- ADDED
 *     if (n != -1) { TruncateMovePath((s8)(m + 1)); ... }
 *
 * SPOT 1 (6 bytes) was W86-C's rule "TWO EXTENSIONS OF ONE VALUE OFF ONE lsls
 * MEANS TWO SOURCE LOCALS" (docs/agbcc-codegen.md).  Wave 60 measured five
 * ONE-local spellings and, reasonably but wrongly, read the ROM's
 * `lsls #24 / movs #0x80 / lsls #0x11 / adds / asrs` as "agbcc expanded the QI
 * add in the high bits" -- an expand-time mystery.  It is not: it is combine
 * distributing the outer sign-extension shift over the add, which it only does
 * when the addend already has to be re-extended, i.e. when it is read out of a
 * SECOND QImode pseudo.  With one local there is one home and nothing to
 * distribute over.  IMPORTANT: both locals are `s8`.  The zero-extended home
 * (`lsrs r1,r0,#0x18`) is not a `u8` in the source -- it is PROMOTE_MODE
 * storing a QImode pseudo zero-extended.  `u8 m` gives the SHORT
 * `adds #1 / lsls / asrs` form; only `s8 m` gives the ROM's five instructions.
 * Measured, all with `s8 n; ... m = n;` in place:
 *     s8  m, (s8)(m + 1)          -> ROM's 5 insns          <- THIS
 *     u8  m, (s8)(m + 1)          -> adds #1 / lsl / asr
 *     u8  m, (s8)(1 + m)          -> same short form
 *     u8  m, m + 1 (no cast)      -> adds #1, no narrowing
 *     u8  m, m++ then (s8)m       -> same short form
 *     u8  m, m++ then m           -> lsl / lsr (wrong sign)
 *     u8  m, s8 t = m + 1         -> same short form
 *     u8  m, (s8)(u8)(m + 1)      -> same short form
 *     u8  m, ((int)m<<24)+0x1000000>>24 -> ROM's 5 insns, but hand-written
 * The declaration ORDER of n and m and the direction of the copy are
 * byte-neutral (`u8 m = f(); s8 n = m;` gives the same triple).
 *
 * SPOT 2 (the `?:` cross-jump, 2 bytes) NEEDED NO CHANGE.  Wave 60 predicted
 * it would move once spot 1 added a live value, and it did: with `m` live
 * across the compare the two arms no longer land the row pointer in the same
 * register, so cross-jumping stops one instruction earlier and the ROM's
 * duplicated `adds r1,r4,rX` appears in both arms.  Do not read a
 * cross-jumping residual as a source difference before fixing every other
 * residual in the function.
 *
 * Everything below this header is wave 60's draft unchanged, and its reading
 * notes still stand:
 *   - `ABS()` is include/global.h's macro; `ABS(a - b) == 1` compiles to the
 *     ROM's `subs; cmp #0; blt; cmp #1; beq` plus a recomputed `subs r0,r6,r0`
 *     in the negative arm, because do_jump splits the COND_EXPR and fold turns
 *     `-(a - b)` into `b - a`.
 *   - the two `gUnknown_03004074 = ...` tails are ONE cross-jumped block in the
 *     ROM (_08038F9A); they must be written twice.
 *   - `(u8)IsMovePathSelfAvoiding()` reproduces the ROM's `lsls r0,r0,#0x18; cmp r0,#0`.
 *     The promoted definition returns `int`, so the cast belongs at the call.
 *   - gUnknown_0849D5F8->unk1e / unk1f are declared `u8` and the ROM reads them
 *     with `ldrsb`; the `(s8)` casts here are that, and the shared struct was
 *     NOT retyped.
 *   - the cost-table expression is src/decomp/c_08038848.c's verbatim.
 */

struct Unk38848Map
{
    /* 0x0000 */ u8 filler_0000[0x1432];
    /* 0x1432 */ u8 plane[0x417A - 0x1432];
    /* 0x417A */ u16 rowOffset[1];
};

void UpdateMovePathToCursor(void)
{
    struct Unk38848Map *map;
    s8 *costs;
    s8 *stack;
    s8 *cur;
    s8 v;
    s8 n;
    s8 m;
    int i;
    int c;

    v = ((s8 *)gUnknown_03003340[gUnknown_030033E4.unk02])[gUnknown_030033E4.unk00];

    if ((s8)gUnknown_0849D5F8->unk1e == gUnknown_030033E4.unk00
        && (s8)gUnknown_0849D5F8->unk1f == gUnknown_030033E4.unk02)
        return;

    SetMovePathLastCursor(gUnknown_030033E4.unk00, gUnknown_030033E4.unk02);

    if (v == -1)
        return;

    n = FindMovePathStep(gUnknown_030033E4.unk00, gUnknown_030033E4.unk02);
    m = n;

    if (n != -1)
    {
        TruncateMovePath((s8)(m + 1));
        gUnknown_03004074 = gUnknown_0849D5F8->unk38[0xc]
            - gUnknown_0849D5F8->unk38[gUnknown_0849D5F8->unk45];
        return;
    }

    i = gUnknown_0849D5F8->unk45;
    stack = gUnknown_0849D5F8->unk38;
    cur = &stack[i];

    costs = gUnknown_085D3DD0[gPlaySt.coAbilities
                ? gPlayers[(gUnknown_03003F38 >> 6) + 1].co
                : 1]
            .power[gPlayers[(gUnknown_03003F38 >> 6) + 1].coMode]
            .movementChart[gPlaySt.weather];

    map = (struct Unk38848Map *)gUnknown_08499590;

    c = (map->plane[map->rowOffset[gUnknown_030033E4.unk02]
                    + gUnknown_030033E4.unk00]
         & 0x1f)
        + gUnknown_085D5ABC[gUnknown_030040D8->unk00].movementType * 32;

    if (*cur >= costs[c]
        && ((ABS(gUnknown_0849D5F8->unk20[gUnknown_0849D5F8->unk45]
                 - gUnknown_030033E4.unk00) == 1
             && gUnknown_0849D5F8->unk2c[gUnknown_0849D5F8->unk45]
                    == gUnknown_030033E4.unk02)
            || (ABS(gUnknown_0849D5F8->unk2c[gUnknown_0849D5F8->unk45]
                    - gUnknown_030033E4.unk02) == 1
                && gUnknown_0849D5F8->unk20[gUnknown_0849D5F8->unk45]
                       == gUnknown_030033E4.unk00)))
    {
        PushMovePathStep(gUnknown_030033E4.unk00, gUnknown_030033E4.unk02);
        gUnknown_03004074 = gUnknown_0849D5F8->unk38[0xc]
            - gUnknown_0849D5F8->unk38[gUnknown_0849D5F8->unk45];
        return;
    }

    if (gUnknown_0849D5F8->unk38[gUnknown_0849D5F8->unk45] == 0)
        TruncateMovePath(1);

    SetWorkingMapPlane(gUnknown_08499590 + 0x2D5A);
    FillMovementMapFromMovePathEnd();

    if (((s8 *)gUnknown_03003340[gUnknown_030033E4.unk02])[gUnknown_030033E4.unk00]
        == -1)
    {
        RebuildBestMovePath();
    }
    else
    {
        GenerateBestMovementScript(gUnknown_030033E4.unk00, gUnknown_030033E4.unk02,
                     gUnknown_03003110);
        RebuildMovePathFromDirections();
        if ((u8)IsMovePathSelfAvoiding() == 0)
            RebuildBestMovePath();
    }

    SetWorkingMapPlane(gUnknown_08499590 + 0x2852);
}
asm(".global sub_08038D7C\n.thumb_set sub_08038D7C, UpdateMovePathToCursor\n");
