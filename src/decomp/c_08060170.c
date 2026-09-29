#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08060170.
 * sub_08060170 @ 0x08060170
 */

/* AiExecuteTurnEnd @ 0x08060170, 88 bytes. MATCHED in Wave 63 with a label/goto
 * loop. That suppresses loop.c entirely, so two explicit pointer variables
 * walk source and destination while the explicit counter remains ascending.
 *
 * PREVIOUSLY PARKED at 93.2%, SIZE EXACT (88/88),
 * 6 of 88 bytes differ, all inside the copy loop. Everything before the loop
 * is byte-exact, including the pool order and `adds r0,#0x2e`.
 *
 * Remaining diff, exactly three instructions:
 *
 *   original            candidate
 *   ldrb r0, [r2]       adds r0, r3, r2
 *                       ldrb r0, [r0]
 *   strb r0, [r1]       strb r0, [r1]
 *   adds r2, #1         --
 *
 * i.e. the ROM strength-reduces BOTH address givs (r2 walks the source, r1
 * walks the destination) and STILL keeps the ascending counter r3 for the exit
 * test. The candidate reduces only the destination giv and leaves the source
 * address as `base + i`.
 *
 * Measured this wave (W49-E), all by compile_probe:
 *
 *  - Any spelling that gets BOTH givs reduced also makes `i` dead, and then
 *    check_dbra_loop REVERSES the counter -- `movs rC,#3; subs rC,#1;
 *    cmp rC,#0; bge` instead of the ROM's `adds r3,#1; cmp r3,#3; ble`.
 *    Confirmed with four independent spellings: `filler_08[i+6]` flat
 *    indexing, `*d++ = *s++` on two local pointers, the same split into three
 *    statements, and `gUnknown_03004490[i+4] = *s++` (the last of these
 *    reproduces the ROM's loop-body instruction ORDER exactly -- ldrb, strb,
 *    inc src, inc dst, counter, cmp, branch -- and differs ONLY in the
 *    counter's direction).
 *  - Any spelling that keeps the counter ascending leaves the source giv
 *    unreduced, because `i` is then still used in the source address.
 *  - Adding ANY unrelated use of `i` inside the body gets both effects at once
 *    (both reduced AND ascending counter), which is what pins the cause: in
 *    the original, `i` must have a use that is not an address giv. No such use
 *    exists in the ROM's instruction stream, so the use has to be one that
 *    costs zero instructions -- not found.
 *
 * RULED OUT, with evidence:
 *  - gUnknown_03004490 is NOT volatile. A volatile destination does not stop
 *    the reversal (still `subs/cmp/bge`) AND it adds a dead `ldrb r1,[r2]`
 *    before the `strb`, which the ROM does not have. A volatile SOURCE is
 *    byte-neutral here and also does not stop the reversal.
 *  - Casting the struct base flat, `((u8 *)&gUnknown_030046C0)[i + 0xe]`, is
 *    byte-identical to the member-array spelling `gUnknown_030046C0.unk0e[i]`
 *    in this function's context: once the earlier statements have put
 *    &gUnknown_030046C0 in a register, the +0xe is hoisted either way and the
 *    component-ref association documented in src/decomp/c_08061178.c does not
 *    apply. (It DOES differ in isolation, where the flat form folds +0xe into
 *    the pool word.)
 *  - Binding either side to a local `u8 *` and subscripting it (`d[i] = s[i]`)
 *    reduces NEITHER giv -- two adds in the body, strictly worse.
 *
 * The struct carving this needed is committed: Unk030046C0 unk0c, unk0d and
 * unk0e[4], carved out of filler_08 in include/unknown-globals.h. That part is
 * independent of the loop shape and is proved by the pre-loop stores. */

void AiExecuteTurnEnd(void)
{
    int i;
    u8 *src;
    u8 *dst;

    gUnknown_030032D8 = 1;
    ReadyCurrentArmyUnits();
    gUnknown_03004780 = 0;
    gPlaySt.unk2e = gUnknown_030046C0.unk06;
    gUnknown_03004490[0] = gUnknown_030046C0.unk07;
    gUnknown_03004490[1] = gUnknown_030046C0.unk0c;
    gUnknown_03004490[2] = gUnknown_030046C0.unk0d;

    i = 0;
    src = gUnknown_030046C0.unk0e;
    dst = gUnknown_03004490 + 4;
loop:
    *dst = *src;
    src++;
    dst++;
    i++;
    if (i <= 3)
        goto loop;
}
asm(".global sub_08060170\n.thumb_set sub_08060170, AiExecuteTurnEnd\n");
