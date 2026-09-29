#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039264.
 * sub_08039264 @ 0x08039264
 */

/* Family F011: `push {lr}; bl f; ldr r0,=g; movs r1,#N; bl h; pop {r0}; bx r0`.
 * Two statements and NOT a nest -- r0 is overwritten by the pool `ldr` between
 * the two calls, so nothing can survive from the first one. `pop {r0}` is the
 * void epilogue.
 *
 * The odd one out: the pool word is a FUNCTION address, not a global, and the
 * second callee is RunOrQueueDrawCallback -- so this is the wave-12 F002 body with a
 * leading `bl`. Third member of the RunOrQueueDrawCallback callback set after
 * ShowDayAndFundsBar and sub_0803A53C, and sub_08039188 is the same kind of body
 * they register: void(void), ignoring whatever RunOrQueueDrawCallback hands it.
 * The `(void *)` cast is that family's house convention and is what makes the
 * pool word relocate against the symbol rather than become a plain constant.
 * RunOrQueueDrawCallback returns a value; this discards it. */
void UpdateMovePathAndQueueDraw(void)
{
    UpdateMovePathToCursor();
    RunOrQueueDrawCallback((void *)sub_08039188, 2);
}
asm(".global sub_08039264\n.thumb_set sub_08039264, UpdateMovePathAndQueueDraw\n");
