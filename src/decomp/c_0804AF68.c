#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804AF68.
 * sub_0804AF68 @ 0x0804AF68, sub_0804AF88 @ 0x0804AF88
 */

/* One five-argument call and nothing else. 0x6827 is too large for a THUMB
 * immediate so it arrives as the function's only pool word, and the fifth
 * argument goes on the stack (`sub sp, #4` / `str r0, [sp]`), which is why the
 * `movs r0, #0` for it is emitted before the r0/r1/r3 immediates: argument
 * setup is grouped by operand class, not by argument order. */
void sub_0804AF68(void)
{
    sub_08043C28(0x38, 0xA0, 0x6827, 0, 0);
}

/* A short-circuit `||`: the first arm falls through to the store on `bne`
 * while the second branches PAST it on `beq`, which is the two-test shape a
 * single `||` produces and not two separate ifs.
 *
 * unk5c is reached as `adds r0, #0x5c; ldrb` -- the ldrb displacement limit of
 * 31, not an address being taken. The neighbour NameEntry_SetFlagIfMode1 spells the same
 * byte `unk2c[0x30]`; the field name is used here because the header's own
 * evidence for unk5c (StartNameEntryMode0 / StartNameEntryMode1 write 0 and 1 into it) makes
 * it a discriminator rather than an array element. Byte-identical either way. */
void NameEntry_SetSkipFlagIfModeOrFlag(void)
{
    if (gUnknown_02028E48 != 0 || gUnknown_030044E0->unk5c != 0)
        gUnknown_03002F1C = 1;
}
asm(".global sub_0804AF88\n.thumb_set sub_0804AF88, NameEntry_SetSkipFlagIfModeOrFlag\n");
