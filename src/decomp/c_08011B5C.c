#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011B5C.
 * sub_08011B5C @ 0x08011B5C
 */

/* The remove half of the AddVBlankHook / RemoveVBlankHook pair on list B
 * (gUnknown_03000000, 16 pointer slots): finds the entry and zeroes its slot.
 *
 * The ROM reaches the array through TWO loads -- `ldr r0, =X; ldr r4, [r0]` --
 * and the word at X (0x0808E518) holds 0x03000000. That is agbcc's own
 * -fforce-addr copy of gUnknown_03000000's address, not a pointer global: the
 * array is referenced four times here (read and write, in the peeled first
 * iteration and in the rotated loop), which is the threshold at which the
 * address constant gets its own .rodata word. The store through the array can
 * alias that word, so it is re-loaded at the top of every iteration.
 *
 * AddVBlankHook, which references the same array only twice, gets the plain
 * `ldr rN, =gUnknown_03000000` instead. */
void RemoveVBlankHook(void *a)
{
    u8 i;

    for (i = 0; i < 16; i++)
    {
        if (gUnknown_03000000[i] == a)
        {
            gUnknown_03000000[i] = 0;
            break;
        }
    }
}
asm(".global sub_08011B5C\n.thumb_set sub_08011B5C, RemoveVBlankHook\n");
