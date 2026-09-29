#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BC3C.
 * sub_0801BC3C @ 0x0801BC3C
 */

/* MATCHED, wave 58 (W58-A). 108 bytes, `relocs: name different symbols that
 * resolve to the same address`. PROMOTION MUST CARRY "rodata": ["0x0808F08C"].
 *
 * Two facts closed this, and the second is new:
 *
 * 1. READ THE CURSOR INTO A LOCAL AT THE TOP OF THE BODY. Waves 40-42 wrote
 *    `*((u32 *)gUnknown_03002F2C)++ = ...` first and bound `dst` afterwards,
 *    which made the SECOND bump reload the global (`ldr r0,[r6]; add r1,r0,#4;
 *    str r1,[r6]`) instead of the ROM's `adds r3,#4`. Hoisting the read to the
 *    head of the body and expressing BOTH write-backs through the local gives
 *    the ROM's `ldr r3,[r6]` ... `adds r3,#4` exactly.
 *
 * 2. THE FIRST WRITE-BACK IS A VOLATILE STORE. With both write-backs written
 *    plainly, agbcc deletes the first one: flow.c's dead-store elimination sees
 *    a later store to the identical MEM and `insn_dead_p` kills the earlier one.
 *    Wave 42 concluded this was a dilemma (keeping the store forced the reload);
 *    it is not -- fact 1 removes the reload, and this removes the deletion.
 *
 * MEASURED THIS WAVE, and this is the reusable rule: an intervening
 * MAY-ALIASING STORE DOES NOT PROTECT A REDUNDANT GLOBAL WRITE-BACK. Both
 * `*(u16 *)dst = ...` and `*(volatile u16 *)dst = ...` between the two
 * write-backs compile byte-identically and the first `str` dies in both --
 * flow.c invalidates `mem_set_list` on a memory READ, never on a store. Nor
 * does re-reading the global at the halfword store help (`*(u16 *)
 * gUnknown_03002F2C = ...`): cse folds that load to the register before flow
 * ever runs, so there is no read left to invalidate anything. `insn_dead_p`
 * tests `! MEM_VOLATILE_P` on the store itself, so qualifying THAT store is
 * the only lever, and it costs zero bytes.
 *
 * Still valid from wave 40, not re-derived: gUnknown_0808F08C is not a pointer
 * global -- the word holds 0x03002620 == gUnknown_03002520 + 0x100, the end of
 * the 32-entry OAM shadow, and naming the real object is what gives the ROM's
 * two levels of indirection. The masks 0x1ff / 0xff pool correctly as written. */

struct Unk1BC3C
{
    /* 0x00 */ u32 unk00;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s16 unk08;
    /* 0x0a */ STRUCT_PAD(0x0a, 0x0c);
};

void PutOamHiOffsetList(struct Unk1BC3C *p, int a, int b)
{
    u32 *dst;
    u32 x;
    u32 y;

    while (p->unk00 != 1 && gUnknown_03002F2C < (void *)(gUnknown_03002520 + 128))
    {
        x = (p->unk06 + a) & 0x1ff;
        y = (p->unk08 + b) & 0xff;
        dst = (u32 *)gUnknown_03002F2C;
        *dst++ = p->unk00 | (x << 16) | y;
        *(u32 *volatile *)&gUnknown_03002F2C = dst;
        *(u16 *)dst = p->unk04;
        gUnknown_03002F2C = dst + 1;
        p++;
    }
}
asm(".global sub_0801BC3C\n.thumb_set sub_0801BC3C, PutOamHiOffsetList\n");
