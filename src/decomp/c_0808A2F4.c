#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808A2F4.
 * sub_0808A2F4 @ 0x0808A2F4
 */

struct Unk8A2F4Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void CoDesignEditor_DrawCoBody(struct Unk8A2F4Proc *proc)
{
    sub_08043C28(0xb0, 0xa0, 0x1040, 2, 1);
    DrawOamObject(gUnknown_03005958[proc->unk58] + 0x3e, 0x20, 0x78, 0, 0);
    sub_08043B60(0x30, 0x78, 0xa2cc, 3);
}
asm(".global sub_0808A2F4\n.thumb_set sub_0808A2F4, CoDesignEditor_DrawCoBody\n");
