#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08052EE4.
 * sub_08052EE4 @ 0x08052EE4
 */

#include "hardware.h"

/* gUnknown_030020B4 is `union DispStatBuf` in include/hardware.h and the bit-3
 * write is `.bits.vblank_int_enable = 1`, the spelling four already-matched
 * files carry (c_0801295C.c, c_08012AAC.c).
 *
 * gUnknown_03001FD0 is declared `u16` and its only other user
 * (src/decomp/c_080123EC.c) just clears it whole, so nothing so far said how
 * wide the object really is. This function BYTE-addresses it at +0 and +1 --
 * two independent `ldrb`/`orrs`/`strb` pairs off one pool word -- which makes
 * the halfword declaration at best incomplete: it is an aggregate of at least
 * two bytes, probably another register shadow given the FlushDisplayRegisters flush that
 * follows. The byte-pointer view is the weakest spelling that reproduces the
 * ROM without retyping a global that a matched file already writes; NOT a claim
 * about the real type. Both masks here are SETS, and per docs/agbcc-codegen.md
 * a set is byte-identical for a bitfield and a scalar OR alike, so this
 * function cannot discriminate either. */

void BattleAnimEnableVBlank(void)
{
    gUnknown_030020B4.bits.vblank_int_enable = 1;

    *(u8 *)&gUnknown_03001FD0 |= 1;
    *((u8 *)&gUnknown_03001FD0 + 1) |= 0x80;

    FlushDisplayRegisters();
}
asm(".global sub_08052EE4\n.thumb_set sub_08052EE4, BattleAnimEnableVBlank\n");
