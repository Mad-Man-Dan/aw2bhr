#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803FECC.
 * sub_0803FECC @ 0x0803FECC, sub_0803FEDC @ 0x0803FEDC, sub_0803FF04 @ 0x0803FF04
 */

#include "proc.h"

/* `adds r3, r2, #0; movs r2, #0` -- the wrapper's own third argument slides
 * into the fourth slot and a literal 0 takes the third. StartBlackCannonExplosion and
 * StartDeathRayExplosion beside it pass -1 and -2 in the same slot. */
void StartExplosionEffectDefault(int a, int b, ProcPtr parent)
{
    StartExplosionEffect(a, b, 0, parent);
}
asm(".global sub_0803FECC\n.thumb_set sub_0803FECC, StartExplosionEffectDefault\n");

/* Two statements and not a nested call: Proc_Start's result is dead (r0 is
 * overwritten by `adds r0, r4, #0` before the second bl) and StartExplosionEffect's
 * fourth parameter is this function's own third argument, held in r6.
 * `movs r2, #1; rsbs r2, r2, #0` is the literal -1. */
void StartBlackCannonExplosion(int a, int b, ProcPtr parent)
{
    Proc_Start(gUnknown_0849FADC, PROC_TREE_3);
    StartExplosionEffect(a, b, -1, parent);
}
asm(".global sub_0803FEDC\n.thumb_set sub_0803FEDC, StartBlackCannonExplosion\n");

/* The twin of StartBlackCannonExplosion, differing only in the literal (-2 instead of -1)
 * that reaches StartExplosionEffect's third parameter. */
void StartDeathRayExplosion(int a, int b, ProcPtr parent)
{
    Proc_Start(gUnknown_0849FADC, PROC_TREE_3);
    StartExplosionEffect(a, b, -2, parent);
}
asm(".global sub_0803FF04\n.thumb_set sub_0803FF04, StartDeathRayExplosion\n");
