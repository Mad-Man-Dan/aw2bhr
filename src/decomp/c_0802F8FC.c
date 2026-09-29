#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802F8FC.
 * sub_0802F8FC @ 0x0802F8FC, sub_0802F9BC @ 0x0802F9BC
 */

#include "hardware.h"

/* The multiplayer send step. It writes the caller's halfword into SIOMLT_SEND
 * and spins until the register reads back what it wrote, then either arms a
 * timeout (SioStartTimer3) or starts the transfer directly by writing
 * gUnknown_03000560 | 0x6080 to SIOCNT.
 *
 * -4 when the session state gUnknown_0300055C is above 3; 5 otherwise, and the
 * whole middle is skipped unless that state is exactly 0.
 *
 * `0xc1 << 7` is 0x6080 and `0x80 << 4` is 0x800 -- write the constants and let
 * agbcc build them.
 *
 * ONE LOCAL, NOT TWO. The gate flag and the timeout value share a variable, and
 * that is what puts the timeout in r1 and costs the `adds r0, r1, #0` in front
 * of the call; two separate locals put it straight in r0 and the copy
 * disappears. Same wave-17 lesson in reverse -- here the ROM has FEWER locals
 * than the obvious spelling.
 *
 * The gate is spelled as a nested if/else and not as `a == 0 || b == 0 ||
 * c < 0`: the flat disjunction jumps all three ways straight into the body and
 * never materialises the flag, while the ROM sets it to 1 on the unk0a path and
 * falls into `cmp r1, #0`. The `else v = 1` being textually LAST is what makes
 * that block the fall-through; with the assignments in the other order agbcc
 * threads the first two and leaves the last one inline, which is the mirror
 * image of the ROM. */
int SioSend16(u16 *reg, int flag)
{
    int v;

    if (gUnknown_0300055C > 3)
        return -4;

    do
    {
        REG_SIOMLT_SEND = *reg;
    } while (REG_SIOMLT_SEND != *reg);

    if (gUnknown_0300055C == 0)
    {
        v = 0;
        if (gUnknown_03003F6C->unk0a != 0)
        {
            if (gUnknown_03000568 == 0 || flag < 0)
                v = 1;
        }
        else
            v = 1;

        if (v)
        {
            if (gUnknown_03000564 & 0x800)
                v = gUnknown_08090C44[gUnknown_03000578];
            else
                v = 0;

            if (v != 0)
                SioStartTimer3(v);
            else
                REG_SIOCNT = gUnknown_03000560 | 0x6080;
        }
    }

    gUnknown_03000568 = 1;
    return 5;
}
asm(".global sub_0802F8FC\n.thumb_set sub_0802F8FC, SioSend16\n");

/* Pulls one halfword per link slot out of the gUnknown_02025C18 ring into the
 * caller's four-halfword buffer. 0x7FFF is the "no sample" filler: it goes out
 * unconditionally in mode 1 (returning 5) and on an empty ring (returning -1).
 *
 * `dst` is WALKED, not indexed -- the ROM copies the parameter into its own
 * pseudo up front and every store is `*dst++`. Indexed `dst[0..3]` gives
 * base+displacement stores instead and loses the `adds r3, r1, #0`.
 *
 * gUnknown_02025C18 is `[cursor][slot]`: the ROM scales the two indices
 * separately (`lsls #1` on the slot, `lsls #3` on the cursor) and adds them to
 * the bare symbol. Flat `[cursor * 4 + slot]` reassociates and moves a shift.
 *
 * The cursor advance is TWO statements, `++` then `&= 0x3ff`, and each carries
 * the dead `ldrh` in front of its `strh` because gUnknown_03003128 is volatile.
 * Folded into one expression the middle load/store pair disappears.
 *
 * The `volatile u16 *p` local and the operand order of the compare are both
 * load-bearing and were found by decomp-permuter: without the local the two
 * scan givs land in r1/r2 instead of the ROM's r2/r5, and with the compare
 * written the other way round agbcc emits `cmp r0, r1` for the ROM's
 * `cmp r1, r0`. */
int SioReadWordRow(int a1, u16 *dst)
{
  volatile u16 *p;
  int i;
  if (gUnknown_03000570 == 1)
  {
    *(dst++) = 0x7fff;
    *(dst++) = 0x7fff;
    *(dst++) = 0x7fff;
    *(dst++) = 0x7fff;
    return 5;
  }
  for (i = 0; i <= 3; i++)
  {
    p = &gUnknown_03003128[i];
    if ((*p) == gUnknown_03003F48[i])
    {
      *(dst++) = 0x7fff;
      *(dst++) = 0x7fff;
      *(dst++) = 0x7fff;
      *(dst++) = 0x7fff;
      return -1;
    }
  }

  for (i = 0; i <= 3; i++)
  {
    *(dst++) = gUnknown_02025C18[gUnknown_03003128[i]][i];
    gUnknown_03003128[i]++;
    gUnknown_03003128[i] &= 0x3ff;
  }

  return 5;
}
asm(".global sub_0802F9BC\n.thumb_set sub_0802F9BC, SioReadWordRow\n");
