#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08084804.
 * sub_08084804 @ 0x08084804
 */

/* Runs the six nullary predicates in gUnknown_08616B00, records each verdict as
 * a byte in gUnknown_03000650 and, when exactly five of the six passed, sets
 * gUnknown_03000650[4] as well. (Five out of six, not all six: the loop makes
 * six calls -- `movs r5, #5` counting down inclusive of zero -- while the tail
 * test is `cmp r7, #5; bne`. Reproduced, not fixed.)
 *
 * Three things here are readable only backwards from the assembly:
 *   - The `movs r5,#5 ... subs; cmp #0; bge` countdown is check_dbra_loop's
 *     reversal of an ascending source loop, not a descending one: `i` survives
 *     only in the exit test once the two givs (the byte pointer and the 4-byte
 *     table pointer) are created, and unlike this block's other predicates the
 *     loop has a single exit, which is what lets the reversal fire.
 *   - The `else` arm stores r0 rather than reloading a zero because cse knows
 *     the register is zero on that edge; the two arms are not cross-jumped
 *     because the `then` tail carries the `adds r7, #1` after its store.
 *   - `ldr r0, .LC; mov r8, r0; ldr r4, [r0]` is agbcc's own -fforce-addr
 *     address pool (the ROM word the splitter calls gUnknown_081D93D8, which
 *     dereferences to 0x03000650), triggered by &gUnknown_03000650 being
 *     referenced on both sides of the loop's exit merge. Naming the global
 *     directly is the original spelling and the build places the word. */

void MainMenuCarousel_UpdateCompletionFlags(void)
{
    int i;
    int n;

    n = 0;
    for (i = 0; i <= 5; i++)
    {
        if (gUnknown_08616B00[i]())
        {
            gUnknown_03000650[i] = 1;
            n++;
        }
        else
        {
            gUnknown_03000650[i] = 0;
        }
    }
    if (n == 5)
        gUnknown_03000650[4] = 1;
}
asm(".global sub_08084804\n.thumb_set sub_08084804, MainMenuCarousel_UpdateCompletionFlags\n");
