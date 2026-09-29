#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808AE30.
 * sub_0808AE30 @ 0x0808AE30
 */

void ReadFlash_Core(const u8 *src, u8 *dst, int n)
{
    int i;

    /* Not `while (n--)`. That spelling decrements the parameter in place, so
     * the -1 sentinel is materialised ahead of the entry guard and the guard
     * compares against it rather than against zero -- correct, but two bytes
     * longer. The ROM guards on the untouched `n` and carries the counter in
     * a register of its own, which needs the count split out first. */
    i = n - 1;

    if (n != 0)
    {
        do
        {
            *dst++ = *src++;
        } while (i-- != 0);
    }
}
asm(".global sub_0808AE30\n.thumb_set sub_0808AE30, ReadFlash_Core\n");
