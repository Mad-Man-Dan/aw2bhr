#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015954.
 * sub_08015954 @ 0x08015954, sub_08015994 @ 0x08015994
 */

/* MATCHED.
 *
 * Wake every live gUnknown_03001470 slot, then drain them: set unk12 bit 0 on
 * every slot with a non-zero unk00, and keep running the sorted wake pass
 * until AnySlotScriptPending reports no slot still carries the bit.
 *
 * The tail is a `do`, not a `while`: the ROM's backward `bne` lands on the
 * `bl RunPendingSlotScripts` with no entry test above it.
 *
 * `push {r4, r5, lr}` saves an r5 the body never uses -- that is agbcc's own
 * doing and falls out of this C unchanged; it is not a missing statement.
 */
void RunAllSlotScripts(void)
{
    u8 i;

    for (i = 0; i < 30; i++)
    {
        if (gUnknown_03001470[i].unk00 != 0)
            gUnknown_03001470[i].unk12 |= 1;
    }

    do
    {
        RunPendingSlotScripts();
    } while (AnySlotScriptPending());
}
asm(".global sub_08015954\n.thumb_set sub_08015954, RunAllSlotScripts\n");

/* MATCHED.
 *
 * RunAllSlotScripts's twin: the same wake-and-drain pass, but skipping slots whose
 * unk14 has bit 7 set. The 0x80 mask is hoisted into r5 while the OR's `1`
 * stays inside the loop -- the mirror image of RunAllSlotScripts, where the `1` is
 * the hoisted one. Both fall straight out of the plain spelling; neither is a
 * lever.
 */
void sub_08015994(void)
{
    u8 i;

    for (i = 0; i < 30; i++)
    {
        if (gUnknown_03001470[i].unk00 != 0
            && !(gUnknown_03001470[i].unk14 & 0x80))
            gUnknown_03001470[i].unk12 |= 1;
    }

    do
    {
        RunPendingSlotScripts();
    } while (AnySlotScriptPending());
}
