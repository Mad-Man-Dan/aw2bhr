#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801A104.
 * sub_0801A104 @ 0x0801A104, sub_0801A148 @ 0x0801A148
 */

/* Returns CreateRootMenu's result -- the epilogue is `pop {r1}; bx r1`, which is
 * the tree's established returns-a-value discriminator (see the sub_08019F2C
 * note in include/unknown-functions.h). The header declared this `void`; that
 * was the whole remaining 2-byte diff and the declaration has been corrected.
 *
 * The three `lsls #0x10; lsrs #0x10` pairs are PROMOTE_MODE on the declared
 * `u16` parameters. sub_0801A604 is NULLARY -- r0 still holds a1 at that `bl`
 * only because nothing has clobbered it. */
int CreateRootMenuWithSfx(const void *a1, u16 a2, u16 a3, u16 a4)
{
    sub_0801A604();
    PlayMusicOrSfx2(0x65);
    return CreateRootMenu(a1, a2, a3, a4, 0);
}
asm(".global sub_0801A104\n.thumb_set sub_0801A104, CreateRootMenuWithSfx\n");

/* A pure forwarder to sub_08019F2C with a literal 0 for the fifth argument, and
 * it RETURNS that call's result: the epilogue is `pop {r1}; bx r1`. Declared
 * `void` before this wave, which cost exactly the two epilogue bytes. */
int CreateSubMenu(const void *a1, u16 a2, u16 a3, u16 a4)
{
    return sub_08019F2C(a1, a2, a3, a4, 0);
}
asm(".global sub_0801A148\n.thumb_set sub_0801A148, CreateSubMenu\n");
