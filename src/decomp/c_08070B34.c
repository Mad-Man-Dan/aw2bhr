#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08070B34.
 * sub_08070B34 @ 0x08070B34, sub_08070BAC @ 0x08070BAC, sub_08070C90 @ 0x08070C90
 */

/* MPlayOpen. The track-clearing loop's counter is re-truncated to u8 on every
 * iteration (`subs; lsls #0x18; lsrs #0x18`), which is what says trackCount is
 * a u8 local rather than an int -- it is the incoming parameter, reused.
 * See MPlayContinue for why the trailing ident stores are needed.
 */
void MPlayOpen_rev01(struct MusicPlayerInfo *mplayInfo, struct MusicPlayerTrack *tracks, u8 trackCount)
{
    struct SoundInfo *soundInfo;
    u32 ident;

    if (trackCount == 0)
        return;

    if (trackCount > 16)
        trackCount = 16;

    soundInfo = gUnknown_03007FF0;
    ident = soundInfo->ident;

    if (ident != MPLAY_ID_NUMBER)
        return;

    soundInfo->ident = ident + 1;

    Clear64byte_rev(mplayInfo);

    mplayInfo->tracks = tracks;
    mplayInfo->trackCount = trackCount;
    mplayInfo->status = MUSICPLAYER_STATUS_PAUSE;

    while (trackCount != 0)
    {
        tracks->flags = 0;
        trackCount--;
        tracks++;
    }

    if (soundInfo->func != 0)
    {
        mplayInfo->func = soundInfo->func;
        mplayInfo->intp = soundInfo->intp;
        soundInfo->func = 0;
    }

    soundInfo->intp = mplayInfo;
    soundInfo->func = sub_0806FDE4;
    soundInfo->ident = MPLAY_ID_NUMBER;
    mplayInfo->ident = MPLAY_ID_NUMBER;
}
asm(".global sub_08070B34\n.thumb_set sub_08070B34, MPlayOpen_rev01\n");

/* MPlayStart. PARKED at 90.4% -- SIZE-EXACT (228 bytes), 22 differing bytes,
 * every one of them a register-allocation tie-break. See data/parked.json.
 *
 * The entry guard is one three-term condition and the whole body is its `if` --
 * there is no early return, which is why the ident test's `bne` and the
 * priority test's `bhi` share one exit label. `mplayInfo->ident` is RELOADED
 * for the `++` rather than held in a local (contrast m4aMPlayFadeInContinue): the branchy
 * guard sits between the test and the increment.
 *
 * The two track loops are one `while` with a two-term guard followed by a
 * second that continues from the same i and track. The duplicated exit test at
 * the top of the first loop is agbcc's loop rotation and must not be authored.
 *
 * REMAINING DIFF, all of it r0-vs-r1 on the SECOND operand of a compare or an
 * `and`, plus the emission order of the `movs rD, #imm` that feeds it:
 *   +0x20  ROM `ldr r1,[r5,#0x2c]; movs r0,#0x40; ldrb r1,[r1]; ands r0,r1`
 *          candidate `ldr r0,[r5,#0x2c]; ldrb r1,[r0]; movs r0,#0x40; ands r0,r1`
 *   +0x38  ROM copies songHeader->priority to r2 BEFORE loading
 *          mplayInfo->priority; the candidate loads both, then copies.
 *   +0x64  ROM `ldrb r1,[r7]` in the loop preheader; candidate `ldrb r0,[r7]`.
 *   +0x7a  ROM `mov r1,r8; str r1,[r4,#0x20]`; candidate uses r0.
 *   +0xa2  ROM `movs r1,#0; mov r8,r1`; candidate uses r0.
 *   +0xc2  ROM `movs r0,#0x80` BEFORE `ldrb r1,[r7,#3]`; candidate after.
 * The instruction stream, the branch targets, the loop rotation, the store
 * order in the body and the single pool word are all already byte-exact.
 */
void MPlayStart_rev01(struct MusicPlayerInfo *mplayInfo, struct SongHeader *songHeader)
{
    s32 i;
    struct MusicPlayerTrack *track;

    if (mplayInfo->ident != MPLAY_ID_NUMBER)
        return;

    if (!mplayInfo->unk_0b
     || ((!mplayInfo->songHeader || !(mplayInfo->tracks->flags & 0x40))
      && (!(u16)mplayInfo->status || (mplayInfo->status & MUSICPLAYER_STATUS_PAUSE)))
     || mplayInfo->priority <= songHeader->priority)
    {
        mplayInfo->ident++;
        mplayInfo->status = 0;
        mplayInfo->songHeader = songHeader;
        mplayInfo->tone = songHeader->tone;
        mplayInfo->priority = songHeader->priority;
        mplayInfo->clock = 0;
        mplayInfo->tempoD = 150;
        mplayInfo->tempoI = 150;
        mplayInfo->tempoU = 0x100;
        mplayInfo->tempoC = 0;
        mplayInfo->fadeOI = 0;

        i = 0;
        track = mplayInfo->tracks;

        while (i < songHeader->trackCount && i < mplayInfo->trackCount)
        {
            sub_0807004C(mplayInfo, track);
            track->flags = MPT_FLG_EXIST | 0x40;
            track->chan = 0;
            track->cmdPtr = songHeader->part[i];
            i++;
            track++;
        }

        while (i < mplayInfo->trackCount)
        {
            sub_0807004C(mplayInfo, track);
            track->flags = 0;
            i++;
            track++;
        }

        if (songHeader->reverb & 0x80)
            SoundMode_rev01(songHeader->reverb);

        mplayInfo->ident = MPLAY_ID_NUMBER;
    }
}
asm(".global sub_08070BAC\n.thumb_set sub_08070BAC, MPlayStart_rev01\n");

/* MPlayStop. The standard m4a re-entrancy sandwich: bump ident, do the work,
 * restamp it. `s32 i` and not a u8 counter -- the `cmp r4,#0; ble` guard on a
 * value loaded with `ldrb` is a SIGNED compare, which a u8 counter cannot
 * produce. Track stride is 0x50, i.e. sizeof(struct MusicPlayerTrack).
 */
void MPlayStop_rev01(struct MusicPlayerInfo * mplayInfo)
{
    s32 i;
    struct MusicPlayerTrack * track;
    u32 ident = mplayInfo->ident;

    if (ident != MPLAY_ID_NUMBER)
        return;

    mplayInfo->ident = ident + 1;
    mplayInfo->status |= MUSICPLAYER_STATUS_PAUSE;
    i = mplayInfo->trackCount;
    track = mplayInfo->tracks;

    while (i > 0)
    {
        sub_0807004C(mplayInfo, track);
        i--;
        track++;
    }

    mplayInfo->ident = MPLAY_ID_NUMBER;
}
asm(".global sub_08070C90\n.thumb_set sub_08070C90, MPlayStop_rev01\n");
