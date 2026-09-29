#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080337D8.
 * sub_080337D8 @ 0x080337D8
 */

#include "proc.h"
/* NOT a Proc, even though Proc_StartBlocking returns it: +0x20 is proc_prev and
 * +0x24 is proc_sleepTime, both inside PROC_HEADER, and src/proc.c (a matching
 * source) pins that layout. STRUCT_PAD(0x29, 0x20) is a compile error, so the
 * whole prefix is filler.
 */
struct Unk337D8
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ u32 unk20;
    /* 0x24 */ u32 unk24;
    /* 0x28 */ u8 filler_28[0x04];
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u8 filler_2e[0x01];
    /* 0x2f */ u8 unk2f;
};

void StartSioBigReceive(u32 a, u32 b, ProcPtr parent)
{
    struct Unk337D8 *proc = Proc_StartBlocking(gUnknown_0849BB28, parent);

    proc->unk20 = b;
    proc->unk24 = a;
    proc->unk2f = 0;
    proc->unk2c = 0;
}
asm(".global sub_080337D8\n.thumb_set sub_080337D8, StartSioBigReceive\n");
