#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08070478.
 * ActivateMusicOrSoundId @ 0x08070478, m4aSongNumStartOrChange @ 0x080704A4, m4aSongNumStartOrContinue @ 0x080704F0, m4aSongNumStop @ 0x08070544, m4aSongNumContinue @ 0x08070578, m4aMPlayAllStop @ 0x080705AC
 */

/* PARKED, wave 31 (W31-C). m4aSongNumStart.
 *
 * Compiles to the right 44 bytes with the right instruction stream and the
 * right literal pool order; the ONLY difference is register allocation.
 * The ROM puts the mplay-table address in r2 and `song->ms` in r3:
 *
 *     ldr r2, =gUnknown_08242308      <- candidate uses r3
 *     ldr r1, =gUnknown_0824238C
 *     lsrs r0, r0, #0xd
 *     adds r0, r0, r1
 *     ldrh r3, [r0, #4]               <- candidate uses r2
 *
 * The candidate produces the same two quantities with r2 and r3 exchanged --
 * every instruction, the pool, and the size are otherwise identical.
 *
 * RULED OUT, twelve source spellings, all producing the identical allocation:
 * with and without a `songTable` local; with and without a `mplay` local; with
 * and without a `u16` / `u32` / `int` local for `ms`; `&t[n]` vs `t + n`;
 * `(p + i)->info` vs `p[i].info`; binding `info` and `header` to locals first;
 * fully inlining both tables into the call; declaring the two tables `const`
 * and non-`const`. Declaration ORDER is not free either -- swapping the two
 * table locals swaps the literal pool words, which the ROM fixes.
 *
 * The two competing quantities are the mplay-table address (2 refs, live
 * across ~8 insns) and `ms` (3 refs, live across 2), and GCC 2.9's local-alloc
 * priority is monotone in refs / live-length, so `ms` is allocated first under
 * every spelling. Whatever makes the ROM allocate the table first is not
 * reachable from the statement shape of this function -- suspect the TYPE model
 * of one of the two tables instead.
 *
 * m4aSongNumStop and m4aSongNumContinue are the same function with a guard added, and
 * park on exactly the same difference.
 *
 * WAVE 32 (W32-B) applied the wave-31 allocation rule to this function -- the
 * rule this same agent measured -- and the rule PREDICTS THE CANDIDATE, NOT THE
 * ROM. That is the finding; it is a limit on the rule, not a lever.
 *
 * The two competing quantities and their gcc-2.9 local-alloc priorities
 * (floor_log2(n_refs) * n_refs * size / (death - birth), sorted DESCENDING):
 *
 *   &gUnknown_08242308   n_refs 2, born at the pool `ldr`, dies at
 *                        `adds r1,r1,r2` -- live 8 insns
 *                        priority = 1 * 2 * 1 / 8 = 0.25
 *   song->ms             n_refs 3, born at the `ldrh`, dies two insns later at
 *                        `adds r1,r1,r3` -- live 2 insns
 *                        priority = 1 * 3 * 1 / 2 = 1.50
 *
 * `ms` is allocated first and takes the lowest free hard register, r2. That is
 * exactly what the candidate does. The ROM has it the other way, so the ROM's
 * source cannot be a cosmetic variant of this one.
 *
 * The three questions, answered:
 *   1. Ref-count power-of-two crossing? NOT REACHABLE. To overtake `ms` the
 *      table address needs floor_log2(n)*n > 12, i.e. n_refs 8; it has 2, and
 *      no spelling creates more without adding instructions. Crossing 3->4
 *      only lifts it to 1.00, still under 1.50.
 *   2. Tied, so the declared-local tiebreak applies? NO -- 0.25 against 1.50 is
 *      not a tie. Measured directly: declaration order moves the LITERAL POOL
 *      WORDS and never these two registers (three more spellings this wave --
 *      fully inlined, mplay-element bound, index bound to a local declared
 *      first -- on top of W31-C's twelve).
 *   3. An index add whose operand order would have to change? NO. Both adds
 *      already have the symbolic base as operand 2, in the ROM and in every
 *      candidate.
 *
 * So all three levers are absent, which means the difference is upstream of
 * statement shape. Recorded as a TYPE-MODEL question, per the rule's own
 * instruction to stop there: something about `struct MusicPlayer`,
 * `struct Song` or the two tables' declared types must give the table address
 * either more references or a shorter live range than this model does.
 *
 * NEW this wave and worth keeping: writing the song lookup INLINE rather than
 * through a `song` local fixes the LITERAL POOL ORDER on the two conditional
 * members (m4aSongNumStartOrChange, m4aSongNumStartOrContinue), taking them from a reversed pool to
 * 93.4% / 94.0% with five bytes differing. All five members of this family now
 * park on the identical single residual.
 */


/* Named per Xenesis's AW2 Subroutine List: "Routine that takes Music/Sound
 * ID and activates it?" -- m4aSongNumStart, per the note above. The old
 * ActivateMusicOrSoundId symbol is kept as a linker alias below so every other unit
 * keeps resolving it unchanged. */
