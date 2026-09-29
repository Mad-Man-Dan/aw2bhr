#include "global.h"

/* WAVE 56, W56-H -- NOT MATCHED. Structurally complete; the ENTIRE residual is
 * 4 bytes at the loop top, and sub_0801D390 and sub_0801DCD4 have exactly the
 * same one:
 *     original   ldrh rV,[p] / ldr rA,=0xFFFFF000 / adds r0,rA,#0
 *                             / adds rB,r0,#0 / ands rB,rV
 *     candidate  ldrh rV,[p] / ldr rB,=0xFFFFF000 / ands rB,rV
 * The ROM routes the mask through THREE pseudos (middle one is r0 in both
 * functions); the honest spelling produces one. `size:` is -4 and the byte
 * percentage is therefore meaningless -- everything after +0x10 is the pool
 * shift. Everything else in the function lines up instruction for instruction;
 * the only other differences are which scratch register holds a copy of `e`.
 *
 * Ruled out by controlled probe (17 spellings, both functions): operand order
 * on the AND, `~0xFFF` / `0xFFFFF000` / `-0x1000` (NOTE: `0xFFFFF000` is
 * `unsigned` in C89 and turns the whole switch tree into `bhi`/`bls` -- the
 * mask MUST be int-typed), binding the switch subject to a local, binding the
 * subject for BOTH the outer and the inner switch, binding the mask to a local
 * inside or outside the loop, `op = v; op &= ~0xFFF`, `op = ~0xFFF; op &= v`,
 * re-reading `*p` in the switch head, `switch ((v = *p) & ...)`, `int v` vs
 * `u16 v` vs `int v = (u16)*p`, and moving `end`'s initialiser inside/outside
 * the loop. decomp-permuter (600 s, ~n/a iterations) found nothing better.
 * A `do { } while (p != NULL)` with an explicit `if (p == NULL) return;` guard
 * DOES reach `size: match` at 63.0%, but only because it adds a redundant guard
 * worth +4 that cancels the -4 -- and it moves the first literal pool 40 bytes,
 * so it is a worse draft, not a better one. Left in the honest `while` form.
 * Wave 65 also tested two chained mask locals and nested `static inline`
 * identity calls. Both compiled identically to the honest one-pseudo mask and
 * left the same 852/856-byte residual, so neither can manufacture the two ROM
 * copies.  Wave 71 additionally tested register-qualified mask chains, a local
 * fixed to r0, and empty/volatile RTL barriers around that fixed local.  All
 * compile to the same direct `ldr mask; and` in BOTH twins; none creates either
 * ROM copy.
 *
 * Two things ARE settled here and should survive:
 *  - the loop-bottom `movs rN,#0; cmp rN,#0; bne <exit>` is a real local flag
 *    that nothing sets, initialised at the TOP of the loop body and tested
 *    before the pointer test; omitting it is 6 bytes short;
 *  - the +0x31 byte tests are SIGNED one-bit bitfields (see the struct above
 *    and the chapter in docs/agbcc-codegen.md).
 */

/* The byte at +0x31 of the gUnknown_0200E438 record -- byte 1 of the eight-byte
 * `struct UnkVec unk30`, i.e. bit 12 of its first word.  The ROM tests it with
 * `ldrb; lsls #0x1b; cmp #0; bge`, which is a SIGNED one-bit bitfield at bit 12
 * and nothing else: a plain `& 0x10` costs an extra `movs`+`ands` pair and a
 * `>> 4 & 1` costs two more.  Wave 56, W56-H. */
struct Unk0801D390Bits
{
    u32 unk00_0:12;
    s32 unk00_c:1;
};

void sub_0801D390(int a1, int a2)
{
  struct Unk0200E438 *e;
  u16 *p;
  u16 v;
  int n;
  int end;
  int t;
  u32 *q;
  gUnknown_03003040 = a1;
  e = &gUnknown_0200E438[a1];
  p = (u16 *) e->unk08;
  while (p != ((void *) 0))
  {
    end = 0;
    v = *p;
    switch (v & (~0xFFF))
    {
      case 0x0000:
        if (a2 == 0)
      {
        return;
      }
        if (((s16) e->unk1c) != 0)
      {
        e->unk1c = e->unk1c - 1;
      }
      else
      {
        e->unk1c = (v & 0xFFF) - 1;
      }
        if (((s16) e->unk1c) != 0)
      {
        return;
      }
        e->unk08 = (u32) (p + 1);
        e->unk02 = 0;
        return;

      case 0x2000:
        n = v & 0xFFF;
        p = ((u16 *) e->unk04) + n;
        break;

      case 0x1000:
        e->unk24 = v & 0xFF;
        p++;
        if (a2 == 0)
      {
        q = (u32 *) e->unk20;
        q += e->unk24;
        if (((struct Unk0801D390Bits *) (&e->unk30))->unk00_c == 0)
        {
          sub_0801D348(e->unk26, (e->unk14 + e->unk0c) / 256, (e->unk18 + e->unk10) / 256, *q, e->unk1e, e->unk28, *((long long *) (&e->unk30)), e->unk38);
        }
      }
        break;

      case 0x5000:
        n = v & 0xFFF;
        p++;
        if (n != 0)
      {
        if (e->unk02 & 8)
        {
          p++;
          break;
        }
        e->unk02 |= 8;
        sub_0801E294(e->unk28, n, *((s16 *) (p++)));
      }
      else
      {
        if (e->unk02 & 4)
        {
          p++;
          break;
        }
        e->unk02 |= 4;
        sub_0801E264(e->unk28, *((s16 *) (p++)));
      }
        break;

      case 0x6000:
        n = v & 0xFFF;
        p++;
        t = *((s16 *) (p++));
        if (n != 0)
      {
        if (e->unk02 & 0x20)
        {
          p++;
          break;
        }
        e->unk02 |= 0x20;
        sub_0801E27C(e->unk28, n, (t * 0x100) / 100, ((*((s16 *) (p++))) * 0x100) / 100);
      }
      else
      {
        if (e->unk02 & 0x10)
        {
          p++;
          break;
        }
        e->unk02 |= 0x10;
        sub_0801E248(e->unk28, (t * 0x100) / 100, ((*((s16 *) (p++))) * 0x100) / 100);
      }
        break;

      case 0xE000:
        e->unk2c = v & 0xFFF;
        p++;
        e->unk08 = (u32) p;
        break;

      case 0xD000:
        e->unk26 = v & 0xFFF;
        p++;
        break;

      case 0xF000:
        switch (v & 0xF00)
      {
        case 0xF00:
          e->unk08 = (u32) p;
          if (a2 != 0)
        {
          return;
        }
          q = (u32 *) e->unk20;
          q += e->unk24;
          if (((struct Unk0801D390Bits *) (&e->unk30))->unk00_c == 0)
        {
          sub_0801D348(e->unk26, (e->unk14 + e->unk0c) / 256, (e->unk18 + e->unk10) / 256, *q, e->unk1e, e->unk28, *((long long *) (&e->unk30)), e->unk38);
        }
          return;

        case 0xE00:
          if (a2 != 0)
        {
          sub_0801D81C(a1);
        }
          return;

        case 0xA00:
          e->unk1e = v;
          p++;
          break;

        case 0x900:
          e->unk08 = (u32) (p++);
          e->unk08 += 2;
          break;

      }

        break;

    }

    if (end != 0)
    {
      break;
    }
  }

}




