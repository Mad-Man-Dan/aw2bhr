#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C53C.
 * sub_0801C53C @ 0x0801C53C
 */

#include "hardware.h"
/* One OAM "sprite script" frame published into OBJ VRAM.
 *
 * a1->unk10 points at a u16 blob laid out as [count][count x 3 OAM halfwords]
 * [count x 1 tile index], so `oam` walks by 3 and `chr` by 1 -- which is what
 * fixes the two induction variables (r5 stepping 6 bytes, sl stepping 2).
 *
 * `while (n-- > 0)` and not `for (i = 0; i < n; i++)`: the ROM tests the
 * PRE-decrement value (`adds r0,r6,#0; subs r6,#1; cmp r0,#0; bgt`) with the
 * first decrement already done in the preheader, and the entry guard is the
 * signed `ble`. A `for` loop over the same range gives agbcc's other dbra
 * form -- an unsigned `beq` guard and a 3-instruction `sub; cmp; bne` bottom.
 *
 * gDispIo is re-read every iteration because RegisterDataMove is a call and
 * clobbers memory; for the same reason both table lookups are recomputed after
 * the call rather than CSEd, which is why the index expression is written out
 * twice rather than bound to a local.
 *
 * The second argument's grouping is measured, not guessed. Written as any of
 * `(u8 *)0x06010000 + off + X`, `(u8 *)(0x06010000 + off) + X` or
 * `X + (u8 *)(0x06010000 + off)`, fold reassociates to `(X + 0x06010000) + off`
 * and emits the pool constant BEFORE the `ldr [sp]`. Casting the tile-base term
 * to a pointer instead leaves `off + 0x06010000` as an int sum that fold will
 * not break up, which is the ROM's `ldr r3,[sp]; ldr r4,=0x06010000;
 * adds r2,r3,r4; adds r1,r1,r2`.
 *
 * Likewise `w * (h << 5)` and not `w * h * 0x20`: with a MULT the constant is
 * reassociated onto the first operand (`lsl` on w), with a shift it stays on
 * the second, which is the ROM's `ldrb; lsls #5; muls`. */

#define OBJ_SZ_IDX(o) ((((o)[0] & 0xc000) >> 12) + (((o)[1] & 0xc000) >> 14)) * 2

void AP_QueueObjGraphics(struct Unk0801C210 *a1)
{
    int n;
    int off;
    u16 *oam;
    u16 *chr;

    if (a1 != NULL && a1->unk00 != NULL)
    {
        n = a1->unk10[0];
        oam = a1->unk10 + 1;
        chr = a1->unk10 + (n * 3 + 1);
        off = 0;

        while (n-- > 0)
        {
            if (gDispIo.disp_ct.obj_mapping)
            {
                RegisterDataMove((u8 *)a1->unk24 + ((*chr & 0x3ff) << 5),
                    (u8 *)((a1->unk22 & 0x3ff) << 5) + (off + 0x06010000),
                    gUnknown_0848B5C4[OBJ_SZ_IDX(oam)]
                        * (gUnknown_0848B5C4[OBJ_SZ_IDX(oam) + 1] << 5));

                off += ((gUnknown_0848B5C4[OBJ_SZ_IDX(oam)]
                    * gUnknown_0848B5C4[OBJ_SZ_IDX(oam) + 1]) & 0x3ff) << 5;
            }

            oam += 3;
            chr++;
        }

        a1->unk20 &= 0xfe;
    }
}
asm(".global sub_0801C53C\n.thumb_set sub_0801C53C, AP_QueueObjGraphics\n");
