#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074A28.
 * sub_08074A28 @ 0x08074A28
 */

#include "proc.h"
struct Unk8074AAC
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ const u8 *unk_2c;
};
struct Unk086142CC
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ s16 unk2a;
    /* 0x2c */ s16 unk2c;
    /* 0x2e */ s16 unk2e;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ u8 filler_32[0x38 - 0x32];
    /* 0x38 */ int unk38;
};

void MarkerReveal_PopInNext(struct Unk8074AAC *proc)
{
    struct Unk086142CC *child;
    const struct Unk08615194 *r;
    s8 id;
    int v;

    v = 0x18;
    id = *proc->unk_2c;

    child = Proc_StartBlocking(gUnknown_086142CC, proc);
    child->unk2a = id;

    r = &gUnknown_08615194[id];
    child->unk2c = r->flagX;
    child->unk2e = r->flagY;

    if (r->specialProperty & 4)
        v = 0x28;

    if (r->specialProperty & 8)
        v = 0x38;

    child->unk30 = v;
    child->unk38 = 0;

    AddWorldMapMarker(id);

    proc->unk_2c++;

    if ((s8)*proc->unk_2c == -1)
        Proc_Break(proc);
    else
        Proc_Goto(proc, 0);
}
asm(".global sub_08074A28\n.thumb_set sub_08074A28, MarkerReveal_PopInNext\n");
