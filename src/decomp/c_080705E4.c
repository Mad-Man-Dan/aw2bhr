#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080705E4.
 * sub_080705E4 @ 0x080705E4
 */

/* m4aMPlayAllContinue -- MPlayContinue on every entry of the player table.
 * Free transcription of m4aMPlayAllStop with MPlayContinue as the callee; the two
 * are byte-identical once the `bl` target is normalised. See m4aMPlayAllStop and
 * the gNumMusicPlayers comment in include/unknown-globals.h for why the count
 * is an absolute symbol and not the literal 11.
 */
void m4aMPlayAllContinue(void)
{
    const struct MusicPlayer * p;
    u32 i;
    u32 n;

    n = (u16)(u32)&gNumMusicPlayers;

    if (n == 0)
        return;

    p = gUnknown_08242308;
    i = n;

    do
    {
        MPlayContinue(p->info);
        p++;
        i--;
    }
    while (i != 0);
}
asm(".global sub_080705E4\n.thumb_set sub_080705E4, m4aMPlayAllContinue\n");
