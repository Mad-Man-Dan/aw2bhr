#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011704.
 * sub_08011704 @ 0x08011704
 */

/* Forwards three narrowed values plus a fixed ROM blob into PutOamHi.
 *
 * The second argument is `b & 0xFF` and NOT `(u8)b`, and the two are different
 * code even though they are the same value. With `& 0xFF`, agbcc keeps the
 * `lsls #0x10` half of the u16 parameter's PROMOTE_MODE zero-extension live
 * and does the mask in that shifted domain -- `movs #0xff; lsls #0x10; ands;
 * lsrs #0x10`, which is the ROM. `(u8)b` is recognised as a byte extract and
 * collapses to `lsls #0x18; lsrs #0x18`, six bytes shorter. The first
 * argument's `a & 0x1FF` folds with its own promote the other way, into a
 * single `lsls #0x17; lsrs #0x17` pair.
 *
 * All three parameters are u16: the third is passed through with nothing but
 * the bare promote pair, which is what a declared-narrow parameter emits. */
void PutGlyphSprite(u16 a, u16 b, u16 c)
{
    PutOamHi(a & 0x1FF, b & 0xFF, (void *)gUnknown_0848930C, c);
}
asm(".global sub_08011704\n.thumb_set sub_08011704, PutGlyphSprite\n");
