#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073480.
 * sub_08073480 @ 0x08073480
 */

/* A per-scanline wave effect on background 3.
 *
 * Each frame this advances two phase accumulators, then fills the 160-entry
 * scanline table at gUnknown_0202FDE4 with a horizontal and a vertical scroll
 * value per line: a base scroll plus a sine wobble whose amplitude and phase
 * come from the process block. DMA channel 0 is then set to copy one entry
 * into BG3's scroll registers at every horizontal blank.
 *
 * Why the C looks odd. Every one of these spellings is needed to reproduce the
 * original's register use; all of them are no-ops to the reader:
 *  - gUnknown_0300200C and gUnknown_03002000, the shadow copies of BG3's
 *    scroll registers, are read through a volatile cast so that each pass
 *    re-reads them instead of caching one value for the whole loop.
 *  - The row pointer is built from an integer offset in two steps rather than
 *    as `gUnknown_0202FDE4 + i * 2`, which keeps the pointer load and the
 *    index arithmetic in the original's order.
 *  - `shift` just holds 8, and `bref` just points at `b`; both keep a value in
 *    a register of its own.
 *  - The dead `b = ...` assignments inside both stores' expressions are
 *    deliberate: each multiply needs its product in a register of its own, and
 *    assigning into a variable that is already dead at that point gives it one.
 */

#include "global.h"
#include "hardware.h"
struct Unk08073480Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
    /* 0x40 */ int unk40;
    /* 0x44 */ int unk44;
    /* 0x48 */ int unk48;
};

void BgWave_Loop(struct Unk08073480Proc *proc)
{
  int i;
  int shift;
  int a;
  int b;
  u16 *dst;
  void *base;
  int *bref;
  gUnknown_0202FDE4 = gUnknown_0202F8DC;
  shift = 8;
  proc->unk44 += proc->unk3c;
  proc->unk48 += proc->unk40;
  for (i = 0; i < 0xa0; i++)
  {
    a = ((i + proc->unk44) * proc->unk30) >> shift;
    b = ((i + proc->unk48) * proc->unk38) >> 8;
    base = gUnknown_0202FDE4;
    dst = (u16 *) (i * 4);
    dst = (u16 *) (((int) dst) + ((int) base));
    bref = &b;
    dst[0] = (((b = (*(((*bref) & 0xff) + gSinLut)) * proc->unk34) >> 20) + (proc->unk34 >> 16)) + (*((volatile u16 *) (&gUnknown_0300200C)));
    dst[1] = (((b = (*((a & 0xff) + gSinLut)) * proc->unk2c) >> 20) + (proc->unk2c >> 16)) + (*((volatile u16 *) (&gUnknown_03002000)));
  }

  *((vu16 *) (0x04000000 + 0x0BA)) = 0;
  *((vu32 *) (0x04000000 + 0x0B0)) = (u32) gUnknown_0202FDE4;
  *((vu32 *) (0x04000000 + 0x0B4)) = (u32) (&(*((vu16 *) (0x04000000 + 0x01C))));
  *((vu16 *) (0x04000000 + 0x0B8)) = 1;
  *((vu16 *) (0x04000000 + 0x0BA)) = 0xA640;
}
asm(".global sub_08073480\n.thumb_set sub_08073480, BgWave_Loop\n");
