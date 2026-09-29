#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08070F44.
 * sub_08070F44 @ 0x08070F44
 */

/* Sound-library CgbModVol: sets a CGB channel's pan from its left and right
 * volumes (hard left, hard right or centre), then its envelope and sustain
 * goals, and masks the pan with the channel's allowed outputs. */

struct SdkCgbChannel
{
    u8 sf;
    u8 ty;
    u8 rightVolume;
    u8 leftVolume;
    u8 at;
    u8 de;
    u8 su;
    u8 re;
    u8 ky;
    u8 ev;
    u8 eg;
    u8 ec;
    u8 echoVolume;
    u8 echoLength;
    u8 d1;
    u8 d2;
    u8 gt;
    u8 mk;
    u8 ve;
    u8 pr;
    u8 rp;
    u8 d3[3];
    u8 d5;
    u8 sg;
    u8 n4;
    u8 pan;
    u8 panMask;
};
static inline int CgbPan(struct SdkCgbChannel *chan)
{
    u32 rightVolume = chan->rightVolume;
    u32 leftVolume = chan->leftVolume;

    if ((rightVolume = (u8)rightVolume) >= (leftVolume = (u8)leftVolume))
    {
        if (rightVolume / 2 >= leftVolume)
        {
            chan->pan = 0x0F;
            return 1;
        }
    }
    else
    {
        if (leftVolume / 2 >= rightVolume)
        {
            chan->pan = 0xF0;
            return 1;
        }
    }

    return 0;
}

void CgbModVol(struct CgbChannel *arg)
{
    struct SdkCgbChannel *chan = (struct SdkCgbChannel *)arg;

    if (!CgbPan(chan))
    {
        chan->pan = 0xFF;
        chan->eg = (chan->rightVolume + chan->leftVolume) / 16u;
    }
    else
    {
        chan->eg = (chan->rightVolume + chan->leftVolume) / 16u;
        if (chan->eg > 15)
            chan->eg = 15;
    }
    chan->sg = (chan->eg * chan->su + 15) >> 4;
    chan->pan &= chan->panMask;
}
asm(".global sub_08070F44\n.thumb_set sub_08070F44, CgbModVol\n");
