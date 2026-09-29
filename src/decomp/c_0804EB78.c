#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804EB78.
 * sub_0804EB78 @ 0x0804EB78
 */

/* MATCHED -- wave 80 (W80-B), byte-exact. The three `.rodata` words are the
 * -fforce-addr address constants for &gUnknown_03001FBC, &gUnknown_02029A10
 * and &gUnknown_02029B94 (ROM symbols gUnknown_08136094/98/9C), carried by
 * promotion.
 *
 * Parked since wave 37 as "frame 48 against 44, one extra spilled value" plus
 * "the ROM re-loads unk20 after the store". Both fell to reading the twin
 * BomberFigure_Loop's matched first region side by side with this one, and to one
 * construct nobody had tried:
 *
 *  - The extra stack slot WAS the `volatile unsigned int new_var2` local that
 *    cached gUnknown_08552148[c]. Wave 73 deleted it together with the plain
 *    re-read and got 60.6% because that also changed the operand order and
 *    the group-stride product. The ROM keeps &gUnknown_08552148[c] in r5 and
 *    RE-READS the global for dst->y, which is exactly what the twin's matched
 *    first region spells: the source entry inline in each statement,
 *    `src->x + gUnknown_085534C4[c * 10 + f * 2]` (entry FIRST), the group
 *    stride bound to a u16 `new_var`, no do/while(0). Transplanting that block
 *    verbatim took this from 82.6% to 98.4% with the frame at 44.
 *  - The post-store reload and the `ands r1, r0` tie are ONE fact: read the
 *    u16 member into an `int` local (`v = e1->unk20; if ((v & 0xf) == 1)`).
 *    `(u16 & 0xf)` is shortened to HImode by the C front end, so a reloaded
 *    value is an HImode pseudo used through a paradoxical SUBREG that
 *    local_alloc will not tie -- every volatile spelling (read-site cast,
 *    u16/int local off a volatile read, volatile member, volatile store) lands
 *    on `ands r0, r1`. An int-typed read is `(zero_extend (mem))`, which cse
 *    does NOT forward the store into (the extend of the register costs more
 *    than the load), and the SImode pseudo ties. Measured on a six-variant
 *    probe, then here and on the twin.
 *  - The `do { } while (0)` round e1 was a trade, not a lever: it moves
 *    c * 180 into ip (as the ROM) but re-allocates the gUnknown_02029B94 temps
 *    (r3/r2/r0 -> r0/r3/r4). With the int read both come out right without it.
 *
 * Settled earlier and still load-bearing: `[c * 10 + f * 2]` and not
 * `[(c * 5 + f) * 2]`; the last SetSlotSpritePosition entry as `(e3 = ...)->x` inside
 * argument 1; `p = *(u16 **)(c * sizeof(u16 *) + (u8 *)gUnknown_084C3F78)`;
 * `(row = gUnknown_02028E5C[c])[1]`. */
void TCopterFigure_Loop(void)
{
    struct Unk56E28 req;
    struct Unk02029A10 *e1;
    struct Unk02029A10 *e2;
    struct Unk02029A10 *e3;
    struct Unk02029A10 *dst;
    u16 c;
    u16 e;
    u16 f;
    unsigned short new_var;
    int v;
    s16 w;
    u16 *p;
    u16 *row;

    c = gUnknown_03001470[gUnknown_03001FBC].unk30;
    e = gUnknown_03001470[gUnknown_03001FBC].unk34;
    e1 = (struct Unk02029A10 *)(e * sizeof(struct Unk02029A10)
                                + c * sizeof(struct Unk02029A10Group)
                                + (u8 *)gUnknown_02029A10);
    e1->unk20 += gUnknown_02029B94[c][e];
    v = e1->unk20;
    if ((v & 0xf) == 1)
    {
        if (++e1->unk22 == 3)
        {
            gUnknown_02029B94[c][e] = 0;
            e1->unk20 = 0xff;
        }
        f = e1->unk22;
        dst = (struct Unk02029A10 *)(f * sizeof(struct Unk02029A10)
                                     + c * sizeof(struct Unk02029A10Group)
                                     + (u8 *)gUnknown_02029A10);
        new_var = sizeof(struct Unk02029A10Group);
        dst->x = ((struct Unk02029A10 *)(gUnknown_08552148[c] * sizeof(struct Unk02029A10)
                                         + c * new_var
                                         + (u8 *)gUnknown_02029A10))->x
            + gUnknown_085534C4[c * 10 + f * 2];
        dst->y = ((struct Unk02029A10 *)(gUnknown_08552148[c] * sizeof(struct Unk02029A10)
                                         + c * new_var
                                         + (u8 *)gUnknown_02029A10))->y
            + gUnknown_085534C4[c * 10 + f * 2 + 1];
        SpawnDebrisEffect(c, f);
    }
    StepFigureSlide(c, e);
    w = StepFigureHitFlash2(c, e, gUnknown_03001FBC);
    p = *(u16 **)(c * sizeof(u16 *) + (u8 *)gUnknown_084C3F78);
    e2 = (struct Unk02029A10 *)(e * sizeof(struct Unk02029A10)
                                + c * sizeof(struct Unk02029A10Group)
                                + (u8 *)gUnknown_02029A10);
    e2->x += gUnknown_08553B28[c][w];
    e2->y -= gUnknown_085644D4[(row = gUnknown_02028E5C[c])[1]];
    if (gUnknown_02029B80[c][e] == 0)
    {
        if (e2->unk20 == 0)
            StepFigureBob(c, e, gUnknown_03001FBC);
    }
    else
    {
        PlayMusicOrSfx(0x23);
        gUnknown_02029B80[c][e] = 0;
        gUnknown_02029B94[c][e] = 1;
        e2->frame = 0;
        req.unk00 = c;
        req.unk02 = e;
        req.unk04 = gUnknown_085534BC[c * 2];
        req.unk06 = gUnknown_085534BC[c * 2 + 1];
        req.unk08 = 0x20;
        req.unk0a = 0x10;
        req.unk0c = 0x64;
        SetFigureSlide(&req);
    }
    SetSlotSpritePosition(gUnknown_03001FBC,
                 (e3 = (struct Unk02029A10 *)(e * sizeof(struct Unk02029A10)
                                              + c * sizeof(struct Unk02029A10Group)
                                              + (u8 *)gUnknown_02029A10))->x,
                 e3->y - *p);
}
asm(".global sub_0804EB78\n.thumb_set sub_0804EB78, TCopterFigure_Loop\n");
