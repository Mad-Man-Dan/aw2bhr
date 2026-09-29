#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08070FAC.
 * sub_08070FAC @ 0x08070FAC
 */

/* CgbSound -- MP2K's per-frame PSG driver, one pass over the four CGB
 * oscillators. This is the Fire Emblem / pokeemerald function of the same name
 * and it keeps that source's shape, including the gotos: the envelope state
 * machine has six entry points that are shared between arms of a switch, and
 * no structured spelling reaches them.
 *
 * `mask` is a local 0xff rather than a literal for the reason the MP2K sources
 * give: every `(s8)` comparison in this function is written `(s8)(x & mask)`,
 * and agbcc emits the `movs rN,#0xff; ands` for it. A bare `channels->at` test
 * folds the mask away and is two bytes short at each of the six sites.
 *
 * `prevC15` is the frame's SoundInfo::c15 snapshot, and the `prevC15--; goto`
 * at the bottom is what runs the envelope a second time on the frame c15 wraps.
 *
 * WAVE 66: this source is byte-exact under old_agbcc. The prior conclusion that
 * old_agbcc was worse was an inverted measurement taken before four canonical
 * MP2K spellings were restored: read `fr`'s high byte through `(u8 *)&fr + 1`,
 * explicitly mask/narrow the NRx4 store, multiply `envelopeGoal * echoVolume`,
 * and bind the decay comparison's signed `ev` and `goal` locals. With those
 * source facts fixed, configured agbcc is 1096/1100 at 70.0%, while old_agbcc
 * is 1100/1100 with identical bytes and relocations. The evidence-backed
 * per-function override is now recorded, and the configured profile is exact.
 *
 * Two small readings are also settled and should not be re-derived:
 *    the phase dispatch is an if/else-if CHAIN, not a `switch` (a switch makes
 *    agbcc binary-search 1/0/2 and the ROM compares 0, 1, 2 in order, worth
 *    ~40 bytes), and `n4 = le ? 0x40 : 0` must be an if/else -- as a `?:` agbcc
 *    goes branchless with `negs/orrs/asrs #31/ands` and loses 6 bytes.
 */
