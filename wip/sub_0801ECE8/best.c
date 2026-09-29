#include "global.h"

/* Queues one sprite for drawing: fills the next free record of
 * gUnknown_0200ED20 (index gUnknown_03002510) with the position, the frame
 * data, the 8-byte OAM template with its hide and flicker bits cleared, and
 * the priority, then inserts the record into the sorted list with
 * sub_0801A718. Returns 1 if the list is full, otherwise bumps the record
 * count and returns 0.
 *
 * Why the C looks odd:
 *  - The template arrives as one 64-bit argument, but only its low word is
 *    masked, so the two halves are read separately and masked as plain 32-bit
 *    values. Masking the 64-bit value instead makes the compiler build a
 *    64-bit mask constant, whose all-ones upper half then costs a literal-pool
 *    word the original does not have.
 *  - struct Unk0200ED20Words describes the same record as
 *    struct Unk0200ED20, with the 64-bit member spelled as its two words, so
 *    the two halves can be stored separately. Storing them through
 *    &record.unk0c casts instead rebuilds the record's address each time; going
 *    through this struct lets them share the address every other field uses.
 *  - The two bit clears are written as separate statements. Combined into one
 *    expression they fold into a single mask, which is shorter than the
 *    original.
 *
 * The sixth parameter is volatile and is read into a local: that pair is what
 * keeps the read as a full-word load at the top of the function, as in the
 * original. See src/decomp/c_0801E338.c, this function's unmasked twin, for
 * the same construct.
 *
 * This is 4 bytes short of the original. The difference is one register: the
 * original keeps the template's high word in a low register and the record
 * array's base address in a high one, and this does the opposite. A base
 * address in a high register cannot be advanced by an immediate, which is what
 * the missing 4 bytes pay for. */

struct Unk0200ED20Words /* 0x14, struct Unk0200ED20 with unk0c as two words */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u32 unk04;
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u16 unk0a;
    /* 0x0c */ u32 unk0c;
    /* 0x10 */ u32 unk10;
};

int sub_0801ECE8(s16 a1, int a2, int a3, int a4, long long a5, volatile int a6)
{
  int lo;
  int hi;
  int t;
  lo = ((int *) (&a5))[0];
  hi = ((int *) (&a5))[1];
  t = a6;
  gUnknown_0200ED20[gUnknown_03002510].unk00 = a2;
  gUnknown_0200ED20[gUnknown_03002510].unk02 = a3;
  gUnknown_0200ED20[gUnknown_03002510].unk04 = a4;
  gUnknown_0200ED20[gUnknown_03002510].unk02 = a3;
  (gUnknown_0200ED20 + gUnknown_03002510)->unk08 = 0;
  lo &= ~0x2000;
  lo &= ~0x1000;
  ((struct Unk0200ED20Words *) gUnknown_0200ED20)[gUnknown_03002510].unk0c = lo;
  ((struct Unk0200ED20Words *) gUnknown_0200ED20)[gUnknown_03002510].unk10 = hi;
  gUnknown_0200ED20[gUnknown_03002510].unk0a = t;
  if (sub_0801A718(&gUnknown_0200ED20[gUnknown_03002510], a1) == (-1))
  {
    return 1;
  }
  gUnknown_03002510++;
  return 0;
}
