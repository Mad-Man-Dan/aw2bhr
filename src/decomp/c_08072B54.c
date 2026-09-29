#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072B54.
 * sub_08072B54 @ 0x08072B54
 */

/* Play song `songNum` panned by the screen x `x`: report the song id to
 * PlayMusicOrSfx2, look the song's player up through the two ROM tables and hand
 * the player's MusicPlayerInfo to the start and panpot calls. Screen2Pan
 * maps 0..0xef onto -0x60..0x5f, which the `lsls #0x18; asrs #0x18` at the
 * call site narrows to s8. */
void PlaySeSpacial(int songNum, int x)
{
    struct MusicPlayerInfo *mp;

    PlayMusicOrSfx2((s16)songNum);

    mp = gUnknown_08242308[gUnknown_0824238C[songNum].ms].info;

    m4aMPlayImmInit(mp);
    MPlayPanpotControl(mp, 0xffff, Screen2Pan(x));
}
asm(".global sub_08072B54\n.thumb_set sub_08072B54, PlaySeSpacial\n");
