#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B15C.
 * sub_0803B15C @ 0x0803B15C, sub_0803B16C @ 0x0803B16C
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* gUnknown_03001470 list. Its counterpart IsDebugEntryScriptActive reaches the same
 * symbol through FindSlotScript rather than sub_080152EC, which is the same
 * allocator seen from the other side.
 */

void EndDebugEntryScript(void)
{
    sub_0801537C(gUnknown_0849E670);
}
asm(".global sub_0803B15C\n.thumb_set sub_0803B15C, EndDebugEntryScript\n");

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
bool8 IsDebugEntryScriptActive(void)
{
    return FindSlotScript((s32)gUnknown_0849E670) != -1;
}
asm(".global sub_0803B16C\n.thumb_set sub_0803B16C, IsDebugEntryScriptActive\n");
