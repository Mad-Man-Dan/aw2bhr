#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B70C.
 * sub_0801B70C @ 0x0801B70C, sub_0801B724 @ 0x0801B724, sub_0801B738 @ 0x0801B738
 */

/* An indirect call into the IWRAM overlay copied by StoreRoutinesToIRAM. The pool word
 * is the callee's VALUE, not a variable's address: 0x03006029 is odd, there is
 * no load through it, and aw2bhr.map allocates nothing there -- gen_lds.py
 * invented the symbol from the word itself. Same situation as sub_0801B6EC's
 * 0x0300619D, and the `&symbol` cast is what reproduces the relocation the ROM
 * carries; the honest literal emits a bare `.word` instead.
 *
 * `bl _call_via_r3` counts the arguments: r0, r1 and r2 are all occupied, so
 * the call takes three. The two `lsls #0x10; lsrs #0x10` pairs type the second
 * and third as u16; the first rides r0 untouched and nothing types it. */
void sub_0801B70C(int a, u16 b, u16 c)
{
    ((void (*)(int, u16, u16))&gUnknown_03006029)(a, b, c);
}

/* A second entry point into the same IWRAM overlay -- 0x03005E8D, again odd,
 * again with no load through it. See sub_0801B70C beside it.
 *
 * `bl _call_via_r3` again means three arguments, but here only the THIRD is
 * narrowed, so the first two pass through untyped and only `c` is u16. */
void sub_0801B724(int a, int b, u16 c)
{
    ((void (*)(int, int, u16))&gUnknown_03005E8D)(a, b, c);
}

/* The third entry point into the IWRAM overlay copied by StoreRoutinesToIRAM, at
 * 0x03005C89 -- one byte past gUnknown_03005C88, the overlay's own base, with
 * the low bit set for THUMB. That adjacency is the clearest evidence in the
 * block that the region really is code: the copy's destination and this call's
 * target are the same address.
 *
 * `bl _call_via_r4` is FOUR arguments -- r0..r3 are all occupied, so gcc has to
 * reach past the scratch registers for the pointer, which is also why this one
 * pushes r4 where its two neighbours do not. `lsls #0x18; lsrs #0x18` types the
 * first as u8; the other three pass through.
 *
 * It RETURNS the result, and the epilogue is the only thing that says so:
 * `pop {r1}; bx r1` preserves r0 across the return where the void form would
 * have popped into r0 and clobbered it. Both are 24 bytes, so size does not
 * discriminate -- the register does. No re-narrowing, so `int`. */
int DrawGlyphRam(u8 a, int b, int c, int d)
{
    return ((int (*)(u8, int, int, int))&gUnknown_03005C89)(a, b, c, d);
}
asm(".global sub_0801B738\n.thumb_set sub_0801B738, DrawGlyphRam\n");
