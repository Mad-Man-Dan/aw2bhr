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
 * NOT a bare CSE-base-choice tie after all.
 *
 * W56-I named the exact requirement: "the ONE thing to change is what makes
 * agbcc re-read the pool word after a call instead of keeping its result".
 * Six levers failed at it (volatile casts W65-O, memory-clobber barrier W72-C,
 * register pins W80-E, the union, the inline returns, the transcribed
 * increments). Wrapping the chase in a helper and calling it at every
 * reference inside the 0x1a/0x86 arm DOES force the re-read:
 *     static __inline__ __typeof__(gUnknown_0849B018) lnk(void)
 *     { return gUnknown_0849B018; }
 *     ... pkt = (struct LinkPkt *)gUnknown_0849B018->unk2c;  ... gUnknown_0849B018->unk06 ...
 * MEASURED: the arm head becomes `ldr r0,.L102 / mov r8,r0 / ldr r2,[r0]` and
 * every later reference in the arm re-derives -- `mov r1,r8 / ldr r2,[r1]`
 * after `bl sub_08030838`, and a fresh `ldr r0,.L106+4 / ldr r1,[r0]` after
 * EACH of the two `bl sub_0802F460` calls (the exact two call sites W56-I
 * named). The cached-across-the-call pseudo is gone. Each inlined call body
 * gets its own pseudo, so cse cannot span the call boundary -- which is
 * precisely what W87 claims the lever does, and it transfers here where the
 * W83 narrow-copy form (W86-B: 0 of 5) does not.
 *
 * CAVEAT, and it is why this is not yet a match: the ROM re-derives through
 * the -fforce-addr word in THREE instructions (`mov r2,r9 / ldr r0,[r2] /
 * ldr r1,[r0]`), while the helper re-derives through a PLAIN pool word in TWO
 * (`mov r1,r8 / ldr r2,[r1]`). The draft now emits both kinds -- the .rodata
 * force-addr word `.LC1` still serves the function head (reached via sl) while
 * the arm gets plain `.word gUnknown_0849B018` pool words. So the lever fixed
 * the LIVE RANGE and changed the ADDRESSING MODE at the same time; the next
 * step is to keep the recut while routing the helper's return through the
 * force-addr word, which is the same address-vs-value distinction measured on
 * sub_08050FF8 this wave (bind the ADDRESS, not the value).
 *
 * Toolchain axis not re-swept (W81-E: 155 drafts x 7 profiles, zero flips). */

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

