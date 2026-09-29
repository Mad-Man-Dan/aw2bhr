#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080424FC.
 * sub_080424FC @ 0x080424FC, sub_0804256C @ 0x0804256C
 */

/* CommitUnitMoveBare with three extra calls and the gPlaySt.unk0d gate.
 * SubtractUnitFuel's parameter is declared `struct Unit *` and the argument
 * is gUnknown_030040D8, so the cast is unavoidable here -- the two struct tags
 * describe the same object and unknown-globals.h records why they are kept
 * apart (retyping the global would turn c_080424BC.c's `->unk05 &= 7` into the
 * bitfield spelling). */
void CommitUnitMove(void)
{
    ResetCaptureProgressIfMoved();
    sub_080176A4();
    gUnknown_030040D8->unk02 = gUnknown_03003100.pos.unk00;
    gUnknown_030040D8->unk03 = gUnknown_03003100.pos.unk02;

    if (gPlaySt.fog == 0)
        SubtractUnitFuel((struct Unit *)gUnknown_030040D8, gUnknown_03004074);

    EndActiveMoveSlide();
    gUnknown_030040D8->unk01 |= 1;

    if (gUnknown_030040D8->unk01 & 8)
        gUnknown_030040D8->unk01 |= 2;

    RebuildMapUnitLayers();
    RunMapEventsAfterUnitAction(gUnknown_030040D8);
    sub_080198D0();
}
asm(".global sub_080424FC\n.thumb_set sub_080424FC, CommitUnitMove\n");

/* The ROM's `movs r5, #0` here is DEAD -- nothing reads r5, and r5 is pushed
 * only to hold it. It is not a tell for a missing statement: this straight
 * transcription reproduces it, so it is just agbcc materialising a constant
 * whose consumer was folded away.
 *
 * gUnknown_030040D8 is re-loaded before each store because the `strb` through
 * it can alias the pointer global itself -- the same reason c_080425B8.c
 * records. Within the final `if` the two reads share one `ldrb`, since no
 * store separates the test from the OR. */
void CommitUnitMoveBare(void)
{
    ResetCaptureProgressIfMoved();
    sub_080176A4();
    gUnknown_030040D8->unk02 = gUnknown_03003100.pos.unk00;
    gUnknown_030040D8->unk03 = gUnknown_03003100.pos.unk02;
    EndActiveMoveSlide();
    gUnknown_030040D8->unk01 |= 1;

    if (gUnknown_030040D8->unk01 & 8)
        gUnknown_030040D8->unk01 |= 2;
}
asm(".global sub_0804256C\n.thumb_set sub_0804256C, CommitUnitMoveBare\n");
