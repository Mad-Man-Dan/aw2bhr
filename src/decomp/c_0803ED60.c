#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803ED60.
 * sub_0803ED60 @ 0x0803ED60
 */

/* MATCHED (wave 49, W49-C), first attempt. 372/372 bytes, relocs match.
 *
 * The proc step that consumes the gUnknown_02028360 list InventionTurn_ResetCursor
 * (src/decomp/c_0803ED54.c) installs at +0x2c: it walks records until it finds
 * one whose guards pass, emits that one's decoration and RETURNS with the cursor
 * advanced, and only reaches Proc_Goto(proc, 1) when the list runs out. The two
 * `proc->unk2c++` sites in the ROM (_0803EE9C and _0803EEAC) are two separate
 * source statements -- the `break` path and the return path.
 *
 * Case bodies come out in SOURCE order, which here is 1, 5, 2, 3/4; cases 3 and
 * 4 share one body and one jump-table pair of entries.
 *
 * The 4-bit unk02_6 selector is read `ldrh [r,#2]; lsls #0x16; lsrs #0x1c` (bits
 * 6..9 span two bytes, so the extract has to be HImode) while the 2-bit unk02_e
 * is read `ldrb [r,#3]; lsrs #6` (bits 14..15 sit wholly inside byte 3, so
 * get_best_mode narrows it to QImode). Both come straight from struct
 * Unk02028360's existing bitfields -- no new type. The `while` test is the same
 * field and comes out as a bare `movs #0xf0; lsls #2; ands` mask, which is what
 * a bitfield `!= 0` always folds to.
 */

#include "proc.h"
struct Unk3ED60Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ struct Unk02028360 *unk2c;
};

void InventionTurn_PrepareNextFire(struct Unk3ED60Proc *proc)
{
    ClearInventionFireList();
    while (proc->unk2c->unk02_6 != 0)
    {
        switch (proc->unk2c->unk02_6)
        {
        case 1:
            if (proc->unk2c->unk06 != 0)
                break;
            PushInventionFireEntry(proc->unk2c->unk00, proc->unk2c->unk01, 0, proc->unk2c->unk02_6);
            ScanUnitsOnCrossRays(proc->unk2c->unk00, proc->unk2c->unk01, proc->unk2c->unk07);
            proc->unk2c++;
            return;
        case 5:
            if (proc->unk2c->unk04 == 0)
                break;
            if (proc->unk2c->unk06 != 0)
                break;
            PushInventionFireEntry(proc->unk2c->unk00, proc->unk2c->unk01, 0, proc->unk2c->unk02_6);
            sub_0803E6C4(proc->unk2c->unk00, proc->unk2c->unk01, proc->unk2c->unk07);
            proc->unk2c++;
            return;
        case 2:
            if (proc->unk2c->unk06 != 0)
                break;
            PushInventionFireEntry(proc->unk2c->unk00, proc->unk2c->unk01, 0, proc->unk2c->unk02_6);
            ScanUnitsOnCellList(gUnknown_0849F728[gUnknown_03004080 & 1], proc->unk2c->unk07);
            proc->unk2c++;
            return;
        case 3:
        case 4:
            if (gPlayers[gUnknown_030033EC].teamColor != 5)
                break;
            if (proc->unk2c->unk04 == 0)
                break;
            if (proc->unk2c->unk06 != 0)
                break;
            PushInventionFireEntry(proc->unk2c->unk00, proc->unk2c->unk01, 0, proc->unk2c->unk02_6);
            PickBestInventionTarget(proc->unk2c->unk00, proc->unk2c->unk01, proc->unk2c->unk07,
                         proc->unk2c->unk02_6, proc->unk2c->unk02_e);
            proc->unk2c++;
            return;
        }
        proc->unk2c++;
    }
    Proc_Goto(proc, 1);
}
asm(".global sub_0803ED60\n.thumb_set sub_0803ED60, InventionTurn_PrepareNextFire\n");
