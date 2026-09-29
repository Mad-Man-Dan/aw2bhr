#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078558.
 * sub_08078558 @ 0x08078558, sub_08078568 @ 0x08078568
 */

#include "proc.h"
/* A method of the gUnknown_08615ACC proc that StartWorldMapScene starts: the +0x2c
 * and +0x30 words it reads back are StartWorldMapScene's first two arguments, stored
 * there by that function. r0 is passed straight through as StartWorldMapCameraPan's
 * ProcPtr parameter, so this proc is its own call's parent. The literal 1 in r3
 * is StartWorldMapCameraPan's narrow flag parameter -- that callee tests it with a bare
 * `lsls #0x18`. */

struct UnkProc8615ACC
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ s32 unk_2c;
    /* 0x30 */ s32 unk_30;
};
/* A four-argument forwarder to the matched StartWorldMapCallout. `adds r3, r0, #0` is
 * NOT a scratch copy made to free r0: StartWorldMapCallout takes four arguments and
 * passes r3 to Proc_Start as the parent, so the copy IS the fourth argument.
 * A three-argument call would have read `ldr r2, [r0, #0x58]` straight off r0
 * and never touched r3. */

struct Unk8078568
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ s32 unk_58;
};

void WorldMapScene_PanCamera(struct UnkProc8615ACC *proc)
{
    StartWorldMapCameraPan(proc, proc->unk_2c, proc->unk_30, 1);
}
asm(".global sub_08078558\n.thumb_set sub_08078558, WorldMapScene_PanCamera\n");

void WorldMapScene_ShowCallout(struct Unk8078568 *proc)
{
    StartWorldMapCallout(0x48, 0x38, proc->unk_58, proc);
}
asm(".global sub_08078568\n.thumb_set sub_08078568, WorldMapScene_ShowCallout\n");
