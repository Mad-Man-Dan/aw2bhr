#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080767A8.
 * sub_080767A8 @ 0x080767A8
 */

#include "proc.h"
struct Unk80767A8
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ u16 unk_64;
};

void DismissWorldMapCallout(void)
{
    ((struct Unk80767A8 *)Proc_Find(gUnknown_086144FC))->unk_64 = 1;
}
asm(".global sub_080767A8\n.thumb_set sub_080767A8, DismissWorldMapCallout\n");
