#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015CE4.
 * sub_08015CE4 @ 0x08015CE4, sub_08015CF4 @ 0x08015CF4
 */

/* One step of the slot's command list: read an 8-byte record through the
 * .unk04 cursor, hand its two fields to sub_080152C0, advance the cursor and
 * report "keep going".
 *
 * The cursor is RE-READ from the struct after the call rather than advanced
 * from the local -- `ldr r0,[r4]; adds r0,#8; str r0,[r4]` off the same
 * `&g[a].unk04` the first load used. Writing `p + 1` with `p` the local is 4
 * bytes longer: it forces `p` into a second callee-saved register across the
 * call (`push {r4, r5, lr}`) instead of letting the call's memory clobber
 * force a reload. The address of the MEMBER is the thing CSE keeps, which is
 * why r4 holds `pool + 4 + a*0x60` and not `&g[a]`.
 */
struct Unk08015CF4
{
    /* 0x00 */ s32 unk00;
    /* 0x04 */ u8 unk04;
    /* 0x05 */ u8 filler_05[0x03];
};

/* A forwarder to the slot teardown that always reports 0. The `movs r0,#0`
 * before the epilogue is the only thing making it non-void; its single caller
 * discards the result, so nothing narrows the return type further.
 */
int SlotOp_End(u8 a)
{
    EndSlotScriptAt(a);

    return 0;
}
asm(".global sub_08015CE4\n.thumb_set sub_08015CE4, SlotOp_End\n");

int SlotOp_StartScript(u8 a)
{
    const struct Unk08015CF4 *p = gUnknown_03001470[a].unk04;

    sub_080152C0(p->unk00, p->unk04);
    gUnknown_03001470[a].unk04 =
        (const struct Unk08015CF4 *)gUnknown_03001470[a].unk04 + 1;

    return 1;
}
asm(".global sub_08015CF4\n.thumb_set sub_08015CF4, SlotOp_StartScript\n");
