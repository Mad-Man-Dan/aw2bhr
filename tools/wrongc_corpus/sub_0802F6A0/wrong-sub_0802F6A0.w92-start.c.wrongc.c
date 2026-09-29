#include "global.h"

/* Wave 74 (W74-C): two drained `--current` permuter runs found no match.
 * Independently reconstructing the strongest semantics-preserving size-exact
 * output by binding the wrapped cursor to `new_var` in the same assignment as
 * `t` improves the active draft from 337 to 327 differing bytes: 604/604,
 * 45.9% identical, first difference +0x0c.  The 49.7/49.8% outputs retained
 * by best.c are invalid because they initialize replacement locals only after
 * an unconditional return, then read those locals on earlier `goto found`
 * paths.  They were rejected.  The residual remains the dst/expSum register-
 * stack inversion; see NOTES.md. */

/* Wave 72 (W72-C): repaired best.c to the proven void * contract and ran a
 * bounded 120-second, 8-thread allocator search.  No match was found, but its
 * strongest size-exact candidate improves configured identity from 26.3% to
 * 44.2% (337/604 bytes differ).  The residual remains the dst/expSum hard-
 * register/stack-slot inversion documented in NOTES.md. */

/* Wave 65 (W65-O): widening only the spilled `expSum` local from u16 to int
 * was byte-neutral at 604/604 and left the same `dst`/`expSum` register-slot
 * inversion and extra gUnknown_03003128 pool word.  Local type width is not
 * the allocation lever here; the proven u16 model below is restored.
 *
 * Wave 62 (W62-A): the old draft no longer compiled because its u16 * parameter
 * contradicted the proven shared `void *` prototype.  Correcting the definition
 * and casting only the payload store restores a current size-exact 604/604
 * candidate.  It still has the documented dst/expSum allocation disagreement.
 * The permuter could not start because best.c retains the obsolete prototype.
 *
 * Wave 50 (W50-D).  PARKED, size-exact instruction stream, ONE CONTESTED
 * REGISTER.  See NOTES.md for the residual.  This is a real compilable
 * attempt, not a write-up. */

s16 sub_0802F6A0(s8 slot, void *dst)
{
  int i;
  s16 len;
  u16 new_var;
  u16 t;
  u16 avail;
  u16 sum;
  u16 comp;
  u16 expSum;
  u16 expComp;
  sum = 0;
  comp = 0;
  if (gUnknown_03003128[slot] == gUnknown_03003F48[slot])
  {
    return -2;
  }
  if (gUnknown_02025C18[gUnknown_03003128[slot]][slot] == 0x4fff)
  {
    goto found;
  }
  if (gUnknown_03003128[slot] == gUnknown_03003F48[slot])
  {
    return -4;
  }
  do
  {
    gUnknown_03003128[slot]++;
    gUnknown_03003128[slot] &= 0x3ff;
    if ((gUnknown_02025C18[gUnknown_03003128[slot]][slot] == 0x4fff) && (gUnknown_03003128[slot] != gUnknown_03003F48[slot]))
    {
      goto found;
    }
  }
  while (gUnknown_03003128[slot] != gUnknown_03003F48[slot]);
  return -4;
  found:
  if (gUnknown_03003F48[slot] < gUnknown_03003128[slot])
  {
    avail = gUnknown_03003F48[slot] - (gUnknown_03003128[slot] + 0xFFFFFC00);
  }
  else
  {
    avail = gUnknown_03003F48[slot] - gUnknown_03003128[slot];
  }

  if (((s16) avail) <= 1)
  {
    return -4;
  }
  t = (new_var = ((gUnknown_03003128[t = slot] + 1) > 0x3ff) ? (0) : (gUnknown_03003128[t] + 1));
  len = gUnknown_02025C18[t][t];
  if (len > 0x80)
  {
    gUnknown_03003128[new_var]++;
    gUnknown_03003128[new_var] &= 0x3ff;
    return -4;
  }
  if ((len + 6) > ((s16) avail))
  {
    return -2;
  }
  gUnknown_03003128[new_var] += 2;
  gUnknown_03003128[new_var] &= 0x3ff;
  expSum = gUnknown_02025C18[gUnknown_03003128[new_var]][new_var];
  gUnknown_03003128[new_var]++;
  gUnknown_03003128[new_var] &= 0x3ff;
  expComp = gUnknown_02025C18[gUnknown_03003128[new_var]][new_var];
  gUnknown_03003128[new_var]++;
  gUnknown_03003128[new_var] &= 0x3ff;
  sum += len + 0x4fff;
  i = 0;
  while (i < len)
  {
    i++;
    sum += gUnknown_02025C18[gUnknown_03003128[new_var]][new_var] * i;
    comp += ~(gUnknown_02025C18[gUnknown_03003128[new_var]][new_var] * i);
    *((u16 *) dst) = gUnknown_02025C18[gUnknown_03003128[new_var]][new_var];
    gUnknown_03003128[new_var]++;
    gUnknown_03003128[new_var] &= 0x3ff;
    dst = ((u16 *) dst) + 1;
  }

  if ((sum != expSum) || (comp != expComp))
  {
    return -3;
  }
  return len * 2;
}