void ActivateMusicOrSoundId(u16 n)
{
    const struct MusicPlayer *mplayTable = gUnknown_08242308;
    const struct Song *songTable = gUnknown_0824238C;
    const struct Song *song = &songTable[n];
    const struct MusicPlayer *mplay = &mplayTable[song->ms];

    MPlayStart_rev01(mplay->info, song->header);
}

asm(".global sub_08070478\n.thumb_set sub_08070478, ActivateMusicOrSoundId\n");

/* PARKED -- wave 32, W32-B. m4aSongNumStartOrChange.
 *
 * Every instruction, the literal-pool ORDER and the size are the ROM's. The
 * only residual is a two-register exchange: the ROM keeps &gUnknown_08242308
 * in r2 and `song->ms` in r3, this keeps them the other way round. Identical
 * cause to ActivateMusicOrSoundId / m4aSongNumStop / m4aSongNumContinue / m4aSongNumStartOrContinue -- see
 * work/ActivateMusicOrSoundId/ActivateMusicOrSoundId.c for the full analysis.
 *
 * NEW this wave: writing the song lookup INLINE (no `song` local) is what fixes
 * the POOL order. With `const struct Song *song = &gUnknown_0824238C[n];` the
 * song table is referenced first and its pool word comes first, which is the
 * reverse of the ROM; inlined, agbcc reaches the mplay table first and emits
 * 0x08242308 then 0x0824238C as the ROM does. That halves the residual on all
 * five and leaves exactly one difference on each. */
void m4aSongNumStartOrChange(u16 n)
{
    if (gUnknown_08242308[gUnknown_0824238C[n].ms].info->songHeader != gUnknown_0824238C[n].header)
        MPlayStart_rev01(gUnknown_08242308[gUnknown_0824238C[n].ms].info, gUnknown_0824238C[n].header);
    else if ((gUnknown_08242308[gUnknown_0824238C[n].ms].info->status & 0xffff) == 0
          || (gUnknown_08242308[gUnknown_0824238C[n].ms].info->status & 0x80000000))
        MPlayStart_rev01(gUnknown_08242308[gUnknown_0824238C[n].ms].info, gUnknown_0824238C[n].header);
}
asm(".global sub_080704A4\n.thumb_set sub_080704A4, m4aSongNumStartOrChange\n");

/* PARKED -- wave 32, W32-B. m4aSongNumStartOrContinue. Same single residual as
 * m4aSongNumStartOrChange (the r2/r3 exchange between the mplay-table address and
 * `song->ms`); see work/ActivateMusicOrSoundId/ActivateMusicOrSoundId.c for the analysis and
 * work/m4aSongNumStartOrChange/m4aSongNumStartOrChange.c for why the lookup is written inline. */
void m4aSongNumStartOrContinue(u16 n)
{
    if (gUnknown_08242308[gUnknown_0824238C[n].ms].info->songHeader != gUnknown_0824238C[n].header)
        MPlayStart_rev01(gUnknown_08242308[gUnknown_0824238C[n].ms].info, gUnknown_0824238C[n].header);
    else if ((gUnknown_08242308[gUnknown_0824238C[n].ms].info->status & 0xffff) == 0)
        MPlayStart_rev01(gUnknown_08242308[gUnknown_0824238C[n].ms].info, gUnknown_0824238C[n].header);
    else if (gUnknown_08242308[gUnknown_0824238C[n].ms].info->status & 0x80000000)
        MPlayContinue(gUnknown_08242308[gUnknown_0824238C[n].ms].info);
}
asm(".global sub_080704F0\n.thumb_set sub_080704F0, m4aSongNumStartOrContinue\n");

