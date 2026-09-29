#include "global.h"

/* Reads one packet for player `slot` out of that player's column of the link
 * receive ring.
 *
 * gUnknown_02025C18 is the ring: 1024 rows of one halfword per player, and
 * gUnknown_03003128[slot] is that player's read cursor into it, wrapping at
 * 0x400. gUnknown_03003F48[slot] is how far the writer has got. The function
 * first walks the cursor forward to the next 0x4FFF start marker, then checks
 * that enough halfwords have arrived, reads the payload length (0x80
 * halfwords at most), the expected checksum and its complement, and copies the
 * payload into dst while recomputing both. It returns the payload length in
 * bytes, or -2 when not enough data has arrived yet, -3 when the checksums
 * disagree, and -4 when there is no packet, the ring is empty or the length is
 * out of range.
 *
 * Why the C looks odd: the cursor entries are volatile, so each `+= 1` and
 * `&= 0x3ff` is written as its own statement and every read of the cursor is
 * spelled out again rather than held in a local.
 */

s16 sub_0802F6A0(s8 slot, void *dst)
{
  int i;
  u16 *q;
  int wrapped;
  s16 len;
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
    goto fail4;
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
  goto fail4;
  found:
  if (gUnknown_03003F48[slot] < gUnknown_03003128[slot])
  {
    wrapped = gUnknown_03003128[slot] + 0xFFFFFC00;
    avail = gUnknown_03003F48[slot] - wrapped;
  }
  else
  {
    avail = gUnknown_03003F48[slot] - gUnknown_03003128[slot];
  }

  if (((s16) avail) <= 1)
  {
    fail4:
    return -4;

  }
  t = ((gUnknown_03003128[slot] + 1) > 0x3ff) ? (0) : (gUnknown_03003128[slot] + 1);
  len = gUnknown_02025C18[t][slot];
  if (len > 0x80)
  {
    gUnknown_03003128[slot]++;
    gUnknown_03003128[slot] &= 0x3ff;
    goto fail4;
  }
  if ((len + 6) > ((s16) avail))
  {
    return -2;
  }
  gUnknown_03003128[slot] += 2;
  gUnknown_03003128[slot] &= 0x3ff;
  expSum = gUnknown_02025C18[gUnknown_03003128[slot]][slot];
  gUnknown_03003128[slot]++;
  gUnknown_03003128[slot] &= 0x3ff;
  expComp = gUnknown_02025C18[gUnknown_03003128[slot]][slot];
  gUnknown_03003128[slot]++;
  gUnknown_03003128[slot] &= 0x3ff;
  sum += len + 0x4fff;
  i = 0;
  q = (u16 *) dst;
  while (i < len)
  {
    i++;
    sum += gUnknown_02025C18[i = gUnknown_03003128[slot]][slot] * i;
    comp += ~(gUnknown_02025C18[gUnknown_03003128[slot]][slot] * i);
    *q = gUnknown_02025C18[gUnknown_03003128[slot]][slot];
    gUnknown_03003128[slot]++;
    gUnknown_03003128[slot] &= 0x3ff;
    q++;
  }

  if ((sum != expSum) || (comp != expComp))
  {
    return -3;
  }
  return len * 2;
}
