#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801DB04.
 * sub_0801DB04 @ 0x0801DB04
 */

#include "hardware.h"

void UpdateSpriteScriptAffine(s16 a1)
{
    struct Unk0200E438 *e = &gUnknown_0200E438[a1];
    s16 *ang;
    s16 sx, sy;

    if (e->unk3a < 0)
        return;

    sy = e->unk3e;
    sx = e->unk3c;
    if (sx == 0)
        sx = 2;
    if (sy == 0)
        sy = 2;

    SetObjAffine(e->unk3a,
                 Div(gSinLut[(*(ang = &e->unk40) & 0xFF) + 0x40] * 16, sx == 0 ? 2 : sx),
                 Div(-gSinLut[*ang & 0xFF] * 16, sy == 0 ? 2 : sy),
                 Div(gSinLut[*ang & 0xFF] * 16, sx == 0 ? 2 : sx),
                 Div(gSinLut[(*ang & 0xFF) + 0x40] * 16, sy == 0 ? 2 : sy));
}
asm(".global sub_0801DB04\n.thumb_set sub_0801DB04, UpdateSpriteScriptAffine\n");
