#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080703B8.
 * sub_080703B8 @ 0x080703B8, sub_080703D4 @ 0x080703D4
 */

/* m4aMPlayContinue. The trailing `mplayInfo->ident = ident` emits nothing --
 * GCC deletes it as redundant -- but it is not optional: it keeps `ident` live
 * across the branch, which is what puts it in r3 rather than r1. Every entry
 * point in this file re-stamps ident on the way out; the ones that touch tracks
 * bump it first, and here the bump is absent so only the liveness survives.
 */
void MPlayContinue(struct MusicPlayerInfo * mplayInfo)
{
    u32 ident = mplayInfo->ident;

    if (ident != MPLAY_ID_NUMBER)
        return;

    mplayInfo->status &= ~MUSICPLAYER_STATUS_PAUSE;
    mplayInfo->ident = ident;
}
asm(".global sub_080703B8\n.thumb_set sub_080703B8, MPlayContinue\n");

/* m4aMPlayFadeOut. See MPlayContinue for why the trailing ident store is needed
 * even though it emits nothing.
 */
void MPlayFadeOut(struct MusicPlayerInfo * mplayInfo, u16 speed)
{
    u32 ident = mplayInfo->ident;

    if (ident != MPLAY_ID_NUMBER)
        return;

    mplayInfo->fadeOC = speed;
    mplayInfo->fadeOI = speed;
    mplayInfo->fadeOV = (64 << FADE_VOL_SHIFT);
    mplayInfo->ident = ident;
}
asm(".global sub_080703D4\n.thumb_set sub_080703D4, MPlayFadeOut\n");
