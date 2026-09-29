#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080859A0.
 * sub_080859A0 @ 0x080859A0
 */

/* A six-argument forwarder onto PutSprite. The last two arrive on the stack
 * (`ldr r1, [sp, #0x18]`, `ldr r7, [sp, #0x1c]`), and the SIXTH is the one that
 * becomes PutSprite's first argument -- so the two are not in a shared order
 * and the shape is not a pass-through.
 *
 * The first two parameters are `u16`: the `lsls #0x10; lsrs #0x10` pairs are
 * PROMOTE_MODE zero-extending declared-narrow parameters into their pseudos,
 * grouped with the other parameter copies at entry.
 *
 * `movs r1, #0x80; lsls r1, r1, #1` is just the constant 0x100 -- THUMB
 * immediates stop at 0xFF. The OAM word is `(c << 12) | (GetUnitSpriteTile(...) +
 * 0x100)`, with the shifted value as the OR's first operand (`orrs r4, r0`). */
void PutArmyUnitSprite(u16 a, u16 b, int c, int d, int e, int f)
{
    PutSprite(f, a, b, gUnknown_0848B690, (c << 12) | (GetUnitSpriteTile(d, e) + 0x100));
}
asm(".global sub_080859A0\n.thumb_set sub_080859A0, PutArmyUnitSprite\n");
