#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802FA64.
 * sub_0802FA64 @ 0x0802FA64
 */

/* PARKED at 87.5% (56/56 bytes, 7 differ, first at +0xa). W31-A drafted this;
 * wave 31 W31-B ran the register-allocation probe the note asked for and the
 * answer is that this one is NOT an allocation problem you can steer. See
 * "MEASURED: what orders two simultaneously-live pointers" in
 * docs/agbcc-codegen.md for the probes.
 *
 * WHAT THE ROM NEEDS: `adds r2, r2, r1` -- the index sum inheriting the BASE's
 * register. We emit `add r1, r1, r3`, the sum inheriting the INDEX's. That one
 * choice causes everything else: whichever operand the destination combines
 * with picks up the sum's two extra references (2 -> 4, across the floor_log2
 * step in block_alloc's priority) and so wins the lower register. The r2/r3
 * swap is a consequence, not the cause.
 *
 * WHY IT IS UNREACHABLE: measured over ten spellings, in `pointer + runtime`
 * a SYMBOLIC base is ALWAYS operand 2; only a base loaded from memory is ever
 * operand 1. Array symbol, `&struct`, symbol dereferenced first, symbol stored
 * to a global first, symbol used for two adds, the sum built in the int domain,
 * `p = base; p += i;`, and a `u8 *const` local declared first -- all put the
 * index first. Two controls with a pointer GLOBAL as the base put the base
 * first. Forcing the symbol into a register early does not help: -fforce-addr
 * materialises it but the RTL still has it as operand 2. The `+ 0x3E` cannot
 * fold into the load either, because ldrb's immediate offset caps at 31.
 *
 * SO: with gPlaySt reached as `&object` this cannot match, and no
 * eleventh rewrite of the address expression will change that -- fourteen
 * spellings across two waves now agree. The open question is a TYPE-MODEL one:
 * what pointer object does the byte at +0x3E actually arrive through? The ROM
 * has a single `ldr rN, =gPlaySt` with no load through it, so it is
 * not simply a pointer global; something about this object's declaration is
 * still wrong. That is where the next look should go.
 *
 * Everything else about this function is settled and correct: the command
 * record, the 0xA9 opcode, the tag 4, the volatile double read of unk06 (both
 * `ldrb [rN,#6]` reproduced exactly), and the pool order. */
void SioSendPingPacket(void)
{
  u8 *p;
  gUnknown_0202575C.unk00 = 0xA9;
  gUnknown_0202575C.unk01 = gUnknown_0849B018->unk06;
  p = ((u8 *) (&gPlaySt)) - (-gUnknown_0849B018->unk06);
  gUnknown_0202575C.unk02 = p[0x3E];
  sub_0802F588(&gUnknown_0202575C, 4);
}
asm(".global sub_0802FA64\n.thumb_set sub_0802FA64, SioSendPingPacket\n");
