#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801E334.
 * sub_0801E334 @ 0x0801E334
 */

/* RETYPED in wave 28: `u16` -> `int`, byte-identical here and required by the
 * caller. `ldrh` zero-extends, so the one-instruction body is the same either
 * way and this function's own bytes cannot settle the return type. Its caller
 * sub_0801E950 can and does: it forwards the result into HideOamObjects's `int`
 * parameter with no re-narrowing, and agbcc re-narrows a narrow-returning
 * callee at every call site. Re-verified with trymatch after the change. */
int GetObjectListCount(u16 *p)
{
    return *p;
}
asm(".global sub_0801E334\n.thumb_set sub_0801E334, GetObjectListCount\n");
