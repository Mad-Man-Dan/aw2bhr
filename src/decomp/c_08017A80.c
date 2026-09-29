#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017A80.
 * sub_08017A80 @ 0x08017A80, sub_08017ABC @ 0x08017ABC, sub_08017AD4 @ 0x08017AD4, sub_08017B08 @ 0x08017B08, sub_08017B50 @ 0x08017B50
 */

/* Calls the node's own +0x04 as a one-argument function on the SLOT, advances
 * the cursor, then reports whether the slot is still installed. `bl
 * _call_via_r1` is the ordinary indirect-call trampoline, and the register
 * index counts the arguments -- r1, so one.
 *
 * The callback has to be bound to a local first. Written inline as
 * `((void (*)(struct Unk0200C528 *))g[a].unk04->unk04)(&g[a])`, agbcc computes
 * the argument's address before the function pointer and lands
 * gUnknown_0200C528 in r1 rather than r2, which swaps two instructions.
 *
 * `s16` return: EventOp_CallFunctionSkippable re-narrows the result `lsls #0x10; asrs #0x10`.
 * See the handler-family note in unknown-functions.h. */
s16 EventOp_CallFunction(s16 a)
{
    void (*f)(struct Unk0200C528 *);

    f = (void (*)(struct Unk0200C528 *))gUnknown_0200C528[a].unk04->unk04;
    f(&gUnknown_0200C528[a]);
    gUnknown_0200C528[a].unk04++;

    if (gUnknown_0200C528[a].unk00 != NULL)
        return TRUE;
    else
        return FALSE;
}
asm(".global sub_08017A80\n.thumb_set sub_08017A80, EventOp_CallFunction\n");

/* The callback sub_08017B08 installs: when sub_080281A0's liveness test fails,
 * clear the slot's own callback -- an install/remove pair, which is what fixes
 * the parameter as `struct Unk0200C528 *` rather than a node.
 *
 * sub_080281A0 is nullary. The pointer survives in r0 across the `bl` only
 * because r0 is where the parameter already sat; the callee overwrites r0 with
 * a pool word before any read. The stored 0 is the compare's own zero,
 * reused. */
void sub_08017ABC(struct Unk0200C528 *slot)
{
    if (sub_080281A0() == 0)
        slot->unk08 = NULL;
}

/* The node's +0x04 is the polymorphic script operand this block casts at every
 * use (see the struct Unk0200C528Node note in unknown-globals.h); here it is
 * the u32 key FindMapIdByMapData scans gUnknown_085C77A0 for. */
bool8 EventOp_SetMapIdFromMapData(s16 a)
{
    gPlaySt.mapID = FindMapIdByMapData((u32)gUnknown_0200C528[a].unk04->unk04);
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}
asm(".global sub_08017AD4\n.thumb_set sub_08017AD4, EventOp_SetMapIdFromMapData\n");

/* Installs sub_08017ABC as the slot's callback, then starts the thing that
 * callback will later tear down. The +8 slot is declared `struct
 * Unk0200C528Node *` because EventOp_InstallCallback stores a node link there; this half
 * of the union stores a function, so it is cast rather than retyped. */
bool8 sub_08017B08(s16 a)
{
    gUnknown_0200C528[a].unk08 = (struct Unk0200C528Node *)sub_08017ABC;
    IncrementMapLock();
    sub_080281D8(gPlaySt.mapID,
        (u32)gUnknown_0200C528[a].unk04->unk04);
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}

bool8 EventOp_EndScript(s16 a)
{
    EndEventScriptSlot(a);
    return FALSE;
}
asm(".global sub_08017B50\n.thumb_set sub_08017B50, EventOp_EndScript\n");
