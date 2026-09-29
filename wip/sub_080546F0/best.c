#include "global.h"

/* Wave 56, W56-O.  STATE: +8 bytes, 26.8% (from +40 / 20.8%).  Wave 72
 * reverified the same configured 1068/1060-byte, 26.8% fixpoint.  NOT a "could
 * not work out how" -- the SHAPE is complete and was read out statement by
 * statement against the ROM: three loop nests plus the CpuFastSet tail, every
 * global, every member offset, every loop bound and every store ORDER already
 * agree.  No codegen question is open about WHAT the function does.  The
 * residual is register allocation, and it has one seed and three symptoms.
 *
 * THE SEED is that agbcc puts the shared loop counter in r5/r6 here and in r7
 * in the ROM, so every scratch below it shifts by one and the frame carries
 * three more spill slots (0x54 against the ROM's 0x48).  Frame size is NOT a
 * code-size term, so the +8 is entirely the symptoms:
 *   - loop 1's increment block needs two `movs rN,#2` where the ROM shares one
 *     (+2), because the register holding the constant is reallocated;
 *   - loop 2 keeps `&gUnknown_02029808[i]` as its own pointer induction
 *     variable stepping by 108, where the ROM reuses the `muls #0x6c` product
 *     it already computed for gUnknown_02029816 and adds the bare symbol
 *     (+2 for the extra `movs`/`add` pair);
 *   - `gUnknown_02029664 = 0` reuses the halfword zero (`strb r1,[r0]`) where
 *     the ROM materialises a second one (`movs r0,#0; strb r0,[r1]`) (-2), and
 *     `gUnknown_020296B0[i]` reaches the element with `add r0,ip` where the ROM
 *     copies ip to a lo register first (-2).
 *
 * MEASURED AND RULED OUT, so do not spend attempts on them again:
 *   - the eight `|= 0xffff` rows.  See the comment below and the wave-56 W56-O
 *     chapter of docs/agbcc-codegen.md; the view-struct spelling is the only
 *     one of six tested that reproduces `ldrh; orrs; strh`.
 *   - the cast SITE.  Casting the array base instead of the element address
 *     was measured at +28 bytes; casting the element's member address costs a
 *     pool word.  The current spelling is the best of the three.
 *   - gUnknown_02028E5C's pair of stores.  `q = gUnknown_02028E5C[i]; q[0]=0;
 *     q[1]=0;` gives the ROM's `strh [r0]; strh [r0,#2]`; the direct
 *     `[i][0]`/`[i][1]` spelling splits the base and does not.
 *   - the frame.  Two `u32` locals in this order, sp+0 then sp+4, is right;
 *     the `mov r0,sp; adds r0,#4` hoisted to the top of the function is
 *     purge_addressof and must not be authored.
 *
 * Wave 72 tested whether reusing `i`/`j` across the loop nests is the seed.
 * Splitting all four outer counters over-corrects to 1044 bytes (-16), and
 * splitting the two inner counters as well reaches 1040 (-20). Individually,
 * splitting outer loops 1/2/3/4 gives +8/+16/-16/-36; splitting either inner
 * counter gives +12. The shared-counter web controls the spill surplus, but
 * none of the selective source-reachable splits lands on the ROM's middle
 * allocation. Wave 78 reverified the configured +8-section-byte (+6 code-byte)
 * residual and retained this higher-scoring draft. */
/* Eight halfword rows in this subsystem are declared `s16` in
 * include/unknown-globals.h, but sub_080546F0 clears them with `x |= 0xffff`
 * and the ROM's `ldrh; orrs; strh` against a POOLED 0x0000ffff can only be
 * produced by an UNSIGNED 16-bit lvalue: on an s16 lvalue agbcc's
 * shorten_binary_op folds the whole expression to the constant -1 and emits a
 * bare `strh` (measured -- see docs/agbcc-codegen.md, wave 56, W56-O).  The
 * shared declarations are left alone; these local views name the same storage
 * as u16 so the ACCESS, and only the access, is unsigned.
 *
 * The cast has to be on the array BASE.  A cast on the ELEMENT ADDRESS
 * (`*(u16 *)&x`, or `((struct C *)&x)->v`) reproduces the unsignedness but
 * folds the member offset into the literal-pool word, which the ROM does not
 * do -- it keeps the bare symbol and reaches the member with `adds rB,#N`. */
struct V98E0 /* 0x90 -- gUnknown_020298E0, unk02 as u16 */
{
    u8 f00[0x02];
    u16 unk02[5];
    u8 f0c[0x84];
};

struct V9808 /* 0x6c -- gUnknown_02029808, unk02 and unk24 as u16 */
{
    u8 f00[0x02];
    u16 unk02[5];
    u8 f0c[0x18];
    u16 unk24[5];
    u8 f2e[0x3e];
};

struct V96B0 /* 0x28 -- gUnknown_020296B0, unk02 as u16 */
{
    u8 f00[0x02];
    u16 unk02[5];
    u8 f0c[0x1c];
};

struct V97C0 /* 0x24 -- gUnknown_020297C0, unk02 as u16 */
{
    u8 f00[0x02];
    u16 unk02[5];
    u8 f0c[0x18];
};

struct V9710 /* 0x58 -- gUnknown_02029710, unk0c as u16 */
{
    u8 f00[0x0c];
    u16 unk0c[5];
    u8 f16[0x42];
};

struct VA10E /* 0x24 -- gUnknown_02029A10 entry, unk18 as u16 */
{
    u8 f00[0x18];
    u16 unk18;
    u8 f1a[0x0a];
};

struct VA10G /* 0xb4 */
{
    struct VA10E entries[5];
};

struct VRow5 /* 0x0a -- gUnknown_02029668 / gUnknown_0202967C rows as u16 */
{
    u16 v[5];
};

