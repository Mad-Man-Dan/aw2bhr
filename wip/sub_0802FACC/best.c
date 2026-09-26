#include "global.h"

/* Wave 80 (W80-E): STRUCTURAL ADVANCE, -12 -> -4 (1384/1388), adopted in the
 * body below.  The ROM's 0x1a/0x86 arm is
 *   if (pkt->unk00 != 0xAD) { next = i + 1; <0xAF part> }
 *   else                    { next = i + 1; <0xAD body> }
 * -- the 0xAD body sits OUT OF LINE after the 0xAF part (ROM `beq _0802FC6C`)
 * and BOTH branches begin with their own `next = i + 1` (ROM +0x166/+0x226).
 * This arm shape alone gains 8 bytes.  Three further results, cap reached:
 *   - Transcribing the ROM's remaining scattered `next = i + 1` sites into the
 *     case-4 arms as source statements (A5/A7/A9 before their tests,
 *     A6/A8/AA/AE at arm end, 0x84 at top) REGRESSES to 1440 (+52) and drops
 *     the r9 pool-address cache.  Those case-4 increments are NOT plain
 *     per-arm source statements; their origin is still open.
 *   - `register int i asm("r7")` (the ROM's register for i, W79-A lever 1)
 *     reports SIZE-EXACT 1388 BUT THE CODE IS WRONG: gcc 2.95 reuses r7 as
 *     the reload scratch for the gUnknown_020257E4 base inside the 0xAD copy
 *     loop while i is live.  Do not pin here, and check any pin's .s for
 *     writes to the pinned register.
 *   - REMAINING DIFF after the arm fix: ONE extra callee-saved value plus 4
 *     code bytes.  `next` lands in sl (ROM: r8) because the candidate still
 *     copies `i << 24` out of r4 (now into r8; the ROM keeps it in r4,
 *     uncopied): the candidate's 0xAD-body 0x32 base computes in r4 where the
 *     ROM's uses r5 -- a lo-reg rotation {i: r6 vs r7, pkt: r5 vs r6,
 *     arm-head &gUnknown_0849B018: r8 vs r5}.  The ROM's r5 pattern is cse
 *     mechanics: the copy loop's label ends the extended basic block, so the
 *     0xAD body's post-call reads re-derive &gUnknown_0849B018 from the pool
 *     word (fresh table) while the loop-free 0xAF path keeps the arm head's
 *     register across its call.
 */
/* Wave 72 (W72-C): an inline memory barrier around both sub_0802F460 results
 * does force the desired post-call state reloads, but leaves the extra sl
 * allocno and the 1376/1388 size unchanged; identity drops from 6.9% to 6.7%.
 * A generic memory-clobber barrier is therefore not the held-base lever.
 *
 * Wave 65 (W65-O): volatile pointer-object casts on the two first state reads
 * after sub_0802F460 were byte-neutral at 1376/1388.  They neither forced the
 * wanted post-call reload nor freed the displaced hard register, so local
 * volatile qualification is ruled out as the held-base lever.
 *
 * PARKED, wave 56 W56-I -- compiles, 1376 bytes against 1388 (-12), structure
 * verified against the ROM block by block.  THE REMAINING DEFECT IS REGISTER
 * ALLOCATION, ONE PSEUDO, AND IT IS FULLY DIAGNOSED:
 *
 *   The ROM saves r8 and r9 only (`mov r7,r9; mov r6,r8; push {r6,r7}`); this
 *   candidate saves r8, r9 AND sl.  The extra pseudo is a COPY of `i << 24`:
 *   the ROM computes `lsls r4, r7, #0x18` at the loop top and keeps the shifted
 *   value in r4 for the whole body (`asrs r0, r4, #0x18` at both
 *   sub_0802F460 call sites); this candidate emits the same shift into r4 and
 *   then `adds r7, r4, #0`, because r4 is wanted again inside the 0xAD arm for
 *   `&pkt->unk06` (the `adds rB,#0x32` base).  The ROM puts that base in r5,
 *   and it CAN, because r5 -- which held &gUnknown_0849B018 at the top of the
 *   0x1a/0x86 arm -- is already dead by then: the ROM RE-DERIVES the record
 *   from the pool word (`mov r2,r9; ldr r0,[r0]; ldr r1,[r0]`) at every
 *   reference in that arm.  This candidate instead caches &gUnknown_0849B018
 *   in r8 across the sub_0802F460 / sub_0802F588 calls, so it stays live, r5
 *   is unavailable, the 0x32 base lands in r4, `i << 24` is displaced, and the
 *   spill cascades into sl.
 *
 *   So the ONE thing to change is what makes agbcc re-read the -fforce-addr
 *   pool word after a call instead of keeping its result.  Everything else --
 *   control flow, the goto-tail structure, the packet layout, the switch trees
 *   -- already lines up.
 *
 * TWO THINGS ALREADY RULED OUT, do not re-run them:
 *   - A `union { u16; s8; u8; }` for the +2 field.  agbcc gives a union 4-byte
 *     alignment even when every member is <= 2 bytes, so it lands at +4 and
 *     pushes unk04/unk06 out by four.  Hence the three separate structs below.
 *   - Writing the four early-exit arms as inline `return`s.  gcc does not
 *     cross-jump them far enough and the constant 2 then survives in r4, which
 *     costs a further register.  The `goto abort` tail below is worth +24
 *     bytes and is how the ROM is shaped (the shared block sits immediately
 *     before the epilogue).
 *
 * PROMOTION WILL NEED "rodata": ["0x08090CA8"].
 */

