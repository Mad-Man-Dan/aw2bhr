#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808AED0.
 * sub_0808AED0 @ 0x0808AED0
 */

/* MATCHED (wave 44, W44-G). The verify half of the SRAM pair whose copy half is
 * the promoted src/decomp/c_0808AE30.c, and it reuses that file's counter idiom
 * unchanged: the count is split out as `i = n - 1` AHEAD of an `if (n != 0)`
 * guard and the loop is a `do/while (i-- != 0)`, because `while (n--)`
 * materialises the -1 sentinel before the guard and costs two bytes. The ROM's
 * `movs r2, #1; rsbs r2, r2, #0` then `cmp r4, r2` IS that sentinel, held in a
 * register of its own because the counter is a second variable rather than the
 * parameter.
 *
 * All this adds to the sibling is the early exit, and the returned value is the
 * SECOND parameter's pre-increment value (`subs r0, r3, #1`) -- a pointer to
 * the first mismatching byte, NULL when every byte agrees. Both loads happen
 * before both increments and `dst` is loaded first, which is what fixes the
 * comparison's operand order.
 *
 * VerifyFlashSector and VerifyFlashSectorNBytes relocate this function's machine code onto the
 * stack and call it there, so its address is a pool word in both -- see the
 * "Self-relocating code" chapter in docs/agbcc-codegen.md. */
u8 *VerifyFlashSector_Core(const u8 *src, u8 *dst, int n)
{
    int i;

    i = n - 1;

    if (n != 0)
    {
        do
        {
            if (*dst++ != *src++)
                return dst - 1;
        } while (i-- != 0);
    }

    return NULL;
}
asm(".global sub_0808AED0\n.thumb_set sub_0808AED0, VerifyFlashSector_Core\n");
