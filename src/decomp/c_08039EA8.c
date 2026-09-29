#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039EA8.
 * sub_08039EA8 @ 0x08039EA8
 */

#include "proc.h"
struct Unk39EA8Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x2c);
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ STRUCT_PAD(0x2e, 0x34);
    /* 0x34 */ struct Unk0801C210 *unk34;
};

/* The sprite is stored and then written through in the same breath, so the
 * returned pointer is still in r0 when `strh r1, [r0, #0x22]` runs -- no
 * reload. */

void SparkleEffect_StartApAnim(struct Unk39EA8Proc *proc)
{
    proc->unk34 = AP_Create(gUnknown_0810BB9C, 0, 1);
    proc->unk34->unk22 = proc->unk2c;
}
asm(".global sub_08039EA8\n.thumb_set sub_08039EA8, SparkleEffect_StartApAnim\n");
