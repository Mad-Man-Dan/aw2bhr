#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806412C.
 * sub_0806412C @ 0x0806412C
 */

/* Seeds the eight gUnknown_0202F140 vectors from the gUnknown_0858089C table
 * (scaled into 20.12), fills the six gUnknown_0202F110 byte rows from
 * gUnknown_085808CC, then stores the eight u16 parameters.
 *
 * STILL PARKED, wave 46 (W46-K, after W46-B), at 52.2% -- READ THE DIFF, NOT
 * THE SCORE. The function is SIZE-EXACT at 232 bytes and the residual is two
 * facts, both in the loop optimiser. W46-K was told this draft was
 * "structurally wrong" and should be deleted and re-derived; it is not, and the
 * brief's own wave-39 rule says so: a byte-identity percentage is POSITIONAL,
 * and one reversed counter shifts every later byte. Do not throw this away.
 *
 * (a) check_dbra_loop REVERSES the first loop's counter here and did not in the
 *     ROM. The ROM keeps `movs r1,#0` in the preheader with `adds r1,#1;
 *     cmp r1,#7; ble` at the bottom ALONGSIDE the 0x18 giv; this draft emits
 *     `movs r3,#7` with `subs r3,#1; cmp r3,#0; bge` and no separate init. That
 *     also costs the SECOND loop's `adds r3,r1,#1` / `adds r1,r3,#0` shuffle,
 *     which is that loop reusing r1 for both the counter and the row pointer.
 * (b) The second loop's base comes out as one pool word relocated
 *     `gUnknown_0202F110+0x2`, where the ROM materialises the bare symbol and
 *     biases it (`ldr r0,=g; adds r4,r0,#2`) so the single pool word is shared
 *     with the six halfword stores after the loop. ROM six pool words, draft
 *     seven.
 *
 * W46-K NARROWED (a): THE GIV IS ALREADY RIGHT, so the reversal is the whole
 * cause. The draft emits `adds rD, r5, r2` with the ROM's own operand order
 * (`adds r3, r5, r2`) and differs only in the DESTINATION register -- r3 is
 * unavailable precisely because the reversed counter took it. Defect (b) is
 * independent and worth one pool word.
 *
 * WHAT IS SETTLED, so a re-attempt does not re-derive it:
 *  - BINDING `a7 * 0x1000` AND `a8 * 0x1000` TO v7/v8 IS LOAD-BEARING and was
 *    the whole first half of the fix. Written inline at their two stores, the
 *    address of gUnknown_0202F140 wins ip, a7 spills to a fifth frame slot
 *    (`sub sp,#0x14` instead of `#0x10`, every stack argument +4), and the
 *    address is LICM-hoisted out of the first loop -- where the ROM
 *    rematerialises it every iteration. With v7/v8 bound, ip and r8 hold them,
 *    the frame is 0x10 and the hoist is gone. This is the wave-17 "two address
 *    constants tie and the loser is rematerialised" rule seen from the side
 *    that wins.
 *  - All eight parameters are u16; the first four really are spilled whole and
 *    read back with `ldrh`. PROMOTE_MODE plus pressure, not narrower locals.
 *  - Both tables are walked by ONE pointer never reset between rows.
 *
 * RULED OUT:
 *  - Loop FORM does not move the reversal: `for (; i != 8; i++)`,
 *    `for (i = 0; i < 8; i++)` and `do { } while (++i <= 7);` all reverse, with
 *    the inner loop ascending or descending, and with `i = 0` before or after
 *    the v7/v8 statements.
 *  - W46-K probed the obvious reading of the ROM's preheader -- that `movs
 *    r1,#0` sitting AHEAD of the v7/v8 insns means the source wrote `i = 0;` as
 *    its own statement there -- in two forms, `i = 0;` before v7/v8 with
 *    `for (; i <= 7; i++)`, and the same init with
 *    `do { ... i++; } while (i <= 7);`. BOTH STILL REVERSE. The counter init's
 *    source position is not the lever.
 *  - A GOTO loop DOES keep the counter ascending (loop.c never sees it -- see
 *    the goto-loop chapter in docs/agbcc-codegen.md), but it also loses
 *    strength reduction, so the 0x18 stride has to be an explicit source
 *    variable and agbcc then hoists that increment ABOVE the inner loop where
 *    the ROM has it at the bottom beside the counter. The ROM needs BOTH the
 *    giv and the un-reversed counter and nothing tried gives both.
 *  - Binding gUnknown_0202F110's base to a `u8 *` local to force the ROM's
 *    two-instruction preheader is much WORSE: agbcc emits a `.LC` rodata word
 *    holding the address and loads through it indirectly at every use.
 *
 * WAVE 73 (W73-G) -- THE LAST UNTESTED IDEA IN THIS HEADER IS REFUTED, AND
 * THE DISCRIMINATOR IS NOW NAMED.
 *
 * The idea was wave 25's rule read forwards: what blocks the reversal is a
 * SURVIVING USE of the index, so what use of `i` did the ROM's source have?
 * Probed by compile_probe: `gUnknown_030005F4 = i;` immediately AFTER the
 * first loop does NOT stop the reversal. The loop still comes out
 * `movs r3,#7` / `subs r3,#1` / `cmp r3,#0` / `bge`, and the added use is
 * satisfied with a constant, `mov r3,#8` -- gcc knows the biv's final value,
 * so a use after the loop costs check_dbra_loop nothing.
 *
 * The same probe supplies the control, because this function contains one.
 * The SECOND loop does not reverse: `mov r3,#0` / `add r3,r3,#1` /
 * `cmp r3,#5` / `ble`, ascending, same function, same flags. The difference is
 * that its address arithmetic is `lsl r0,r3,#3` -- `i * 8` is a single shift,
 * which strength_reduce does not find profitable, so `i` keeps a live in-body
 * use. The first loop's `i * 0x18` IS profitable, is reduced to the
 * accumulating giv, and leaves `i` used only by the exit test, which is
 * exactly check_dbra_loop's precondition.
 *
 * SO THE BLOCKING USE MUST BE INSIDE THE LOOP BODY AND MUST SURVIVE STRENGTH
 * REDUCTION. A use after the loop is worthless. The ROM has BOTH the reduced
 * 0x18 giv and the ascending counter, so its source must contain an in-body
 * use of `i` that is not itself reducible -- and nothing in the settled
 * statement set supplies one. That, not the loop form and not the init
 * position, is the whole remaining question.
 *
 * NEXT LEVER, one question only: suppress check_dbra_loop on the first loop
 * while KEEPING strength reduction. Every axis tried so far trades one for the
 * other. */
