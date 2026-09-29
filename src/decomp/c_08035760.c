#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035760.
 * sub_08035760 @ 0x08035760
 */

#include "proc.h"
/* Copies sixteen bytes into the proc's +0x4c buffer, resets its two state
 * bytes and, unless the gUnknown_030032D8 == 0x13 special case is in force,
 * re-aims the camera at the proc's stored position.
 *
 * The position pair at +0x42/+0x44 is in 1/16ths -- `cmp #0; bge; adds #0xf;
 * asrs #4` is signed division by 16, not a shift -- and sub_08035850 is the
 * producer that stores `arg * 16` into the same two fields.
 *
 * The proc pointer is dereferenced through a cast at each use rather than
 * bound to a local: a `struct Unk35760Proc *proc = procPtr;` local is a second
 * pseudo and costs one extra `adds rN, r0, #0` in the prologue, which the ROM
 * does not have. */
struct Unk35760Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x34);
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ STRUCT_PAD(0x36, 0x42);
    /* 0x42 */ s16 unk42;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ STRUCT_PAD(0x46, 0x4c);
    /* 0x4c */ u8 unk4c[0x10];
};

void BeginMoveSlidePath(ProcPtr proc, void *src)
{
    u8 *s = src;
    int i;

    for (i = 0; i < 16; i++)
        ((struct Unk35760Proc *)proc)->unk4c[i] = *s++;

    ((struct Unk35760Proc *)proc)->unk34 = 0;
    ((struct Unk35760Proc *)proc)->unk35 = 4;

    if (gPlaySt.savingEnabled == 0 || gUnknown_030032D8 != 0x13)
        ScrollCameraToKeepCellInView(((struct Unk35760Proc *)proc)->unk42 / 16,
                     ((struct Unk35760Proc *)proc)->unk44 / 16);

    gUnknown_030040E4 = 1;
    StartMoveSlideMoveSound(proc);
}
asm(".global sub_08035760\n.thumb_set sub_08035760, BeginMoveSlidePath\n");
