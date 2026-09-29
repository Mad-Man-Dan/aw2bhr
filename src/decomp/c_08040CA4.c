#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08040CA4.
 * sub_08040CA4 @ 0x08040CA4
 */

#include "hardware.h"
/* The proc this initialises; only the fields it touches are named. */
struct Unk08040CA4
{
               STRUCT_PAD(0x00, 0x2C);
    /* 0x2C */ struct Unk0801C210 *unk2c;
               STRUCT_PAD(0x30, 0x34);
    /* 0x34 */ struct Unk0801C210 *unk34;
    /* 0x38 */ struct Unk0801C210 *unk38;
               STRUCT_PAD(0x3C, 0x42);
    /* 0x42 */ u16 unk42;
    /* 0x44 */ u8 unk44;
               STRUCT_PAD(0x45, 0x46);
    /* 0x46 */ u16 unk46;
    /* 0x48 */ u8 unk48;
               STRUCT_PAD(0x49, 0x4A);
    /* 0x4A */ u8 unk4a;
};

/* Sets up the three animations and the three OBJ affine slots for a screen.
 * The angle is a literal 0, so slots 1 and 2 come out as the identity matrix
 * and slot 3 is the identity scaled on one axis by a countdown in unk48.
 *
 * `Div(...) ? Div(...) : 2` really is the divisor spelled twice: the ROM calls
 * Div, tests the result, and calls it a second time on the non-zero path.
 * agbcc cannot CSE the two because Div is an ordinary call, so the duplicate
 * call IS the source and not a codegen artefact. */
void CaptureAnimSprites_Init(struct Unk08040CA4 *proc)
{
    proc->unk34 = 0;
    proc->unk38 = 0;
    proc->unk2c = 0;

    LoadCapturePropertySprite(proc->unk4a, 0x22A, 5);
    LoadCaptureUnitSprite(proc->unk44, 0x24A, 4);
    Decompress(gUnknown_081214B4, (void *)0x06013940);
    ApplyPaletteExt(gUnknown_0812189C, 0x260, 0x20);

    proc->unk34 = AP_Create(gUnknown_08121344, 0, 1);
    AP_SwitchAnimation(proc->unk34, 0);
    proc->unk34->unk22 = 0x424A;

    proc->unk38 = AP_Create(gUnknown_081240BC, 0, 1);
    AP_SwitchAnimation(proc->unk38, 0);
    proc->unk38->unk22 = 0x522A;

    proc->unk2c = AP_Create(gUnknown_08121870, 0, 1);
    AP_SwitchAnimation(proc->unk2c, 0);
    proc->unk2c->unk22 = 0x31CA;

    SetObjAffine(1, Div(gSinLut[0x40] << 4, 0x100),
                 Div(-gSinLut[0] << 4, 0x100),
                 Div(gSinLut[0] << 4, 0x100),
                 Div(gSinLut[0x40] << 4, 0x100));

    SetObjAffine(3, Div(gSinLut[0x40] << 4, 0x100),
                 Div(-gSinLut[0] << 4,
                     Div((0x14 - proc->unk48) << 8, 0x14)
                         ? Div((0x14 - proc->unk48) << 8, 0x14)
                         : 2),
                 Div(gSinLut[0] << 4, 0x100),
                 Div(gSinLut[0x40] << 4,
                     Div((0x14 - proc->unk48) << 8, 0x14)
                         ? Div((0x14 - proc->unk48) << 8, 0x14)
                         : 2));

    SetObjAffine(2, Div(gSinLut[0x40] << 4, 0x100),
                 Div(-gSinLut[0] << 4, 0x100),
                 Div(gSinLut[0] << 4, 0x100),
                 Div(gSinLut[0x40] << 4, 0x100));

    proc->unk46 = proc->unk48;
    proc->unk42 = 0;
}
asm(".global sub_08040CA4\n.thumb_set sub_08040CA4, CaptureAnimSprites_Init\n");
