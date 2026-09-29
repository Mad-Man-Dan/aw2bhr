#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08030D1C.
 * sub_08030D1C @ 0x08030D1C, sub_08030D4C @ 0x08030D4C
 */

/* Poll the link state machine, park the answer, and report whether it is still
 * running. The field is re-READ for the test rather than the returned value
 * being reused, which is what volatile costs and what makes this two statements
 * rather than one.
 *
 * The `lsls #0x18; asrs #0x18` before the compare is unk06's own s8 width, not
 * a narrowing of SioPollingMsg's int return -- the `strb` above it truncates and
 * needs no shifts. */
int LinkPollSelfId(void)
{
    gUnknown_0849B018->unk06 = SioPollingMsg();

    if (gUnknown_0849B018->unk06 == -1)
        return 0;

    return 1;
}
asm(".global sub_08030D1C\n.thumb_set sub_08030D1C, LinkPollSelfId\n");

/* SioSend16 takes the ADDRESS of gGameClock, not its value: the ROM
 * loads the pool word straight into r0 with no `ldr r0, [r0]` after it.
 *
 * Its second argument is the literal 1 and costs no instruction -- r1 is still
 * holding the 1 that the `gPlaySt.unk32 = 1` store put there, and
 * agbcc reuses it rather than re-materialising the constant. Dropping the
 * argument would compile too, and identically, which is exactly why the
 * declared arity is what settles it. */
void LinkMarkConnected(void)
{
    gUnknown_0849B018->unk04 = 5;
    gPlaySt.savingEnabled = 1;
    SioSend16((u16 *)&gGameClock, 1);
    ClearSlotScriptCallback(gUnknown_03001FBC);
}
asm(".global sub_08030D4C\n.thumb_set sub_08030D4C, LinkMarkConnected\n");