/* WAVE 88 (W88-B). Verified via compile_probe that the draft still COMPILES
 * (worth running: sub_08050FF8 in the same batch had been non-compiling since
 * wave 86 and nothing noticed). No source change, deliberately: W56-I's
 * diagnosis holds and six levers are ruled out above. */

/* WAVE 88 (W88-C). THE ONE UNTESTED MECHANISM -- the wave-87 `static inline`
 * live-range-recut lever -- WAS TESTED, AND IT REACHES THIS RESIDUAL. It is
 * NOT a bare CSE-base-choice tie after all.  -4 -> size-exact, 6.9% -> 23.34%.
 *
 * W56-I named the exact requirement: "the ONE thing to change is what makes
 * agbcc re-read the pool word after a call instead of keeping its result".
 * Six levers failed at it (volatile casts W65-O, memory-clobber barrier W72-C,
 * register pins W80-E, the union, the inline returns, the transcribed
 * increments). Wrapping the chase in a helper and calling it at every
 * reference inside the 0x1a/0x86 arm DOES force the re-read:
 *     static __inline__ __typeof__(gUnknown_0849B018) lnk(void)
 *     { return gUnknown_0849B018; }
 * Each inlined call body gets its own pseudo, so cse cannot span the call
 * boundary -- precisely what W87 claims the lever does, and it transfers here
 * where the W83 narrow-copy form (W86-B: 0 of 5) does not.
 *
 * CAVEAT recorded at the time: the size-exactness is a COSTUME. The epilogue
 * was still `pop {r3,r4,r5} / mov r8,r3 / mov r9,r4 / mov sl,r5` against the
 * ROM's `pop {r3,r4} / ...`, i.e. the extra sl allocno W56-I diagnosed was
 * still there and the function had merely gained 4 bytes elsewhere. */

