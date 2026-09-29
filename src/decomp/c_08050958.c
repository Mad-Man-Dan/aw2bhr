#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08050958.
 * sub_08050958 @ 0x08050958
 */

/* MATCHED. Not a sibling of the SmokeEffect_Init cluster despite sharing its
 * callees and its globals (measured shape ratio 0.16). One tick of a unit's
 * movement: bump the slot's counter, and once it passes the record's
 * threshold, step two 8.8 fixed-point axes and re-place the sprite, then fire
 * EndProjectileEffect when the moving side reaches its bound.
 *
 * Two things had to be settled before it would fall, both recorded on the
 * types rather than here:
 *
 *  - `gUnknown_03001470[proc].unk2c` is SIGNED. Its compare against the u16
 *    gUnknown_02029710[side].unk16 is `bgt`; as a `u32` member the u16 is
 *    promoted unsigned and agbcc emits `bhi`. Retyped in unknown-globals.h and
 *    the three promoted `= 0` writers re-verified.
 *
 *  - FOUR locals, in two pairs holding the same two values. `a = c = ...`
 *    does NOT work: the copy is coalesced away, one stack slot disappears and
 *    the frame comes out 8 bytes short. Four independent assignments keep four
 *    slots, and the ROM's store order (0x10, 8, 0x14, 0xc) is what fixes the
 *    order they are written in. The pair at 0x10/0x14 drives the body; the
 *    pair at 8/0xc is only ever handed to EndProjectileEffect in the non-zero arm --
 *    which is also the proof they are distinct variables, because in the
 *    `c == 0` arm gcc const-folds c into `movs r0,#0` and still loads [sp,#8]
 *    in the other one.
 *
 * `if (gUnknown_0300454C[1] == 1) gUnknown_0300454C[1] = 1;` is not a
 * transcription error -- the ROM really does `ldrh; cmp #1; bne; strh` with
 * the loaded register, which is a self-assignment cse rewrote from the
 * literal. It is kept verbatim because anything else changes the bytes. */
void ProjectileEffect_Loop(void)
{
    struct OamData oam;
    u16 a;
    u16 b;
    u16 c;
    u16 d;

    c = gUnknown_03001470[gUnknown_03001FBC].unk30;
    a = gUnknown_03001470[gUnknown_03001FBC].unk30;
    d = gUnknown_03001470[gUnknown_03001FBC].unk34;
    b = gUnknown_03001470[gUnknown_03001FBC].unk34;
    gUnknown_0300454C[0] = gUnknown_03001470[gUnknown_03001FBC].unk30;
    gUnknown_0300454C[1] = gUnknown_03001470[gUnknown_03001FBC].unk34;
    gUnknown_03001470[gUnknown_03001FBC].unk2c++;

    if (gUnknown_03001470[gUnknown_03001FBC].unk2c > gUnknown_02029710[c].unk16)
    {
        if (gUnknown_0300454C[1] == 1)
            gUnknown_0300454C[1] = 1;

        CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *)&oam);
        oam.priority = 3;
        SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);

        gUnknown_02029710[c].unk26[d] += gUnknown_02029710[c].unk30[d];
        gUnknown_0202972C[c][d] += gUnknown_02029710[c].unk26[d] >> 8;
        gUnknown_02029710[c].unk44[d] += gUnknown_02029710[c].unk4e[d];
        gUnknown_02029710[c].unk3a[d] += gUnknown_02029710[c].unk44[d] >> 8;

        SetEffectScreenPosition(c, gUnknown_03001FBC, gUnknown_0202972C[c][d],
                     gUnknown_02029710[c].unk3a[d]);

        if (c == 0)
        {
            if (gUnknown_0202972C[c][d] >= gUnknown_02029710[c].unk18
             || gUnknown_02029710[c].unk3a[d] >= gUnknown_02029710[c].unk1a)
                EndProjectileEffect(c, d, gUnknown_03001FBC);
        }
        else
        {
            if (gUnknown_0202972C[c][d] <= gUnknown_02029710[c].unk18
             || gUnknown_02029710[c].unk3a[d] >= gUnknown_02029710[c].unk1a)
                EndProjectileEffect(a, b, gUnknown_03001FBC);
        }
    }
}
asm(".global sub_08050958\n.thumb_set sub_08050958, ProjectileEffect_Loop\n");
