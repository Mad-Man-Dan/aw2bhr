#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080611D8.
 * sub_080611D8 @ 0x080611D8
 */

/* AI: picks a map cell for the active unit by trying sub_08061308 in up to
 * five modes, falling back to a table walk; returns 1 when a target was found.
 *
 * Why the C looks odd: `r` holds the result of the mark path and of the last
 * call as a local, and the final test is `r == 0 -> return 0`, so the two
 * `return 1` tails are not merged and the single `return 0` sits last, as in
 * the original. The pool word for gUnknown_085766E4 is the compiler's own
 * address copy (0x0816DAE8). */

u8 AiPickBuildCell(void *arg)
{
  u16 *out;
  u16 none;
  int i;
  int best;
  u8 t;
  u8 r;
  out = arg;
  i = 0;
  best = 0;
  none = 9999;
  out[0] = none;
  if (gUnknown_030046C0.unk07 != 5)
  {
    t = gUnknown_085D5ABC[gUnknown_030046C0.unk06].deployLocation;
    if ((((((gUnknown_030046C0.unk06 == 20) && (sub_08061308(t, 3, out) == 1)) || ((gUnknown_030046C0.unk06 == 23) && (sub_08061308(t, 4, out) == 1))) || (sub_08061308(t, 0, out) == 1)) || ((gUnknown_030046C0.unk06 <= 2) && (sub_08061308(0x10, 1, out) == 1))) || ((gUnknown_0857680F[gUnknown_030046C0.unk06] != 2) && (sub_08061308(t, 2, out) == 1)))
    {
      goto alt;
    }
  }
  while (gUnknown_085766E4[i].unk00 != 0xff)
  {
    if ((gUnknown_085766E4[i].unk03 <= 0xfd) && (gUnknown_0857680F[gUnknown_030046C0.unk06] == gUnknown_085766E4[i].unk02))
    {
      out[0] = gUnknown_085766E4[i].unk00;
      out[1] = gUnknown_085766E4[i].unk01;
      best = i;
      break;
    }
    i++;
  }

  r = 1;
  if (out[0] == 9999)
  {
    return 0;
  }
  gUnknown_085766E4[best].unk03 = 0xfe;
  return r;
  alt:
  r = AiPickBestScoredBuildSite(out);
  if (r == 0)
    return 0;
  return 1;
}
asm(".global sub_080611D8\n.thumb_set sub_080611D8, AiPickBuildCell\n");
