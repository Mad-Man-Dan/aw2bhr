#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012C58.
 * sub_08012C58 @ 0x08012C58
 */

#include "hardware.h"
/*
 * SetupBackgrounds -- set all four backgrounds up from one descriptor and blank
 * them.
 *
 * arg points at sixteen words, four per background: unk00 and unk04 are that
 * background's tile-data and tilemap addresses in VRAM, unk08 is the tile
 * number used to blank the map, and unk0c is a fourth value. ResetBgShadows
 * resets things first; then SetBgCntChrBlock, SetBgCntTilemapBlock and SetBgCntScreenSize write
 * each background's control shadow -- gUnknown_03002B6C, gUnknown_03001FE8,
 * gUnknown_030030B4 and gUnknown_0300251C for BG0 to BG3.
 *
 * The display control word is set to mode 0 with all four backgrounds, the
 * sprites, free H-blank access and one-dimensional sprite mapping enabled, and
 * the four backgrounds take priorities 0, 2, 1 and 3.
 *
 * All 0x400 entries of each tilemap buffer are then filled with that
 * background's blank tile number, and the sixteen halfwords at
 * gUnknown_08489334 are written into each background's blank tile itself, at
 * tile-data base + tile number * 32. Finally each buffer's 0x800 bytes are sent
 * to the tilemap address the descriptor gave.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - `p3` is assigned once in the middle of the setup calls and again after the
 *     first loop. The first assignment is never read, but it changes the order
 *     in which the compiler lifts values out of that loop, and that order is the
 *     original's. Keep both.
 *   - The blank tile is written with one chained assignment,
 *     `p0[i] = p1[i] = p2[i] = p3[i] = ...`. Chaining right to left is what
 *     stores p3 first and p0 last, as the original does.
 *   - The eight display-control fields are written in this order: the mode, the
 *     five enable bits, then the two remaining bits of the first byte. The
 *     compiler keeps one read-modify-write of the first byte live across the
 *     store to the second, which is why the output interleaves them although
 *     the source does not.
 *   - The parameter stays `void *`, to agree with the header, and the struct is
 *     reached through the local `s` rather than by casting at each use.
 */

struct Unk8012C58
{
    /* 0x00 */ u32 unk00;
    /* 0x04 */ u32 unk04;
    /* 0x08 */ u32 unk08;
    /* 0x0c */ u32 unk0c;
    /* 0x10 */ u32 unk10;
    /* 0x14 */ u32 unk14;
    /* 0x18 */ u32 unk18;
    /* 0x1c */ u32 unk1c;
    /* 0x20 */ u32 unk20;
    /* 0x24 */ u32 unk24;
    /* 0x28 */ u32 unk28;
    /* 0x2c */ u32 unk2c;
    /* 0x30 */ u32 unk30;
    /* 0x34 */ u32 unk34;
    /* 0x38 */ u32 unk38;
    /* 0x3c */ u32 unk3c;
};

void SetupBackgrounds(void *arg)
{
  struct Unk8012C58 *s = arg;
  u16 *p0;
  u16 *p1;
  u16 *p2;
  u16 *p3;
  u16 i;
  ResetBgShadows();
  SetBgCntChrBlock((struct Unk8012C30 *) (&gUnknown_03002B6C), s->unk00);
  SetBgCntTilemapBlock((struct Unk8012C30 *) (&gUnknown_03002B6C), s->unk04);
  SetBgCntScreenSize((struct Unk8012C30 *) (&gUnknown_03002B6C), s->unk0c);
  SetBgCntChrBlock((struct Unk8012C30 *) (&gUnknown_03001FE8), s->unk10);
  SetBgCntTilemapBlock((struct Unk8012C30 *) (&gUnknown_03001FE8), s->unk14);
  SetBgCntScreenSize((struct Unk8012C30 *) (&gUnknown_03001FE8), s->unk1c);
  SetBgCntChrBlock((struct Unk8012C30 *) (&gUnknown_030030B4), s->unk20);
  p3 = (u16 *) (s->unk30 + (s->unk38 << 5));
  SetBgCntTilemapBlock((struct Unk8012C30 *) (&gUnknown_030030B4), s->unk24);
  SetBgCntScreenSize((struct Unk8012C30 *) (&gUnknown_030030B4), s->unk2c);
  SetBgCntChrBlock((struct Unk8012C30 *) (&gUnknown_0300251C), s->unk30);
  SetBgCntTilemapBlock((struct Unk8012C30 *) (&gUnknown_0300251C), s->unk34);
  SetBgCntScreenSize((struct Unk8012C30 *) (&gUnknown_0300251C), s->unk3c);
  gDispIo.disp_ct.mode = 0;
  gDispIo.disp_ct.bg0_enable = 1;
  gDispIo.disp_ct.bg1_enable = 1;
  gDispIo.disp_ct.bg2_enable = 1;
  gDispIo.disp_ct.bg3_enable = 1;
  gDispIo.disp_ct.obj_enable = 1;
  gDispIo.disp_ct.hblank_interval_free = 1;
  gDispIo.disp_ct.obj_mapping = 1;
  gUnknown_03002B6C.bits.priority = 0;
  gUnknown_03001FE8.bits.priority = 2;
  gUnknown_030030B4.bits.priority = 1;
  gUnknown_0300251C.bits.priority = 3;
  for (i = 0; i <= 0x3ff; i++)
  {
    gBG0TilemapBuffer[i] = s->unk08;
    gBG1TilemapBuffer[i] = s->unk18;
    gBG2TilemapBuffer[i] = s->unk28;
    gBG3TilemapBuffer[i] = s->unk38;
  }

  p0 = (u16 *) (s->unk00 + (s->unk08 << 5));
  p1 = (u16 *) (s->unk10 + (s->unk18 << 5));
  p2 = (u16 *) (s->unk20 + (s->unk28 << 5));
  p3 = (u16 *) (s->unk30 + (s->unk38 << 5));
  for (i = 0; i <= 0xf; i++)
  {
    p0[i] = (p1[i] = (p2[i] = (p3[i] = gUnknown_08489334[i])));
  }

  CpuCopyAuto(gBG0TilemapBuffer, (void *) s->unk04, 0x800);
  CpuCopyAuto(gBG1TilemapBuffer, (void *) s->unk14, 0x800);
  CpuCopyAuto(gBG2TilemapBuffer, (void *) s->unk24, 0x800);
  CpuCopyAuto(gBG3TilemapBuffer, (void *) s->unk34, 0x800);
}
asm(".global sub_08012C58\n.thumb_set sub_08012C58, SetupBackgrounds\n");
