#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049C38.
 * sub_08049C38 @ 0x08049C38
 */

/* MATCHED (wave 77, W77-I). Was parked at 97.9% since wave 36 with a
 * two-instruction residual inside `case 1`:
 *
 *   ROM        ldrb r0,[r2] / cmp r0,#0 / beq / adds r1,r0,#0 / lsrs r1,r1,#1
 *   candidate  ldrb r0,[r2] / lsls r0,r0,#24 / cmp r0,#0 / beq / lsrs r1,r0,#25
 *
 * agbcc fused the QImode zero-extension of the `ldrb` into both the compare and
 * the shift, reusing one `x << 24` for both (`(x << 24) >> 25` is `(u8)x >> 1`).
 *
 * THE LEVER IS THE WIDTH OF THE LOCAL, AND IT IS `u32`. Wave 36 measured `u8 v`
 * (removes the extend but destroys the address CSE -- 96.4%) and `int v` (a
 * signed `>>= 1` emits `asr` where the ROM has `lsr`), concluded the two effects
 * were coupled, and stopped. They are not coupled; both earlier spellings were
 * failing for the SAME reason from opposite sides. A `u32` local is an SImode
 * pseudo, so there is no QImode value for combine to fuse an extension into and
 * no sign to make the shift arithmetic: the load stays a plain zero-extending
 * `ldrb`, the compound `v >>= 1` gives the ROM's copy-then-shift-in-place
 * (`adds r1,r0,#0 / lsrs r1,r1,#1`), and the address CSE off &unk27 survives
 * because the local never participates in it.
 *
 * Also measured this wave and NOT the answer: binding the address as
 * `u8 *q = &proc->unk27;` and writing unk26 as `*(q - 1) += v;` to author the
 * ROM's `subs r2,#1` by hand -- 96.4%, the same place as every other `u8 v`
 * placement.
 *
 * Everything else was settled in wave 36 and is unchanged: the 0..3 switch, the
 * s16 unk1e/unk22/unk24 widths, `0x50 / (proc->unk28 + 3)` through __divsi3,
 * `proc->unk28 * 12` as `x*2 + x` then `<< 2`, and the tail call.
 *
 * gUnknown_0812A16C is the -fforce-addr word holding &gUnknown_084C3240.
 * ON PROMOTION this function needs:  "rodata": ["0x0812A16C"]
 */

struct Unk08049C38Proc
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
    /* 0x20 */ u8 filler_20[0x02];
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ u8 unk26;
    /* 0x27 */ u8 unk27;
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 unk29;
};

void DefeatFlow_BannerPiece_Step(struct Unk08049C38Proc *proc)
{
    switch (proc->unk24)
    {
    case 0:
        proc->unk26 >>= 1;
        if (proc->unk26 == 0)
        {
            proc->unk27 = 0x28;
            proc->unk24++;
        }
        break;

    case 1:
        if (proc->unk27 != 0)
        {
            u32 v;

            v = proc->unk27;
            v >>= 1;
            proc->unk27 = v;
            proc->unk26 += v;
        }
        else
        {
            proc->unk24++;
        }
        break;

    case 2:
        proc->unk26 >>= 1;
        if (proc->unk26 == 0)
        {
            proc->unk27 = 0x50 / (proc->unk28 + 3);

            if (proc->unk28 <= 9)
            {
                proc->unk28++;
                proc->unk24--;
            }
            else
            {
                proc->unk29 = proc->unk28 * 12;
                proc->unk24++;
            }
        }
        break;

    case 3:
        if (gUnknown_084C3240->unk2a == 0 && proc->unk22 == 3)
        {
            gUnknown_02028E3C++;
            gUnknown_084C3240->unk2a++;
        }

        proc->unk29--;
        if (proc->unk29 == 0)
        {
            proc->unk28 = 0;
            proc->unk24 -= 2;
        }
        break;
    }

    PutOamHi(proc->unk1e, (gUnknown_084C3240->unk20 - proc->unk26) & 0xff,
                 gUnknown_0848B698, (proc->unk22 << 4) + 0x31ca);
}
asm(".global sub_08049C38\n.thumb_set sub_08049C38, DefeatFlow_BannerPiece_Step\n");
