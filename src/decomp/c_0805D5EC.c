#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805D5EC.
 * sub_0805D5EC @ 0x0805D5EC
 */

/* Publishes gUnknown_030046C0 into the 20-byte command block at
 * gUnknown_030044B0 and hands it to LinkQueueCommand, whose `u8 *src` parameter is
 * what that global's declaration is typed from.
 *
 * gUnknown_0816DA40 is NOT a global. The ROM word there holds 0x030044B0 --
 * dereferenced against baserom.gba -- so it is agbcc's own -fforce-addr
 * address constant for gUnknown_030044B0, parked in .rodata because the
 * address is live across the 6-byte loop and re-read afterwards for the call
 * (`adds r5, r3, #0` keeps the POOL WORD's address, not the object's). The
 * honest spelling names the global and lets the build place the word. Note
 * this is one level of indirection, not two: `ldr r1, [r3]` yields the object's
 * ADDRESS, so the offsets below are the object's own.
 *
 * The copy is field-by-field at matching offsets with a word at +8, so both
 * sides are one type. gUnknown_030044B0 is declared `u8 []` on the evidence of
 * SendMoveCommand / SendActionCommand / LinkQueueCommand and struct Unk030046C0's
 * +0x04/+0x05, +0x08 and +0x0c..+0x12 are still filler, so the view is
 * file-local: retyping either shared declaration to suit this one function is
 * exactly what the "never reshape a shared member" rule forbids, and nothing
 * here discriminates the members it would have to invent.
 *
 * The assignment ORDER is the source's statement order (0, 2, 3, 4, 5, then 1,
 * then the word at 8, then 6, 7, then 0x12) and is load-bearing -- agbcc emits
 * these in order and any tidying into ascending offsets moves bytes.
 *
 * The REPEATED CAST EXPRESSIONS below are load-bearing too, and they are the
 * whole difference between this and a 95.7% near-miss. Bound to two locals
 * `d` and `s`, the 6-byte loop comes out as `adds r0, r3, r4` -- base plus
 * index -- because a COMPONENT_REF/ARRAY_REF expands through get_inner_reference
 * as `plus (base, offset)`, always in that order. The ROM has `adds r0, r4, r3`,
 * index first, which is emit_iv_add_mult's `plus (biv, add_val)`: loop.c
 * strength-reduced the address, and it only does that when the base is
 * rematerialised at the use rather than living in a pseudo across the loop.
 * Writing the cast out at each use is what produces that. This is wave 17's
 * "binding locals are punctuation, not storage" rule read backwards, and the
 * four spellings that do NOT work are recorded in docs/agbcc-codegen.md.
 * Found by decomp-permuter at 92 bytes, well under the 256-byte threshold the
 * brief gives for reaching for it. */

struct Unk0805D5ECCmd
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 unk03;
    /* 0x04 */ u8 unk04;
    /* 0x05 */ u8 unk05;
    /* 0x06 */ u8 unk06;
    /* 0x07 */ u8 unk07;
    /* 0x08 */ u32 unk08;
    /* 0x0c */ u8 unk0c[6];
    /* 0x12 */ u8 unk12;
};

void AiSendActionCommand(void)
{
  struct Unk0805D5ECCmd *d = (struct Unk0805D5ECCmd *) gUnknown_030044B0;
  struct Unk0805D5ECCmd *s = (struct Unk0805D5ECCmd *) (&gUnknown_030046C0);
  int i;
  ((struct Unk0805D5ECCmd *) gUnknown_030044B0)->unk00 = s->unk00;
  ((struct Unk0805D5ECCmd *) gUnknown_030044B0)->unk02 = s->unk02;
  ((struct Unk0805D5ECCmd *) gUnknown_030044B0)->unk03 = s->unk03;
  ((struct Unk0805D5ECCmd *) gUnknown_030044B0)->unk04 = s->unk04;
  ((struct Unk0805D5ECCmd *) gUnknown_030044B0)->unk05 = s->unk05;
  ((struct Unk0805D5ECCmd *) gUnknown_030044B0)->unk01 = ((struct Unk0805D5ECCmd *) (&gUnknown_030046C0))->unk01;
  ((struct Unk0805D5ECCmd *) gUnknown_030044B0)->unk08 = ((struct Unk0805D5ECCmd *) (&gUnknown_030046C0))->unk08;
  ((struct Unk0805D5ECCmd *) gUnknown_030044B0)->unk06 = ((struct Unk0805D5ECCmd *) (&gUnknown_030046C0))->unk06;
  ((struct Unk0805D5ECCmd *) gUnknown_030044B0)->unk07 = ((struct Unk0805D5ECCmd *) (&gUnknown_030046C0))->unk07;
  ((struct Unk0805D5ECCmd *) gUnknown_030044B0)->unk12 = ((struct Unk0805D5ECCmd *) (&gUnknown_030046C0))->unk12;
  for (i = 0; i <= 5; i++)
  {
    ((struct Unk0805D5ECCmd *) gUnknown_030044B0)->unk0c[i] = ((struct Unk0805D5ECCmd *) (&gUnknown_030046C0))->unk0c[i];
  }

  LinkQueueCommand(gUnknown_030044B0);
}
asm(".global sub_0805D5EC\n.thumb_set sub_0805D5EC, AiSendActionCommand\n");
