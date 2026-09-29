#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074384.
 * sub_08074384 @ 0x08074384, sub_080743B8 @ 0x080743B8, sub_080743E8 @ 0x080743E8, sub_08074410 @ 0x08074410, sub_0807443C @ 0x0807443C, sub_08074460 @ 0x08074460
 */

#include "proc.h"

void UnitSelectedEvent_Init(void)
{
    gUnknown_03002F08.unk00 = 0xf;
    ApplyWindowFramePalette(gPlayers[gUnknown_030033EC].teamColor - 1, 0xf);
}
asm(".global sub_08074384\n.thumb_set sub_08074384, UnitSelectedEvent_Init\n");

/* The proc pointer is only used for Proc_Break; the `bl sub_08019260` reads r0
 * only because nothing has overwritten it, and sub_08019260 is nullary.
 */
void UnitSelectedEvent_WaitForScripts(ProcPtr proc)
{
    if (!sub_08019260())
    {
        gUnknown_03002F08.unk00 = 8;
        LoadArmyObjPalette(gUnknown_030033EC);
        Proc_Break(proc);
    }
}
asm(".global sub_080743B8\n.thumb_set sub_080743B8, UnitSelectedEvent_WaitForScripts\n");

u8 RunMapEventsAfterUnitAction(struct Unk030040D8 *a)
{
    const struct Unk08074584 *p = GetMapEventTable();

    if (p != 0 && p->unk0c != 0)
        return RunMapEventRecords(p->unk0c, a, 0);

    return 0;
}
asm(".global sub_080743E8\n.thumb_set sub_080743E8, RunMapEventsAfterUnitAction\n");

/* The only member of the group that forwards both of its own parameters, and
 * it forwards them CROSSED: its first argument becomes RunMapEventRecords's third
 * and its second becomes RunMapEventRecords's second.
 */
u8 RunMapEventsForAction(int a, struct Unk030040D8 *b)
{
    const struct Unk08074584 *p = GetMapEventTable();

    if (p != 0 && p->unk10 != 0)
        return RunMapEventRecords(p->unk10, b, a);

    return 0;
}
asm(".global sub_08074410\n.thumb_set sub_08074410, RunMapEventsForAction\n");

u8 RunMapEventsAtMatchEnd(void)
{
    const struct Unk08074584 *p = GetMapEventTable();

    if (p != 0 && p->unk14 != 0)
        return RunMapEventRecords(p->unk14, 0, 0);

    return 0;
}
asm(".global sub_0807443C\n.thumb_set sub_0807443C, RunMapEventsAtMatchEnd\n");

u8 RunMapEventsAtTurnStart(void)
{
    const struct Unk08074584 *p = GetMapEventTable();

    if (p != 0 && p->unk00 != 0)
        return RunMapEventRecords(p->unk00, 0, 0);

    return 0;
}
asm(".global sub_08074460\n.thumb_set sub_08074460, RunMapEventsAtTurnStart\n");