void sub_0806412C(u16 a1, u16 a2, u16 a3, u16 a4, u16 a5, u16 a6, u16 a7, u16 a8)
{
  const s16 *src;
  int new_var;
  const u8 *tbl;
  int *dst;
  u8 *q;
  u8 *base;
  int v7;
  struct Unk0202F110Entry *entries;
  int v8;
  int i;
  int j;
  src = gUnknown_0858089C;
  tbl = gUnknown_085808CC;
  new_var = 2;
  i = 0;
  v7 = a7 * 0x1000;
  for (; i <= 7; i++)
  {
    j = i;
    dst = gUnknown_0202F140[j].unk00;
    for (j = 2; j >= 0; j--)
    {
      v8 = a8 * 0x1000;
      *(dst++) = (*(src++)) * 0x1000;
    }

  }

  for (i = 0; i <= 5;)
  {
    int k = i + 1;
    entries = gUnknown_0202F110;
    base = (u8 *) entries;
    q = (u8 *)((i * 8) + (int)(base + new_var));
    for (j = 3; j >= 0; j--)
    {
      *(q++) = *(tbl++);
    }

    i = k;
  }

  gUnknown_0202F110[0].unk00 = a1;
  i = 4;
  gUnknown_0202F110[1].unk00 = a2;
  gUnknown_0202F110[2].unk00 = a3;
  gUnknown_0202F110[3].unk00 = a4;
  gUnknown_0202F110[i].unk00 = a5;
  gUnknown_0202F110[5].unk00 = a6;
  gUnknown_030005F4 = v7;
  gUnknown_030005F8 = v8;
}
