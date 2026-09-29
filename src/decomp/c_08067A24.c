#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067A24.
 * sub_08067A24 @ 0x08067A24
 */

#include "proc.h"
struct Unk67A24Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x50);
    /* 50 */ u8 unk50;
};

void TriggerIntroBgAffineTween(void)
{
    ((struct Unk67A24Proc *)Proc_Find(gUnknown_08581014))->unk50 = 1;
}
asm(".global sub_08067A24\n.thumb_set sub_08067A24, TriggerIntroBgAffineTween\n");
