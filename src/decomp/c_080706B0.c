#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080706B0.
 * sub_080706B0 @ 0x080706B0
 */

/* MPlayExtender. The m4a driver's CGB-channel bring-up, and it is the MP2K
 * function of that name essentially line for line.
 *
 * Three parts, in source order:
 *   1. Poke the GBA's four PSG oscillators quiet -- SOUNDCNT_X on with all four
 *      channels enabled, master volume to zero, then NR12/NR22/NR42 = 8 and
 *      NR14/NR24/NR44 = 0x80 (envelope off + restart) and NR30 = 0, and only
 *      then SOUNDCNT_L back up to 0x77. The 0x04000080 pool word is shared by
 *      the halfword store at the top and the BYTE store at the bottom, which is
 *      why r3 stays live across the whole block.
 *   2. Patch nine handlers into the jump table at &gUnknown_03005740 --
 *      indices 8, 17, 19 and 28..33. Everything up to [31] fits in a word `str`
 *      displacement; [32] and [33] do not, hence the two `adds` at the end.
 *   3. Zero the four CgbChannel records with one CpuSet (0x05000040 == 0x40
 *      words, source fixed, 32-bit == 4 * sizeof(struct CgbChannel)) and seed
 *      each one's `ty` and `panMask`.
 *
 * The CpuSet source word is assigned at its USE for the same reason
 * src/decomp/c_080707F4.c records -- initialising it at the declaration moves
 * the stack store into the prologue.
 *
 * maxLines is NOT a literal zero: the ROM loads it from the literal pool
 * (`ldr r0,=0x00000000`) while the instruction beside it materialises the
 * CpuSet's zero with `movs r1,#0`. agbcc would have CSEd one literal into both,
 * so the pooled one is an absolute SYMBOL of value 0 -- MP2K's MAX_LINES. See
 * gMaxLines in include/unknown-globals.h.
 *
 * WAVE 60 (W60-B), TWO CORRECTIONS ABOUT THAT POOL WORD:
 *
 *  - The symbol is `gMaxLines`, defined as 0 on line 5 of aw2bhr.lds. W56-N
 *    spelled it `gUnknown_00000000` and recorded that nothing defined it and
 *    that a promotion would need a new linker-script line. Both were wrong; the
 *    definition was already there. Renamed here.
 *  - THE POOL WORD IS NOT ONE OF THE 18 DIFFERING BYTES, and a wave-60 brief
 *    said it was. Because gMaxLines links to 0, the ROM's `.4byte 0x00000000`
 *    and the object's `.word 0` are byte-IDENTICAL at +0x110; only the
 *    relocation differed, which used to make the function unmatchable in
 *    principle but contributed no differing byte. So the 18 bytes were never
 *    18-minus-one: every one of them is the r4/r5 allocation described below,
 *    and fixing the oracle did not and could not move the score. Re-measured at
 *    93.6% after the rename.
 *
 * MATCHED in wave 60 (W60-B). IT TOOK BOTH HALVES AND NEITHER ALONE WOULD HAVE
 * DONE IT, which is the useful thing to know about this function:
 *
 *   1. decomp-permuter (SHELL tools/permute.py) closed the r4/r5 allocation
 *      described below. It had never been run on this function -- the park
 *      listed the spelling sweep and the old_agbcc probe, not the permuter.
 *   2. The relocation on the MAX_LINES pool word still had to be accepted by
 *      trymatch. Even at ZERO differing bytes this function reported
 *      "bytes: match, but relocations differ", because reloc_equivalent()
 *      paired relocations by position and the candidate carries one the ROM
 *      does not. Fixed in the same wave; see the "Absolute symbols" chapter of
 *      docs/agbcc-codegen.md.
 *
 * THE TAIL BELOW IS A PERMUTER ARTEFACT AND SHOULD NOT BE TIDIED. `new_var2 =
 * &cgbChans;` with the last store written `(*new_var2)->panMask` is not
 * plausible original source, but the MECHANISM it exploits is exactly the one
 * the park predicted was needed: taking the parameter's address changes how
 * that pseudo is ranked, so `soundInfo` wins r4 and the parameter takes r5 --
 * the ROM's order. A future agent may find the honest spelling that does the
 * same thing; the note below records everything already ruled out. Whoever
 * tries: re-run try_match after ANY edit here, including cosmetic ones.
 *
 * WHAT THE RESIDUAL WAS, at size EXACT 280 bytes, 18 of 280 differing (93.6%).
 * Everything above the last CgbChannel was byte-exact -- the whole hardware
 * block, the ident guard, all nine jump-table stores including the two `adds`
 * that fall off the end of the word displacement, the three hook stores, the
 * pooled MAX_LINES word and the CpuSet. The residual was ONE coupled pair of
 * facts about the FOURTH channel's two stores, pulling against each other:
 *
 *   ROM      adds r1, #0x24  /  strb r0,[r1,#1]  /  strb r0,[r1,#0x1c]
 *   `[3]`    adds r1, #0x25  /  strb r0,[r1]     / adds r1,#0x1b / strb r0,[r1]
 *   `+= 3`   adds r4, #0xc0  /  strb r0,[r4,#1]  /  strb r0,[r4,#0x1c]
 *
 * The ROM keeps the RECORD base (+0xc0) and folds `ty` and `panMask` into the
 * store displacements. Written as `cgbChans[3].ty` agbcc folds the member
 * offset into the ADDRESS instead and needs one extra `adds` (+2 bytes, +2
 * more of alignment padding, 81.1% at +4). The ONLY spelling that reproduces
 * the record base is reassigning the PARAMETER (`cgbChans += 3`, used here) --
 * a separate local, `&cgbChans[3]`, `(chan = cgbChans + 3)->ty`, a `u8 *`
 * cursor and a `chan++` walk were all probed and all fold identically.
 *
 * But reassigning the parameter costs one extra ref on that pseudo, which
 * makes it TIE with soundInfo on allocno priority, and the tie-break is
 * allocno number, so the parameter (created first) takes r4 and soundInfo is
 * pushed to r5 -- the ROM has them the other way round. That accounts for all
 * 18 differing bytes. Counting the ROM's own references confirms the true
 * source did NOT reassign the parameter: r5 is referenced exactly six times
 * and NEVER holds 0xc0, so the record base came out of cse's address chain
 * (r1 + 0x24), which neither compiler will produce from any of the spellings
 * above.
 *
 * NOT the compiler configuration: byte-for-byte identical under old_agbcc
 * (93.6% for this source, 81.1% for the `[3]` source), so the m4a-region
 * override does not apply here. Do NOT add an override entry for it.
 */
