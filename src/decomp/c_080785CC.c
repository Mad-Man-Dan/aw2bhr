#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080785CC.
 * sub_080785CC @ 0x080785CC
 */

#include "proc.h"
/* The starter for the gUnknown_08615ACC proc, and the shared tail of
 * WorldMapReturn_ShowSceneIfMissions0F17Cleared, sub_08078404 and sub_08078420. Same shape as
 * src/decomp/c_080784E4.c's starter with one more field and one more argument:
 * the fifth parameter is the parent and arrives at [sp, #0x14] (0x14 = the
 * 16 bytes of `push {r4, r5, r6, lr}` plus the 4 of `push {r6}`).
 *
 * The store ORDER is +0x2c, +0x30, +0x58, +0x54 -- not address order -- which
 * is why the fourth argument is the one that has to survive in r8. */

struct UnkProc8615ACC
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ s32 unk_2c;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ u8 filler_34[0x20];
    /* 0x54 */ const void *unk_54;
    /* 0x58 */ s32 unk_58;
};

void StartWorldMapScene(s32 a, s32 b, s32 c, const void *d, ProcPtr parent)
{
    struct UnkProc8615ACC *proc = Proc_StartBlocking(gUnknown_08615ACC, parent);

    proc->unk_2c = a;
    proc->unk_30 = b;
    proc->unk_58 = c;
    proc->unk_54 = d;
}
asm(".global sub_080785CC\n.thumb_set sub_080785CC, StartWorldMapScene\n");
