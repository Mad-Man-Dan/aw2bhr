#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08070CD0.
 * sub_08070CD0 @ 0x08070CD0
 */

/* WAVE 78 (W78-D + coordinator): MATCHED configured, 200/200, using
 * old_agbcc without -fprologue-bugfix. The canonical compound member
 * assignments recover the copied 0xFFFF mask and pool alignment under that
 * allocator. The same source under default agbcc is size-exact but leaves 13
 * register/order bytes different; the compiler override records the measured
 * discriminator and both verdicts.
 *
 * WAVE 77, W77-E -- SIGNATURE FIXED; THIS DRAFT HAD NOT COMPILED SINCE THE
 * PROTOTYPE LANDED. include/unknown-functions.h carried
 * `void FadeOutBody_rev01(void);`, an argument-less placeholder generated for the
 * MPlayExtender jump table (whose entry is explicitly cast
 * `(void (*)(void *, void *))` and so never constrained it). Against this
 * draft's `struct MusicPlayerInfo *` that is a hard `conflicting types` error,
 * so trymatch never got past the compile, never wrote a _cand.bin, and every
 * screen in the project has been reading a stale artifact and reporting
 * "NOT MEASURED". The HEADER was corrected -- a `(void)` prototype carries no
 * information at all, so the weakest-model rule has nothing to prefer, and the
 * body dereferences the parameter throughout.
 *
 * RE-MEASURED after the fix: 10.0% identical, 196 of 200 bytes (-4), first
 * difference at +0x8 -- i.e. exactly the wave-47 park below, unchanged. The
 * analysis in that note stands and none of it needs re-deriving. The two
 * candidates it names (a second variable assigned from the first with a
 * narrowing between them; declaring the mask `const`) are still untried.
 *
 * FadeOutBody. PARKED at -4 bytes (196 of 200), wave 47 (W47-C), and the
 * residual is ONE instruction plus the alignment padding it would create.
 *
 * Everything else is byte-exact: both fade arms, the shutdown loop, the
 * status write, the final per-track loop, the dead `movs r7,#0` in the tail
 * preheader, and the two known-zero register reuses (`strh r3,[r6,#0x24]`
 * for `fadeOI = 0`, `strb r0,[r4]` for `track->flags = 0`, both of which CSE
 * supplies from the preceding `cmp rN,#0`).
 *
 * THE WHOLE PARK IS THE 0xFFFF MASK REGISTER. The ROM materialises it as
 *     ldr  r3, =0x0000FFFF
 *     adds r2, r3, #0        <-- this copy is what is missing
 * and then zero-extends the two fadeOV re-reads with `ands r0, r2` rather
 * than with an `lsls #16; lsrs #16` pair. Losing the copy also loses the
 * `.short 0x0000` the pool needs for alignment, so the two together are the
 * whole -4.
 *
 * WHAT WAS MEASURED (all by compile_probe, none of it guesswork):
 *  - A plain `(u16)` conversion of a member re-read NEVER produces the `ands`
 *    form -- it is always `lsls #16; lsrs #16`. Ruled out: a bare member
 *    re-read, an explicit `& 0xFFFF` literal, a `u16` local, an `s32` local,
 *    and a `(u16)` cast on any of them.
 *  - The `ands` form appears ONLY when 0xFFFF is already sitting in a
 *    register, i.e. when it comes from a VARIABLE. `u32 m; m = 0xFFFF;` gets
 *    it, and it also reproduces the ROM's placement of the `ldr` (agbcc's
 *    scheduler sinks the pool load to just after the `strh` of the fadeOC
 *    decrement, which is exactly where the ROM has it).
 *  - It also explains the ROM's redundant `ands r0,r2` in FRONT of the
 *    `lsls r0,r0,#0x10` in the fade-out arm: combine cannot fold an AND whose
 *    mask is a REGISTER, so the mask survives where a constant would have
 *    been absorbed by the shift.
 *  - `(u16)` on the masked value in the fade-IN arm is load-bearing: without
 *    it agbcc re-loads the member after the `strh` instead of reusing the
 *    stored register. With it the arm is byte-exact.
 *
 * WHAT I WOULD TRY NEXT: the copy is an uncoalesced constant-load temp. The
 * one probe that produced a copy at all was `u32 m = 0xFFFF;` written as an
 * INITIALISER (rather than an assignment), which moves the `ldr` to the top
 * of the function and then emits `adds r3,r4,#0; ands r3,r3,r0` at the
 * fadeOC test -- i.e. the copy lands at the USE site instead of the DEF site,
 * and the fadeOC test stops using shifts. So the copy is reachable; what is
 * needed is the spelling that keeps the fadeOC test on shifts while still
 * costing a copy at the definition. Two candidates not yet tried: a second
 * variable assigned from the first with a narrowing between them, and
 * declaring the mask `const`.
 *
 * TWO SIZE-NEUTRAL BYTE DIFFS ARE ALSO OPEN and are pure register/order
 * facts, not size: the ROM emits `movs r0,#1` BEFORE `ldrh r7,[r6,#0x28]` at
 * both `fadeOV & 1` sites (the candidate emits the load first), and it keeps
 * the fadeOV loads inside both loops in r7. Writing `1 & mplayInfo->fadeOV`
 * does NOT fix the order -- it hoists the constant into the loop preheader
 * instead, which is worse. Binding the load to a local (as below) gets r7 in
 * the shutdown loop but not in the tail loop.
 */
void FadeOutBody_rev01(struct MusicPlayerInfo *mplayInfo)
{
    s32 i;
    struct MusicPlayerTrack *track;

    if (mplayInfo->fadeOI == 0)
        return;

    if (--mplayInfo->fadeOC != 0)
        return;

    mplayInfo->fadeOC = mplayInfo->fadeOI;

    if (mplayInfo->fadeOV & FADE_IN)
    {
        if ((u16)(mplayInfo->fadeOV += (4 << FADE_VOL_SHIFT)) >= (64 << FADE_VOL_SHIFT))
        {
            mplayInfo->fadeOV = 64 << FADE_VOL_SHIFT;
            mplayInfo->fadeOI = 0;
        }
    }
    else
    {
        if ((s16)(mplayInfo->fadeOV -= (4 << FADE_VOL_SHIFT)) <= 0)
        {
            i = mplayInfo->trackCount;
            track = mplayInfo->tracks;

            while (i > 0)
            {
                sub_0807004C(mplayInfo, track);

                if (!(mplayInfo->fadeOV & 1))
                    track->flags = 0;

                i--;
                track++;
            }

            if (mplayInfo->fadeOV & 1)
                mplayInfo->status |= MUSICPLAYER_STATUS_PAUSE;
            else
                mplayInfo->status = MUSICPLAYER_STATUS_PAUSE;

            mplayInfo->fadeOI = 0;
            return;
        }
    }

    i = mplayInfo->trackCount;
    track = mplayInfo->tracks;

    while (i > 0)
    {
        if (track->flags & 0x80)
        {
            track->volX = mplayInfo->fadeOV >> FADE_VOL_SHIFT;
            track->flags |= 3;
        }

        i--;
        track++;
    }
}
asm(".global sub_08070CD0\n.thumb_set sub_08070CD0, FadeOutBody_rev01\n");
