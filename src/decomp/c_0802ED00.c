#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802ED00.
 * sub_0802ED00 @ 0x0802ED00
 */

#include "hardware.h"

/* Stop the link timer, then -- only if a session is live -- rewrite SIOCNT
 * from the shadow with the multiplayer start/IRQ bits forced on.
 *
 * The two tests share one arm, and the ROM reaches it by falling through from
 * the first `bne` into the second block's label: that is `a || b`, not two
 * separate ifs. gUnknown_03000564 is already volatile, which is why its `ldr`
 * cannot be hoisted above the timer store.
 *
 * 0x6080 is `movs #0xc1; lsls #7`, and the `adds r1, r3, #0` beside it is
 * agbcc keeping the constant in its own register for the `orrs`. */
void SioHandleIrq_Timer3(void)
{
    REG_TM3CNT_H = 0;

    if (gUnknown_03000570 != 0 || (gUnknown_03000564 & 3) == 2)
        REG_SIOCNT = gUnknown_03000560 | 0x6080;
}
asm(".global sub_0802ED00\n.thumb_set sub_0802ED00, SioHandleIrq_Timer3\n");