/* PARKED, wave 31 (W31-C). m4aSongNumStop. Same 52-byte instruction stream as
 * the ROM; the only difference is that the mplay-table address lands in r3 and
 * `song->ms` in r2, where the ROM has them the other way round. See the long
 * note in work/ActivateMusicOrSoundId/ActivateMusicOrSoundId.c for the spellings ruled out.
 *
 * WAVE 32 (W32-B) applied the wave-31 allocation rule to this function -- the
 * rule this same agent measured -- and the rule PREDICTS THE CANDIDATE, NOT THE
 * ROM. That is the finding; it is a limit on the rule, not a lever.
 *
 * The two competing quantities and their gcc-2.9 local-alloc priorities
 * (floor_log2(n_refs) * n_refs * size / (death - birth), sorted DESCENDING):
 *
 *   &gUnknown_08242308   n_refs 2, born at the pool `ldr`, dies at
 *                        `adds r1,r1,r2` -- live 8 insns
 *                        priority = 1 * 2 * 1 / 8 = 0.25
 *   song->ms             n_refs 3, born at the `ldrh`, dies two insns later at
 *                        `adds r1,r1,r3` -- live 2 insns
 *                        priority = 1 * 3 * 1 / 2 = 1.50
 *
 * `ms` is allocated first and takes the lowest free hard register, r2. That is
 * exactly what the candidate does. The ROM has it the other way, so the ROM's
 * source cannot be a cosmetic variant of this one.
 *
 * The three questions, answered:
 *   1. Ref-count power-of-two crossing? NOT REACHABLE. To overtake `ms` the
 *      table address needs floor_log2(n)*n > 12, i.e. n_refs 8; it has 2, and
 *      no spelling creates more without adding instructions. Crossing 3->4
 *      only lifts it to 1.00, still under 1.50.
 *   2. Tied, so the declared-local tiebreak applies? NO -- 0.25 against 1.50 is
 *      not a tie. Measured directly: declaration order moves the LITERAL POOL
 *      WORDS and never these two registers (three more spellings this wave --
 *      fully inlined, mplay-element bound, index bound to a local declared
 *      first -- on top of W31-C's twelve).
 *   3. An index add whose operand order would have to change? NO. Both adds
 *      already have the symbolic base as operand 2, in the ROM and in every
 *      candidate.
 *
 * So all three levers are absent, which means the difference is upstream of
 * statement shape. Recorded as a TYPE-MODEL question, per the rule's own
 * instruction to stop there: something about `struct MusicPlayer`,
 * `struct Song` or the two tables' declared types must give the table address
 * either more references or a shorter live range than this model does.
 *
 * NEW this wave and worth keeping: writing the song lookup INLINE rather than
 * through a `song` local fixes the LITERAL POOL ORDER on the two conditional
 * members (m4aSongNumStartOrChange, m4aSongNumStartOrContinue), taking them from a reversed pool to
 * 93.4% / 94.0% with five bytes differing. All five members of this family now
 * park on the identical single residual.
 */


void m4aSongNumStop(u16 n)
{
    const struct MusicPlayer *mplayTable = gUnknown_08242308;
    const struct Song *songTable = gUnknown_0824238C;
    const struct Song *song = &songTable[n];
    const struct MusicPlayer *mplay = &mplayTable[song->ms];

    if (mplay->info->songHeader == song->header)
        MPlayStop_rev01(mplay->info);
}
asm(".global sub_08070544\n.thumb_set sub_08070544, m4aSongNumStop\n");

/* PARKED, wave 31 (W31-C). m4aSongNumContinue. Same 52-byte instruction stream
 * as the ROM; the only difference is that the mplay-table address lands in r3
 * and `song->ms` in r2, where the ROM has them the other way round. See the
 * long note in work/ActivateMusicOrSoundId/ActivateMusicOrSoundId.c for the spellings ruled out,
 * and for wave 32 (W32-B)'s verdict: the wave-31 allocation rule predicts the
 * CANDIDATE's registers, not the ROM's, so all three of its levers are absent
 * here and the difference is upstream of statement shape -- a type-model
 * question about the two tables, recorded there in full.
 */

void m4aSongNumContinue(u16 n)
{
    const struct MusicPlayer *mplayTable = gUnknown_08242308;
    const struct Song *songTable = gUnknown_0824238C;
    const struct Song *song = &songTable[n];
    const struct MusicPlayer *mplay = &mplayTable[song->ms];

    if (mplay->info->songHeader == song->header)
        MPlayContinue(mplay->info);
}
asm(".global sub_08070578\n.thumb_set sub_08070578, m4aSongNumContinue\n");

/* m4aMPlayAllStop -- MPlayStop on every entry of the player table.
 *
 * The loop count is NOT the literal 11. It is `(u16)(u32)&gNumMusicPlayers`,
 * MP2K's absolute symbol, defined as 11 on line 4 of aw2bhr.lds; see the
 * comment on that declaration in include/unknown-globals.h for why a literal is
 * refuted (11 fits `movs #imm8` and would never reach the pool, the `(u16)`
 * truncation survives unfolded, and the `cmp #0` zero-trip guard would have
 * been deleted for a known-positive bound). Wave 47 spelled it
 * `gUnknown_0000000B`, which nothing defined; wave 60 found the real symbol
 * already in the linker script and corrected its value from 9 to 11.
 *
 * `n` and `i` are separate: the guard tests `n` in r0 and the countdown copies
 * it into r4 in the preheader, which is the `adds r4, r0, #0`. Binding the
 * table pointer AFTER the guard is what puts `ldr r5` below the `beq` -- with
 * `p = gUnknown_08242308` written before the loop agbcc hoists it above the
 * `cmp` instead.
 */
void m4aMPlayAllStop(void)
{
    const struct MusicPlayer * p;
    u32 i;
    u32 n;

    n = (u16)(u32)&gNumMusicPlayers;

    if (n == 0)
        return;

    p = gUnknown_08242308;
    i = n;

    do
    {
        MPlayStop_rev01(p->info);
        p++;
        i--;
    }
    while (i != 0);
}
asm(".global sub_080705AC\n.thumb_set sub_080705AC, m4aMPlayAllStop\n");
