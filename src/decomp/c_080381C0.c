#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080381C0.
 * sub_080381C0 @ 0x080381C0, sub_08038240 @ 0x08038240
 */

/* MATCHED byte-for-byte (wave 43, W43-E).
 *
 * NEEDS ITS .rodata POOL WORD PLACED: "rodata": ["0x08090EFC"]. That ROM word
 * holds 0x08499598, i.e. &gPlayers, so it is agbcc's own -fforce-addr
 * address constant and not a global. The honest `gPlayers[i]`
 * subscript emits the full three-level chain (`ldr rA,=<pool>; ldr rB,[rA];
 * ldr rC,[rB]`) by itself -- see the note above the declaration in
 * include/unknown-globals.h. trymatch reports only `relocs: name different
 * symbols that resolve to the same address`.
 *
 * The else arm's spelling is load-bearing and cost this function its one miss.
 * The ROM has ONE `movs r0,#0` block, at the end of the `unk01 == 3` path and
 * before the literal pool, which the IsPlayer1TeamAlive path reaches by branching
 * BACKWARDS into it (`beq _08038220`). Writing that arm as
 * `else if (IsPlayer1TeamAlive() == 0) return 0;` emits a `bne` to the return-1 tail
 * plus a SECOND copy of the return-0 block: +4 bytes, 89.1%. Writing it as
 * `if (...) return 1; return 0;` -- the early return spelled with the value
 * the fall-through does NOT use -- makes agbcc invert the branch, and the
 * duplicate block cross-jumps into the shared one. Same rule as IsPlayer1TeamAlive
 * in this block; see docs/agbcc-codegen.md.
 *
 * `buf` is 5 bytes, not 4: the frame is `sub sp, #8` and agbcc rounds to the
 * 8-byte stack boundary, so a 4-byte array measures `sub sp, #4` and a 5..8
 * byte one measures 8. Five is the size every other per-army array in this
 * tree uses (slots 1..4 with element 0 unused), though only the 5..8 band is
 * proved.
 *
 * `buf[n] = i; n++;` and NOT `buf[n++] = i`: the post-increment form computes
 * the new n before the store, which lets agbcc clobber n's register with the
 * address and drops the `mov r5, sp` the ROM spends on keeping n live. Read
 * backwards, the ROM's extra callee-saved register IS the tell that the
 * increment is a separate statement.
 */


/* Wave 43 (W43-F): return type changed from `int` to `bool8`. The note above
 * says nothing narrows the result; EndOfGame_PrepareSummary, the one caller, does -- it
 * truth-tests with `lsls r0,#0x18; cmp r0,#0`, and that shift only appears when
 * agbcc re-narrows a sub-word return. Byte-neutral here (both arms return a
 * literal), and re-verified as still MATCHED after the change.
 *
 * W43-E re-verified this independently and found the CONTROL PAIR that settles
 * it, inside that one caller: EndOfGame_PrepareSummary calls IsHardCampaignMode at 0x08038302 and
 * sub_080381C0 at 0x0803830E, sixteen bytes apart, and truth-tests both results
 * directly with no intervening local. The first is `cmp r0,#0` with NO shift
 * and is declared `int`; the second is `lsls r0,#0x18; cmp r0,#0`. Same
 * function, same compiler invocation, same use shape, differing only in the
 * declared return type -- so the shift here is the return width and nothing
 * else. */
bool8 sub_080381C0(void)
{
    u8 buf[5];
    u8 n;
    u8 i;

    if (gPlaySt.savingEnabled == 0)
    {
        if (gPlaySt.gameMode == 3)
        {
            n = 0;
            for (i = 1; i <= 4; i++)
            {
                if (gPlayers[i].aiControlled == 1)
                {
                    buf[n] = i;
                    n++;
                }
            }
            if (n == 1 && gPlayers[buf[0]].unk31 == 1)
                return 0;
        }
        else
        {
            if (IsPlayer1TeamAlive() != 0)
                return 1;
            return 0;
        }
    }
    return 1;
}

/* Per-map setup: registers the two slot callbacks, clears gUnknown_0202FDEC,
 * then partitions armies 1..4 into its two 4-entry lists on IsPlayerAliveAndActive's
 * predicate before publishing the funds figure and the turn limit.
 *
 * TWO -fforce-addr pool words, and they must stay in this order: 0x08090F00
 * holds &gUnknown_0202FDEC and 0x08090F04 holds &gPlayers. Both are
 * agbcc's own address constants, so the honest spelling emits them and the
 * promotion carries "rodata": ["0x08090F00", "0x08090F04"].
 *
 * THE LAST `if` IS WRITTEN INVERTED ON PURPOSE. The ROM branches `bne` to the
 * AddBattleMapPoints block and reaches UnlockMainMenu by falling through, so the
 * UnlockMainMenu arm has to be the THEN arm in the source; spelling it the
 * natural way round emits the same instructions in the opposite order. Same
 * lever W43-E recorded on sub_080381C0 in this block.
 *
 * The two `break`s are real: `cmp #3; bhi` leaves the loop entirely rather than
 * skipping an iteration, and both arms jump to the same post-loop label. The
 * clear loop reads as descending (`subs; cmp #0; bge`) but is an ascending
 * `for (i = 0; i < 4; i++)` -- check_dbra_loop rewrote the counter, and the
 * pointer still walks forwards with `adds r0,#1`. */
void EndOfGame_PrepareSummary(void)
{
    int i;

    LockMainMenu();
    sub_080366D0(DefaultVBlankCallback);
    sub_080366C4(DefaultMainLoopCallback);
    sub_08011B18();

    for (i = 0; i < 4; i++)
    {
        gUnknown_0202FDEC.unk00[i] = 0;
        gUnknown_0202FDEC.unk04[i] = 0;
    }

    gUnknown_0202FDEC.unk08 = 0;
    gUnknown_0202FDEC.unk09 = 0;

    for (i = 1; i <= 4; i++)
    {
        if (gPlayers[i].aiControlled)
        {
            if (IsPlayerAliveAndActive(i))
            {
                if (gUnknown_0202FDEC.unk08 > 3)
                    break;

                gUnknown_0202FDEC.unk00[gUnknown_0202FDEC.unk08++] = i;
            }
            else
            {
                if (gUnknown_0202FDEC.unk09 > 3)
                    break;

                gUnknown_0202FDEC.unk04[gUnknown_0202FDEC.unk09++] = i;
            }
        }
    }

    gUnknown_0202FDEC.unk0c = gUnknown_0200C420.unk00;

    if (gPlaySt.gameMode == 3)
    {
        gUnknown_0202FDEC.unk0a = 0;
    }
    else
    {
        gUnknown_0202FDEC.unk0a = gPlayers[GetResultsArmy()].totalScore;

        if (IsHardCampaignMode())
            gUnknown_0202FDEC.unk0a = gUnknown_0202FDEC.unk0a * 2;
    }

    if (!sub_080381C0())
        UnlockMainMenu();
    else
        AddBattleMapPoints(gUnknown_0202FDEC.unk0a);
}
asm(".global sub_08038240\n.thumb_set sub_08038240, EndOfGame_PrepareSummary\n");
