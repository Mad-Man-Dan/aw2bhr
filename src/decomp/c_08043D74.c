#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08043D74.
 * sub_08043D74 @ 0x08043D74
 */

/* Clamps the sprite/terrain id 11 down to 10 and passes everything else
 * through -- and it does so through a MISSING `return`, not a second one.
 *
 * The function is `int` and only the 0xa path executes a `return` statement.
 * The pass-through works by accident of the ABI: `a` is still sitting in r0
 * where the caller left it, so falling off the end returns it.  Every spelling
 * with an explicit `return a;` costs an instruction -- gcc then needs a real
 * pseudo for `a`, which loses r0 to the (u8) temp and pays `adds r0, r1, #0`
 * to put the value back.  Same shape as PlayArmyCoMusic in this block, which is
 * also implicit-int with no return statement at all.
 */
int sub_08043D74(int a)
{
    if ((u8)a == 0xb)
        return 0xa;
}
