#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F38C.
 * sub_0803F38C @ 0x0803F38C, sub_0803F3C8 @ 0x0803F3C8
 */

#include "proc.h"
/* Same proc as VolcanoFire_ScrollToVolcano: +0x4c is a cursor into a ROM script of u16 PAIRS,
 * terminated by 0xFFFF. The compare is against a pool `0x0000FFFF` and the load
 * is a bare `ldrh`, so the sentinel is 65535 and not -1. */
struct UnkF38CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x4c);
    /* 4c */ u16 *unk4c;
};
struct UnkF3C8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
};

/* The two `lsls #0x10; asrs #0x10` pairs come from re-reading the two word
 * fields that were just stored, not from the ldrh values: agbcc keeps the
 * stored register live and the pair is the implicit conversion to
 * ScrollCameraToKeepCellInView's `s16` parameters. Writing `ScrollCameraToKeepCellInView(p[0], p[1])` instead
 * costs two `ldrsh` reloads. */
void VolcanoFire_NextRock(struct UnkF38CProc *proc)
{
    u16 *p = proc->unk4c;

    if (p[0] == 0xFFFF)
    {
        Proc_Goto(proc, 1);
    }
    else
    {
        proc->unk2c = p[0];
        proc->unk30 = p[1];

        ScrollCameraToKeepCellInView(proc->unk2c, proc->unk30);

        p += 2;
        proc->unk4c = p;
    }
}
asm(".global sub_0803F38C\n.thumb_set sub_0803F38C, VolcanoFire_NextRock\n");

/* `movs r0, #0xec; lsls r0, r0, #1` is how agbcc materialises 0x1D8 -- a sound
 * id for PlayMusicOrSfx2, which takes `int`. `adds r2, r4, #0` puts the proc
 * itself in the third argument, so this proc is its own child's parent. */
void VolcanoFire_StartRock(struct UnkF3C8Proc *proc)
{
    PlayMusicOrSfx2(0x1D8);
    StartVolcanoRock(proc->unk2c, proc->unk30, proc);
}
asm(".global sub_0803F3C8\n.thumb_set sub_0803F3C8, VolcanoFire_StartRock\n");
