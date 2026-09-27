#include "global.h"

/* CgbModVol from the sound library: sets a Game Boy-style sound channel's
 * stereo pan and its target volumes from the channel's left and right volumes.
 *
 * It pans hard to one side when that side is at least twice as loud as the
 * other, and to both sides otherwise. Either way the envelope target becomes
 * (left + right) / 16, clamped to 15 on the hard-panned paths, and the sustain
 * level is scaled from it. The last line masks the pan byte with the channel's
 * own set of allowed pan bits.
 *
 * Build this file with the older SDK compiler, as its neighbours in the sound
 * library are (`--profile old-agbcc`).
 *
 * Why the C looks odd:
 *  - The two volumes are read into locals once, but the sum re-reads both
 *    bytes from the channel. That is not redundant: the volume members are
 *    volatile, so each read is a real load.
 *  - `(u32)(left + right) >> 4` needs the cast; without it the shift is
 *    arithmetic rather than logical.
 *  - The `goto`s are the only arrangement that puts the blocks in the
 *    original's order: both hard-panned arms jump forward past the both-sides
 *    block to one shared clamp, so that block has to sit between them.
 *  - `(x + 15) >> 4` is a biased shift, not a division; a signed `/ 16` costs
 *    a sign-correction sequence.
 */
void sub_08070F44(struct CgbChannel *chan)
{
    u8 right;
    u8 left;

    right = chan->rightVolume;
    left = chan->leftVolume;

    if (right >= left)
    {
        if (right / 2 >= left)
        {
            chan->pan = 0x0F;
            goto CLAMPED;
        }
    }
    else
    {
        if (left / 2 >= right)
        {
            chan->pan = 0xF0;
            goto CLAMPED;
        }
    }

    chan->pan = 0xFF;
    chan->envelopeGoal = (u32)(chan->leftVolume + chan->rightVolume) >> 4;
    goto REST;

CLAMPED:
    chan->envelopeGoal = (u32)(chan->leftVolume + chan->rightVolume) >> 4;

    if (chan->envelopeGoal > 15)
        chan->envelopeGoal = 15;

REST:
    chan->unk19 = (chan->envelopeGoal * chan->unk06 + 15) >> 4;
    left = chan->pan;
    right = chan->panMask;
    chan->pan = right & left;
}
