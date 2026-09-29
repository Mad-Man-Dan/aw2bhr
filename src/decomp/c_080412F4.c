#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080412F4.
 * sub_080412F4 @ 0x080412F4, sub_08041334 @ 0x08041334, sub_0804134C @ 0x0804134C
 */

#include "proc.h"
struct Unk412F4Owner
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x46);
    /* 46 */ u16 unk46;
};
struct Unk412F4Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4c);
    /* 4c */ s16 unk4c;
    /* 4e */ s16 unk4e;
};
struct Unk41334Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4c);
    /* 4c */ s16 unk4c;
};
struct Unk4134COwner
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x34);
    /* 34 */ struct Unk0801C210 *unk34;
    /* 38 */ STRUCT_PAD(0x38, 0x46);
    /* 46 */ u16 unk46;
};
struct Unk4134CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4c);
    /* 4c */ s16 unk4c;
};

/* Both `movs rN, #0; ldrsh rD, [rB, rN]` pairs are SIGNED halfword loads at a
 * ZERO displacement -- the address was already materialised -- so the compare
 * is a signed one between two s16 fields. The `ldrh r3, [r2]` before it is the
 * unsigned copy agbcc keeps live for the increment. */
void CaptureAnimCountUp_Loop(struct Unk412F4Proc *proc)
{
    struct Unk412F4Owner *p = Proc_Find(gUnknown_0849FD44);

    if (proc->unk4c >= proc->unk4e)
        Proc_Break(proc);
    else
        p->unk46 = ++proc->unk4c;
}
asm(".global sub_080412F4\n.thumb_set sub_080412F4, CaptureAnimCountUp_Loop\n");

/* The proc pointer survives the call in r4 only so that the store can happen
 * after it; `adds r4, #0x4c` is the strh displacement limit again. */
void CaptureAnimCountDown_Init(struct Unk41334Proc *proc)
{
    PlayMusicOrSfx2(0x6F);
    proc->unk4c = 0x12;
}
asm(".global sub_08041334\n.thumb_set sub_08041334, CaptureAnimCountDown_Init\n");

/* The countdown twin of CaptureAnimCountUp_Loop: same proc, same +0x46 mirror, and the
 * Proc_Break arm additionally releases the sprite handle. Two statements in
 * that arm and not a nested call -- Proc_Break is void. */
void CaptureAnimCountDown_Loop(struct Unk4134CProc *proc)
{
    struct Unk4134COwner *p = Proc_Find(gUnknown_0849FD44);

    if (proc->unk4c == 0)
    {
        Proc_Break(proc);
        AP_SwitchAnimation(p->unk34, 3);
    }
    else
    {
        p->unk46 = --proc->unk4c;
    }
}
asm(".global sub_0804134C\n.thumb_set sub_0804134C, CaptureAnimCountDown_Loop\n");