void sub_0802FACC(void)
{
  struct Unk08090CD8Entry *p;
  struct LinkPkt *pkt;
  int len;
  int i;
  int iFirst;
  int j;
  int n;
  int next;
  if (gUnknown_0849B018->unk04 <= 4)
  {
    return;
  }
  if (gUnknown_0849B018->unk01 == 0)
  {
    return;
  }
  gUnknown_0849B018->unk1a++;
  if (gUnknown_0849B018->unk04 == 6)
  {
    if (gUnknown_0849B018->unk1a > 0x3C)
    {
      gUnknown_0849B018->unk0a[gUnknown_0849B018->unk06] = 0;
      goto abort;
    }
    if ((gUnknown_0849B018->unk01 != 0) && (sub_0802F408() == 0))
    {
      gUnknown_0849B018->unk0a[gUnknown_0849B018->unk06] = 0;
      goto abort;
    }
    if ((gUnknown_0849B018->unk01 == 2) && ((gUnknown_0849B018->unk1b > 0x78) || (gUnknown_0849B01C->unk04 == gUnknown_0849B01C->unk05)))
    {
      gUnknown_0849B018->unk0a[gUnknown_0849B018->unk06] = 0;
      goto abort;
    }
    for (i = 0; i <= 3; i++)
    {
      if (gUnknown_0849B018->unk16[i] > 0x3C)
      {
        goto abort_i;
      }
    }

  }
  if (gUnknown_0849B018->unk01 == 1)
  {
    gUnknown_0849B018->unk1c |= 1 << gUnknown_0849B018->unk06;
    i = 0;
    while (i <= 3)
    {
      again:
      n = sub_0802F6A0(i, (struct LinkPkt *) gUnknown_0849B018->unk2c);

      next = i + 1;
      if (n > 0)
      {
        switch (n)
        {
          case 0x1A:

          case 0x86:
            pkt = (struct LinkPkt *) gUnknown_0849B018->unk2c;
            if (pkt->unk00 != 0xAD)
          {
            next = i + 1;
            if ((pkt->unk00 == 0xAF) && (pkt->unk01 != gUnknown_0849B018->unk06))
            {
              if (gUnknown_0849B018->unk24[pkt->unk01] != pkt->unk02)
              {
                gUnknown_0202575C.unk00 = 0xAE;
                gUnknown_0202575C.unk01 = gUnknown_0849B018->unk06;
                gUnknown_0202575C.unk02 = gUnknown_0849B018->unk24[pkt->unk01];
                sub_0802F588(&gUnknown_0202575C, 4);
                goto again;
              }
              sub_08030838((struct Unk08090CD8Entry *) pkt);
              gUnknown_0202575C.unk00 = 0xAE;
              gUnknown_0202575C.unk01 = gUnknown_0849B018->unk06;
              gUnknown_0202575C.unk02 = gUnknown_0849B018->unk24[pkt->unk01] + 1;
              sub_0802F588(&gUnknown_0202575C, 4);
            }
          }
          else
          {
            iFirst = i;
            next = i + 1;
            for (j = 0; j <= 0xC; j++)
            {
              gUnknown_020257E4[i][j] = pkt->unk06[j];
            }

            if ((((sub_0802F460(iFirst) == 0) && (gUnknown_0849B018->unk00 == ((struct LinkPktB *) pkt)->unk02)) && (gUnknown_0849B018->unk04 <= 5)) || (sub_0802F460(i) == 1))
            {
              if (gUnknown_0849B018->unk06 == 0)
              {
                gUnknown_0202575C.unk00 = 0xA6;
                gUnknown_0202575C.unk01 = gUnknown_0849B018->unk06;
                gUnknown_0202575C.unk02 = i;
                sub_0802F588(&gUnknown_0202575C, 4);
              }
            }
            else
              if (gUnknown_0849B018->unk06 == 0)
            {
              gUnknown_0202575C.unk00 = (gUnknown_0849B018->unk00 == ((struct LinkPktB *) pkt)->unk02) ? (0xA5) : (0xA7);
              gUnknown_0202575C.unk01 = gUnknown_0849B018->unk06;
              gUnknown_0202575C.unk02 = i;
              sub_0802F588(&gUnknown_0202575C, 4);
            }
          }
            break;

          case 0x84:
            pkt = (struct LinkPkt *) gUnknown_0849B018->unk2c;
            if (pkt->unk00 == 0xAB)
          {
            sub_08030038(i, (struct Unk02025564 *) (&pkt->unk04));
            gUnknown_03003F1C |= 1 << pkt->unk01;
          }
            break;

          case 4:
            pkt = (struct LinkPkt *) gUnknown_0849B018->unk2c;
            switch (pkt->unk00)
          {
            case 0xA5:
              if (sub_0802F460(((struct LinkPktS *) pkt)->unk02) == 0)
            {
              gUnknown_0849B018->unk0a[pkt->unk02] = 2;
              gUnknown_0849B018->unk04 = 6;
            }
              break;

            case 0xA6:
              gUnknown_0849B018->unk0a[pkt->unk02] = 5;
              gUnknown_0849B018->unk09 |= 1 << pkt->unk02;
              gUnknown_0849B018->unk16[pkt->unk02] = 0;
              break;

            case 0xA7:
              if (sub_0802F460(((struct LinkPktS *) pkt)->unk02) == 0)
            {
              gUnknown_0849B018->unk0a[(gUnknown_0849B018->unk02 & 0x30) >> 4] = 2;
              gUnknown_0849B018->unk0a[pkt->unk02] = 2;
              gUnknown_0849B018->unk04 = 6;
            }
              break;

            case 0xA8:
              gUnknown_0300410C = gUnknown_030040CC;
              gUnknown_0849B018->unk0a[pkt->unk01] = 0;
              gUnknown_0849B018->unk09 &= ~(1 << pkt->unk01);
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
 do { if (((gUnknown_030044D8 != 0) && (pkt->unk01 != gUnknown_0849B018->unk06)) && (pkt->unk02 == ((u16) (gUnknown_0849B018->unk22 + 1)))) { gUnknown_0849B018->unk1c |= 1 << i; } else if (pkt->unk01 == gUnknown_0849B018->unk06) { goto again; } } while (0);
              break;

          }

            break;

        }

      }
      i = next;
    }

    if (gUnknown_0849B018->unk1d == 0)
    {
      if (gUnknown_0849B018->unk1e > 0x3C)
      {
        sub_0802EA24();
        gUnknown_0849B018->unk04 = 2;
        return;
      }
      p = sub_080307E0(&len);
      if (p != ((void *) 0))
      {
        if (sub_0802F588((struct Unk0202575C *) p, (u16) (len + 6)) > 0)
        {
          gUnknown_0849B018->unk1d = 0;
          gUnknown_0849B018->unk1e++;
          gUnknown_030044D8 = 1;
        }
        else
        {
          gUnknown_0849B018->unk1d = 0;
          gUnknown_0849B018->unk1e++;
        }
      }
    }
    if ((gUnknown_030044D8 != 0) && ((gUnknown_0849B018->unk1c & gUnknown_0849B018->unk09) == gUnknown_0849B018->unk09))
    {
      gUnknown_0849B018->unk22++;
      gUnknown_0849B018->unk12c[gUnknown_0849B018->unk1aac].unk00 = 0;
      gUnknown_0849B018->unk1aac++;
      gUnknown_0849B018->unk1aac &= 0x1F;
      gUnknown_030044D8 = 0;
      gUnknown_0849B018->unk1d = (gUnknown_0849B018->unk1e = (gUnknown_0849B018->unk1c = 0));
    }
    else
    {
      gUnknown_0849B018->unk1d++;
      gUnknown_0849B018->unk1d &= 0xF;
    }
  }
  else
    if ((gUnknown_0849B018->unk01 == 2) || (gUnknown_0849B018->unk01 == 3))
  {
    if (gUnknown_0849B018->unk06 == 0)
    {
      sub_0802F8FC((u16 *) (&gUnknown_0849B01C->unk06), 1);
      gUnknown_0849B01C->unk06 = 0x5FFF;
    }
  }
  return;
  abort_i:
  gUnknown_0849B018->unk0a[i] = 0;

  abort:
  sub_0802EA24();

  gUnknown_0849B018->unk04 = 2;
}