void CgbSound(void)
{
    s32 ch;
    struct CgbChannel *channels;
    s32 evAdd;
    s32 prevC15;
    struct SoundInfo *soundInfo;
    vu8 *nrx0ptr;
    vu8 *nrx1ptr;
    vu8 *nrx2ptr;
    vu8 *nrx3ptr;
    vu8 *nrx4ptr;
    int mask;

    mask = 0xff;
    soundInfo = gUnknown_03007FF0;

    if (soundInfo->c15)
        soundInfo->c15--;
    else
        soundInfo->c15 = 14;

    for (ch = 1, channels = soundInfo->cgbChans; ch <= 4; ch++, channels++)
    {
        if (!(channels->sf & 0xc7))
            continue;

        switch (ch)
        {
        case 1:
            nrx0ptr = (vu8 *)0x04000060;
            nrx1ptr = (vu8 *)0x04000062;
            nrx2ptr = (vu8 *)0x04000063;
            nrx3ptr = (vu8 *)0x04000064;
            nrx4ptr = (vu8 *)0x04000065;
            break;
        case 2:
            nrx0ptr = (vu8 *)0x04000061;
            nrx1ptr = (vu8 *)0x04000068;
            nrx2ptr = (vu8 *)0x04000069;
            nrx3ptr = (vu8 *)0x0400006c;
            nrx4ptr = (vu8 *)0x0400006d;
            break;
        case 3:
            nrx0ptr = (vu8 *)0x04000070;
            nrx1ptr = (vu8 *)0x04000072;
            nrx2ptr = (vu8 *)0x04000073;
            nrx3ptr = (vu8 *)0x04000074;
            nrx4ptr = (vu8 *)0x04000075;
            break;
        default:
            nrx0ptr = (vu8 *)0x04000071;
            nrx1ptr = (vu8 *)0x04000078;
            nrx2ptr = (vu8 *)0x04000079;
            nrx3ptr = (vu8 *)0x0400007c;
            nrx4ptr = (vu8 *)0x0400007d;
            break;
        }

        prevC15 = soundInfo->c15;
        evAdd = *nrx2ptr;

        if (channels->sf & 0x80)
        {
            if (channels->sf & 0x40)
                goto oscOff;

            channels->sf = 3;
            channels->mo = 3;
            CgbModVol(channels);

            switch (ch)
            {
            case 1:
                *nrx0ptr = channels->sw;
                goto square;
            case 2:
            square:
                *nrx1ptr = channels->le + ((u32)channels->wp << 6);
                break;
            case 3:
                if (channels->wp != channels->cp)
                {
                    *nrx0ptr = 0x40;
                    *(vu32 *)0x04000090 = channels->wp[0];
                    *(vu32 *)0x04000094 = channels->wp[1];
                    *(vu32 *)0x04000098 = channels->wp[2];
                    *(vu32 *)0x0400009c = channels->wp[3];
                    channels->cp = channels->wp;
                }

                *nrx0ptr = 0;
                *nrx1ptr = channels->le;

                if (channels->le)
                    channels->n4 = 0xc0;
                else
                    channels->n4 = -0x80;

                goto attack;
            default:
                *nrx1ptr = channels->le;
                *nrx3ptr = (u32)channels->wp << 3;
                break;
            }

            evAdd = channels->at + 8;

            if (channels->le)
                channels->n4 = 0x40;
            else
                channels->n4 = 0;

        attack:
            channels->ec = channels->at;

            if ((s8)(channels->at & mask))
            {
                channels->ev = 0;
                goto tickEc;
            }

            goto release;
        }
        else if (channels->sf & 0x04)
        {
            channels->echoLength--;

            if ((s8)(channels->echoLength & mask) <= 0)
            {
            oscOff:
                CgbOscOff(ch);
                channels->sf = 0;
                goto done;
            }

            goto emit;
        }
        else if ((channels->sf & 0x40) && (channels->sf & 0x03))
        {
            channels->sf &= 0xfc;
            channels->ec = channels->re;

            if ((s8)(channels->re & mask))
            {
                channels->mo |= 1;

                if (ch != 3)
                    evAdd = channels->re;

                goto tickEc;
            }

            goto echo;
        }

    envelope:
        if (channels->ec == 0)
        {
            if (ch == 3)
                channels->mo |= 1;

            CgbModVol(channels);

            if ((channels->sf & 3) == 0)
            {
                channels->ev--;

                if ((s8)(channels->ev & mask) <= 0)
                {
                echo:
                    channels->ev = (channels->envelopeGoal * channels->echoVolume + 0xff) >> 8;

                    if ((s8)channels->ev == 0)
                        goto oscOff;

                    channels->sf |= 0x04;
                    channels->mo |= 1;

                    if (ch != 3)
                        evAdd = 8;

                    goto emit;
                }

                channels->ec = channels->re;
            }
            else if ((channels->sf & 3) == 1)
            {
            sustain:
                channels->ev = channels->unk19;
                channels->ec = 7;
            }
            else if ((channels->sf & 3) == 2)
            {
                int ev;
                int goal;

                channels->ev--;
                ev = (s8)(channels->ev & mask);
                goal = (s8)channels->unk19;

                if (ev <= goal)
                {
                decayEnd:
                    if (channels->unk06 == 0)
                    {
                        channels->sf &= 0xfc;
                        goto echo;
                    }

                    channels->sf--;
                    channels->mo |= 1;

                    if (ch != 3)
                        evAdd = 8;

                    goto sustain;
                }

                channels->ec = channels->de;
            }
            else
            {
                channels->ev++;

                if ((u8)(channels->ev & mask) >= channels->envelopeGoal)
                {
                release:
                    channels->sf--;
                    channels->ec = channels->de;

                    if ((s8)(channels->de & mask))
                    {
                        channels->mo |= 1;
                        channels->ev = channels->envelopeGoal;

                        if (ch != 3)
                            evAdd = channels->de;

                        goto tickEc;
                    }

                    goto decayEnd;
                }

                channels->ec = channels->at;
            }
        }

        goto tickEc;

    tickEc:
        channels->ec--;

        if (prevC15 == 0)
        {
            prevC15--;
            goto envelope;
        }

    emit:
        if (channels->mo & 2)
        {
            if (ch <= 3 && (channels->ty & 8))
            {
                s32 biasH = *(vu8 *)0x04000089;

                if (biasH <= 0x3f)
                    channels->fr = (channels->fr + 2) & 0x7fc;
                else if (biasH <= 0x7f)
                    channels->fr = (channels->fr + 1) & 0x7fe;
            }

            if (ch != 4)
                *nrx3ptr = channels->fr;
            else
                *nrx3ptr = (*nrx3ptr & 0x08) | channels->fr;

            channels->n4 = (channels->n4 & 0xc0)
                         + *((u8 *)&channels->fr + 1);
            *nrx4ptr = (s8)(channels->n4 & mask);
        }

        if (channels->mo & 1)
        {
            *(vu8 *)0x04000081 = (*(vu8 *)0x04000081 & ~channels->panMask)
                               | channels->pan;

            if (ch == 3)
            {
                *nrx2ptr = gUnknown_081B9FC8[channels->ev];

                if (channels->n4 & 0x80)
                {
                    *nrx0ptr = 0x80;
                    *nrx4ptr = channels->n4;
                    channels->n4 &= 0x7f;
                }
            }
            else
            {
                evAdd &= 0xf;
                *nrx2ptr = (channels->ev << 4) + evAdd;
                *nrx4ptr = channels->n4 | 0x80;

                if (ch == 1 && !(*nrx0ptr & 0x08))
                    *nrx4ptr = channels->n4 | 0x80;
            }
        }

    done:
        channels->mo = 0;
    }
}
asm(".global sub_08070FAC\n.thumb_set sub_08070FAC, CgbSound\n");
