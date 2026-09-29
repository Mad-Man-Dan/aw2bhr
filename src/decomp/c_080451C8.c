#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080451C8.
 * sub_080451C8 @ 0x080451C8, sub_08045210 @ 0x08045210, sub_08045254 @ 0x08045254
 */

#include "hardware.h"
#include "proc.h"
struct UnkP45210
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x32);
    /* 32 */ s16 unk32;
};

/* The near-twin of sub_080039E4 (src/decomp/c_080039E4.c): the same
 * SetDefaultColorEffects call, the same byte-at-a-time read-modify-write of the BLDCNT
 * shadow, and the same pair of trailing halfword stores.
 *
 * This function does NOT have the discrimination problem that a pure OR-in
 * would: `movs r0,#0x3f; ands r0,r1` before the `orrs #0x40` is the CLEAR half
 * of a two-bit bitfield assignment, which is what `bits.effect = 1` emits and
 * what a plain `gU8 |= 0x40` cannot produce. The five single-bit fields after
 * it are byte-identical either way, but the model is already fixed by the
 * multi-bit field in the same statement group. */
void CoPowerOverlayBlend_Init(void)
{
    SetDefaultColorEffects();

    gUnknown_030030E0.bits.effect = 1;
    gUnknown_030030E0.bits.target1_enable_bg0 = 1;
    gUnknown_030030E0.bits.target2_enable_bg1 = 1;
    gUnknown_030030E0.bits.target2_enable_bg2 = 1;
    gUnknown_030030E0.bits.target2_enable_bg3 = 1;
    gUnknown_030030E0.bits.target2_enable_obj = 1;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0x10;
}
asm(".global sub_080451C8\n.thumb_set sub_080451C8, CoPowerOverlayBlend_Init\n");

/* A three-frame tick that raises gUnknown_03002020 and lowers
 * gUnknown_03002B28 until the first reaches 10.
 *
 * THERE IS NO gUnknown_08091394. The ROM word at 0x08091394 is 0x03002020,
 * i.e. &gUnknown_03002020, and 0x08091398 holds the same address for
 * CoPowerOverlayBlend_FadeOut -- they are agbcc's own `-fforce-addr` address-constant pool,
 * one private copy per function, exactly like the 0x0808E558 case written up
 * in include/unknown-globals.h. Naming the global honestly is what reproduces
 * the two-level `ldr r3,=<pool>; ldr r1,[r3]` and the reload before the second
 * read. Verified against the ROM image, not inferred. */
void CoPowerOverlayBlend_FadeIn(struct UnkP45210 *proc)
{
    if (proc->unk32++ > 1) {
        gUnknown_03002020++;
        gUnknown_03002B28--;
        proc->unk32 = 0;
    }
    if (gUnknown_03002020 == 10)
        Proc_Break(proc);
}
asm(".global sub_08045210\n.thumb_set sub_08045210, CoPowerOverlayBlend_FadeIn\n");

/* CoPowerOverlayBlend_FadeIn run backwards: gUnknown_03002020 counts DOWN to 0 while
 * gUnknown_03002B28 counts up. See CoPowerOverlayBlend_FadeIn for why the 0x08091398 pool
 * word is not a global. */
void CoPowerOverlayBlend_FadeOut(struct UnkP45210 *proc)
{
    if (proc->unk32++ > 1) {
        gUnknown_03002020--;
        gUnknown_03002B28++;
        proc->unk32 = 0;
    }
    if (gUnknown_03002020 == 0)
        Proc_Break(proc);
}
asm(".global sub_08045254\n.thumb_set sub_08045254, CoPowerOverlayBlend_FadeOut\n");
