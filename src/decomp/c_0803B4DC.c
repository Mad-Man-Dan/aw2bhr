#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B4DC.
 * sub_0803B4DC @ 0x0803B4DC
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


/* NO cast here, unlike its two siblings: PlayMusicOrSfx's parameter is `s16`
 * (five call sites, all signed, one of them a sum of three ints sign-narrowed
 * at the call), so the conversion is implicit. That refines the note already in
 * include/unknown-functions.h, which reads this shift pair correctly as a
 * conversion at a use inside the body but does not say the source has no cast.
 *
 * The `int` parameter is NOT a free choice and was settled before this wave:
 * SlotOp_PlayMusic/SlotOp_PlaySfx pass a `u16` out of a script stream with a bare
 * `ldrh`, which an `s16` parameter would fold into `movs r1,#4; ldrsh`, and
 * src/proc.c passes an `s16` dataImm, which rules out `u16`.
 */

void PlayMusicOrSfx2(int a)
{
    PlayMusicOrSfx(a);
}
asm(".global sub_0803B4DC\n.thumb_set sub_0803B4DC, PlayMusicOrSfx2\n");
