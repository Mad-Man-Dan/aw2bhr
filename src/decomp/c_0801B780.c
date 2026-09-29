#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B780.
 * sub_0801B780 @ 0x0801B780
 */

#include "hardware.h"

/* Loads a 0x500-byte tile blob into VRAM at a slot derived from the caller's
 * id, and bumps the pair of counters at gUnknown_03002B80 first.
 *
 * The destination is
 *   BG char base * 0x4000  +  ((a + 1) & 0x3FF) * 32  +  0x06000000
 * where the char base comes from the BGCNT shadow's 2-bit chr_block field
 * (`ldr`, then `lsls #0x1c; lsrs #0x1e`) and 32 bytes is one 4bpp tile.
 *
 * THE ASSOCIATION IS LOAD-BEARING and cost a probe round. The ROM adds
 * 0x06000000 to the TILE term (`adds r4, r4, r2` before `adds r1, r1, r4`).
 * Written as `chr * 0x4000 + (tile * 32 + 0x06000000)` fold hoists the constant
 * out of the parentheses and re-associates it onto the char base instead,
 * giving the same 64 bytes in the wrong order. Casting the CHAR BASE to a
 * pointer keeps the integer sum intact, because fold will not re-associate an
 * integer constant across the pointer-typed operand. */
void InitTextTileCache(int a)
{
    ResetTextTileCache(a + 0x28);
    RegisterDataMove(gUnknown_080A5524,
                 (u8 *)(gUnknown_03002B6C.bits.chr_block * 0x4000)
                     + (((a + 1) & 0x3FF) * 32 + 0x06000000),
                 0x500);
}
asm(".global sub_0801B780\n.thumb_set sub_0801B780, InitTextTileCache\n");
