#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080708EC.
 * sub_080708EC @ 0x080708EC, sub_08070990 @ 0x08070990, sub_08070A28 @ 0x08070A28
 */

#include "hardware.h"

/* SampleFreqSet. Every division here is `bl __divsi3`, not `__udivsi3`, so all
 * three SoundInfo members involved are SIGNED -- that is the only evidence for
 * their signedness. Note divFreq divides by the value just stored in pcmFreq,
 * not by pcmSamplesPerVBlank.
 *
 * The two VCOUNT spins are "wait out this scanline, then wait for the next";
 * the register must be volatile or the compiler deletes the second loop, and
 * the volatility is also why the 0x04000006 pool word is re-loaded between
 * them instead of the address staying live.
 */
void SampFreqSet_rev01(u32 mode)
{
    struct SoundInfo *soundInfo = gUnknown_03007FF0;

    mode = (mode & 0x000F0000) >> 16;
    soundInfo->freq = mode;
    soundInfo->pcmSamplesPerVBlank = gUnknown_081B9ED8[mode - 1];
    soundInfo->pcmDmaPeriod = 0x630 / soundInfo->pcmSamplesPerVBlank;
    soundInfo->pcmFreq = (soundInfo->pcmSamplesPerVBlank * 597275 + 5000) / 10000;
    soundInfo->divFreq = (16777216 / soundInfo->pcmFreq + 1) >> 1;

    REG_TM0CNT_H = 0;
    REG_TM0CNT_L = -(280896 / soundInfo->pcmSamplesPerVBlank);

    SoundVSyncOn_rev01();

    while (*(vu8 *)0x04000006 == 0x9F)
        ;

    while (*(vu8 *)0x04000006 != 0x9F)
        ;

    REG_TM0CNT_H = 0x80;
}
asm(".global sub_080708EC\n.thumb_set sub_080708EC, SampFreqSet_rev01\n");

/* m4aSoundMode. Five independent field tests on one mode word, each keeping the
 * masked value in the SAME local -- that reuse is what puts every one of them
 * in r4. The fourth uses two different masks on one field: the test is
 * 0x00B00000 (three bits) and the extraction is 0x00300000 (two), which is one
 * source-level pair and not a redundancy to fold away.
 * See MPlayContinue for why the trailing ident store is needed.
 */
void SoundMode_rev01(u32 mode)
{
    struct SoundInfo *soundInfo = gUnknown_03007FF0;
    u32 ident = soundInfo->ident;
    u32 temp;

    if (ident != MPLAY_ID_NUMBER)
        return;

    soundInfo->ident = ident + 1;

    temp = mode & 0x000000FF;

    if (temp)
        soundInfo->reverb = temp & 0x7F;

    temp = mode & 0x00000F00;

    if (temp)
    {
        struct SoundChannel *chan;

        soundInfo->unk06 = temp >> 8;
        temp = 12;
        chan = &soundInfo->chans[0];

        while (temp != 0)
        {
            chan->status = 0;
            temp--;
            chan++;
        }
    }

    temp = mode & 0x0000F000;

    if (temp)
        soundInfo->unk07 = temp >> 12;

    temp = mode & 0x00B00000;

    if (temp)
    {
        temp = (temp & 0x00300000) >> 14;
        *(vu8 *)0x04000089 = (*(vu8 *)0x04000089 & 0x3F) | temp;
    }

    temp = mode & 0x000F0000;

    if (temp)
    {
        SoundVSyncOff_rev01();
        SampFreqSet_rev01(temp);
    }

    soundInfo->ident = MPLAY_ID_NUMBER;
}
asm(".global sub_08070990\n.thumb_set sub_08070990, SoundMode_rev01\n");

/* SoundClear. Two array walks, each stepping a `void *` cursor by the record
 * size rather than indexing: twelve PCM channels from +0x50, then the four CGB
 * oscillators, indices 1..4 INCLUSIVE, each announced to the CgbOscOff hook at
 * SoundInfo+0x2c. The `lsls #0x18; lsrs #0x18` in front of that indirect call
 * is PROMOTE_MODE on the hook's own u8 parameter, not a cast in the source.
 *
 * The CGB loop increments i BEFORE advancing the cursor; the other order costs
 * nothing but swaps the two `adds`. See MPlayContinue for why the trailing ident
 * store is needed.
 */
void SoundClear_rev01(void)
{
    struct SoundInfo *soundInfo = gUnknown_03007FF0;
    u32 ident = soundInfo->ident;
    s32 i;
    void *chan;

    if (ident != MPLAY_ID_NUMBER)
        return;

    soundInfo->ident = ident + 1;

    i = 12;
    chan = &soundInfo->chans[0];

    while (i > 0)
    {
        ((struct SoundChannel *)chan)->status = 0;
        i--;
        chan = (void *)((s32)chan + sizeof(struct SoundChannel));
    }

    chan = soundInfo->cgbChans;

    if (chan)
    {
        i = 1;

        while (i <= 4)
        {
            soundInfo->unk2c(i);
            ((struct CgbChannel *)chan)->sf = 0;
            i++;
            chan = (void *)((s32)chan + sizeof(struct CgbChannel));
        }
    }

    soundInfo->ident = MPLAY_ID_NUMBER;
}
asm(".global sub_08070A28\n.thumb_set sub_08070A28, SoundClear_rev01\n");
