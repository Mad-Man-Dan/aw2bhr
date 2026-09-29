#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080867BC.
 * sub_080867BC @ 0x080867BC
 */

/* WAVE 31 (W31-C). Inherited at 93.5%; the draft did not compile any more --
 * its local prototypes for sub_0808B6E8 and GetMapPreviewScrollY had been overtaken by
 * include/unknown-functions.h. Deleting them is half the fix: the header types
 * GetMapPreviewScrollY `u16`, and the re-narrow that forces at each call site is real
 * code the `int` spelling was missing.
 *
 * The other half: at the three sites in the unk52 branch that assign
 * gUnknown_03002B34 from GetMapPreviewScrollX(), the ROM loads the DESTINATION address
 * before the `bl` and keeps it across. Writing the store as
 * `*(u16 *)&gUnknown_03002B34 = f() ...` puts that `ldr` after the call
 * instead; binding the destination to a pointer local first reproduces the ROM.
 * The gUnknown_03002F18 stores nearby are NOT the same -- they have nothing
 * after the call but the `strh`, so their address load stays after it, and
 * binding those would break them. Same rule as the base-pointer local in
 * src/decomp/c_0803E560.c, applied to a store destination. */

struct Unk080867BCProc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ u8 filler_30[0x1c];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ s16 unk4e;
    /* 0x50 */ u8 filler_50[0x02];
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u8 filler_54[0x04];
    /* 0x58 */ int unk58;
    /* 0x5c */ u32 unk5c;
};

void MapSelectList_StepTransition(struct Unk080867BCProc *proc)
{
    u16 buf[9];
    u16 *dst;

    sub_0808B6E8(buf, gUnknown_081D9424, 0x12);
    if (proc->unk4e != 0)
    {
        if (proc->unk4c <= 8)
            gUnknown_03002F18 = GetMapPreviewScrollY() + buf[proc->unk4c] * proc->unk4e;
        else if (proc->unk4c == 0xc)
            StartMapSelectPreview(proc->unk58);
        else if (proc->unk4c > 0x13)
        {
            if (proc->unk4c == 0x14)
            {
                gUnknown_03002F18 = GetMapPreviewScrollY() - proc->unk4e * 0x100;
                *(u16 *)&gUnknown_03002B34 = GetMapPreviewScrollX();
            }
            else if (proc->unk4c <= 0x1b)
                gUnknown_03002F18 = GetMapPreviewScrollY() - buf[0x1c - proc->unk4c] * proc->unk4e;
        }
        proc->unk4c++;
        if (proc->unk4c == 0x1c)
        {
            UpdateMapSelectPropertyCounts(proc);
            gUnknown_03002F18 = GetMapPreviewScrollY();
            proc->unk4e = 0;
        }
    }
    else if (proc->unk52 != 0)
    {
        if (proc->unk4c <= 8)
        {
            dst = (u16 *)&gUnknown_03002B34;
            *dst = GetMapPreviewScrollX() + buf[proc->unk4c] * proc->unk52;
        }
        else if (proc->unk4c == 0xc)
            StartMapSelectPreview(proc->unk58);
        else if (proc->unk4c > 0x13)
        {
            if (proc->unk4c == 0x14)
            {
                dst = (u16 *)&gUnknown_03002B34;
                *dst = GetMapPreviewScrollX() - (proc->unk52 << 8);
                gUnknown_03002F18 = GetMapPreviewScrollY();
            }
            else if (proc->unk4c <= 0x1b)
            {
                dst = (u16 *)&gUnknown_03002B34;
                *dst = GetMapPreviewScrollX() - buf[0x1c - proc->unk4c] * proc->unk52;
            }
        }
        switch (proc->unk2c)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            gUnknown_03001FF8 = Interpolate(1, 0, proc->unk52 * 72, proc->unk2c, 8);
            break;
        case 8:
            InitTextTileCache(0);
            break;
        case 9:
            gUnknown_03001FF8 = proc->unk52 * 72;
            DrawMapListFirstTwoRows(proc->unk5c, gUnknown_03005928, 0);
            break;
        case 10:
            DrawMapListRowsFromThird(proc->unk5c, gUnknown_03005928, 0);
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            gUnknown_03001FF8 = Interpolate(4, -(proc->unk52 * 72), 0, proc->unk2c - 8, 8);
            break;
        default:
            gUnknown_03001FF8 = 0;
            break;
        }
        proc->unk4c++;
        proc->unk2c++;
        if (proc->unk4c == 0x1c)
        {
            UpdateMapSelectPropertyCounts(proc);
            *(u16 *)&gUnknown_03002B34 = GetMapPreviewScrollX();
        }
        if (proc->unk4c > 0x1b && proc->unk2c > 0x11)
            proc->unk52 = 0;
    }
}
asm(".global sub_080867BC\n.thumb_set sub_080867BC, MapSelectList_StepTransition\n");
