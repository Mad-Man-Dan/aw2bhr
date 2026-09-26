#include "global.h"
#include "hardware.h"
#include "proc.h"

/* WAVE 71 (W71-F): configured refresh remains size-exact 816/816 at 75.9%.
 * The established permuter-derived control flow was preserved; no stale
 * header-regression candidate and no further permuter search were used.
 *
 * PARKED at 75.9%, SIZE EXACT (816/816), wave 54 (W54-D). Every statement,
 * every branch, the jump table, both cross-jump merge points and all six pool
 * words are right; the residual is entirely which scratch register each
 * `movs rN,#0; ldrsh` pair picks, plus one `mov r1,r8` the ROM spends copying
 * the shared constant 1 back out of r8. First difference is at +0xd4 -- the
 * whole prologue, the switch and the first two arms are byte-exact.
 *
 * SETTLED:
 *  - The parameter is `s16 *`, the same view src/decomp/c_08085168.c and its
 *    three siblings take (+0x64 is p[0x32], +0x66 p[0x33], +0x4c p[0x26],
 *    +0x4e p[0x27]), which is why all four of those calls need no cast.
 *  - Pool words, all dereferenced in baserom.gba and all -fforce-addr, NOT
 *    objects: 0x081D93EC -> gUnknown_03005940, 0x081D93F0 -> gUnknown_08616C24,
 *    0x081D93F4 -> &gpKeySt (so the triple `ldr; ldr; ldrh [r0,#4]` is just
 *    `gpKeySt->pressed`).
 *  - The switch's DEFAULT ARM MUST COME FIRST in the source. Written with the
 *    `case 0: case 1: default:` arm last, gcc lays the four arms out in source
 *    order and the jump table's five entries point the other way round; the
 *    ROM has the default arm immediately after the table.
 *  - The 0x20 arm's unk66 update is a `?:` on the VALUE, not an if/else with
 *    two stores: the ROM has ONE `strh r0,[r6]` at the merge. The 0x10 arm IS
 *    an if/else and gcc cross-jumps its two stores into one anyway, so the two
 *    arms are not spelled the same way.
 *  - `p[0x27] = -1` does NOT reproduce the store in the `& 3` arm; the ROM has
 *    `ldr r0,=0xffff`, which needs the constant to stay 65535, i.e. a u16
 *    lvalue: `*(u16 *)&p[0x27] = 0xFFFF;`. The ROM also emits a DEAD
 *    `ldrh r3,[r1]` there that nothing reproduces -- same open case as
 *    src/decomp/c_08078xxx's dead sign-extended load (wave 52, W52-A).
 *  - `t = gPlayers;` in the 0x20 arm and the zero-trip
 *    `do { } while (0);` on `case 0:` are decomp-permuter finds and are
 *    LOAD-BEARING, worth 36 bytes between them. Without the `t` binding the
 *    two arms' sub_08043FA8 blocks are register-identical and the post-reload
 *    cross-jump pass merges them; the ROM does NOT merge them, because there
 *    its two arms happen to pick different scratch registers for the `movs
 *    rN,#0`. Binding the array base in one arm only is what desynchronises
 *    them. See the chapter added to docs/agbcc-codegen.md.
 *  - `u8 v` holding the ternary result is also a permuter find. It is a real
 *    narrowing on the `p[0x33] - 1` path and may well not be what the original
 *    wrote, but it is byte-better here and the values involved are 1..4.
 *
 * HISTORICAL SEARCH: two runs took it 39.8% -> 68.5% -> 75.9%; one 480 s run
 * reached 79.2% on "raw permuter
 * output" but only 75.9% "spliced onto the original preamble", which is the
 * wave-51/W51-B header-regression signature -- the randomizer had changed a
 * declaration, so that candidate is not usable as-is. Wave 66 closed the
 * documented dead-load axis: adding a volatile u16 read immediately before
 * the 0xFFFF store kept the section size exact but regressed 75.9% to 72.8%.
 * No further permuter search was run; this is the current hand-source fixpoint.
 */