void sub_080546F0(void)
{
  u32 sp0;
  int new_var;
  u32 sp4;
  int i;
  int j;
  u8 *r;
  u16 *q;
  int new_var2;
  new_var2 = 0;
  gUnknown_0300453C = 0;
  gUnknown_0300451C = 0;
  gUnknown_03004508 = 0;
  gUnknown_03004534 = 0;
  gUnknown_030045B0 = 0;
  gUnknown_02029664 = (new_var = 0);
  for (i = new_var; i < 2; i++)
  {
    gUnknown_02029C0C[i] = new_var;
    gUnknown_02029C10[i] = new_var;
    gUnknown_030045A0[i] = new_var;
    gUnknown_020298E0[i].unk18 = new_var;
    gUnknown_02029710[i].unk00 = new_var;
    gUnknown_02029808[i].unk00 = new_var;
    gUnknown_02029808[i].unk2e = new_var;
    gUnknown_03004548[i] = new_var;
    r = gUnknown_02028E4C + (i * 8);
    *((u16 *) r) = new_var;
    *((u16 *) (r + 2)) = 0;
    r[4] = 0;
    r[5] = 1;
    q = gUnknown_02028E5C[i];
    q[0] = 0;
    q[1] = new_var;
    gUnknown_020298E0[i].unk16 = new_var;
    gUnknown_020296B0[i].unk18 = new_var2;
    gUnknown_020296B0[i].unk1a = 0;
    gUnknown_020296B0[i].unk1b = 0;
    gUnknown_020298E0[i].unk8c = new_var;
    gUnknown_020298E0[i].unk8d = new_var;
    gUnknown_020296B0[i].unk1c = new_var;
    gUnknown_02029C00[i] = new_var;
    gUnknown_02029C04[i] = 0;
    gUnknown_02029C08[i] = 0;
    gUnknown_02029BE8[i] = 0;
  }

  for (i = 0; i < 2; i++)
  {
    for (j = 0; j < 6; j++)
    {
      gUnknown_020298E0[i].unk1a[j] = 0xff;
      ((struct V9808 *) (&gUnknown_02029808[i]))->unk02[j] = 0xff;
      gUnknown_02029816[i][j] = 0xff;
      gUnknown_020296BC[i][j] = 0xff;
    }

  }

  for (i = 0; i < 2; i++)
  {
    for (j = 0; j < 5; j++)
    {
      gUnknown_02029B80[i][j] = 0;
      gUnknown_020298E0[i].unk26[j] = 0;
      gUnknown_020298E0[i].unk30[j] = new_var;
      gUnknown_020298E0[i].unk3a[j] = 1;
      gUnknown_02029A10[i].entries[j].x = 0;
      gUnknown_02029A10[i].entries[j].y = new_var;
      gUnknown_02029A10[i].entries[j].xSub = 0;
      gUnknown_02029A10[i].entries[j].ySub = new_var;
      gUnknown_02029A10[i].entries[j].xStep = new_var;
      gUnknown_02029A10[i].entries[j].yStep = new_var;
      gUnknown_02029A10[i].entries[j].frameCount = new_var;
      gUnknown_02029A10[i].entries[j].frame = 0;
      gUnknown_02029A10[i].entries[j].unk1c = 0;
      gUnknown_02029A10[i].entries[j].unk1e = 0;
      gUnknown_02029A10[i].entries[j].unk20 = 0;
      gUnknown_02029A10[i].entries[j].unk22 = 0;
      gUnknown_02029BEC[i][j] = new_var;
      ((struct VA10E *) (&gUnknown_02029A10[i].entries[j]))->unk18 |= 0xffff;
      ((struct V9808 *) (&gUnknown_02029808[i]))->unk24[j] |= 0xffff;
      gUnknown_02029808[i].unk30[j] = new_var;
      gUnknown_02029B94[i][j] = new_var;
      gUnknown_02029822[i][j] = 0xff;
      ((struct V98E0 *) (&gUnknown_020298E0[i]))->unk02[j] |= 0xffff;
      ((struct V96B0 *) (&gUnknown_020296B0[i]))->unk02[j] |= 0xffff;
      ((struct V97C0 *) (&gUnknown_020297C0[i]))->unk02[j] |= 0xffff;
      ((struct V9710 *) (&gUnknown_02029710[i]))->unk0c[j] |= 0xffff;
      gUnknown_020297CC[i][j] = 0;
      q = gUnknown_02029668[i];
      ((struct VRow5 *) q)->v[j] |= 0xffff;
      ((struct VRow5 *) gUnknown_0202967C[i])->v[j] |= 0xffff;
      gUnknown_020296CE[i][j] = 0xff;
      gUnknown_020298EC[i][j] = 0xff;
      gUnknown_02029C14[i][j] = new_var;
    }

  }

  for (i = new_var; i < 0x800; i++)
  {
    gUnknown_085519FC[i] = 0x1ff;
  }

  if (gUnknown_03004504.bit0 == 0)
  {
    CpuFastSet(gUnknown_08551A00, (void *) 0x06002000, 0x200);
    CpuFastSet(gUnknown_08551A04, (void *) 0x06002800, 0x200);
    sp0 = new_var;
    CpuFastSet(&sp0, (void *) 0x06000000, 0x01002000);
  }
  i = 0;
  sp0 = new_var;
  CpuFastSet(&sp0, (void *) 0x06008000, 0x01002000);
  sp4 = i;
  CpuFastSet(&sp4, gUnknown_0200FC50, 0x01002000);
  sub_0801D8B4();
  CpuFastSet(gUnknown_08551A00, (void *) 0x06002000, 0x200);
  CpuFastSet(gUnknown_08551A04, (void *) 0x06002800, 0x200);
}








