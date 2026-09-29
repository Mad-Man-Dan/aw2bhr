#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803A53C.
 * sub_0803A53C @ 0x0803A53C
 */

/* F002 shape, third callee: `RunOrQueueDrawCallback(fn, K)`. The pool word holds a
 * FUNCTION address, not a global -- UnitInfoPanel_DrawPictureAndFuel is a void(void) body, and
 * RunOrQueueDrawCallback either calls it straight away or queues it. The `(void *)` cast
 * is the QueueVBlankCallback / AddVBlankHook house convention for handing a function to
 * a `void *` parameter in C89, and it is what makes the pool word relocate
 * against the symbol rather than becoming a plain constant.
 * RunOrQueueDrawCallback returns a value (`pop {r1}; bx r1`); this wrapper discards it and
 * is itself void (`pop {r0}`). */
void sub_0803A53C(void)
{
    RunOrQueueDrawCallback((void *)UnitInfoPanel_DrawPictureAndFuel, 1);
}
