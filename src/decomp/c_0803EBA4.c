#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803EBA4.
 * sub_0803EBA4 @ 0x0803EBA4
 */

/* MATCHED (wave 49, W49-C), first attempt. 324/324 bytes, relocs match.
 *
 * The proc step behind the InventionFire_EndWhenDone filter (src/decomp/c_0803EB84.c), whose
 * +0x4c s16 cursor this one walks and increments.
 *
 * The `subs #1; cmp #4; bhi` switch is the dense jump-table form, so table entry
 * N is source value N + 1, and the five case bodies come out in source order
 * 1..5. Cases 3 and 4 are cross-jumped onto one tail at _0803ECAA -- they differ
 * only in the fifth argument (3 vs 4), which is why the shared code starts at
 * `str r4,[sp]`; that is jump.c, not a grouped case label.
 *
 * `p = gUnknown_03003338;` is the c_0803E560.c idiom: the pointer global is read
 * once into r4 and every record reached as `p[i]`, which is what keeps the base
 * in one register across the whole function.
 */

#include "proc.h"
struct Unk3EBA4Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4c);
    /* 4c */ s16 unk4c;
};

void InventionFire_StartEffect(struct Unk3EBA4Proc *proc)
{
    struct Unk03003338 *p;
    struct Unit *u;
    struct Unk02028360 *q;
    int i;

    p = gUnknown_03003338;
    for (i = 0; p[i].unk00 == 0; i++)
        ;
    u = &gUnits[p[i].unk00];
    if (p[proc->unk4c].unk00 != 0)
        return;
    switch (p[proc->unk4c].unk02)
    {
    case 1:
        StartLaserFire(p[proc->unk4c].unk04, p[proc->unk4c].unk06, proc);
        break;
    case 2:
        StartVolcanoFire(p[proc->unk4c].unk04, p[proc->unk4c].unk06,
                     (int)gUnknown_0849F728[gUnknown_03004080 & 1], proc);
        break;
    case 3:
        q = FindInventionAt(p[proc->unk4c].unk04, p[proc->unk4c].unk06);
        StartCannonFire(p[proc->unk4c].unk04, p[proc->unk4c].unk06, u->x, u->y,
                     3, q->unk02_e, proc);
        break;
    case 4:
        q = FindInventionAt(p[proc->unk4c].unk04, p[proc->unk4c].unk06);
        StartCannonFire(p[proc->unk4c].unk04, p[proc->unk4c].unk06, u->x, u->y,
                     4, q->unk02_e, proc);
        break;
    case 5:
        StartDeathRayFire(p[proc->unk4c].unk04, p[proc->unk4c].unk06, proc);
        break;
    }
    proc->unk4c++;
    Proc_Goto(proc, 0);
}
asm(".global sub_0803EBA4\n.thumb_set sub_0803EBA4, InventionFire_StartEffect\n");