void sub_08084C14(s16 *p)
{
  struct PlayerStruct *t;
  u8 new_var;
  if (p[0x32] != 0)
  {
    sub_080852A8((struct Unk080852A8 *) p);
    p[0x32] = 0;
  }
  switch (gUnknown_03005940)
  {
    case 0:
      do
    {
    }
    while (0);

    case 1:

    default:
      sub_08085168(p);
      break;

    case 2:
      sub_080851CC(p);
      break;

    case 3:
      sub_08085208(p);
      break;

    case 4:
      sub_08085244(p);
      break;

  }

  sub_08085044((struct Unk8085044 *) p);
  sub_08084F44((struct Unk8084F44 *) p);
  if (sub_080116A0())
  {
    return;
  }
  if (Proc_Find(gUnknown_08616C24) != 0)
  {
    return;
  }
  if ((gpKeySt->pressed & 0x40) && (p[0x27] >= 0))
  {
    if (gUnknown_03005940 <= 0)
    {
      return;
    }
    gUnknown_03005940--;
    if (gUnknown_03005940 == 3)
    {
      sub_080858C0();
    }
    sub_08085298(p);
    p[0x32] = 1;
    sub_0803B4DC(0x64);
  }
  else
    if ((gpKeySt->pressed & 0x80) && (p[0x27] >= 0))
  {
    if (gUnknown_03005940 > 3)
    {
      return;
    }
    gUnknown_03005940++;
    if (1)
    {
      if (gUnknown_03005940 == 4)
      {
        sub_08085908();
      }
      sub_08085298(p);
    }
    p[0x32] = 1;
    sub_0803B4DC(0x64);
  }
  else
    if ((gpKeySt->pressed & 0x20) && (p[0x27] == 0))
  {
    new_var = (p[0x33] == 1) ? ((gPlaySt.gameMode == 2) ? (sub_0802490C(gPlaySt.mapID)) : (sub_080248F8())) : (p[0x33] - 1);
    p[0x33] = new_var;
    p[0x26] = 0;
    p[0x27] = 1;
    sub_08043B14(gPlayers[p[0x33]].co, 0xAB * 4);
    sub_08043FA8(gPlayers[p[0x33]].co, (void *) 0x06015700, 0x16);
    t = gPlayers;
    sub_0802D5CC(gUnknown_08616B1C[t[p[0x33]].teamColor], 0);
    sub_08043834(p[0x33]);
    sub_08085950(0, p[0x33]);
    sub_08085298(p);
    p[0x32] = 1;
    sub_0803B4DC(0x64);
  }
  else
    if ((gpKeySt->pressed & 0x10) && (p[0x27] == 0))
  {
    if (p[0x33] == ((gPlaySt.gameMode == 2) ? (sub_0802490C(gPlaySt.mapID)) : (sub_080248F8())))
    {
      p[0x33] = 1;
    }
    else
    {
      p[0x33] = p[0x33] + 1;
    }
    p[0x26] = 0;
    p[0x27] = 1;
    sub_08043B14(gPlayers[p[0x33]].co, 0xAB * 4);
    sub_08043FA8(gPlayers[p[0x33]].co, (void *) 0x06015700, 0x16);
    sub_0802D5CC(gUnknown_08616B1C[gPlayers[p[0x33]].teamColor], 0);
    sub_08043834(p[0x33]);
    sub_08085950(0, p[0x33]);
    sub_08085298(p);
    p[0x32] = 1;
    sub_0803B4DC(0x64);
  }
  else
    if ((gpKeySt->pressed & 3) && (p[0x27] == 0))
  {
    *((u16 *) (&p[0x27])) = 0xFFFF;
    Proc_Start(gUnknown_08616BFC, p);
    sub_0803B4DC(0x66);
    sub_08011B5C((void *) sub_08043590);
  }
  else
    if (((gpKeySt->pressed & 0x100) && (p[0x27] == 0)) && (gUnknown_03005940 == 4))
  {
    sub_0803B4DC(0x76);
    Proc_Start(gUnknown_08616C24, p);
  }
}




