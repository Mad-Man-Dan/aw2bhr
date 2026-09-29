#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08041258.
 * sub_08041258 @ 0x08041258
 */

/* `lsls r1, r0, #4; subs r1, r1, r0; lsls r1, r1, #2` is a multiply by
 * 15 * 4 = 0x3c, which is exactly sizeof(struct PlayerStruct) -- so this is an
 * array index on gPlayers and not hand-rolled arithmetic. Neither
 * parameter is narrowed at entry, so both are `int`.
 *
 * Nested `if`s and not a three-way chain: the `bne` on unk1b jumps PAST both
 * inner arms to the 0x1DA call, and `movs r0, #0xed; lsls r0, r0, #1` is how
 * agbcc materialises 0x1DA. */
void PlayCaptureCompleteSound(int a, int b)
{
    if (gPlayers[a].aiControlled == 1)
    {
        if ((b & 0x1F) == 8)
            PlayMusicOrSfx2(0x83);
        else
            PlayMusicOrSfx2(0x1D9);
    }
    else
    {
        PlayMusicOrSfx2(0x1DA);
    }
}
asm(".global sub_08041258\n.thumb_set sub_08041258, PlayCaptureCompleteSound\n");
