#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078198.
 * sub_08078198 @ 0x08078198, sub_080781F0 @ 0x080781F0
 */

#include "proc.h"

bool8 IsAnyWorldMapProcRunning(void)
{
    return (Proc_Find(gUnknown_086147FC) != NULL)
         | (Proc_Find(gUnknown_0861485C) != NULL)
         | (Proc_Find(gUnknown_08614894) != NULL)
         | (Proc_Find(gUnknown_08614614) != NULL);
}
asm(".global sub_08078198\n.thumb_set sub_08078198, IsAnyWorldMapProcRunning\n");

/* "Does any of the 42 bytes of gUnknown_0202FDFC.unk12 have bit 1 set?" --
 * the any-of version of WorldMapReturn_CheckRevealCondition's count. 0x2a is exactly unk12's length,
 * which is the corroboration that the array bound in that struct is right.
 *
 * The early `return 1` is a real return, not a flag: `movs r0, #1; b` jumps
 * straight to the epilogue from inside the loop. `gUnknown_0202FE0E` is
 * `&gUnknown_0202FDFC.unk12`; the candidate spells the same address as
 * `gUnknown_0202FDFC+0x12`. */

/* Wave 37, W37-Q2: retyped s32 -> bool8. The body is byte-identical either
 * way (it returns only the literals 1 and 0), so nothing here settled it; the
 * first cross-unit caller does. sub_0803BBD4 re-narrows the result with
 * `lsls r0,#0x18; lsrs r0,#0x18` before its zero test, which is agbcc
 * re-narrowing a byte-returning callee and is what an s32 return cannot
 * produce. Re-verified byte-identical after the change. */
bool8 IsAnyWorldMapMissionCleared(void)
{
    s32 i;

    for (i = 0; i < 0x2a; i++)
    {
        if (gUnknown_0202FDFC.unk12[i] & 2)
            return 1;
    }

    return 0;
}
asm(".global sub_080781F0\n.thumb_set sub_080781F0, IsAnyWorldMapMissionCleared\n");
