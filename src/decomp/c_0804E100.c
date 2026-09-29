#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804E100.
 * sub_0804E100 @ 0x0804E100
 */

/* F087 -- three members that differ only in one size constant. The whole
 * function is one call to RegisterDataMove(src, dest, size) with size 0x800.
 *
 * Both `lsls #N; lsrs #0x10` pairs are a MULTIPLY that agbcc shortened to
 * HImode, not a shift and not a mask: `shorten_binary_op` applies to
 * MULT_EXPR, so `(u16)(x * 0x800)` becomes `lsl #27; lsr #0x10` in two
 * instructions while `(u16)(x << 11)` would need three (the shift, then the
 * zero-extend pair). That is why the family's `varies` entry is the shift
 * immediate -- 0x19/0x1a/0x1b IS 0x200/0x400/0x800, the same constant that
 * reappears as the size argument.
 *
 * The two products have to be u16 LOCALS and not casts written inline: the
 * locals put both narrowings ahead of the address arithmetic, which is the
 * order the original has. Spelled as casts inside the call the address
 * computation floats up between them and the narrowed parameter needs an
 * `adds r1, r0, #0` copy, one instruction the original does not have.
 *
 * The second parameter is never read -- r1 is written by `lsls r1, r0, #0x1d`
 * before any use -- so it is present in the signature and unused. The
 * destination base is a bare VRAM literal, not a symbol.
 */
void CopyFigurePose800(u16 a, int unused, u16 c)
{
    u16 src = c * 0x800;
    u16 dest = a * 0x2000;

    RegisterDataMove((u8 *)gUnknown_02029BA8[a].unk18[0] + src,
                 (void *)(0x06010000 + dest), 0x800);
}
asm(".global sub_0804E100\n.thumb_set sub_0804E100, CopyFigurePose800\n");
