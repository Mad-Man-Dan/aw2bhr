#include "global.h"
#include "proc.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803C2E8.
 * sub_0803C2E8 @ 0x0803C2E8, sub_0803C2FC @ 0x0803C2FC
 */

/* F002: `push {lr}; ldr r0,=g; movs r1,#0; bl sub_080152EC; pop {r0}; bx r0`.
 * One statement, result discarded -- `pop {r0}` makes the wrapper void.
 * sub_080152EC's first parameter is `const void *`, which takes the
 * `const u8 []` blob with no cast; a plain `void *` would discard the const and
 * -Werror turns that into an error. src/decomp/c_0802BF80.c is the matched
 * exemplar for this shape. */
void sub_0803C2E8(void)
{
    sub_080152EC(ProcScr_Versus, 0);
}

/* Family F020 (tools/families.py): seven 32-byte leaves that are all
 * `return <slot lookup>(script) != -1;`. The whole body is one `!=` compiled
 * as a VALUE rather than a branch: agbcc's do_store_flag turns `x != 0` into
 * `(x | -x) >> 31`, and the `mvns` in front of it is the `-1` folded into the
 * comparison (`x != -1` is `~x != 0`). The `lsls #N; asrs #N` pair is the
 * callee's declared signed return width re-extended at the call site --
 * `#0x18` for FindSlotScript's `s8`, `#0x10` for FindEventScriptSlot's `s16`.
 * Probed and byte-identical, so none of them is evidence: `bool8` vs `int`
 * return, and an `s8`/`s16` local for the result before the compare. The
 * explicit `if (...) return TRUE; return FALSE;` spelling is NOT -- it emits
 * the branching four-block form from the Control-flow table in
 * docs/agbcc-codegen.md and is four bytes longer.
 */
bool8 sub_0803C2FC(void)
{
    return FindSlotScript((s32)ProcScr_Versus) != -1;
}
