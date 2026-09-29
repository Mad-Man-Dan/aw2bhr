#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08076B7C.
 * sub_08076B7C @ 0x08076B7C, sub_08076BC4 @ 0x08076BC4, sub_08076BE0 @ 0x08076BE0, sub_08076BF0 @ 0x08076BF0, sub_08076C1C @ 0x08076C1C, sub_08076C64 @ 0x08076C64, sub_08076C8C @ 0x08076C8C
 */

#include "proc.h"

/* SetupWorldMapForResume's twin without the proc tail, plus one guarded extra step.
 * The proc is taken and forwarded to SetupWorldMapScreen and nothing else, which costs
 * zero instructions -- see the SetupWorldMapScreen note in unknown-functions.h for why
 * wave 34 gave that callee a parameter. */
void SetupWorldMapAfterMission(ProcPtr proc)
{
    SetupWorldMapScreen(proc);
    Decompress(gUnknown_081D0BAC, gUnknown_08614280);

    if (gUnknown_0202FDFC.unk11 != 0)
        sub_08076B20();

    RestoreWorldMapMarkers2();
    PaintClearedWorldMapSections();
    RegisterDataMove(gUnknown_08614280, (void *)0x0600F000, 0x1000);
}
asm(".global sub_08076B7C\n.thumb_set sub_08076B7C, SetupWorldMapAfterMission\n");

/* A five-argument forwarder: the fifth goes on the stack, which is what the
 * `sub sp, #4` / `str r0, [sp]` frame is -- not a local. */
void StartWorldMapIntroScene(ProcPtr parent)
{
    StartWorldMapScene(0, 0x78, 0, gUnknown_084BA6D0, parent);
}
asm(".global sub_08076BC4\n.thumb_set sub_08076BC4, StartWorldMapIntroScene\n");

/* A pass-through wrapper: r0 is never written, so the proc arrives and is
 * forwarded unchanged and costs zero instructions -- the arity is read off the
 * callee, not off this body. StartWorldMapCameraPan returns s32 and the result is
 * dropped (`pop {r0}; bx r0`). */
void PanWorldMapCameraToStartCorner(ProcPtr proc)
{
    StartWorldMapCameraPan(proc, 0, 0xAF, 1);
}
asm(".global sub_08076BE0\n.thumb_set sub_08076BE0, PanWorldMapCameraToStartCorner\n");

/* The table read happens BEFORE the guard in the ROM -- a local bound outside
 * the `if`, the same shape as sub_0806366C. `lsls #3` is the 8-byte record
 * stride of gUnknown_0861500C. */
void StartWorldMapReturnIfMissionWon(ProcPtr parent)
{
    void *p;

    p = gUnknown_0861500C[gUnknown_0202FDFC.unk0c].unk_04;

    if (gUnknown_0202FDFC.unk11 == 1)
        StartWorldMapReturn(p, parent);
}
asm(".global sub_08076BF0\n.thumb_set sub_08076BF0, StartWorldMapReturnIfMissionWon\n");

/* `lsls #1; adds; lsls #4` is a MULTIPLY by 3 << 4 == 0x30, i.e. an index into
 * an array of 0x30-byte structs -- which is exactly sizeof(struct Unk08615194)
 * -- rather than hand-rolled address arithmetic. Both arms compute an address
 * and agbcc tail-merges the single `ldr` that follows the join. */
void StartWorldMapAfterMissionScript(ProcPtr parent)
{
    void *p;

    if (gUnknown_0202FDFC.unk11 == 0)
        p = gUnknown_08615194[gUnknown_0202FDFC.unk0c].unk1c;
    else
        p = gUnknown_08615194[gUnknown_0202FDFC.unk0c].unk18;

    if (p != NULL)
        StartBlockingEventScript(p, parent);
}
asm(".global sub_08076C1C\n.thumb_set sub_08076C1C, StartWorldMapAfterMissionScript\n");

/* StartWorldMapIntroScene behind a guard, with a different (x, y). */
void StartWorldMapHardModeIntroScene(ProcPtr parent)
{
    if (IsHardCampaignMode())
        StartWorldMapScene(0x50, 0x70, 0, gUnknown_084BA6D0, parent);
}
asm(".global sub_08076C64\n.thumb_set sub_08076C64, StartWorldMapHardModeIntroScene\n");

/* Two starters under the caller's own proc. */
void StartWorldMapCursorProcs(ProcPtr proc)
{
    StartWorldMapScope(NULL, proc);
    StartWorldMapSelectionFrame(0, 0, 0, proc);
}
asm(".global sub_08076C8C\n.thumb_set sub_08076C8C, StartWorldMapCursorProcs\n");
