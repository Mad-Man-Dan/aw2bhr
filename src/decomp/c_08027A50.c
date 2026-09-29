#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08027A50.
 * sub_08027A50 @ 0x08027A50, sub_08027B10 @ 0x08027B10
 */

#include "proc.h"
struct Unk27B10Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ int unk34;
    /* 38 */ int unk38;
    /* 3c */ STRUCT_PAD(0x3c, 0x48);
    /* 48 */ s16 unk48;
    /* 4a */ u16 unk4a;
    /* 4c */ STRUCT_PAD(0x4c, 0x64);
    /* 64 */ s16 unk64;
    /* 66 */ STRUCT_PAD(0x66, 0x6a);
    /* 6a */ s16 unk6a;
};

void sub_08027A50(u16 a1, u16 a2, u16 a3)
{
    s8 id;
    u32 tile;
    struct UnkVec v;

    id = StartSlotScriptWithSprite(gUnknown_08499DE8, 0, gUnknown_08499DDC, NULL, 0);

    v = GetSlotSpriteAttrs(id);
    v.unk04 = (v.unk04 & 0xFFFF0FFF) | 0x3000;
    tile = (a3 + 0x1ca) & 0x3ff;
    v.unk04 = (v.unk04 & 0xFFFFFC00) | tile;
    SetSlotSpriteAttrs(id, v);

    SetSlotSpritePosition(id, a1, a2);
    EnableSlotSpriteAffine(id);
    SetSlotSpriteDoubleSize(id);
    SetSlotSpriteScaleX(id, 0x200);
    SetSlotSpriteScaleY(id, 0x200);
    SetSlotSpriteRotation(id, 0x20);
}

void StartDayStartGlyph(int a1, int a2, int a3, int a4, ProcPtr a5)
{
    struct Unk27B10Proc *proc = Proc_Start(ProcScr_DayStart, a5);

    proc->unk2c = a1;
    proc->unk30 = a2;
    proc->unk34 = 0x200;
    proc->unk38 = 0x200;
    proc->unk48 = 0x20;
    proc->unk4a = (a3 + 0x1ca) | 0x3000;
    proc->unk6a = a4;
}
asm(".global sub_08027B10\n.thumb_set sub_08027B10, StartDayStartGlyph\n");
