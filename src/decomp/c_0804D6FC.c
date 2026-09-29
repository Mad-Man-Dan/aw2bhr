#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804D6FC.
 * sub_0804D6FC @ 0x0804D6FC
 */

/* F087, the fourth member -- same shape as the matched src/decomp/c_0804D6C8.c
 * and typed from it. The one structural difference is the `+ 0x400` in the
 * destination offset: it breaks `shorten_binary_op`'s fold, so where CopyFigurePose400
 * emits `lsls #0x1d; lsrs #0x10` for `(u16)(a * 0x2000)` this emits
 * `lsls #0xd`, the add, then the `lsls #0x10; lsrs #0x10` pair.
 *
 * `lsls r2, #0x1a; lsrs r2, #0x10` on the third parameter is the same folded
 * `(u16)(c * 0x400)` multiply c_0804D6C8 documents -- read the pair as
 * `(u32)x << 26 >> 16`, a net left shift of ten, and write it as a MULTIPLY.
 *
 * Both products must be u16 LOCALS declared in this order, not casts inline in
 * the call: the locals put the two narrowings ahead of the address arithmetic,
 * which is where the ROM has them.
 *
 * `movs r4, #0x80; lsls r4, r4, #3` is the constant 0x400, materialised once
 * and reused for both the offset add and the size argument.
 *
 * The second parameter is never read (r1 is written by `lsls r1, r0, #0xd`
 * before any use) and is present in the signature unused, exactly as in
 * CopyFigurePose400. */
void CopyFigurePose400Upper(u16 a, int unused, u16 c)
{
    u16 src = c * 0x400;
    u16 dest = a * 0x2000 + 0x400;

    RegisterDataMove((u8 *)gUnknown_02029BA8[a].unk18[0] + src,
                 (void *)(0x06010000 + dest), 0x400);
}
asm(".global sub_0804D6FC\n.thumb_set sub_0804D6FC, CopyFigurePose400Upper\n");
