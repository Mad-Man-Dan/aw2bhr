#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804D0FC.
 * sub_0804D0FC @ 0x0804D0FC
 */

/* gUnknown_0813606C is agbcc's own -fforce-addr address constant for
 * &gUnknown_03001FBC, not an object: the word at 0x0813606C holds 0x03001FBC
 * and so does the word at 0x08136070 (CruiserFigure_Loop's private copy). The honest
 * spelling `gUnknown_03001FBC` produces the ROM's `ldr rN,=.LC; ldr rM,[rN];
 * ldrsh` pair by itself, including the reload after the call. */
void CruiserVariant2Figure_Loop(void)
{
    u16 c;
    u16 e;
    u16 t;

    c = gUnknown_03001470[gUnknown_03001FBC].unk30;
    e = gUnknown_03001470[gUnknown_03001FBC].unk34;
    t = sub_080156C4(gUnknown_03001FBC);

    if (t == 2)
    {
        if (gUnknown_020296B0[c].unk0c[gUnknown_020296B0[c].unk18] != 0xff)
        {
            SpawnThirdEffectAndProjectile(c, gUnknown_02029A10[c].entries[e].unk1e);
            gUnknown_02029A10[c].entries[e].unk1e++;
        }

        gUnknown_020296B0[c].unk18++;
    }

    sub_0804CA98(c, e, gUnknown_03001FBC);
    RidePartOnFigure(c, e, gUnknown_03001FBC);
    StepFigureSlide(c, e);
}
asm(".global sub_0804D0FC\n.thumb_set sub_0804D0FC, CruiserVariant2Figure_Loop\n");
