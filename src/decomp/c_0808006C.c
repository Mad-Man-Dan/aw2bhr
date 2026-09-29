#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808006C.
 * sub_0808006C @ 0x0808006C, sub_08080094 @ 0x08080094
 */

#include "proc.h"
struct Unk0808006C
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void CoPowerSceneEnd_WaitForBlend(struct Unk0808006C *proc)
{
    if (Proc_Find(gUnknown_086167EC) == NULL)
    {
        proc->unk58 = gUnknown_030033EC;
        Proc_Break(proc);
    }
}
asm(".global sub_0808006C\n.thumb_set sub_0808006C, CoPowerSceneEnd_WaitForBlend\n");

void sub_08080094(void)
{
    sub_0801A5B0(gUnknown_030033EC);
    sub_08043834(gUnknown_030033EC);
}
