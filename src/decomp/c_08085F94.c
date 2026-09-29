#include "global.h"
#include "proc.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08085F94.
 * sub_08085F94 @ 0x08085F94
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* MATCHED wave 34, W34-E.
 *
 * Fills in the map/unit cursor pane's proc and hands it to UpdateMapSelectPropertyCounts,
 * choosing between the gUnknown_03005900/30 pair and the
 * gUnknown_03005990[gUnknown_0300596C]/80 pair on `gUnknown_081D940C->unk01
 * == 2`. That choice is already described from the other side on the
 * gUnknown_03005900 note in unknown-globals.h; struct Unk081D940C was added
 * there for this function.
 *
 * The whole cost of this one was two statements at the top, and they are an
 * instance of the three-local rule recorded in docs/agbcc-codegen.md (W34-E):
 *
 *   ldr  r1, =gUnknown_081D940C   <- forced by reading ->unk01
 *   ldr  r0, [r1]
 *   ldrb r0, [r0, #1]             <- kind = gUnknown_081D940C->unk01;
 *   adds r5, r1, #0               <- pp   = &gUnknown_081D940C;
 *
 * `kind` and `pp` are two pseudos joined by a copy, and the ORDER of the two
 * statements is what puts the copy after the `ldrb` rather than before it.
 * Measured on the way here, all size-exact:
 *   - naming the global at both sites and binding nothing: agbcc parks the
 *     address in this unit's .rodata (a `.LC` word) and reaches it through a
 *     second `ldr` at each site, +4 bytes.
 *   - `pp = &g;` alone, before or after the unk48 store: 85.7%, ONE pseudo
 *     loaded straight into r5, no copy.
 *   - `st = *pp;` as the intermediate: 85.7%, agbcc folds `st` away.
 *   - `st = g; pp = &g;`: 96.4% -- the copy appears but lands BEFORE the
 *     `ldrb`, because binding the pointer rather than the byte leaves the
 *     address dead one instruction too early.
 *
 * The second read after the call must stay `(*pp)->unk01`; spelling it as the
 * global again brings the `.LC` straight back. */
struct Unk8085F94Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ u8 filler_34[0x14];
    /* 0x48 */ u16 unk48;
    /* 0x4a */ u8 filler_4a[0x02];
    /* 0x4c */ u16 unk4c;
    /* 0x4e */ u16 unk4e;
    /* 0x50 */ u8 filler_50[0x02];
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u8 filler_54[0x04];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
};

void MapSelectList_Init(struct Unk8085F94Proc *proc)
{
    struct Unk081D940C **pp;
    int kind;

    proc->unk48 = 0x40;

    kind = gUnknown_081D940C->unk01;
    pp = &gUnknown_081D940C;

    if (kind == 2)
    {
        proc->unk58 = gUnknown_03005900 + gUnknown_03005930;
        proc->unk5c = gUnknown_03005900;
    }
    else
    {
        proc->unk58 = gUnknown_03005990[gUnknown_0300596C] + gUnknown_03005980;
        proc->unk5c = gUnknown_03005990[gUnknown_0300596C];
    }

    proc->unk4e = 0;
    proc->unk30 = 0;
    proc->unk52 = 0;
    proc->unk2c = 0x14;
    proc->unk4c = 0;

    UpdateMapSelectPropertyCounts(proc);

    if ((*pp)->unk01 == 2)
    {
        StartEnemyCoMinimugs(proc->unk58, proc);

        if (gUnknown_0200C420.unk0c != 0)
            ToggleMapRecordsPanel(proc->unk58, proc);
    }
}

asm(".global sub_08085F94\n.thumb_set sub_08085F94, MapSelectList_Init\n");

extern void MapSelectList_DrawLoop(void);
extern void MapSelectList_InputLoop(void);

struct ProcCmd CONST_DATA ProcScr_PutMapPropertiesPreview[] =
{
    PROC_CALL(MapSelectList_Init),
    PROC_REPEAT(MapSelectList_DrawLoop),
    PROC_REPEAT(MapSelectList_InputLoop),
    PROC_REPEAT(MapSelectList_DrawLoop),
    PROC_END,
};

asm(".global gUnknown_08616CF4\n.set gUnknown_08616CF4, ProcScr_PutMapPropertiesPreview\n");