/* ===========================================================================
 * WAVE 89 (W89-B). THE THIRD CALLEE-SAVED REGISTER IS GONE. The callee-saved
 * set now MATCHES THE ROM EXACTLY -- r8 and r9 only:
 *     push {r4,r5,r6,r7,lr} / mov r7,r9 / mov r6,r8 / push {r6,r7}
 * This is the measurement to read on this function, not the percentage; W88-C
 * warned that `size: match` here was a costume and it was right.
 *
 * THE CAUSE WAS A MIXED SPELLING, NOT A MISSING LEVER. W88-C applied `lnk()`
 * ONLY inside the 0x1a/0x86 arm and left every other reference in the function
 * as a bare `gUnknown_0849B018`. Those are two DIFFERENT address expressions
 * for one symbol: the bare references make agbcc park the address in a
 * -fforce-addr `.rodata` word reached through one register, while the helper's
 * references get plain `.word gUnknown_0849B018` pool entries reached through
 * another. Two spellings of one symbol therefore kept TWO bases live at once,
 * and that second base -- not `i << 24`, not the 0x32 base -- is what forced
 * the third callee-saved register. W56-I's cascade (`r5 unavailable -> 0x32
 * base in r4 -> i<<24 displaced -> spill into sl`) was read correctly but from
 * the wrong end: the head of it is the duplicate base.
 *
 * THE FIX is to use `lnk()` at EVERY reference to gUnknown_0849B018 in the
 * function -- prologue guards, the unk1a increment, the case-4 arms, the
 * post-loop block and the abort tail included -- so exactly one address
 * expression exists. Nothing else changed.
 *
 * RULE (appended to docs/agbcc-codegen.md, wave 89 W89-B): a live-range-recut
 * helper must be applied to EVERY reference to its symbol, not only to the
 * references inside the region being fixed. A partially-applied helper is not
 * a weaker version of the lever -- it ADDS a live value, because the two
 * spellings are two address expressions that cse cannot merge. Measure a
 * helper by the callee-saved SET, which is immune to the positional score.
 *
 * Toolchain axis not re-swept (W81-E: 155 drafts x 7 profiles, zero flips).
 * PROMOTION WILL NEED "rodata": ["0x08090CA8"] (from W56-I) -- RE-CHECK, the
 * unified spelling may have changed which pool words are force-addr words.
 * =========================================================================== */

#include "global.h"

struct LinkPkt
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u8 unk06[13];
};

struct LinkPktS
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ s8 unk02;
};

struct LinkPktB
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 unk02;
};

static inline struct Unk0849B018 **lnkp(void)
{
    return &gUnknown_0849B018;
}

