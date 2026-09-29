#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F990.
 * sub_0803F990 @ 0x0803F990
 */

#include "map.h"
static inline u8 flags(int f) { return f; }
void DrawMapObjectSprites(void) {
  int k;
  int h;
  s16 cy;
  s16 cx;
  int w;
  s16 i;

  cx = gMap->scrollX / 16;
  cy = gMap->scrollY / 16;
  w = 15;
  h = 10;
  if (cx + 14 == gMap->width - 1)
    w = 14;
  if (cy + 9 == gMap->height - 1)
    h = 9;
  DrawInventionSprites(cx, cy, w, h);

  for (i = 0; i <= 0x5B; i++) {
    s8 raw = gProperty[i].flags;

    if (flags(raw) == 0xff)
      return;

    if (cx > gProperty[i].x || gProperty[i].x > cx + w ||
        cy > gProperty[i].y || gProperty[i].y > cy + h)
      continue;
    k = MAP_OBJ_ARMY(gProperty[i].flags);
    switch (MAP_OBJ_TERRAIN(gProperty[i].flags)) {
    case TERRAIN_CITY:
      PutMapObjectSprite(gProperty[i].x, gProperty[i].y,
                   gUnknown_0849F990, k, 1);
      break;
    case TERRAIN_BASE:
      if (gMap->unk234A[gMap->rowOffset[gProperty[i].y] +
                        gProperty[i].x] != 0)
        PutMapObjectSprite(gProperty[i].x, gProperty[i].y,
                     gUnknown_0849FAB0[k], k, 1);
      else
        PutMapObjectSprite(gProperty[i].x, gProperty[i].y,
                     gUnknown_0849FAB0[0], 0, 1);
      break;
    case TERRAIN_AIRPORT:
      PutMapObjectSprite(gProperty[i].x, gProperty[i].y,
                   gUnknown_0849F998, k, 1);
      break;
    case TERRAIN_PORT:
      PutMapObjectSprite(gProperty[i].x, gProperty[i].y,
                   gUnknown_0849F9A0, k, 1);
      break;
    case TERRAIN_HQ:
      PutMapObjectSprite(gProperty[i].x, gProperty[i].y,
                   gUnknown_0849FAC4[k], k, 1);
      break;
    case TERRAIN_LAB:
      PutMapObjectSprite(gProperty[i].x, gProperty[i].y,
                   gUnknown_0849F9A8, k, 1);
      break;
    case TERRAIN_SILO:
      PutMapObjectSprite(gProperty[i].x, gProperty[i].y,
                   gUnknown_0849F9B0, k, 1);
      break;
    case TERRAIN_SILO_USED:
      PutMapObjectSprite(gProperty[i].x, gProperty[i].y,
                   gUnknown_0849F9B8, k, 1);
      break;
    case TERRAIN_MINICANNON_S:
      PutMapObjectSprite(gProperty[i].x, gProperty[i].y,
                   gUnknown_0849F9D0, GetArmyByTeamColor(5), 0);
      break;
    case TERRAIN_MINICANNON_N:
      PutMapObjectSprite(gProperty[i].x, gProperty[i].y,
                   gUnknown_0849F9D8, GetArmyByTeamColor(5), 0);
      break;
    case TERRAIN_MINICANNON_W:
      PutMapObjectSprite(gProperty[i].x, gProperty[i].y,
                   gUnknown_0849F9C0, GetArmyByTeamColor(5), 0);
      break;
    case TERRAIN_MINICANNON_E:
      PutMapObjectSprite(gProperty[i].x, gProperty[i].y,
                   gUnknown_0849F9C8, GetArmyByTeamColor(5), 0);
      break;
    case TERRAIN_LASER:
      PutMapObjectSprite(gProperty[i].x, gProperty[i].y,
                   gUnknown_0849F9E0, GetArmyByTeamColor(5), 0);
      break;
    case 0xFF:
      return;
    }
  }
}
asm(".global sub_0803F990\n.thumb_set sub_0803F990, DrawMapObjectSprites\n");
