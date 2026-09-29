#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801815C.
 * sub_0801815C @ 0x0801815C
 */

#include "hardware.h"

/* `gUnknown_03002B6C.bits.chr_block * 0x4000` is the promoted c_08013C00.c
 * spelling of `lsls #0x1c; lsrs #0x1e; lsls #0xe` -- a BGCNT character-block
 * base -- and 0x06004160 is an offset into it, so the second argument is a
 * VRAM address.
 *
 * The `v = 9` preset with a conditional override to 1 is this block's house
 * idiom; sub_08018018 and sub_08018194 open the same way. The bare
 * `lsls r0, #0x18` on sub_08078198's result is what retyped that function from
 * `s32` to `bool8` -- see src/decomp/c_08078198.c. */
void sub_0801815C(u8 a)
{
    int v;

    v = 9;
    if (sub_08078198())
        v = 1;

    LoadCoFace(a, (void *)(0x06004160 + gUnknown_03002B6C.bits.chr_block * 0x4000), v);
}