void sub_0802FACC(void)
{
  struct Unk08090CD8Entry *p;
  struct LinkPkt *pkt;
  int len;
  struct Unk0849B018 *new_var2;
  int i;
  int j;
  int n;
  struct LinkPkt *new_var3;
  int next;
  long new_var;
  if ((*lnkp())->unk04 <= 4)
  {
    return;
  }
  if ((*lnkp())->unk01 == 0)
  {
    return;
  }
  (*lnkp())->unk1a++;
  if ((*lnkp())->unk04 == 6)
  {
    if (((*lnkp())->unk1a - 1) > (0x3C - 1))
    {
      (*lnkp())->unk0a[(*lnkp())->unk06] = 0;
      goto abort;
    }
    if (((*lnkp())->unk01 != 0) && (sub_0802F408() == 0))
    {
      (*lnkp())->unk0a[(*lnkp())->unk06] = 0;
      goto abort;
    }
    if (((*lnkp())->unk01 == 2) && (((*lnkp())->unk1b > 0x78) || (gUnknown_0849B01C->unk04 == gUnknown_0849B01C->unk05)))
    {
      (*lnkp())->unk0a[(*lnkp())->unk06] = 0;
      goto abort;
    }
    for (i = 0; i <= 3; i++)
    {
      if ((*lnkp())->unk16[i] > 0x3C)
      {
        goto abort_i;
      }
    }

  }
  if ((*lnkp())->unk01 == 1)
  {
    (*lnkp())->unk1c |= 1 << (*lnkp())->unk06;
    i = 0;
    while (i <= 3)
    {
      again:
      n = sub_0802F6A0(i, (struct LinkPkt *) (*lnkp())->unk2c);

      next = i + 1;
      if (n > 0)
      {
        switch (n)
        {
          case 0x1A:

          case 0x86:
            pkt = (struct LinkPkt *) (*lnkp())->unk2c;
            if (pkt->unk00 != 0xAD)
          {
            next = i + 1;
            if ((pkt->unk00 == 0xAF) && (pkt->unk01 != (*lnkp())->unk06))
            {
              new_var3 = pkt;
              if ((*lnkp())->unk24[new_var3->unk01] != new_var3->unk02)
              {
                gUnknown_0202575C.unk00 = 0xAE;
                gUnknown_0202575C.unk01 = (*lnkp())->unk06;
                gUnknown_0202575C.unk02 = (*lnkp())->unk24[new_var3->unk01];
                sub_0802F588(&gUnknown_0202575C, 4);
                goto again;
              }
              sub_08030838((struct Unk08090CD8Entry *) new_var3);
              gUnknown_0202575C.unk00 = 0xAE;
              gUnknown_0202575C.unk01 = (*lnkp())->unk06;
              gUnknown_0202575C.unk02 = (*lnkp())->unk24[new_var3->unk01] + 1;
              sub_0802F588(&gUnknown_0202575C, 4);
            }
          }
          else
          {
            next = i + 1;
            for (j = 0; j <= 0xC; j++)
            {
              gUnknown_020257E4[i][j] = new_var3->unk06[j];
            }

            if ((((sub_0802F460(i) == 0) && ((*lnkp())->unk00 == ((struct LinkPktB *) new_var3)->unk02)) && ((*lnkp())->unk04 <= 5)) || (sub_0802F460(i) == 1))
            {
              if ((*lnkp())->unk06 == 0)
              {
                gUnknown_0202575C.unk00 = 0xA6;
                gUnknown_0202575C.unk01 = (*lnkp())->unk06;
                gUnknown_0202575C.unk02 = i;
                sub_0802F588(&gUnknown_0202575C, 4);
              }
            }
            else
              if ((*lnkp())->unk06 == 0)
            {
              new_var = ((*lnkp())->unk00 == ((struct LinkPktB *) new_var3)->unk02) ? (0xA5) : (0xA7);
              new_var2 = *lnkp();
              gUnknown_0202575C.unk00 = new_var;
              gUnknown_0202575C.unk01 = new_var2->unk06;
              gUnknown_0202575C.unk02 = i;
              sub_0802F588(&gUnknown_0202575C, 4);
            }
          }
            break;

          case 0x84:
            pkt = (struct LinkPkt *) (*lnkp())->unk2c;
            if (pkt->unk00 == 0xAB)
          {
            sub_08030038(i, (struct Unk02025564 *) (&pkt->unk04));
            gUnknown_03003F1C |= 1 << pkt->unk01;
          }
            break;

          case 4:
            pkt = (struct LinkPkt *) (*lnkp())->unk2c;
            switch (pkt->unk00)
          {
            case 0xA5:
              if (sub_0802F460(((struct LinkPktS *) pkt)->unk02) == 0)
            {
              (*lnkp())->unk0a[pkt->unk02] = 2;
              (*lnkp())->unk04 = 6;
            }
              break;

            case 0xA6:
              (*lnkp())->unk0a[pkt->unk02] = 5;
              (*lnkp())->unk09 |= 1 << pkt->unk02;
              (*lnkp())->unk16[pkt->unk02] = 0;
              break;

            case 0xA7:
              if (sub_0802F460(((struct LinkPktS *) pkt)->unk02) == 0)
            {
              (*lnkp())->unk0a[((*lnkp())->unk02 & 0x30) >> 4] = 2;
              (*lnkp())->unk0a[pkt->unk02] = 2;
              (*lnkp())->unk04 = 6;
            }
              break;

            case 0xA8:
              gUnknown_0300410C = gUnknown_030040CC;
              (*lnkp())->unk0a[pkt->unk01] = 0;
              (*lnkp())->unk09 &= ~(1 << pkt->unk01);
              break;

            case 0xA9:
              if (gUnknown_030032D8 == 0xD)
            {
              sub_0802FA9C(pkt->unk01);
            }
              break;

            case 0xAA:
              gUnknown_030044C4 |= 1 << i;
              break;

            case 0xAE:
              if (((gUnknown_030044D8 != 0) && (pkt->unk01 != (*lnkp())->unk06)) && (pkt->unk02 == ((u16) ((*lnkp())->unk22 + 1))))
            {
              (*lnkp())->unk1c |= 1 << i;
            }
            else
              if (pkt->unk01 == (*lnkp())->unk06)
            {
              goto again;
            }
              break;

          }

            break;

        }

      }
      i = next;
    }

    if ((*lnkp())->unk1d == 0)
    {
      if ((*lnkp())->unk1e > 0x3C)
      {
        sub_0802EA24();
        (*lnkp())->unk04 = 2;
        return;
      }
      p = sub_080307E0(&len);
      if (p != ((void *) 0))
      {
        if (sub_0802F588((struct Unk0202575C *) p, (u16) (len + 6)) > 0)
        {
          (*lnkp())->unk1d = 0;
          (*lnkp())->unk1e++;
          gUnknown_030044D8 = 1;
        }
        else
        {
          (*lnkp())->unk1d = 0;
          (*lnkp())->unk1e++;
        }
      }
    }
    if ((gUnknown_030044D8 != 0) && (((*lnkp())->unk1c & (*lnkp())->unk09) == (*lnkp())->unk09))
    {
      (*lnkp())->unk22++;
      (*lnkp())->unk12c[(*lnkp())->unk1aac].unk00 = 0;
      (*lnkp())->unk1aac++;
      (*lnkp())->unk1aac &= 0x1F;
      gUnknown_030044D8 = 0;
      (*lnkp())->unk1d = ((*lnkp())->unk1e = ((*lnkp())->unk1c = 0));
    }
    else
    {
      (*lnkp())->unk1d++;
      (*lnkp())->unk1d &= 0xF;
    }
  }
  else
    if (((*lnkp())->unk01 == 2) || ((*lnkp())->unk01 == 3))
  {
    if ((*lnkp())->unk06 == 0)
    {
      sub_0802F8FC((u16 *) (&gUnknown_0849B01C->unk06), 1);
      gUnknown_0849B01C->unk06 = 0x5FFF;
    }
  }
  return;
  abort_i:
  (*lnkp())->unk0a[i] = 0;

  abort:
  sub_0802EA24();

  (*lnkp())->unk04 = 2;
}

