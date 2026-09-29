/* WAVE 87 (W87-F) -- pre-registered hoist-barrier hypothesis REFUTED.
 * SIX `do { } while (0)` placements (round the then-block carrying `i + 1`,
 * round loop 2's body, round the Decompress call, round the blit nest, and two
 * compositions) are ALL BYTE-IDENTICAL to the baseline. `i + 1` stays hoisted
 * to loop 2's dominator and the gUnknown_0200FC50 pool address stays in r8 in
 * every one. Reason: the hoist is a GCSE common-subexpression hoist to a
 * dominating block, decided BEFORE global.c runs; the wrapper only re-ranks
 * allocnos and cannot undo a pre-allocation pass. Draft unchanged at 18.3%.
 * The permuter run named by the wave-54/59 parks is STILL the open axis and the
 * tool works again. See work/sub_0807F434/W87-notes.md. */
#include "global.h"

/* sub_0807F434 @ 0x0807F434, 240 bytes, THUMB. PARKED, +8 bytes, and the
 * residual is REGISTER ALLOCATION with ONE root cause.  Re-verified by exit
 * code in wave 59 (W59-G): still +8, 18.3% identical, first difference +0xe.
 *
 * Screen setup: compacts gUnknown_030058E0 into gUnknown_030058D4 and calls
 * sub_08043B14 per entry, then sub_08043BA4 per entry except index 2, then
 * decompresses gUnknown_08234B10 into gUnknown_0200FC50 and blits it into
 * VRAM at 0x06015000 as 4 x 8 pages, then applies one OBJ palette and clears
 * proc->unk4c.
 *
 * The instruction stream is otherwise correct: both `bl`-conditioned loops,
 * the dbra rewrite of the inner blit loop, all three .rodata address-constant
 * words (0x081D936C/9370/9374) and their order, and the whole tail come out
 * exact. The +8 is four spots and three of them are downstream of the first:
 *
 *   1. LOOP 2 EMITS ONE EXTRA `adds r6, r5, #1` at the TOP of the body.
 *      `i + 1` is the third argument in the then-block AND the loop
 *      increment on both paths, so it is FULLY redundant and gcc hoists it to
 *      the dominator. The ROM computes `adds r2, r5, #1` inside the block and
 *      `adds r5, #1` at the merge, i.e. no hoist. That hoisted pseudo takes
 *      r6, and r6 is exactly the register the ROM wants for something else:
 *   2. the middle .rodata pool address (0x081D9370, for gUnknown_0200FC50)
 *      goes to r8 instead of the ROM's r6, so the prologue costs
 *      `ldr r0, [pc]; mov r8, r0` where the ROM has `ldr r6, [pc]` (+2);
 *   3. at the Decompress call that forces `mov r1, r8; ldr r1, [r1];
 *      mov r8, r1` where the ROM has `ldr r4, [r6]; adds r1, r4, #0` (+2),
 *      because a high register cannot be the base of the `ldr`;
 *   4. and in the blit nest j lands in r2 rather than r5 with src/dst swapped
 *      between r4 and r5 -- same instruction count, different bytes.
 *
 * RULED OUT for spot 1, each by controlled probe:
 *   - `if (i != 2) { ... }` and `if (i == 2) continue;` are identical here.
 *   - A separate counter `n = 1; ... n++;` is WORSE: gcc does not eliminate
 *     it against i, so the body gets two increments instead of one.
 * RULED OUT for the blit nest: naming `gUnknown_0200FC50` directly instead of
 * binding it to `p` loses the accumulator entirely (the base is re-`ldr`ed
 * from .rodata inside the innermost loop and no giv is formed), AND drops the
 * third .rodata word. Assigning `p` in a statement BEFORE the call also drops
 * the third .rodata word; `p = gUnknown_0200FC50` inside the argument is what
 * restores all three, and that is the form kept below.
 *
 * NEXT STEP: this is order-wrong/slot-wrong register allocation with the
 * instruction sequence otherwise correct, which docs/agbcc-codegen.md names as
 * decomp-permuter's actual case. Run the permuter on this draft, or find the
 * source form that stops `i + 1` being hoisted in loop 2 -- everything else
 * should follow from that one register.
 *
 * WAVE 59 (W59-G): THE PERMUTER STEP ABOVE WAS NOT RUN AND IS STILL OPEN.
 * `mcp permute` is broken -- it raises KeyError on 'exit_code' and strands raw
 * header-expanded output in work/<fn>/<fn>.c, destroying the draft (the docs
 * claiming this was fixed in wave 17 are wrong). Do NOT read this function's
 * axis list as exhausted: the single highest-value untried axis on it is a
 * permuter run, once the tool works, because a pure slot/order allocation
 * residual is the one class the permuter is documented to reach. Copy the
 * draft before running it. */
struct Unk0807F434
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ u16 unk4c;
};

void sub_0807F434(struct Unk0807F434 *proc)
{
  int i;
  int j;
  int k;
  u8 *p;
  int nv;
  int x;
  int *new_var;
  for (i = 0; i < sub_0803BD14(); i++)
  {
    new_var = &i;
    gUnknown_030058D4[i] = gUnknown_030058E0[i];
    sub_08043B14(gUnknown_030058D4[i], ((*new_var) * 12) + 0x3c0);
  }

  for (i = 0; i < sub_0803BD14(); i++)
  {
    if (i != 2)
    {
      sub_08043BA4(gUnknown_030058D4[i], DivRem(i, 2) * 320, -(~i));
    }
  }

  Decompress(gUnknown_08234B10, gUnknown_0200FC50);
  for (j = 0; j <= 3; )
  {
    int nj = j + 1;
    int lv;
    lv = j * 0x800;
    for (k = 0, nv = 0; k <= 7; k++)
    {
      CpuFastSet(&gUnknown_0200FC50[(j * 0x100) + (k * 0x400)], (void *) ((0x06015000 + lv) + nv), 0x40);
      nv += 0x100;
    }
    j = nj;
  }

  j = 0;
  ApplyPaletteExt(gUnknown_082352DC, 0x280, 0x20);
  proc->unk4c = j;
}
