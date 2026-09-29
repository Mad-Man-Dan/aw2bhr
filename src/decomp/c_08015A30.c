#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015A30.
 * sub_08015A30 @ 0x08015A30
 */

/* The gUnknown_03001470 script interpreter: five guards, then run commands
 * through the gUnknown_0848A160 opcode table until one reports 0.
 *
 * The `.unk10` guard is one `&&`, not a nested `if`: the ROM tests the member,
 * skips FORWARD past the decrement when it is 0, and returns when the
 * decremented value is non-zero -- which is exactly a short-circuit whose two
 * exits go to different places. The decremented value is tested without a
 * reload (`lsls r0,r0,#0x10` on the register that was just stored), so the
 * decrement is an expression and not a statement of its own.
 *
 * `.unk00` is tested TWICE, once at the top and once after the `.unk08` guard.
 * That is real: agbcc keeps the second `ldr r0,[r1]` even though nothing
 * between them writes memory, so the source has both tests.
 */
struct Unk08015A30Cmd
{
    /* 0x00 */ u8 filler_00[0x06];
    /* 0x06 */ u16 unk06;
};

void StepSlotScript(u8 a)
{
    if (gUnknown_03001470[a].unk00 == 0)
        return;

    if (gUnknown_03001470[a].unk10 != 0 && --gUnknown_03001470[a].unk10 != 0)
        return;

    if (gUnknown_03001470[a].unk08 != 0)
        return;

    if (gUnknown_03001470[a].unk00 == 0)
        return;

    if (gUnknown_03001470[a].unk04 == NULL)
        return;

    while (gUnknown_0848A160[
        ((const struct Unk08015A30Cmd *)gUnknown_03001470[a].unk04)->unk06](a))
        ;
}
asm(".global sub_08015A30\n.thumb_set sub_08015A30, StepSlotScript\n");
