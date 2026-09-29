#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08070D98.
 * sub_08070D98 @ 0x08070D98
 */

/* m4a TrkVolPitSet. MATCHED in wave 79 (W79-D) after being parked since wave 35.
 *
 * NEEDS THE m4a BLOCK'S OVERRIDE -- old_agbcc, -fprologue-bugfix removed -- now
 * recorded in data/compiler-overrides.json, the same entry its neighbours
 * FadeOutBody_rev01, MPlayStart_rev01 and CgbSound carry. The parked diagnosis
 * ("allocno-priority tie, the ROM puts track in r2 and the accumulator in r3,
 * one pseudo lands one register over and renumbers the whole function, no
 * source lever in three waves") was measured under the DEFAULT compiler. The
 * same source under old_agbcc is 92.2%, not 10.0%, and there was no allocation
 * problem at all -- only two evaluation-order facts:
 *
 *  - `t` EXISTS TO FORCE THE MULTIPLY AHEAD OF THE tune LOAD. The ROM computes
 *    bend * bendRange, then loads tune, then adds. Written inline as
 *    `track->tune + track->bend * track->bendRange` the tune load comes first.
 *    Commuting the operands is NOT the fix -- that is +4 bytes and 64.4%. The
 *    lever is the statement boundary. Do not fold `t` back in.
 *  - `track->vol * track->volX`, IN THAT ORDER, even though the ROM loads volX
 *    (+0x13) before vol (+0x12). agbcc expands the SECOND operand of this
 *    multiply first, so the member loaded first is written second. The wave-35
 *    note had this backwards and it cost the last 2 of 180 bytes.
 *
 * SETTLED EARLIER, do not re-derive:
 *  - `s8 tune` at +0x0c, not u8 (the ROM reads it `movs r1,#12; ldrsb`).
 *    modM at +0x16 is really s8 too and is cast at each of its three uses.
 *  - `(u32)(track->volX * track->vol) >> 5` -- the (u32) is what makes all four
 *    volume shifts `lsrs` while the pitch shift stays `asrs`.
 *  - `x` and `y` must be BLOCK-scoped inside the first if, and the pitch
 *    accumulator a separate local in the second.
 *  - the first parameter is DEAD: it arrives in r0, is never read, and the
 *    function carries agbcc's own epilogue. Ordinary compiler output with a
 *    dead leading argument, not hand-written assembly.
 *
 * The wave-47 open tell -- the ROM emitting `movs r0,#1` before `ldrb r1,[r2,#0]`
 * for the first flags test, also seen in FadeOutBody_rev01 -- is EXPLAINED: it is
 * old_agbcc's emission order and it disappears with the right compiler. It was
 * never a source-operand-order question.
 */
void TrkVolPitSet_rev01(struct MusicPlayerInfo *mplayInfo, struct MusicPlayerTrack *track)
{
    if (track->flags & 1)
    {
        s32 x;
        s32 y;

        x = (u32)(track->vol * track->volX) >> 5;

        if (track->modT == 1)
            x = (u32)(x * ((s8)track->modM + 128)) >> 7;

        y = 2 * track->pan + track->panX;

        if (track->modT == 2)
            y += (s8)track->modM;

        if (y < -128)
            y = -128;
        else if (y > 127)
            y = 127;

        track->volMR = (u32)(x * (y + 128)) >> 8;
        track->volML = (u32)(x * (127 - y)) >> 8;
    }

    if (track->flags & 4)
    {
        s32 bend;
        s32 t;

        t = track->bend * track->bendRange;
        bend = (track->tune + t) * 4
            + (track->keyShift << 8) + (track->keyShiftX << 8) + track->pitX;

        if (track->modT == 0)
            bend += (s8)track->modM << 4;

        track->keyM = bend >> 8;
        track->pitM = bend;
    }

    track->flags &= 0xfa;
}
asm(".global sub_08070D98\n.thumb_set sub_08070D98, TrkVolPitSet_rev01\n");
