#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08055654.
 * sub_08055654 @ 0x08055654
 */

/* Wave 63, W63-A. MATCHED. The inner counter must be block-scoped, and the
 * entry test must stay duplicated in the two gUnknown_0300450C arms until
 * cross-jumping merges their tails. The final zero-cost self-assignment keeps
 * only the outer counter live past the loop; using entries[j] there instead
 * rotates the inner loop's r2/r3/r4 allocation and leaves a 19-byte residual. */
void BuildShotLists(u16 a1, u16 a2)
{
  u16 counts[2];
  u16 i;
  counts[0] = 0;
  counts[1] = 0;
  for (i = 0; i < 2; i++)
  {
    u16 j;
    for (j = 0; j < 5; j++)
    {
      if (gUnknown_0300450C == i)
      {
        if ((gUnknown_02029A10[i].entries[j].unk00 != 0) && (gUnknown_03004580[i][2] != 0))
          counts[i]++;
      }
      else
      {
        if ((gUnknown_02029A10[i].entries[j].unk01 != 0) && (gUnknown_03004580[i][2] != 0))
          counts[i]++;
      }
    }

  }

  if ((a1 == 2) && (gUnknown_0300450C == 0))
  {
    counts[1] = 0;
  }
  if ((a2 == 2) && (gUnknown_0300450C == 1))
  {
    counts[0] = 0;
  }
  if ((a1 == 2) && (gUnknown_0300450C == 1))
  {
    counts[0] = 0;
  }
  if ((a2 == 2) && (gUnknown_0300450C == 0))
  {
    counts[1] = 0;
    gUnknown_02029A10[i].entries[0].unk00 = gUnknown_02029A10[i].entries[0].unk00;
  }
  sub_08055768(0, counts[0]);
  sub_08055768(1, counts[1]);
}
asm(".global sub_08055654\n.thumb_set sub_08055654, BuildShotLists\n");
