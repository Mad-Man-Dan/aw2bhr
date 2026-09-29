#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019404.
 * sub_08019404 @ 0x08019404, sub_08019470 @ 0x08019470
 */

#include "hardware.h"

/*
 * StepEventScriptSlot -- run one gUnknown_0200C528 slot's script for this frame.
 *
 * Gives up if the slot holds no script, if a callback is installed, or if the
 * cursor is NULL. A nonzero delay counter is decremented here, one per frame,
 * and the slot waits until it reaches 0.
 *
 * Otherwise this is the dispatcher for the script commands: byte 0 of the
 * current node indexes the handler table gUnknown_0848A244, the handler is
 * called with the slot index, and handlers keep being called for as long as one
 * returns nonzero -- a handler returning 0 ends the slot's turn for this frame.
 * Each handler moves the cursor itself. include/unknown-functions.h lists the
 * family beside EventOp_CallFunction.
 *
 * Why the C looks odd: the loop re-reads `gUnknown_0200C528[a].unk04` every
 * pass, which it has to, because the handlers move it. The slot's .unk00 is
 * tested twice, before and after the callback test, as the original does; the
 * store to .unk0c in between is what stops the compiler merging the two reads.
 */
void StepEventScriptSlot(s16 a)
{
    if (gUnknown_0200C528[a].unk00 == NULL)
        return;
    if (gUnknown_0200C528[a].unk0c != 0)
    {
        gUnknown_0200C528[a].unk0c--;
        if (gUnknown_0200C528[a].unk0c != 0)
            return;
    }
    if (gUnknown_0200C528[a].unk08 != NULL)
        return;
    if (gUnknown_0200C528[a].unk00 == NULL)
        return;
    if (gUnknown_0200C528[a].unk04 == NULL)
        return;
    while (gUnknown_0848A244[gUnknown_0200C528[a].unk04->filler_00[0]](a) != 0)
        ;
}
asm(".global sub_08019404\n.thumb_set sub_08019404, StepEventScriptSlot\n");

/*
 * RunEventScripts -- run the whole gUnknown_0200C528 script system for one frame.
 *
 * Does nothing while AreEventScriptsPaused reports the system busy. Otherwise
 * gUnknown_03002EF0 is cleared and each of the ten slots that holds a script
 * gets its callback run, if it has one, and then its script stepped by
 * StepEventScriptSlot above. Afterwards, when gUnknown_03001404 is set, all four key
 * words in gpKeySt are forced to gUnknown_03002EF0 and two more globals are
 * cleared -- so a running script can feed a button state to the rest of the
 * game by writing that global.
 *
 * The callback field is declared as a node pointer, so calling it needs the
 * cast to a function pointer.
 *
 * Why the C looks odd: the loop counter is an `s16`, so the compiler carries it
 * pre-shifted and sign-extends it at each use; an int counter tidies that up
 * and the output no longer matches. gUnknown_03001404 has to stay s16 for the
 * same reason -- the original reads it as a signed halfword, and a `(s16)` cast
 * on a u16 global would fold away at a zero test.
 */
void RunEventScripts(void)
{
    s16 i;

    if (AreEventScriptsPaused() != 0)
        return;

    gUnknown_03002EF0 = 0;

    for (i = 0; i < 10; i++)
    {
        if (gUnknown_0200C528[i].unk00 != NULL)
        {
            if (gUnknown_0200C528[i].unk08 != NULL)
                ((void (*)(struct Unk0200C528 *))gUnknown_0200C528[i].unk08)(&gUnknown_0200C528[i]);
            StepEventScriptSlot(i);
        }
    }

    if (gUnknown_03001404 != 0)
    {
        gpKeySt->repeated = gUnknown_03002EF0;
        gpKeySt->pressed = gUnknown_03002EF0;
        gpKeySt->held = gUnknown_03002EF0;
        gpKeySt->previous = gUnknown_03002EF0;
        gUnknown_03004538 = 0;
        gUnknown_03004518 = 0;
    }
}
asm(".global sub_08019470\n.thumb_set sub_08019470, RunEventScripts\n");
