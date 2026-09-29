#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BF10.
 * sub_0803BF10 @ 0x0803BF10
 */

/* MATCHED wave 59 (W59-D), 96/96 bytes, relocs match. Parked at 93.8% since
 * wave 32 and re-derived from scratch in wave 35; what closed it was not a new
 * spelling of the address but the discovery that the DRAFT NO LONGER COMPILED.
 *
 * Wave 37 (W37-Q1) re-carved struct PlaySt's tail into four parallel
 * 5-byte slot arrays at 0x33/0x38/0x3d/0x42 and deleted the member `unk3e`
 * this draft was written against. The promoted files that used the old names
 * were re-verified at the time; the PARKED DRAFT WAS NOT, because nothing
 * recompiles a parked draft. It sat for 22 waves failing to compile at all,
 * and every "still 93.8%" in the note it carried was a stale number.
 *
 * The fix is the access the header had already predicted in its own comment on
 * unk3d: this function's cursor is biased, so it addresses unk3d[1..] as the
 * old unk3e[0..]. Writing it that way -- `q->co[i + 1]` -- produces the
 * ROM's five instructions exactly, and it is the same index expression the
 * MATCHED ApplyMatchSettingsRecord uses against all four of these arrays.
 *
 * WHY IT WORKS, and this is the transferable part. The wanted code is
 *     ldr r1,=gPlaySt ; ldrb r0,[..,#1] ; adds r1,r1,r0 ; adds r1,#0x3e
 * -- the variable index added to the base FIRST, the constant second, with the
 * base register winning the destination. Four spellings failed to produce it,
 * all of them trying to control the address arithmetic directly. What produces
 * it is having TWO constants to fold: the member offset 0x3d and the index bias
 * +1. fold reassociates `(q + 0x3d) + (i + 1)` into `(q + i) + 0x3e`, which is
 * the ROM. A single constant has nothing to reassociate, so `unk3e[i]`
 * materialises `q + 0x3e` as the array address and adds the index second, and
 * `(u8 *)&g + i` then `[0x3e]` gets the order right but lets the index win the
 * destination. See docs/agbcc-codegen.md.
 *
 * The rest of the body was already instruction-exact, including `strh r1,[r3]`
 * in the zero arm -- cse.c knows r1 is 0 on that edge, so `gUnknown_03002F1C =
 * 0;` is the right spelling and the `adds r1,r0,#0` copy is not a second
 * variable. */
void sub_0803BF10(void)
{
    struct PlaySt *q;

    if (gUnknown_03002F1C != 0)
    {
        if (gUnknown_0849ECDC->unk01 != 0)
            gUnknown_0849ECDC->unk01--;
        else
            gUnknown_03002F1C = 0;
    }
    else
    {
        q = &gPlaySt;
        q->co[gUnknown_0849ECDC->unk01 + 1] = gUnknown_020288B0;

        gUnknown_0849ECDC->unk01++;

        if (gUnknown_0849ECDC->unk01 != gUnknown_0849ECDC->unk00)
            gUnknown_03002F1C = 1;
    }
}
