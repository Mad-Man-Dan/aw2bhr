#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B750.
 * sub_0801B750 @ 0x0801B750
 */

/* Family F011: `push {lr}; bl f; ldr r0,=g; movs r1,#N; bl h; pop {r0}; bx r0`.
 * Two statements and NOT a nest -- r0 is overwritten by the pool `ldr` between
 * the two calls, so nothing can survive from the first one. `pop {r0}` is the
 * void epilogue.
 *
 * THE ONE MEMBER OF THE FAMILY THAT TAKES A PARAMETER, and the tell is
 * entirely on the callee side: r0 is not written before `bl InitTextTileCache`, and
 * InitTextTileCache reads it (`adds r4, r0, #0; adds r0, #0x28`). A forwarded
 * parameter costs zero instructions, so the alternative -- `InitTextTileCache(0)`
 * -- is what would have been visible, as a `movs r0, #0` ahead of the call.
 * `int` and not a narrow type: there is no shift pair anywhere in the body. */
void InitTextTileCacheAndPreload(int a)
{
    InitTextTileCache(a);
    PreloadTextStrings(gUnknown_0808EF64, 0);
}
asm(".global sub_0801B750\n.thumb_set sub_0801B750, InitTextTileCacheAndPreload\n");
