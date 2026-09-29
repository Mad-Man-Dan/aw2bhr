#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015568.
 * sub_08015568 @ 0x08015568
 */

/* Family F066 (data/families.json): `push {lr}; lsls r0,r0,#0x10;
 * asrs r0,r0,#0x10; bl S; pop {r0}; bx r0` -- 16 bytes, three members, and
 * `varies` has exactly one entry, the callee. `lsls`+`asrs` is a value-kept
 * SIGNED 16-bit narrowing (docs/agbcc-codegen.md), not a truth test.
 *
 * Where the narrowing comes from has to be settled callee-side, because
 * PROMOTE_MODE makes `s16` and `u16` parameters identical at entry. Probed:
 * `int` param plus an explicit `(s16)` cast, `s16` param with no cast, and
 * either of those against an `s16` callee are all byte-identical; only a `u16`
 * parameter differs, giving `lsr` where the ROM has `asr`. See
 * include/unknown-functions.h.
 */


/* FreeSpriteScript takes an `int` -- its prologue has no shift pair at all and the
 * value goes straight into `muls r0,r7,r0` with r7 = 0x4c, and its other caller
 * at 0x0801D840 passes a bare `adds r0,r3,#0`. So the narrowing here cannot be
 * an implicit conversion and is spelled as a cast. An `s16` parameter with no
 * cast is byte-identical and equally consistent with this function's single
 * caller (`movs r1,#0x26; ldrsh r0,[r4,r1]`, an s16 object); `int` is declared
 * as the weaker contract, and the choice has no oracle.
 */

void FreeSpriteScript2(int a)
{
    FreeSpriteScript((s16)a);
}
asm(".global sub_08015568\n.thumb_set sub_08015568, FreeSpriteScript2\n");
