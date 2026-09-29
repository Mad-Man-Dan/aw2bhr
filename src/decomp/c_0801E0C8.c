#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801E0C8.
 * sub_0801E0C8 @ 0x0801E0C8
 */

/* Hides `n` OAM entries starting at object `a`: attr0 = 0xa0 (OBJ disabled),
 * attr1 = attr2 = 0, skipping the affine halfword -- hence the 2/2/4 walk.
 *
 * The base MUST be advanced in a statement of its own (`p = g; p += a * 4;`).
 * Written as one expression the address add lands in the index's register,
 * which costs the parameter `n` its register and adds a copy plus a
 * `push {r4, lr}`. This is the same "`p += C` in a separate statement escapes
 * the fold" split recorded in docs/agbcc-codegen.md, and it applies to a
 * variable index too. */
void HideOamObjects(int a, int n)
{
    u16 *p;
    int i;

    p = gUnknown_03002520;
    p += a * 4;

    for (i = 0; i < n; i++)
    {
        *p = 0xa0;
        p++;
        *p = 0;
        p++;
        *p = 0;
        p += 2;
    }
}
asm(".global sub_0801E0C8\n.thumb_set sub_0801E0C8, HideOamObjects\n");