/* ===========================================================================
 * WAVE 89 (W89-F). THE W89-B TRADE IS RESOLVED: THIS DRAFT IS SIZE-EXACT AND
 * HAS THE ROM'S CALLEE-SAVED SET AT THE SAME TIME. Those two facts were
 * mutually exclusive in every draft before this one.
 *
 * READ THE STRUCTURE, NOT THE PERCENTAGE (wave-88 rule). This draft scores
 * 20.7% where the W88-C fixpoint scored 23.1%, and it is STRICTLY BETTER:
 *   W88-C fixpoint   size match, 23.1%, push {r4,r5,r6,r7,lr}+{r5,r6,r7}
 *                    = r8, r9, sl saved  -- WRONG, first difference +0x2
 *   THIS DRAFT       size match, 20.7%, push {r4,r5,r6,r7,lr}+{r6,r7}
 *                    = r8, r9 saved      -- ROM-EXACT, first difference +0xa
 * The whole 10-byte prologue is byte-identical to the ROM's
 * `push {r4,r5,r6,r7,lr} / mov r7,sb / mov r6,r8 / push {r6,r7} / sub sp,#4`,
 * and `sl` is never saved. The score fell because the first difference moved
 * 8 bytes LATER; it is positional.
 *
 * THE LEVER IS W89-B'S NAMED PROBE AND IT CONFIRMED: a helper returning the
 * ADDRESS, not the value.
 *     static inline struct Unk0849B018 **lnkp(void)
 *     { return &gUnknown_0849B018; }
 *     ... (*lnkp())->unk04 ...   at EVERY reference
 * `__typeof__(gUnknown_0849B018) *` and the explicit `struct Unk0849B018 **`
 * are byte-identical, and so are `__inline__` and `inline` -- but pycparser
 * rejects `__inline__` and `__typeof__`, so ONLY the `static inline` +
 * explicit-type spelling can be permuted. Use it.
 *
 * THE FULL SPELLING TABLE, all measured this wave under `configured`:
 *   spelling                       size    score   callee-saved   .rodata word
 *   all bare                        -4      6.3%   r8,r9,sl        YES
 *   mixed bare + lnk()  (W88-C)   match    23.1%   r8,r9,sl        yes
 *   all lnk()           (W89-B)    -4      11.5%   r8,r9  OK       no
 *   mixed bare + lnkp()           match    23.1%   r8,r9,sl        yes
 *   all lnkp()          THIS      match    20.7%   r8,r9  OK       no
 * Only the all-lnkp row gets both size and the save set. Any MIXTURE brings
 * back the third callee-saved register: a bare reference and a helper
 * reference are two address expressions cse never merges, and that is true of
 * `lnkp()` (which returns the same address the bare reference computes) just
 * as it was of `lnk()`. W89-B's rule that a helper must be applied at EVERY
 * reference is confirmed a second time, on a different helper.
 *
 * THE REMAINING RESIDUAL IS ONE NAMED FACT and the header already explains it.
 *   ROM:        ldr r1,[pc,#52] / ldr r5,[r1] / ldr r4,[r5] / ldrh r0,[r4,#4]
 *               / mov r9,r1          <- r9 keeps &(the .rodata cell)
 *   candidate:  ldr r4,[pc,#48] / ldr r2,[r4] / ldrh r0,[r2,#4]
 * The ROM reaches the record through the agbcc `-fforce-addr` address constant
 * at 0x08090CA8 (a .rodata cell holding &gUnknown_0849B018), one indirection
 * deeper than the candidate, whose pool word relocates straight to
 * gUnknown_0849B018. The candidate emits EIGHT plain `.word gUnknown_0849B018`
 * dump points and NO .rodata section at all.
 *
 * WHY, and it is a hard trade -- see include/unknown-globals.h:9170-9199
 * (wave 41 W41-E / wave 42 W42-K). That note names the control: sub_08030DEC
 * "names gUnknown_0849B018 ONCE, gets a direct `ldr =gUnknown_0849B018` with
 * no ROM word at all". A helper names the symbol exactly ONCE in the source
 * text, so it lands in the control's case and the force-addr cell disappears.
 * The honest repeated bare spelling is what reproduces the cell -- and it is
 * also what costs the third callee-saved register. Confirmed here: the all-bare
 * row is the only one that emits a `.section .rodata`.
 *
 * THIS ALSO BOUNDS W89-C's ADVICE. "Leave at least one bare reference or you
 * destroy the force-addr mechanism" does NOT work on this function: putting a
 * single bare reference back (tested at the loop-body site W86-F's rule names,
 * `gUnknown_0849B018->unk16[i]` inside `for (i = 0; i <= 3; i++)`) changed
 * nothing at all -- same 1388 bytes, same 20.7%, same prologue, still no
 * .rodata section. ONE bare reference is not enough to trigger the cell; the
 * trigger is the symbol being named MANY times, per W41-E's n=1 control. So
 * "leave one bare reference" is not a general repair, and the force-addr word
 * and the single base are genuinely exclusive in the C text here.
 *
 * PERMUTER: RAN FOR THE FIRST TIME EVER on this function (nothing blocked it
 * before -- it simply had never been tried). Chained from this draft:
 *     20.7% -> 25.72% -> 29.03% -> 31.05%, SIZE-EXACT at every rung.
 * Snapshots w89f-perm{1,2,3}-*.c. best.c holds the 31.05% chain point but it
 * is a 195KB header-expanded blob -- DO NOT PROMOTE IT and do not read
 * best.json as a statement about this draft. best.json was a fossil when this
 * wave started (13,252-byte wave-26 file, 23.3%, older than the draft); it has
 * been corrected to describe what best.c actually holds.
 *
 * Toolchain axis not re-swept (W81-E). Six earlier levers remain ruled out
 * (volatile casts W65-O, memory-clobber barrier W72-C, register pins W80-E,
 * the union, the inline returns, the transcribed `next = i + 1` increments).
 * PROMOTION: re-check the "rodata" entry -- this spelling emits NO .rodata
 * word, so W56-I's `"rodata": ["0x08090CA8"]` does NOT apply to this draft.
 * =========================================================================== */
