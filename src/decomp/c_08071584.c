#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071584.
 * sub_08071584 @ 0x08071584, sub_080715F8 @ 0x080715F8
 */

/*
 * m4aMPlayModDepthSet. MATCHES -- but only under `old_agbcc`, and it now has an
 * entry in data/compiler-overrides.json, so `try_match` reports the match
 * directly. Wave 38 (W38-A).
 *
 * Same skeleton as MPlayVolumeControl (see that file); the body writes `mod` at
 * +0x17 -- NOT modM at +0x16, and the neighbouring `mod`/`modM` pair is the one
 * place a layout quoted from memory slides everything by one. See the
 * MusicPlayerTrack block in include/unknown-globals.h for why +0x17 is settled.
 *
 * What the default compiler does differently. It gets 110 of 116 bytes (94.8%)
 * with the SIZE ALREADY EXACT, and the whole deficit is two independent 3-byte
 * deltas inside the loop body, neither reachable from the C:
 *
 *   - the ROM emits `movs r0,#0x80` BEFORE `ldrb r1,[r4]`; agbcc emits the
 *     ldrb first. Pure scheduling.
 *   - the re-read of the just-stored `track->mod` lands in r1 under old_agbcc
 *     (`mov r1,sb; cmp r1,#0`) and in r0 under agbcc (`mov r0,r9; cmp r0,#0`)
 *     -- and r0 is the register the `bl MP_clear_modM` two instructions later
 *     needs for its argument.
 *
 * Both compilers agree on everything else, INCLUDING substituting the stored
 * register for the `track->mod` re-read rather than reloading it (the value is
 * copied into a high register in the preheader and the test reads the copy).
 * So this is NOT the MPlayTempoControl store-substitution difference; it is purely
 * the allocator, and the trigger is a call inside the loop body. Three siblings
 * in the same block with no call in the body match under both compilers.
 *
 * The `if (!track->mod)` spelling is the pokeemerald one and is what produces
 * the preheader copy. Keep it -- do not rewrite it as `if (!modDepth)`.
 */
void MPlayModDepthSet(struct MusicPlayerInfo * mplayInfo, u16 trackBits, u8 modDepth)
{
    s32 i;
    u32 bit;
    struct MusicPlayerTrack * track;

    if (mplayInfo->ident != MPLAY_ID_NUMBER)
        return;

    mplayInfo->ident++;

    i = mplayInfo->trackCount;
    track = mplayInfo->tracks;
    bit = 1;

    while (i > 0)
    {
        if (trackBits & bit)
        {
            if (track->flags & MPT_FLG_EXIST)
            {
                track->mod = modDepth;

                if (!track->mod)
                    MP_clear_modM(track);
            }
        }

        i--;
        track++;
        bit <<= 1;
    }

    mplayInfo->ident = MPLAY_ID_NUMBER;
}
asm(".global sub_08071584\n.thumb_set sub_08071584, MPlayModDepthSet\n");

/*
 * m4aMPlayLFOSpeedSet. MATCHES -- but only under `old_agbcc`; it has an entry
 * in data/compiler-overrides.json. Wave 38 (W38-A).
 *
 * The exact twin of MPlayModDepthSet: same 116 bytes, same skeleton, same loop,
 * same call to MP_clear_modM/ClearModM, differing ONLY in which member the body
 * writes (lfoSpeed at +0x19 here, mod at +0x17 there). The default compiler
 * produces the same two 3-byte deltas at the same offsets and the same 94.8%.
 * The full mechanism is written up in work/MPlayModDepthSet/MPlayModDepthSet.c -- read
 * that one, not this one.
 */
void MPlayLFOSpeedSet(struct MusicPlayerInfo * mplayInfo, u16 trackBits, u8 lfoSpeed)
{
    s32 i;
    u32 bit;
    struct MusicPlayerTrack * track;

    if (mplayInfo->ident != MPLAY_ID_NUMBER)
        return;

    mplayInfo->ident++;

    i = mplayInfo->trackCount;
    track = mplayInfo->tracks;
    bit = 1;

    while (i > 0)
    {
        if (trackBits & bit)
        {
            if (track->flags & MPT_FLG_EXIST)
            {
                track->lfoSpeed = lfoSpeed;

                if (!track->lfoSpeed)
                    MP_clear_modM(track);
            }
        }

        i--;
        track++;
        bit <<= 1;
    }

    mplayInfo->ident = MPLAY_ID_NUMBER;
}
asm(".global sub_080715F8\n.thumb_set sub_080715F8, MPlayLFOSpeedSet\n");
