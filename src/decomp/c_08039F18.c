#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039F18.
 * sub_08039F18 @ 0x08039F18
 */

/* One nested expression, which is what loads gTextTable's address first
 * into the callee-saved r4 and keeps it live to the end. The `adds r3, #0x38`
 * on the base rather than a folded displacement is the `g[i].member` hoist for
 * a word access on an array global.
 *
 * The result is a NUL-terminated string: the only caller, CoPowerIntro_FadeInLoop, hands
 * it straight to StartCoPowerNameBanner, which copies bytes until the first zero. */
u8 *GetArmyCoPowerName(int a)
{
    return gTextTable[gUnknown_085D3DD0[gPlayers[a].co]
                                 .power[gPlayers[a].coMode]
                                 .powerNameId];
}
asm(".global sub_08039F18\n.thumb_set sub_08039F18, GetArmyCoPowerName\n");
