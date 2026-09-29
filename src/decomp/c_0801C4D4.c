#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C4D4.
 * sub_0801C4D4 @ 0x0801C4D4
 */

/* MATCHED in wave 60 (W60-A) BY THE PERMUTER, after being parked since wave 30
 * as an unreachable local-alloc tie-break. The permuter had never been run on
 * it; it found this in 219 iterations of a single 300 s run.
 *
 * THE LEVER IS THE `do { ... } while (0)` AROUND THE TWO STORES. It is what
 * gives the handle r2 and the script base r3 (the ROM's assignment) instead of
 * the other way round -- the wave-30 park had ruled out every reordering and
 * retyping of the two values themselves, but not a statement grouping that
 * changes where their live ranges end. DO NOT TIDY IT: unwrapping the block is
 * the exact edit that puts the ten bytes back.
 *
 * Everything the wave-30 park settled still holds: `/ 2 * 2` is `lsrs #1;
 * lsls #1` and NOT `x & ~1` (which costs a register for -2), and the second
 * parameter is the entry INDEX into the script's table. AP_LoadDefinition next door
 * is the same axis and the same lever. */
void AP_SwitchAnimation(struct Unk0801C210 *a1, int a2)
{
  void *v;
  u8 *base;
  if (a1 == ((void *) 0))
  {
    return;
  }
  base = (u8 *) a1->unk00;
  if (base == ((void *) 0))
  {
    return;
  }
  if (a1->unk20 & 2)
  {
    u8 *p = base + ((((u16 *) base)[1] / 2) * 2);
    p += (((u16 *) p)[a2] / 2) * 2;
    v = p;
  }
  else
  {
    v = ((void **) base)[a2 + 1];
  }
 do { a1->unk08 = v; a1->unk0c = v; } while (0);
  AP_ExecDummyFrame(a1);
}
asm(".global sub_0801C4D4\n.thumb_set sub_0801C4D4, AP_SwitchAnimation\n");
