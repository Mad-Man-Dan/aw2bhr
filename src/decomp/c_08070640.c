#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08070640.
 * sub_08070640 @ 0x08070640
 */

/* m4aMPlayFadeInContinue. See MPlayContinue for why the trailing ident store is
 * needed even though it emits nothing.
 */
void m4aMPlayFadeInContinue(struct MusicPlayerInfo * mplayInfo, u16 speed)
{
    u32 ident = mplayInfo->ident;

    if (ident != MPLAY_ID_NUMBER)
        return;

    mplayInfo->fadeOC = speed;
    mplayInfo->fadeOI = speed;
    mplayInfo->fadeOV = FADE_IN;
    mplayInfo->status &= ~MUSICPLAYER_STATUS_PAUSE;
    mplayInfo->ident = ident;
}
asm(".global sub_08070640\n.thumb_set sub_08070640, m4aMPlayFadeInContinue\n");
