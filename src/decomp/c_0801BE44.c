#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BE44.
 * sub_0801BE44 @ 0x0801BE44
 */

/* Four halfwords of one 16-entry OAM group. `a * 16` is one `lsls #4` shared
 * by all four subscripts; the fifth argument arrives on the stack and is
 * loaded with a plain `ldr`, which is what makes it a word-sized parameter
 * rather than a u16. */
void PutObjectAffine(int a, int b, int c, int d, int e)
{
    gUnknown_03002520[a * 16 + 3] = b;
    gUnknown_03002520[a * 16 + 7] = c;
    gUnknown_03002520[a * 16 + 11] = d;
    gUnknown_03002520[a * 16 + 15] = e;
}
asm(".global sub_0801BE44\n.thumb_set sub_0801BE44, PutObjectAffine\n");
