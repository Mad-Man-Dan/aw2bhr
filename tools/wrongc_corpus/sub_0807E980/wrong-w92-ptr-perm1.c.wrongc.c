#include "global.h"
#include "proc.h"
#include "hardware.h"

struct Unk807E980
{
    /* 00
 *  WAVE 85, three counter spellings measured against the tail loop, none beat
 *  99.0%: `for (j = 7; j >= 0; j--)` with j-indexed addresses fixes the counter
 *  init position (movs r6,#7 first) but the address givs then fold +7*0x100 and
 *  step NEGATIVE via pool words (95.9%, +8); (7 - j) indexing breaks the
 *  pre-loop region (83.5%); explicit body-stepped src/dst pointer locals cost
 *  the frame (54.1%, +12, first diff +0xc).  The ROM's shape -- source counter
 *  init before the giv inits, positive steps, dst(0x100) before src(0x400) --
 *  is the pointer-variable form without the pseudo cost; unresolved.
 */ STRUCT_PAD(0x00, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ int unk34;
    /* 38 */ STRUCT_PAD(0x38, 0x48);
    /* 48 */ s16 unk48;
    /* 4a */ STRUCT_PAD(0x4a, 0x4c);
    /* 4c */ s16 unk4c;
    /* 4e */ STRUCT_PAD(0x4e, 0x52);
    /* 52 */ u16 unk52;
    /* 54 */ STRUCT_PAD(0x54, 0x58);
    /* 58 */ int unk58;
    /* 5c */ int unk5c;
    /* 60 */ int unk60;
    /* 64 */ s16 unk64;
};

void sub_0807E980(struct Unk807E980 *proc)
{
  u8 *new_var2;
  int i;
  int j;
  int x;
  int new_var;
  int src_p;
  int vram_p;
  if (gUnknown_03001FFC <= 8)
  {
    sub_08043FD8(0x40, 0x454, 0x2180, 5);
    new_var2 = gUnknown_0300599C;
    sub_08043FD8(0x88, 0x43c, 0x31a4, 5);
    sub_08043FD8(0xd0, 0x424, 0x41c8, 5);
    PutSpriteExt(0, 0x20, 0x41c, gUnknown_0848B6A0, 0);
    for (i = 0; i < proc->unk64; i++)
    {
      for (j = 0; j < 3; j++)
      {
        if (gUnknown_030058D4[i] == gUnknown_030058E0[(DivRem(proc->unk52 + j, gUnknown_03005948[proc->unk58 - proc->unk60]) + proc->unk5c) + (proc->unk60 * gUnknown_03005948[proc->unk58 - proc->unk60])])
        {
          sub_0801F34C(i + 0x4a, (j * 0x48) + 0x25, 0x400 | (0x43 - (j * 0x18)), 0, 0);
        }
      }

    }

    if (gUnknown_030059C0[proc->unk64])
    {
      for (i = 0; i < proc->unk64; i++)
      {
        sub_0801F34C(gUnknown_03005958[new_var2[i]] + 0x3e, 0x14, (i * 0x10) + 0x68, 0, 2);
      }

    }
    else
    {
      for (i = 0; i < sub_0803BD14(); i++)
      {
        sub_0801F34C(gUnknown_03005958[i] + 0x3e, 0x14, (i * 0x10) + 0x68, 0, 2);
      }

    }
    if (DivRem(proc->unk48, 0x40) <= 0x1f)
    {
      if ((((int) gUnknown_03005944) > 1) && gUnknown_030059C0[proc->unk64])
      {
        sub_0801F34C(0x43, 0x38, 0x1c, 0, 2);
        sub_0801F34C(0x44, 0x38, 0x54, 0, 2);
      }
      sub_0801F34C(0x48, 0x18 - ((DivRem(proc->unk48, 0x40) >> 3) * 2), (DivRem(proc->unk48, 0x40) >> 3) + 0x3c, 0, 0);
      sub_0801F34C(0x49, ((DivRem(proc->unk48, 0x40) >> 3) * 2) + 0x58, 0x2c - (DivRem(proc->unk48, 0x40) >> 3), 0, 0);
    }
    sub_0801F34C(0x62, 0xb9, 0x48e, 0, 0);
    sub_0801F34C(0x65, 0xb9, 0x496, 0, 0);
  }
  x = ((((3 - sub_0803BD14()) * 0x10) + 7) >> 1) + 0x68;
  if (proc->unk4c <= 8)
  {
    proc->unk2c = Interpolate(1, 0x2a, 0x75, proc->unk4c, 8);
    proc->unk30 = Interpolate(1, 0x68, x, proc->unk4c, 8);
  }
  else
  {
    proc->unk2c = Interpolate(4, 0x75, 0xc0, proc->unk4c - 8, 8);
    proc->unk30 = Interpolate(4, x, ((3 - sub_0803BD14()) * 0x10) + 0x6f, proc->unk4c - 8, 8);
  }
  if (proc->unk4c <= 7)
  {
    proc->unk34 = Interpolate(1, 0, 0x78, proc->unk4c, 8);
    sub_08043C28(proc->unk34 + 0xb0, 0xa0, 0x1040, 0, 1);
  }
  else
    if (proc->unk4c > 0xf)
  {
    sub_08043BA4(gUnknown_030058D4[0], DivRem(0, 2) * 0x140, 1);
    sub_08043BA4(gUnknown_030058D4[1], DivRem(1, 2) * 0x140, 2);
    Decompress(gUnknown_08234B10, gUnknown_0200FC50);
    for (i = 0; i < 4; i++)
    {
      src_p = (int) (&gUnknown_0200FC50[i * 0x100]);
      j = 7;
      vram_p = i;
      ;
      do
      {
        CpuFastSet((void *) src_p, (void *) (0x06015000 + (vram_p * 0x800)), 0x40);
        vram_p += 0x100;
        src_p += 0x400;
        j--;
      }
      while (j >= 0);
    }

    ApplyPaletteExt(gUnknown_082352DC, 0x280, 0x20);
    proc->unk4c = 0;
    Proc_Break(proc);
  }
  for (i = 0; i < proc->unk64; i++)
  {
    sub_08043B60(proc->unk2c, proc->unk30 + (i * 0x10), 0x9000 | ((i * 0xc) + 0x3c0), 0);
  }

  proc->unk4c++;
}
