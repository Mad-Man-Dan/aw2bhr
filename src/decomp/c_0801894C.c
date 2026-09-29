#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801894C.
 * sub_0801894C @ 0x0801894C, sub_08018984 @ 0x08018984, sub_080189B8 @ 0x080189B8, sub_080189EC @ 0x080189EC, sub_08018A28 @ 0x08018A28, sub_08018A64 @ 0x08018A64, sub_08018AA8 @ 0x08018AA8, sub_08018ADC @ 0x08018ADC
 */

bool8 EventOp_ApplyCurrentArmyFramePalette(s16 a)
{
    ApplyArmyWindowFramePalette(gUnknown_030033EC);
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_0801894C\n.thumb_set sub_0801894C, EventOp_ApplyCurrentArmyFramePalette\n");

/* The arms are written INVERTED relative to the ROM's block order, the
 * c_08017CF0.c rule: when both arms of an if/else end in `return`, agbcc emits
 * the ELSE inline and branches to the THEN. Writing it the natural way round --
 * `if (g == 0xc) { advance; return TRUE; } return FALSE;` -- puts `movs r0, #0`
 * inline and the advance after the literal pool, which is the mirror image of
 * the ROM (measured with compile_probe). */
bool8 EventOp_WaitUntilControllerDispatch(s16 a)
{
    if (gUnknown_030032D8 != 0xc)
        return FALSE;
    else
    {
        gUnknown_0200C528[a].unk04++;
        return TRUE;
    }
}
asm(".global sub_08018984\n.thumb_set sub_08018984, EventOp_WaitUntilControllerDispatch\n");

/* EventOp_WaitUntilControllerDispatch with the predicate inverted; same inverted-arm spelling. */
bool8 EventOp_WaitWhileControllerDispatch(s16 a)
{
    if (gUnknown_030032D8 == 0xc)
        return FALSE;
    else
    {
        gUnknown_0200C528[a].unk04++;
        return TRUE;
    }
}
asm(".global sub_080189B8\n.thumb_set sub_080189B8, EventOp_WaitWhileControllerDispatch\n");

/* EventOp_CallFunction's shape with a different callback signature: the node's +0x04
 * is the function and its +0x0c the single argument, rather than the slot.
 * `bl _call_via_r1` is the ordinary one-argument indirect trampoline.
 *
 * The callback must be bound to a local first -- that is what loads the
 * function pointer BEFORE the argument, which is the order the ROM has; written
 * inline agbcc evaluates the argument first (the c_08017A80.c note). */
s16 EventOp_CallFunctionWithArg(s16 a)
{
    void (*f)(u32);

    f = (void (*)(u32))gUnknown_0200C528[a].unk04->unk04;
    f(gUnknown_0200C528[a].unk04->unk0c);
    gUnknown_0200C528[a].unk04++;

    if (gUnknown_0200C528[a].unk00 != NULL)
        return TRUE;
    else
        return FALSE;
}
asm(".global sub_080189EC\n.thumb_set sub_080189EC, EventOp_CallFunctionWithArg\n");

/* The callback EventOp_WaitForDayAndArmyTurn installs, and the other half of the install/remove
 * pair that fixes the parameter as `struct Unk0200C528 *`: the `str` is at +8
 * of the 0x18-byte SLOT, and +0x11 / +0x12 are the two members EventOp_WaitForDayAndArmyTurn
 * had just written there.
 *
 * `movs r1, #0x12; ldrsh r0, [r2, r1]` is not a register-offset idiom worth
 * modelling -- `ldrsh` simply has no immediate-offset form, and the scratch
 * register it borrows differs (r1, then r3) between the two reads of the SAME
 * member four instructions apart. What it does prove is that unk12 is signed.
 *
 * The `||` is a real short-circuit: the second operand re-reads unk12 rather
 * than reusing the first read. */
void EventCb_ClearOnDayAndArmyTurn(struct Unk0200C528 *slot)
{
    if (gUnknown_030032D8 == 0xc)
        if (slot->unk12 < 0 || gUnknown_03004080 == slot->unk12)
            if (gUnknown_030033EC == slot->unk11)
                slot->unk08 = NULL;
}
asm(".global sub_08018A28\n.thumb_set sub_08018A28, EventCb_ClearOnDayAndArmyTurn\n");

/* Copies the node's unk08/unk0a into the SLOT's unk11/unk12, installs
 * EventCb_ClearOnDayAndArmyTurn as the slot's callback and then calls it immediately -- the
 * install/remove pair whose remove half is EventCb_ClearOnDayAndArmyTurn.
 *
 * The call is INDIRECT in the ROM (`bl _call_via_r2`) even though the target is
 * a known constant, so the source has to reach it back through the slot: CSE
 * keeps the value it just stored in r2 rather than reloading it. Reading the
 * member back is what produces that; naming EventCb_ClearOnDayAndArmyTurn directly would emit a
 * plain `bl`. The +8 slot is `struct Unk0200C528Node *` for EventOp_InstallCallback's
 * sake, so the function goes in cast, the sub_08017B08 precedent. */
bool8 EventOp_WaitForDayAndArmyTurn(s16 a)
{
    gUnknown_0200C528[a].unk11 = gUnknown_0200C528[a].unk04->unk08;
    gUnknown_0200C528[a].unk12 = gUnknown_0200C528[a].unk04->unk0a;
    gUnknown_0200C528[a].unk08 = (struct Unk0200C528Node *)EventCb_ClearOnDayAndArmyTurn;
    ((void (*)(struct Unk0200C528 *))gUnknown_0200C528[a].unk08)(&gUnknown_0200C528[a]);
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}
asm(".global sub_08018A64\n.thumb_set sub_08018A64, EventOp_WaitForDayAndArmyTurn\n");

/* EventCb_ClearOnDayAndArmyTurn's twin: the same install/remove callback shape, keyed on
 * IsCoPowerActive(slot->unk11) instead of the unk12 comparison. */
void EventCb_ClearWhenCoPowerActiveOnArmy1Turn(struct Unk0200C528 *slot)
{
    if (gUnknown_030032D8 == 0xc)
        if (IsCoPowerActive(slot->unk11))
            if (gUnknown_030033EC == 1)
                slot->unk08 = NULL;
}
asm(".global sub_08018AA8\n.thumb_set sub_08018AA8, EventCb_ClearWhenCoPowerActiveOnArmy1Turn\n");

/* EventOp_WaitForDayAndArmyTurn without the unk12 copy, installing EventCb_ClearWhenCoPowerActiveOnArmy1Turn instead. */
bool8 EventOp_WaitForCoPowerActiveOnArmy1Turn(s16 a)
{
    gUnknown_0200C528[a].unk11 = gUnknown_0200C528[a].unk04->unk08;
    gUnknown_0200C528[a].unk08 = (struct Unk0200C528Node *)EventCb_ClearWhenCoPowerActiveOnArmy1Turn;
    ((void (*)(struct Unk0200C528 *))gUnknown_0200C528[a].unk08)(&gUnknown_0200C528[a]);
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}
asm(".global sub_08018ADC\n.thumb_set sub_08018ADC, EventOp_WaitForCoPowerActiveOnArmy1Turn\n");
