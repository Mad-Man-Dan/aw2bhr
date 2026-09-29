#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804EA54.
 * sub_0804EA54 @ 0x0804EA54, sub_0804EAEC @ 0x0804EAEC
 */

/* Queues one unit sprite's tile data into OBJ VRAM. FIVE BOUND LOCALS, and
 * every one of them is load-bearing -- this is the wave-32 worked example of
 * "binding locals are punctuation".
 *
 *   t    forces gUnknown_03004580 to the FRONT of the literal pool. Inline, the
 *        whole destination expression is evaluated after the source and the
 *        pool comes out reversed.
 *   o1   the source's byte offset. Second, because the ROM computes it before
 *        the destination even though it belongs to the first argument.
 *   o2   the destination's tile offset -- WITHOUT the 0x06010000, which the ROM
 *        adds last of all, after the source is finished. That split is why o2
 *        is a local and the VRAM base is not.
 *   k    the tile-source index. Bound, the inner subscript is evaluated before
 *        gUnknown_02029BA8's own pool word is loaded; inline, agbcc loads that
 *        base first and holds it across the whole chain, which costs a second
 *        high register.
 *   row  gives the gUnknown_085D6A48 ROW ADDRESS a second use, so [9] stays a
 *        `ldrh [rN, #0x12]` displacement instead of being folded into the base
 *        (`adds rN, #0x12`). Two bytes. Same lever as the `e1 = &pos[k]`
 *        binding in c_0804EEFC.c.
 *
 * The two shift pairs are MULTIPLIES, per docs/agbcc-codegen.md: `c * 0x480` is
 * `lsls #3; adds; lsls #0x17; lsrs #0x10` (times nine, then times 0x80, then
 * truncated to u16) and `x * 0x20` is `lsls #0x15; lsrs #0x10`. Written as
 * shifts they cost an instruction each. */
void StreamBodyPose(u16 a, u16 b, u16 c)
{
    u16 t;
    u16 o1;
    u16 o2;
    u16 k;
    u16 *row;

    t = gUnknown_03004580[a][1];
    o1 = c * 0x480;
    o2 = (a * 0x100 + b * (row = gUnknown_085D6A48[t])[9]) * 0x20;
    k = gUnknown_08551D1C[gUnknown_02029A10[a].entries[b].unk00];

    RegisterDataMove((u8 *)gUnknown_02029BA8[a].unk18[k] + o1,
                 (void *)(0x06010000 + o2), 0x480);
}
asm(".global sub_0804EA54\n.thumb_set sub_0804EA54, StreamBodyPose\n");

/* StreamBodyPose's sibling: the same queue with a 0x100-byte block, the source
 * offset stepped by 0x2d frames and the destination 0x28 tiles further on. See
 * the note on StreamBodyPose in src/decomp/c_0804EA54.c for what each of the five
 * bound locals buys -- they are the same five and they matter for the same
 * reasons.
 *
 * `(c + 0x2d) * 0x100` truncated to u16 is `lsls #0x18; <constant>; adds;
 * lsrs #0x10`: agbcc distributes the shift over the sum, so the 0x2d arrives
 * pre-shifted as `movs #0xb4; lsls #0x16`. The PROMOTE_MODE narrowing of `c`
 * disappears because `<< 24` discards everything it would have cleared. */
void StreamCrewPose(u16 a, u16 b, u16 c)
{
    u16 t;
    u16 o1;
    u16 o2;
    u16 k;
    u16 *row;

    t = gUnknown_03004580[a][1];
    o1 = (c + 0x2d) * 0x100;
    o2 = (a * 0x100 + b * (row = gUnknown_085D6A48[t])[9] + 0x28) * 0x20;
    k = gUnknown_08551D1C[gUnknown_02029A10[a].entries[b].unk00];

    RegisterDataMove((u8 *)gUnknown_02029BA8[a].unk18[k] + o1,
                 (void *)(0x06010000 + o2), 0x100);
}
asm(".global sub_0804EAEC\n.thumb_set sub_0804EAEC, StreamCrewPose\n");
