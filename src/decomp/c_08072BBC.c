#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072BBC.
 * sub_08072BBC @ 0x08072BBC
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


/* Forwards into PlayMusicOrSfx2, which is the F066 member above and is already
 * declared `void(int)`, so -- as in FreeSpriteScript2 -- the narrowing has to be
 * spelled. This function has no callers anywhere in asm/ or src/, so nothing
 * constrains its own parameter and `s16` with no cast would be byte-identical.
 */

void PlaySeFunc(int a)
{
    PlayMusicOrSfx2((s16)a);
}
asm(".global sub_08072BBC\n.thumb_set sub_08072BBC, PlaySeFunc\n");
