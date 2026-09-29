#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080408A0.
 * sub_080408A0 @ 0x080408A0
 */

#include "hardware.h"
#include "proc.h"

/* The cursor-confirm loop for the gUnknown_0849FC64 script the matched
 * SiloTargetSelect_Init installs: A commits the picked square, B restores the saved
 * cursor and restarts the script.
 *
 * gpKeySt->pressed is NAMED TWICE rather than bound to a local. CSE collapses the
 * two reads to the single `ldrh r1,[r0,#4]` the ROM has either way, but the
 * liveness differs: with a local the value is dead after the second mask and
 * agbcc reuses its register as the AND's destination (`ands r1,r0`), where the
 * ROM keeps the mask in the constant's register (`ands r0,r1`). Worth 2 bytes,
 * and the only difference between 99.0% and the match.
 *
 * The A-branch reads gUnknown_030033E4's two halves as u8 for SendActionCommand's
 * declared u8 third and fourth parameters (`ldrb`), while the B-branch reads
 * the same two as s16 for ScrollCameraToKeepCellInView (`ldrsh`) -- the widths come from the
 * callees, not from the object, exactly as in the matched InventionAttack_CommitMove. */
void SiloTargetSelect_Loop(ProcPtr proc)
{
    if (sub_08019260() != 0)
        return;

    HandleMoveMapCursor();
    HandleGameMapCursorInput();
    HandleMoveCameraWithMapCursor(4);
    StepMapCursorAndDrawTwo(2, 0x12);

    if (IsMapCursorSettled() == 0)
        return;

    if (gpKeySt->pressed & 1)
    {
        CommitSiloLaunchMove(proc);
        StartSiloFire(gUnknown_03003100.pos.unk00, gUnknown_03003100.pos.unk02,
                     gUnknown_030033E4.unk00, gUnknown_030033E4.unk02, proc);

        if (gPlaySt.savingEnabled != 0)
            SendActionCommand(0x14, gUnknown_03003F38,
                         gUnknown_030033E4.unk00, gUnknown_030033E4.unk02);

        Proc_Break(proc);
    }
    else if (gpKeySt->pressed & 2)
    {
        gUnknown_030033E4.unk00 = gUnknown_03003100.pos.unk00;
        gUnknown_030033E4.unk02 = gUnknown_03003100.pos.unk02;

        ScrollCameraToKeepCellInView(gUnknown_030033E4.unk00, gUnknown_030033E4.unk02);

        Proc_Goto(proc, 0);
        Proc_Break(proc);
    }
}
asm(".global sub_080408A0\n.thumb_set sub_080408A0, SiloTargetSelect_Loop\n");
