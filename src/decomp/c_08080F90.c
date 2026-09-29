#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080F90.
 * sub_08080F90 @ 0x08080F90
 */

/* Takes ONE argument that this body ignores, and the parameter is byte-neutral
 * here -- which is why it was first promoted `(void)`. The evidence is entirely
 * caller-side: InitGameSystems emits `movs r0, #0` immediately before its
 * `bl ResetMainMenuCarouselState`, with no other consumer of r0, and that instruction only
 * exists if an argument is being passed. Wave 27 briefly settled this the other
 * way from the definition and the merged-unit check rejected it. */
void ResetMainMenuCarouselState(int a)
{
    gUnknown_03005968 = 0;
    gUnknown_03005920 = 0;
    gUnknown_03005924 = 6;
    gUnknown_030058FC = 0;
}
asm(".global sub_08080F90\n.thumb_set sub_08080F90, ResetMainMenuCarouselState\n");
