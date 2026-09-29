#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08070610.
 * sub_08070610 @ 0x08070610
 */

void m4aMPlayFadeOut(void *a1, u16 a2)
{
    MPlayFadeOut(a1, a2);
}
asm(".global sub_08070610\n.thumb_set sub_08070610, m4aMPlayFadeOut\n");
