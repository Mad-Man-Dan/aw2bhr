#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808603C.
 * sub_0808603C @ 0x0808603C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

/* Three calls with the same proc. The FIRST one costs no argument setup because
 * r0 still holds it on entry -- that is a pass-through, not a nullary call, and
 * the two `adds r0, r4, #0` after it are what a saved parameter costs.
 *
 * Wave 44 (W44-C) retyped MapSelectList_StepScrollRedraw's parameter from `ProcPtr` to
 * `struct Unk8606CProc *`; ProcPtr is `void *`, so the argument converts
 * implicitly and this stays byte-for-byte identical. Re-verified. */
void MapSelectList_InputLoop(ProcPtr proc)
{
    MapSelectList_StepScrollRedraw(proc);
    MapSelectList_HandleInput(proc);
    MapSelectList_DrawFrame(proc);
}

asm(".global sub_0808603C\n.thumb_set sub_0808603C, MapSelectList_InputLoop\n");
