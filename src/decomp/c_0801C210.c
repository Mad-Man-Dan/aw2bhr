#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C210.
 * sub_0801C210 @ 0x0801C210
 */

/* Allocates an animation handle and initialises it, or returns NULL when the
 * pool is full. The three parameters are its own (a bare pointer copy, then
 * `lsls #0x10; lsrs #0x10` and `lsls #0x18; lsrs #0x18`, i.e. PROMOTE_MODE on
 * u16 and u8) and they are forwarded to AP_Init unchanged. */
struct Unk0801C210 *AP_Create(void *a1, u16 a2, u8 a3)
{
    struct Unk0801C210 *p = AP_Find(0);

    if (p == NULL)
        return NULL;

    AP_Init(p, a1, a2, a3);
    return p;
}
asm(".global sub_0801C210\n.thumb_set sub_0801C210, AP_Create\n");
