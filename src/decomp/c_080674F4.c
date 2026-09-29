#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080674F4.
 * sub_080674F4 @ 0x080674F4
 */

/* Returns a full word, not the `s16` this was first promoted as. The callee
 * body cannot tell you: `gUnknown_08580E64` is `const s16 []`, so the `ldrsh`
 * sign-extends into the whole register and `s16`, `int` and `u32` returns are
 * byte-identical HERE. The evidence is entirely caller-side, and there are
 * three of them -- sub_080688E4, sub_08068AC4 and sub_0806A054 all consume the
 * result with no `lsls #16; asrs #16` after the `bl`. An `s16` return makes
 * agbcc re-narrow at every call site, which is exactly +4 bytes each and was
 * +12 across the ROM at wave 13 promotion. Settled from the callers, as the
 * "stub's return type carries no information" rule requires. */
int GetIntroSceneDuration(int index)
{
    return gUnknown_08580E64[index];
}
asm(".global sub_080674F4\n.thumb_set sub_080674F4, GetIntroSceneDuration\n");
