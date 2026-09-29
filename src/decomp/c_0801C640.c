#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C640.
 * sub_0801C640 @ 0x0801C640
 */

/* MATCHED in wave 60 (W60-A) BY THE PERMUTER, parked since wave 30 on what the
 * park called "local-alloc's priority order between the cursor and the one-use
 * offset temp". The permuter had never been run on it; 11,050 iterations of one
 * 300 s run found this.
 *
 * THE LEVER IS THE `do { ... } while (0)` AROUND THE TWO STATEMENTS OF THE
 * THEN-ARM, exactly as in AP_SwitchAnimation next door, which fell to the same lever
 * in the same wave. Grouping statements into a block moves where the live
 * ranges of the values they define end, and that is what flips local-alloc's
 * tie-break -- the wave-30 park had ruled out every retyping and reordering of
 * the values themselves but never a statement grouping. DO NOT TIDY IT:
 * unwrapping the block puts the seven bytes back.
 *
 * The wave-30 park's settled facts still hold and should not be re-derived:
 * `lsrs #1; lsls #1` is `x / 2 * 2` and NOT `x & ~1` -- `& ~1` must build -2 in
 * a register, which gives this leaf a `push {r4, lr}` the ROM does not have, so
 * the absence of a prologue is what proves the shift spelling. Handle +0x20
 * bit 1 selects a u16 self-relative encoding against a flat u32 pointer table.
 */
void AP_LoadDefinition(struct Unk0801C210 *a1, void *a2)
{
  void *v;
  a1->unk00 = a2;
  if (a1->unk20 & 2)
  {
    u16 *b = (u16 *) a2;
    u8 *p;
 do { ; a1->unk04 = ((u8 *) a2) + ((b[0] / 2) * 2); p = ((u8 *) a2) + ((b[1] / 2) * 2); } while (0);
    p += (((u16 *) p)[0] / 2) * 2;
    v = p;
  }
  else
  {
    a1->unk04 = ((void **) a2)[0];
    v = ((void **) a2)[1];
  }
  a1->unk08 = v;
  a1->unk0c = v;
}
asm(".global sub_0801C640\n.thumb_set sub_0801C640, AP_LoadDefinition\n");
