#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803355C.
 * sub_0803355C @ 0x0803355C, sub_080335CC @ 0x080335CC
 */

#include "proc.h"
struct Unk3355CProc
{
    /* 0x00 */ u8 filler_00[0x28];
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 filler_29[0x01];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u8 filler_2c[0x02];
    /* 0x2e */ u8 unk2e;
};
struct Unk335CCProc
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ void (*unk20)(struct Unk335CCProc *);
    /* 0x24 */ u32 unk24;
    /* 0x28 */ u8 filler_28[0x02];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u8 filler_2e[0x01];
    /* 0x2f */ u8 unk2f;
};

void SioBigSend_Init(struct Unk3355CProc *proc)
{
    struct Unk0849B018 *p = gUnknown_0849B018;

    gUnknown_030044D8 = 0;

    p->unk22 = p->unk20 = 0;
    p->unk24[0] = p->unk24[1] = p->unk24[2] = p->unk24[3] = 0;

    SioResetBuffers();

    gUnknown_03004400[0] = proc->unk28;
    gUnknown_03004400[1] = proc->unk2a >> 8;
    gUnknown_03004400[2] = proc->unk2a;
    gUnknown_03004400[3] = proc->unk2e;

    LinkQueueCommand((u8 *)gUnknown_03004400);

    gUnknown_030044D8 = 1;
}
asm(".global sub_0803355C\n.thumb_set sub_0803355C, SioBigSend_Init\n");

void SioBigSend_Loop(struct Unk335CCProc *proc)
{
    u8 t;

    if (proc->unk20 != NULL)
        proc->unk20(proc);

    t = gUnknown_030044D8;

    if (t == 0)
    {
        if (proc->unk2c != gUnknown_0849B018->unk22 - 1)
        {
            proc->unk24 += 0x80;
            proc->unk2f = proc->unk2c * 100 / proc->unk2a;
            proc->unk2c++;
        }

        LinkQueueBlock(proc->unk24);
        gUnknown_0849B018->unk1d = t;

        if (proc->unk2c >= proc->unk2a)
            Proc_Break(proc);
    }
}
asm(".global sub_080335CC\n.thumb_set sub_080335CC, SioBigSend_Loop\n");
