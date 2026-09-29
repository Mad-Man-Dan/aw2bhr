#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080116A0.
 * sub_080116A0 @ 0x080116A0
 */

#include "proc.h"

/* `FadeExists` in the Fire Emblem decomps: is any of the four fade procs
 * running. The four scripts are the ones src/decomp/c_08011610.c starts.
 *
 * SEPARATE ifs, not a `||` chain, and the two are distinguishable here -- this
 * is the mirror image of the case src/decomp/c_0802C550.c works out. Four
 * separate `if (...) return TRUE;` merge their TRUE arms and move the merged
 * block PAST the fallthrough FALSE, which puts the literal pool between the
 * `movs #0` and the `movs #1`. That is exactly the ROM's order. The `||`
 * inside one `if` produces the opposite arrangement. */
bool8 FadeExists(void)
{
    if (Proc_Find(ProcScr_FadeLoadMap))
        return TRUE;

    if (Proc_Find(ProcScr_SomeFade))
        return TRUE;

    if (Proc_Find(ProcScr_DesignRoomFadeIn))
        return TRUE;

    if (Proc_Find(gUnknown_0848923C))
        return TRUE;

    return FALSE;
}
asm(".global sub_080116A0\n.thumb_set sub_080116A0, FadeExists\n");
