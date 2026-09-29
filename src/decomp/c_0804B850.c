#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804B850.
 * sub_0804B850 @ 0x0804B850
 */

/* Parameters 6 and 7 are dead here but the promoted caller c_0804B744.c
 * materialises all seven. Parameter 1 is u16, not the `int` the header used to
 * declare: the prologue truncates r0 and r1 with back-to-back
 * `lsls #0x10; lsrs #0x10` pairs, the PROMOTE_MODE entry pattern. */
void LoadBattleBackdropArt(u16 a, u16 b, void *c, void *d, void *e, void *f, void *g)
{
    int i;

    for (i = 0; i <= 0x3ff; i++)
        gUnknown_085519FC[i] = 0x1ff;

    LoadBackdropTiles(b, c, 0x4000);
    sub_0804BB74(b, d, 0x800, a);
    LoadBackdropPalette(b, e, 0x60);
}
asm(".global sub_0804B850\n.thumb_set sub_0804B850, LoadBattleBackdropArt\n");
