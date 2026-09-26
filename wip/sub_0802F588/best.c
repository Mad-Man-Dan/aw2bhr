#include "global.h"

/* Sends one packet on the link.
 *
 * The send ring gUnknown_02025818 holds 512 halfwords and gUnknown_0300410C is
 * the write cursor into it; gUnknown_030040CC is the read cursor, and the
 * function gives up with -1 the moment writing would make the write cursor
 * catch up with it. Into the ring it writes a 0x4FFF start marker, the payload
 * length in halfwords (a2 / 2), a checksum -- 0x4FFF plus the length plus each
 * payload halfword multiplied by its 1-based position -- and the sum of the
 * complements of those same products, then the payload itself from a1. On
 * success it stores the advanced write cursor and returns the halfword count.
 *
 * Why the C looks odd, in three places. The checksum's starting value is
 * assigned in two steps because one expression puts the add in the wrong
 * place. Each product is written as `payload * position` rather than the other
 * way round because the original loads the payload halfword first. And the
 * 0x4FFF marker and the returned count are each copied into a local before
 * being used: the copies change nothing about what runs, they only stop the
 * compiler from reusing a register the original leaves alone.
 */

int sub_0802F588(struct Unk0202575C *a1, u16 a2)
{
  u16 sum;
  u16 chk;
  int new_var2;
  int new_var;
  int n;
  int i;
  int cur;
  int prod;
  sum = 0;
  chk = sum;
  cur = gUnknown_0300410C;
  n = a2 >> 1;
  sum = 0x4fff;
  sum = n + sum;
  new_var = 0x4fff;
  gUnknown_02025818[cur] = new_var;
  cur = (cur + 1) & 0x1ff;
  if (cur == gUnknown_030040CC)
  {
    return -1;
  }
  gUnknown_02025818[cur] = n;
  cur = (cur + 1) & 0x1ff;
  if (cur == gUnknown_030040CC)
  {
    return -1;
  }
  for (i = 0; i < n; i++)
  {
    prod = ((u16 *) a1)[i] * (i + 1);
    sum += prod;
    chk += ~prod;
  }

  gUnknown_02025818[cur] = sum;
  cur = (cur + 1) & 0x1ff;
  if (cur == gUnknown_030040CC)
  {
    return -1;
  }
  gUnknown_02025818[cur] = chk;
  cur = (cur + 1) & 0x1ff;
  if (cur == gUnknown_030040CC)
  {
    return -1;
  }
  for (i = 0; i < n; i++)
  {
    gUnknown_02025818[cur] = ((u16 *) a1)[i];
    new_var2 = 0x1ff;
    cur = (cur + 1) & new_var2;
    if (cur == gUnknown_030040CC)
    {
      return -1;
    }
  }

  chk = n;
  gUnknown_0300410C = cur;
  return chk;
}









