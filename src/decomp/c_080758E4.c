#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080758E4.
 * sub_080758E4 @ 0x080758E4
 */

#include "proc.h"
struct Unk080758E4
{
    /* 0x00 */ u8 filler_00[0x40];
    /* 0x40 */ int unk40;
};

int IsWorldMapReticleShrinkDone(void)
{
    struct Unk080758E4 *proc = Proc_Find(gUnknown_086143E0);

    if (proc->unk40 != 0)
        return 1;
    else
        return 0;
}
asm(".global sub_080758E4\n.thumb_set sub_080758E4, IsWorldMapReticleShrinkDone\n");