void MPlayExtender(struct CgbChannel *cgbChans)
{
  struct CgbChannel **new_var2;
  struct SoundInfo *soundInfo;
  int new_var;
  void (**jumpTable)(void *, void *);
  u32 ident;
  u32 zero;
  *((vu16 *) 0x04000084) = 0x8f;
  *((vu16 *) 0x04000080) = 0;
  *((vu8 *) 0x04000063) = 8;
  *((vu8 *) 0x04000069) = 8;
  *((vu8 *) 0x04000079) = 8;
  *((vu8 *) 0x04000065) = 0x80;
  *((vu8 *) 0x0400006d) = 0x80;
  *((vu8 *) 0x0400007d) = 0x80;
  *((vu8 *) 0x04000070) = 0;
  *((vu8 *) 0x04000080) = 0x77;
  soundInfo = gUnknown_03007FF0;
  ident = soundInfo->ident;
  if (ident != MPLAY_ID_NUMBER)
  {
    return;
  }
  soundInfo->ident = ident + 1;
  jumpTable = &gUnknown_03005740;
  jumpTable[8] = (void (*)(void *, void *)) ply_memacc;
  jumpTable[17] = (void (*)(void *, void *)) sub_08070328;
  new_var = 19;
  jumpTable[new_var] = (void (*)(void *, void *)) sub_0807033C;
  jumpTable[28] = (void (*)(void *, void *)) ply_xcmd;
  jumpTable[29] = (void (*)(void *, void *)) sub_080702C0;
  jumpTable[30] = (void (*)(void *, void *)) SampFreqSet_rev01;
  jumpTable[31] = (void (*)(void *, void *)) sub_0807004C;
  jumpTable[32] = (void (*)(void *, void *)) FadeOutBody_rev01;
  jumpTable[33] = (void (*)(void *, void *)) TrkVolPitSet_rev01;
  soundInfo->cgbChans = cgbChans;
  soundInfo->unk28 = CgbSound;
  soundInfo->unk2c = CgbOscOff;
  soundInfo->unk30 = (void (*)(void)) MidiKey2CgbFr;
  soundInfo->maxLines = (u8) ((u32) (&gMaxLines));
  zero = 0;
  CpuSet(&zero, cgbChans, 0x05000040);
  cgbChans[0].ty = 1;
  cgbChans[0].panMask = 0x11;
  cgbChans[1].ty = 2;
  cgbChans[1].panMask = 0x22;
  cgbChans[2].ty = 3;
  cgbChans[2].panMask = 0x44;
  cgbChans += 3;
  new_var2 = &cgbChans;
  cgbChans->ty = 4;
  (*new_var2)->panMask = 0x88;
  soundInfo->ident = ident;
}
asm(".global sub_080706B0\n.thumb_set sub_080706B0, MPlayExtender\n");
