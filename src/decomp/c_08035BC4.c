#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035BC4.
 * sub_08035BC4 @ 0x08035BC4
 */

/* Spawns a gUnknown_0849BDE0 sprite at the cell (x, y) if that cell is on
 * screen, stashes the cell in the new gUnknown_03001470 slot's unk1e/unk20,
 * positions it relative to the camera, and rewrites two fields of the slot's
 * OBJ attributes.
 *
 * BOTH BLOCKERS RECORDED BY THE EARLIER PARKED DRAFT WERE WRONG, and the
 * corrections are the useful part of this function:
 *
 * 1. StartSlotScriptWithSprite's fourth parameter does NOT need retyping. The old note
 *    argued the `void *` in include/unknown-functions.h was wrong because
 *    this caller passes a sign-extended s16 VALUE, and that fixing it meant
 *    editing five promoted files. It does not: `(void *)(int)a3` casts
 *    through `int` first, so there is no "pointer from integer of different
 *    size" warning, and it emits exactly the ROM's `lsls #0x10; asrs #0x10`.
 *    The prototype is left alone.
 *
 * 2. The OBJ-attribute tail needs no store back, and struct UnkVec by value
 *    is right. The old note said the bitfield spelling writes the modified
 *    word to the stack local first while the ROM has no `str`. The answer was
 *    already in src/decomp/c_0803B2BC.c and c_08027A50.c, both matched and
 *    both doing this exact triple: when the whole edit is ONE expression
 *    assigned to `v.unk04`, agbcc forwards it into the by-value argument and
 *    emits no store. Two statements would have made it store.
 *
 * The three parameters are s16, read forwards from PROMOTE_MODE: each is
 * zero-extended once at entry and sign-extended again at every arithmetic
 * use, while the unk1e/unk20 stores use the RAW zero-extended copies -- which
 * is exactly `strh` of an s16 parameter.
 *
 * The X bound is ONE unsigned compare (`cmp #0xef; bhi`) and the Y bound is
 * TWO signed ones. That asymmetry is the source's: fold merges
 * `a < 0 || a > N` into an unsigned `> N` inside one expression but not
 * across two separate `if` statements.
 *
 * SetSlotSpritePosition's two arguments are computed in HImode -- `ldrh` on the camera
 * origin despite unk04/unk06 being s16, then one `lsls #0x10; asrs #0x10` --
 * because that callee's parameters are s16, exactly as
 * src/decomp/c_08035B80.c documents for the same call. The bound tests above
 * read the SAME two members with `ldrsh`, because there the subtraction is
 * SImode. One declaration serves both sites.
 *
 * `id` is `s16`, and that is 4 bytes rather than a preference: it is what
 * puts the `ldr =gUnknown_03001470` BETWEEN the `lsls #0x18` and the
 * `asrs #0x18` of StartSlotScriptWithSprite's s8 return re-narrow. With `s8 id` the pool
 * load lands ahead of both shifts and with `int id` behind both; only s16
 * splits the pair the way the ROM does. Found by decomp-permuter from a
 * 98.0% hand draft -- an order-only residual, which is precisely its case.
 */
void sub_08035BC4(s16 x, s16 y, s16 a3)
{
    struct Unk03001470 *p;
    struct UnkVec v;
    s16 id;

    if (x - gMap->scrollX < 0
        || x - gMap->scrollX > 0xef)
        return;

    if (y - gMap->scrollY < 0)
        return;

    if (y - gMap->scrollY > 0x9f)
        return;

    id = StartSlotScriptWithSprite(gUnknown_0849BDE0, 5, gUnknown_0849BFD8, (void *)(int)a3, 2);

    p = &gUnknown_03001470[id];
    p->unk1e = x;
    p->unk20 = y;

    SetSlotSpritePosition(id,
                 x - gMap->scrollX,
                 y - gMap->scrollY);

    v = GetSlotSpriteAttrs(id);
    v.unk04 = (((v.unk04 & 0xFFFFFC00) | 0x176) & 0xFFFF0FFF) | 0x8000;
    SetSlotSpriteAttrs(id, v);
}
