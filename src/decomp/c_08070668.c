#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08070668.
 * sub_08070668 @ 0x08070668
 */

/* MPlayImmInit. Resets every track that both exists and is flagged 0x40
 * (MPT_FLG_START) back to its power-on state. Same `s32 i = trackCount` /
 * 0x50-stride walk as MPlayStop_rev01.
 *
 * The two mask tests must be NESTED ifs, not `&&`. agbcc's fold_truthop
 * rewrites `(f & 0x80) && (f & 0x40)` into the single `(f & 0xc0) == 0xc0` --
 * measured, 8 bytes short. Two statements are two tests, and each mask then
 * gets a callee-saved register it shares with the store that reuses the same
 * literal (0x80 into flags, 0x40 into volX), which is where r6 and r7 in the
 * push list come from.
 */
void m4aMPlayImmInit(struct MusicPlayerInfo * mplayInfo)
{
    s32 i;
    struct MusicPlayerTrack * track;

    i = mplayInfo->trackCount;
    track = mplayInfo->tracks;

    while (i > 0)
    {
        if (track->flags & MPT_FLG_EXIST)
        {
            if (track->flags & 0x40)
            {
                Clear64byte_rev(track);
                track->flags = MPT_FLG_EXIST;
                track->bendRange = 2;
                track->volX = 0x40;
                track->lfoSpeed = 0x16;
                track->tone.type = 1;
            }
        }

        i--;
        track++;
    }
}
asm(".global sub_08070668\n.thumb_set sub_08070668, m4aMPlayImmInit\n");
