#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803EF70.
 * sub_0803EF70 @ 0x0803EF70
 */

/* The same proc as the matched StartLaserFire (src/decomp/c_0803EF44.c) starts:
 * unk2c/unk30 are the cell coordinates, scaled by 16 and shifted by the camera
 * pair at gUnknown_08499590 + 4 / + 6, then offset by four fixed sprite
 * positions and wrapped to the 0x1ff / 0xff OAM ranges. The 0x1000 in the second
 * call is the horizontal-flip bit, which is why only that one ORs the x. */
struct Unk3EF70Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
};

void LaserFire_StartEffects(struct Unk3EF70Proc *proc)
{
    int x;
    int y;

    PlayMusicOrSfx2(0x1c9);
    Decompress(gUnknown_08113BE0, (void *)0x06013940);
    Decompress(gUnknown_081143D0, (void *)0x06014940);
    ApplyPaletteExt(gUnknown_08114E34, 0x260, 0x40);
    x = proc->unk2c * 16 - gMap->scrollX;
    y = proc->unk30 * 16 - gMap->scrollY;
    APProc_Create(gUnknown_08113F84, (x + gUnknown_0849F820[0]) & 0x1ff,
                 (y + gUnknown_0849F828[0]) & 0xff, 0x31ca, 0, 0);
    APProc_Create(gUnknown_08113F84, ((x + gUnknown_0849F820[1]) & 0x1ff) | 0x1000,
                 (y + gUnknown_0849F828[1]) & 0xff, 0x31ca, 0, 0);
    APProc_Create(gUnknown_08114740, (x + gUnknown_0849F820[2]) & 0x1ff,
                 (y + gUnknown_0849F828[2]) & 0xff, 0x324a, 0, 0);
    APProc_Create(gUnknown_08114740, (x + gUnknown_0849F820[3]) & 0x1ff,
                 (y + gUnknown_0849F828[3]) & 0xff, 0x324a, 1, 0);
}
asm(".global sub_0803EF70\n.thumb_set sub_0803EF70, LaserFire_StartEffects\n");
