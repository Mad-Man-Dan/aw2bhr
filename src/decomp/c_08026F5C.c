#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026F5C.
 * sub_08026F5C @ 0x08026F5C
 */

/* "Is this unit on the current army's side?" -- the same unk2a comparison as
 * src/decomp/c_08026F28.c, but with the second index derived from a unit id
 * rather than supplied. `(a1 >> 6) + 1` is the usual 1-based army number, and
 * the `+ 1` never appears as an add: agbcc folds it into the displacement,
 * which is why the second `adds` is #0x66 = 0x3c + 0x2a. `lsls #0x10;
 * asrs #0x16` is the s16 sign-extension fused with the `>> 6`, so the shift is
 * ARITHMETIC and a1 is signed.
 *
 * `movs #0` and `movs #1` split across an unconditional `b` is the one shape
 * `return <cmp>;` cannot produce (docs/agbcc-codegen.md): a returned comparison
 * goes through do_store_flag and arrives with no branch at all. */

bool8 IsUnitOnCurrentTeam(s16 a1)
{
    if (gPlayers[gUnknown_030033EC].team == gPlayers[(a1 >> 6) + 1].team)
        return TRUE;

    return FALSE;
}
asm(".global sub_08026F5C\n.thumb_set sub_08026F5C, IsUnitOnCurrentTeam\n");
