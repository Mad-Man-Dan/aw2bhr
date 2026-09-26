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
 * second callee is sub_0801F024 -- so this is the wave-12 F002 body with a
 * leading `bl`. Third member of the sub_0801F024 callback set after
 * sub_0802BCD8 and sub_0803A53C, and sub_08039188 is the same kind of body
 * they register: void(void), ignoring whatever sub_0801F024 hands it.
 * The `(void *)` cast is that family's house convention and is what makes the
 * pool word relocate against the symbol rather than become a plain constant.
 * sub_0801F024 returns a value; this discards it. */
void sub_08039264(void)
{
    sub_08038D7C();
    sub_0801F024((void *)sub_08039188, 2);
}
