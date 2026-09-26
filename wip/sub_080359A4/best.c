#include "global.h"
#include "proc.h"
#include "map.h"

/* Draws this proc's map sprite, if the tile it stands on is on screen, visible
 * and occupied.
 *
 * The proc carries the sprite's map position in halfwords at +0x42 (x) and
 * +0x44 (y). Both are compared against the camera origin in gMap (scrollX /
 * scrollY) to reject anything outside the visible window; in one mode the
 * camera is scrolled to follow instead. When the current player is not in the
 * state that skips the check, the cell's byte in gMap->unk234A must be
 * non-zero. sub_080255F4 then decides whether the unit may be drawn at all,
 * and sub_0801C254 places the sprite at the on-screen offset.
 *
 * Why the C looks odd: the camera call is wrapped in a do/while that runs
 * once. That changes nothing about what runs, but it keeps the compiler from
 * reusing one register the original leaves alone.
 */

struct Unk359A4Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x2c);
    /* 0x2c */ struct Unk0801C210 *unk2c;
    /* 0x30 */ struct Unit *unk30;
    /* 0x34 */ u8 filler_34[0x01];
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 filler_36[0x0c];
    /* 0x42 */ s16 unk42;
    /* 0x44 */ s16 unk44;
};

void sub_080359A4(ProcPtr procArg)
{
  struct Unk359A4Proc *proc = procArg;
  s16 *py;
  s16 *px;
  s16 *py2;
  int y;
  py = &proc->unk44;
  y = *py;
  if (((u32) ((y - gMap->scrollY) + 8)) > 0xa8)
  {
    return;
  }
  px = &proc->unk42;
  if (((*px) - gMap->scrollX) < (-8))
  {
    return;
  }
  if (((*px) - gMap->scrollX) > 0xf0)
  {
    return;
  }
  if ((proc->unk35 == 2) && ((gPlaySt.savingEnabled == 0) || (gUnknown_030032D8 != 0x13)))
  {
    do
    {
      sub_080358C4(*px, *py);
      py2 = py;
    }
    while (0);
  }
  if ((gPlayers[gUnknown_030033EC].turnState & 2) == 0)
  {
    if (gMap->unk234A[gMap->rowOffset[((*py2) + 8) / 16] + ((proc->unk42 + 8) / 16)] == 0)
    {
      return;
    }
  }
  if (sub_080255F4(proc->unk30, (proc->unk42 + 8) / 16, ((*py2) + 8) / 16) == 0)
  {
    return;
  }
  sub_0801C254(proc->unk2c, ((*px) - gMap->scrollX) + 8, ((*py) - gMap->scrollY) + 5);
}
