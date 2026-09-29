#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BB10.
 * sub_0801BB10 @ 0x0801BB10
 */

#include "hardware.h"

/* The whole interface to the interrupt-enable shadow at 0x03000060: op 0 sets
 * it, 1 masks it, 2 ors into it, and any other value leaves it alone; the tail
 * then pushes it to REG_IE and derives REG_IME from the software flag at
 * bit 16.
 *
 * IT CANNOT BE DECLARED AS ITS OWN SYMBOL -- aw2bhr.lds names gUnknown_0300005C
 * and then gUnknown_03000068, so 0x03000060 falls inside the former's region
 * and tools/proto_check.py rejects an invented `gUnknown_03000060` (the split
 * build would fail with `undefined reference` even though trymatch matches).
 * `((u32 *)&gUnknown_0300005C)[1]` is the spelling, exactly as the wave-32 note
 * on 0x03000600/0x03000602 prescribes; see include/unknown-globals.h.
 *
 * IT MUST BE THE REPEATED EXPRESSION AND NOT A `u32 *` LOCAL. Bound to a local
 * the whole -fforce-addr level collapses: one direct pool word, no `ldr
 * rN,[rN]` indirection, and the function comes out 88 bytes against 120.
 * Written out at each of the six uses, agbcc parks the address in this unit's
 * .rodata and reproduces the ROM's two-level load -- promotion must carry the
 * rodata entry for 0x0808F088, whose ROM content is 0x03000060.
 *
 * `REG_IME = 0` in the else arm re-uses the register the 0x10000 test left at
 * zero rather than materialising a fresh constant, which is ordinary constant
 * propagation off the `cmp; beq` and needs no special spelling.
 *
 * NOTE for the header sweep: include/unknown-functions.h declares this as
 * `void UpdateInterruptEnable();` at ~line 98, i.e. with NO arity checking at all.
 * Arity 2 is read off the body: r0 selects the switch and r1 is copied to r3
 * and used as a word mask. */
void UpdateInterruptEnable(int op, u32 mask)
{
    switch (op)
    {
    case 0:
        ((u32 *)&gUnknown_0300005C)[1] = mask;
        break;
    case 1:
        ((u32 *)&gUnknown_0300005C)[1] &= mask;
        break;
    case 2:
        ((u32 *)&gUnknown_0300005C)[1] |= mask;
        break;
    }

    REG_IE = ((u32 *)&gUnknown_0300005C)[1];
    if (((u32 *)&gUnknown_0300005C)[1] & 0x10000)
        REG_IME = 1;
    else
        REG_IME = 0;
}
asm(".global sub_0801BB10\n.thumb_set sub_0801BB10, UpdateInterruptEnable\n");
