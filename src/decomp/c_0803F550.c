#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F550.
 * sub_0803F550 @ 0x0803F550
 */

/* The two scroll subtractions are BOUND, but assigned after the two calls
 * rather than at the declarations: that puts both `subs` ahead of the sprite
 * descriptor's pool load and both masks after it, which is the ROM's order.
 * Initialising them at the declaration hoists both subtractions above
 * Decompress; leaving them inline in the call puts the descriptor first and
 * interleaves each mask with its own subtraction (which is what its twin
 * VolcanoFire_StartEruption does, and that one is inline for exactly that reason). */
struct UnkF550Sub
{
    /* 0x00 */ u8 filler_00[0x24];
    /* 0x24 */ void *unk24;
};
struct UnkF550Ret
{
    /* 0x00 */ u8 filler_00[0x50];
    /* 0x50 */ struct UnkF550Sub *unk50;
};
struct UnkF550Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
};

void DeathRayFire_StartBeam(struct UnkF550Proc *proc)
{
    int x;
    int y;

    Decompress(gUnknown_08117380, gUnknown_0200FC50);
    ApplyPaletteExt(gUnknown_081190D8, 0x260, 0x40);

    x = proc->unk2c * 16 - gMap->scrollX;
    y = proc->unk30 * 16 - gMap->scrollY;

    ((struct UnkF550Ret *)APProc_Create(gUnknown_081183EC, (x + 0x18) & 0x1FF,
                                       (y + 0x4E) & 0xFF, 0x31CA, 0, 0))->unk50->unk24 = gUnknown_0200FC50;
}
asm(".global sub_0803F550\n.thumb_set sub_0803F550, DeathRayFire_StartBeam\n");
