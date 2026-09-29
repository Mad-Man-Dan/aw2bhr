#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808AC44.
 * sub_0808AC44 @ 0x0808AC44
 */

/* Installs the timer IRQ handler and publishes the timer's counter address.
 *
 * The second parameter was declared `int` in unknown-functions.h on the grounds
 * that "nothing types it"; this body does -- `str r0, [r2]` writes
 * &FlashTimerIntr through it, so it is a pointer to a `void (*)(void)`. Retyped
 * there in the same edit; the only caller is unpromoted.
 *
 * gUnknown_03000F70 is re-read with `ldrb` immediately after its own `strb`,
 * which is the `volatile` on its declaration doing its job -- naming the global
 * again rather than reusing `id` is what reproduces it.
 *
 * `cmp r1, #3; bhi` is the unsigned reject, and returning 1 there puts the
 * `movs r0, #1` block after the pool, which is why the success path's
 * `movs r0, #0` needs the `b`. */
u16 SetFlashTimerIntr(u8 id, void (**out)(void))
{
    if (id > 3)
        return 1;

    gUnknown_03000F70 = id;
    gUnknown_03000F78 = (vu16 *)(0x04000100 + gUnknown_03000F70 * 4);
    *out = FlashTimerIntr;

    return 0;
}
asm(".global sub_0808AC44\n.thumb_set sub_0808AC44, SetFlashTimerIntr\n");
