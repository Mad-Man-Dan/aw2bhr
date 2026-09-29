#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808AD68.
 * sub_0808AD68 @ 0x0808AD68
 */

u8 ReadFlash1(u8 *p)
{
    return *p;
}
asm(".global sub_0808AD68\n.thumb_set sub_0808AD68, ReadFlash1\n");
