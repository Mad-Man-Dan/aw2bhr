#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B0D8.
 * sub_0803B0D8 @ 0x0803B0D8
 */

/* F002: `push {lr}; ldr r0,=g; movs r1,#0; bl sub_080152EC; pop {r0}; bx r0`.
 * One statement, result discarded -- `pop {r0}` makes the wrapper void.
 * sub_080152EC's first parameter is `const void *`, which takes the
 * `const u8 []` blob with no cast; a plain `void *` would discard the const and
 * -Werror turns that into an error. src/decomp/c_0802BF80.c is the matched
 * exemplar for this shape. */
void StartDebugBackupUtility(void)
{
    sub_080152EC(gUnknown_0849E648, 0);
}
asm(".global sub_0803B0D8\n.thumb_set sub_0803B0D8, StartDebugBackupUtility\n");
