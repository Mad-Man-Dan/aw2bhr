#include "global.h"
#include "hardware.h"
#include "map.h"

void MakeTileSimple(int x, int y, int val) {
  if (val < 0)
    return;

  gMap->tile[gMap->rowOffset[y] + x] = val & 0x1FF;
}

asm(".global sub_08001158\n.thumb_set sub_08001158, MakeTileSimple\n");

int IsTerrainAtCoordsType(int x, int y, int k) {
  if (x < 0 || y < 0 || y > gMap->height - 1 || x > gMap->width - 1)
    return 0;

  return gMap->terrain[gMap->rowOffset[y] + x] == k;
}

asm(".global sub_0800119C\n.thumb_set sub_0800119C, IsTerrainAtCoordsType\n");

void SetTerrainAt(int x, int y, int val) {
  gMap->terrain[gMap->rowOffset[y] + x] = val;

  if (val == TERRAIN_SEA)
    EnsureValidTile(x, y);
}

asm(".global sub_080011F4\n.thumb_set sub_080011F4, SetTerrainAt\n");

int GetDesignRoomOption(int a) {
  switch (a & 0x1f) {
  case TERRAIN_PLAIN:
    return TERRAIN_PLAIN;
  case TERRAIN_RIVER:
    return TERRAIN_RIVER;
  case TERRAIN_MOUNTAIN:
    return TERRAIN_MOUNTAIN;
  case TERRAIN_WOOD:
    return TERRAIN_WOOD;
  case TERRAIN_ROAD:
    return TERRAIN_ROAD;
  case TERRAIN_CITY:
    return TERRAIN_CITY;
  case TERRAIN_SEA:
    return TERRAIN_SEA;
  case TERRAIN_HQ:
    return TERRAIN_HQ;
  case TERRAIN_AIRPORT:
    return TERRAIN_AIRPORT;
  case TERRAIN_PORT:
    return TERRAIN_PORT;
  case TERRAIN_BRIDGE:
    return TERRAIN_BRIDGE;
  case TERRAIN_SHOAL:
    return TERRAIN_SHOAL;
  case TERRAIN_BASE:
    return TERRAIN_BASE;
  case TERRAIN_REEF:
    return TERRAIN_REEF;
  case TERRAIN_PIPE:
    return TERRAIN_PIPE;
  case TERRAIN_PIPE_SEAM:
    return TERRAIN_PIPE_SEAM;
  case TERRAIN_SILO:
    return TERRAIN_SILO;
  }

  return 0;
}

asm(".global sub_08001230\n.thumb_set sub_08001230, GetDesignRoomOption\n");

int GetDefaultTileForTerrain(int a) {
  int r;

  r = 0;

  switch (a) {
  case TERRAIN_CITY:
    r = TILE_CITY;
    break;
  case TERRAIN_CITY_OS:
    r = TILE_CITY_OS;
    break;
  case TERRAIN_CITY_BM:
    r = TILE_CITY_BM;
    break;
  case TERRAIN_CITY_GE:
    r = TILE_CITY_GE;
    break;
  case TERRAIN_CITY_YC:
    r = TILE_CITY_YC;
    break;

  case TERRAIN_HQ:
    r = TILE_HQ;
    break;
  case TERRAIN_HQ_OS:
    r = TILE_HQ_OS;
    break;
  case TERRAIN_HQ_BM:
    r = TILE_HQ_BM;
    break;
  case TERRAIN_HQ_GE:
    r = TILE_HQ_GE;
    break;
  case TERRAIN_HQ_YC:
    r = TILE_HQ_YC;
    break;

  case TERRAIN_AIRPORT:
    r = TILE_AIRPORT;
    break;
  case TERRAIN_AIRPORT_OS:
    r = TILE_AIRPORT_OS;
    break;
  case TERRAIN_AIRPORT_BM:
    r = TILE_AIRPORT_BM;
    break;
  case TERRAIN_AIRPORT_GE:
    r = TILE_AIRPORT_GE;
    break;
  case TERRAIN_AIRPORT_YC:
    r = TILE_AIRPORT_YC;
    break;

  case TERRAIN_PORT:
    r = TILE_PORT;
    break;
  case TERRAIN_PORT_OS:
    r = TILE_PORT_OS;
    break;
  case TERRAIN_PORT_BM:
    r = TILE_PORT_BM;
    break;
  case TERRAIN_PORT_GE:
    r = TILE_PORT_GE;
    break;
  case TERRAIN_PORT_YC:
    r = TILE_PORT_YC;
    break;

  case TERRAIN_BASE:
    r = TILE_BASE;
    break;
  case TERRAIN_BASE_OS:
    r = TILE_BASE_OS;
    break;
  case TERRAIN_BASE_BM:
    r = TILE_BASE_BM;
    break;
  case TERRAIN_BASE_GE:
    r = TILE_BASE_GE;
    break;
  case TERRAIN_BASE_YC:
    r = TILE_BASE_YC;
    break;

  case TERRAIN_SILO:
    r = TILE_SILO;
    break;
  }

  return r;
}

asm(".global sub_080012DC\n.thumb_set sub_080012DC, GetDefaultTileForTerrain\n");

int IsTerrainLand(int x, int y) {
  struct Map *map = gMap;
  int v;
  int w;
  int s;
  int r;

  v = map->terrain[map->rowOffset[y] + x];

  if (v == TERRAIN_RIVER)
    s = sub_080094EC(x, y);
  else
    s = 0;

  w = sub_08008C34(x, y);

  r = 0;
  if (v != TERRAIN_SEA && v != TERRAIN_SHOAL && v != TERRAIN_REEF && w == 0 &&
      s == 0)
    r = 1;

  return r;
}

asm(".global sub_080015E4\n.thumb_set sub_080015E4, IsTerrainLand\n");

int IsTerrainNotWater(int x, int y) {
  struct Map *map = gMap;
  int v;
  int r;

  v = map->terrain[map->rowOffset[y] + x];

  r = 0;

  if (v != TERRAIN_SEA && v != TERRAIN_SHOAL)
    r = (v != TERRAIN_REEF);

  return r;
}

asm(".global sub_0800164C\n.thumb_set sub_0800164C, IsTerrainNotWater\n");

int IsTerrainNotWaterOrRiver(int x, int y) {
  struct Map *map = gMap;
  int v;
  int r;

  v = map->terrain[map->rowOffset[y] + x];

  r = 0;

  if (v != TERRAIN_SEA && v != TERRAIN_SHOAL && v != TERRAIN_REEF)
    r = (v != TERRAIN_RIVER);

  return r;
}

asm(".global sub_0800168C\n.thumb_set sub_0800168C, IsTerrainNotWaterOrRiver\n");

int GetTileWithShadowAt(int x, int y) {
  struct Map *map = gMap;

  return GetTileWithShadow(x, y, map->tile[map->rowOffset[y] + x]);
}

asm(".global sub_080016D0\n.thumb_set sub_080016D0, "
    "GetTileWithShadowAt\n");

int GetTileWithShadow(int x, int y, int v) {
  const s16 *pa;
  const s16 *pb;
  int off;
  int i;

  pa = gUnknown_0848591C[0];
  pb = gUnknown_0848591C[1];

  if (x <= 0) {
    for (i = 0; i <= 0x30; i++) {
      if (v == *pb)
        return *pa;
      pa++;
      pb++;
    }
  } else {
    off = gMap->rowOffset[y] - 1;
    off += x;

    switch (gMap->terrain[off]) {
    case TERRAIN_MOUNTAIN:
    case TERRAIN_WOOD:
    case TERRAIN_CITY:
    case TERRAIN_HQ:
    case TERRAIN_AIRPORT:
    case TERRAIN_PORT:
    case TERRAIN_BASE:
    case TERRAIN_CITY_OS:
    case TERRAIN_HQ_OS:
    case TERRAIN_AIRPORT_OS:
    case TERRAIN_PORT_OS:
    case TERRAIN_BASE_OS:
    case TERRAIN_CITY_BM:
    case TERRAIN_HQ_BM:
    case TERRAIN_AIRPORT_BM:
    case TERRAIN_PORT_BM:
    case TERRAIN_BASE_BM:
    case TERRAIN_CITY_GE:
    case TERRAIN_HQ_GE:
    case TERRAIN_AIRPORT_GE:
    case TERRAIN_PORT_GE:
    case TERRAIN_BASE_GE:
    case TERRAIN_CITY_YC:
    case TERRAIN_HQ_YC:
    case TERRAIN_AIRPORT_YC:
    case TERRAIN_PORT_YC:
    case TERRAIN_BASE_YC:
      for (i = 0; i <= 0x30; i++) {
        if (v == *pa) {
          if (*pb == 0x21 && IsTerrainAtCoordsType(x, y + 1, TERRAIN_MOUNTAIN))
            return 3;
          return *pb;
        }
        pa++;
        pb++;
      }
      break;
    default:
      for (i = 0; i <= 0x30; i++) {
        if (v == *pb)
          return *pa;
        pa++;
        pb++;
      }
      break;
    }
  }

  return -1;
}

asm(".global sub_08001704\n.thumb_set sub_08001704, GetTileWithShadow\n");

int GetTileWithShadow2(int x, int y, int v) {
  const s16 *pa;
  const s16 *pb;
  int off;
  int i;

  pa = gUnknown_0848591C[0];
  pb = gUnknown_0848591C[1];

  if (x <= 0) {
    for (i = 0; i <= 0x30; i++) {
      if (v == *pb)
        return *pa;
      pa++;
      pb++;
    }
  } else {
    off = gMap->rowOffset[y] - 1;
    off += x;

    switch (gMap->terrain[off]) {
    case TERRAIN_MOUNTAIN:
    case TERRAIN_WOOD:
    case TERRAIN_CITY:
    case TERRAIN_HQ:
    case TERRAIN_AIRPORT:
    case TERRAIN_PORT:
    case TERRAIN_BASE:
    case TERRAIN_CITY_OS:
    case TERRAIN_HQ_OS:
    case TERRAIN_AIRPORT_OS:
    case TERRAIN_PORT_OS:
    case TERRAIN_BASE_OS:
    case TERRAIN_CITY_BM:
    case TERRAIN_HQ_BM:
    case TERRAIN_AIRPORT_BM:
    case TERRAIN_PORT_BM:
    case TERRAIN_BASE_BM:
    case TERRAIN_CITY_GE:
    case TERRAIN_HQ_GE:
    case TERRAIN_AIRPORT_GE:
    case TERRAIN_PORT_GE:
    case TERRAIN_BASE_GE:
    case TERRAIN_CITY_YC:
    case TERRAIN_HQ_YC:
    case TERRAIN_AIRPORT_YC:
    case TERRAIN_PORT_YC:
    case TERRAIN_BASE_YC:
      for (i = 0; i <= 0x30; i++) {
        if (v == *pa)
          return *pb;
        pa++;
        pb++;
      }
      break;
    default:
      for (i = 0; i <= 0x30; i++) {
        if (v == *pb)
          return *pa;
        pa++;
        pb++;
      }
      break;
    }
  }

  return -1;
}

asm(".global sub_08001A04\n.thumb_set sub_08001A04, GetTileWithShadow2\n");

s16 sub_08001CE8(void) { return gUnknown_0200B224[gActiveMap->selectionIndex].unk00; }

int sub_08001D04(int a1) {
  const u8 *p;
  int result;

  p = gUnknown_084859E0;
  result = 0xE;
  for (;;) {
    if (p[0] == a1) {
      result = p[1];
      break;
    }
    if (p[0] == 0xFF)
      break;
    p += 2;
  }
  return result;
}

int DesignRoomFindItemIndex(int a1) {
  int i;

  if (gActiveMap->editMode == 0) {
    a1 &= 0x1F;
    for (i = 0; i <= 0x10; i++) {
      if ((gUnknown_0200B224[i].unk00 & 0x1F) == a1)
        return i;
    }
  } else {
    a1 &= 0x3F;
    for (i = 0; i <= 0x13; i++) {
      if ((gUnknown_0200B224[i].unk00 & 0x3F) == a1)
        return i;
    }
  }

  return -1;
}
asm(".global sub_08001D24\n.thumb_set sub_08001D24, DesignRoomFindItemIndex\n");

void DesignRoomShowTilePanel(void) { gActiveMap->tilePanelYState = 10; }
asm(".global sub_08001D8C\n.thumb_set sub_08001D8C, DesignRoomShowTilePanel\n");

void DesignRoomHideTilePanel(void) { gActiveMap->tilePanelYState = 0; }
asm(".global sub_08001D9C\n.thumb_set sub_08001D9C, DesignRoomHideTilePanel\n");

void DesignRoomUpdateSelectionPanel(void) {
  int flag;
  int b;
  int sx;
  int sy;
  int idx;
  u16 m1;
  u16 m2;
  u16 m3;
  u16 m4;

  sx = gActiveMap->cursorX - (gMap->scrollX >> 4);
  flag = 1;

  switch (gActiveMap->tilePanelYState) {
  case 0:
    gActiveMap->tilePanelYSpeed += 2;
    if (gActiveMap->tilePanelYSpeed > 7)
      gActiveMap->tilePanelYSpeed = 8;
    gActiveMap->tilePanelY += gActiveMap->tilePanelYSpeed;
    if (gActiveMap->tilePanelY > 0xB7) {
      gActiveMap->tilePanelY = 0xB8;
      gActiveMap->tilePanelYState = flag;
    }
    flag = 0;
    break;
  case 1:
    return;
  case 0xA:
    gActiveMap->tilePanelYSpeed += 1;
    if (gActiveMap->tilePanelYSpeed > 7)
      gActiveMap->tilePanelYSpeed = 8;
    gActiveMap->tilePanelY -= gActiveMap->tilePanelYSpeed;
    if (gActiveMap->tilePanelY <= 0x6A) {
      gActiveMap->tilePanelY = 0x6A;
      gActiveMap->tilePanelYState = 0xB;
    }
    break;
  }

  switch (gActiveMap->tilePanelState) {
  case 0xA:
    gActiveMap->tilePanelX += (0x1180 - gActiveMap->tilePanelX) >> 3;
    if (gActiveMap->tilePanelX > 0xEFF) {
      gActiveMap->tilePanelX = 0xFD80;
      gActiveMap->tilePanelState = 0x14;
    }
    break;
  case 0x14:
    gActiveMap->tilePanelX += (0xA0 - gActiveMap->tilePanelX) >> 3;
    if (gActiveMap->tilePanelX >= 0) {
      gActiveMap->tilePanelX = 0;
      gActiveMap->tilePanelState = 0x1E;
    }
    break;
  case 0:
  case 0x1E:
    if (sx <= 6) {
      gActiveMap->panelSide = 1;
      gActiveMap->tilePanelState = 0x64;
    }
    break;
  case 0x64:
    gActiveMap->tilePanelX += (-960 - gActiveMap->tilePanelX) >> 3;
    if (gActiveMap->tilePanelX < -640) {
      gActiveMap->tilePanelX = 0x11A0;
      gActiveMap->tilePanelState = 0x6E;
    }
    break;
  case 0x6E:
    gActiveMap->tilePanelX += (0xB40 - gActiveMap->tilePanelX) >> 3;
    if (gActiveMap->tilePanelX <= 0xCA0) {
      gActiveMap->tilePanelX = 0xCA0;
      gActiveMap->tilePanelState = 0x78;
    }
    break;
  case 0x78:
    if (sx > 7) {
      gActiveMap->panelSide = 0;
      gActiveMap->tilePanelState = 0xA;
    }
    break;
  }

  sx = gActiveMap->tilePanelX >> 4;
  b = gActiveMap->editMode != 0;

  if (b == 0) {
    if ((gActiveMap->flags & 0x40) == 0)
      DesignRoomDrawTerrainName(0, sx + 7, gActiveMap->tilePanelY + 0x26,
                   gActiveMap->selectedTerrain, b, flag);
    sy = gActiveMap->tilePanelY;
    if ((gActiveMap->selectedTerrain & 0x1F) == 8)
      sy += 6;
  } else {
    m1 = gActiveMap->flags & 0x40;
    if (m1 == 0)
      DesignRoomDrawUnitName(0, sx + 7, gActiveMap->tilePanelY + 0x26, gActiveMap->cursorUnit, m1,
                   flag);
    sy = gActiveMap->tilePanelY;
  }

  if (gActiveMap->cursorMoved != 0) {
    switch (gActiveMap->cursorMoveState) {
    case 0:
      gActiveMap->cursorMoveTimer = 0xA;
      gActiveMap->cursorMoveState = 8;
      gActiveMap->cursorMoveScale = 0;
      break;
    case 9:
      DesignRoomPickUnderCursor();
    case 8:
      gActiveMap->cursorMoveState++;
    case 0xA:
      gActiveMap->cursorMoveScale += 0x20;
      if (gActiveMap->cursorMoveScale > 0xFF)
        gActiveMap->cursorMoveScale = 0x100;
      idx = (0x100 - gActiveMap->cursorMoveScale) >> 4;
      SetObjAffine(0,
                   Div(gSinLut[0x40 + (idx & 0xFF)] << 4,
                       gActiveMap->cursorMoveScale != 0 ? gActiveMap->cursorMoveScale : 2),
                   Div(-gSinLut[idx & 0xFF] << 4, 0x100),
                   Div(gSinLut[idx & 0xFF] << 4,
                       gActiveMap->cursorMoveScale != 0 ? gActiveMap->cursorMoveScale : 2),
                   Div(gSinLut[0x40 + (idx & 0xFF)] << 4, 0x100));
      gActiveMap->cursorMoveTimer--;
      if (gActiveMap->cursorMoveTimer < 0) {
        gActiveMap->cursorMoveState = 0;
        gActiveMap->cursorMoved = 0;
      }
      break;
    }

    m2 = gActiveMap->flags & 0x40;
    if (m2 == 0) {
      if (b == 0)
        DesignRoomDrawTerrainIcon(0, sx + 0xB, sy, gActiveMap->selectedTerrain, 1, b, flag);
      else
        DesignRoomDrawUnitIcon(0, sx + 0xB, sy, gActiveMap->cursorUnit, 1, 0, flag);
    }
  } else {
    m3 = gActiveMap->flags & 0x40;
    if (m3 == 0) {
      if (b == 0)
        DesignRoomDrawTerrainIcon(0, sx + 0xB, sy, gActiveMap->selectedTerrain,
                     gActiveMap->spriteFrame, m3, flag);
      else
        DesignRoomDrawUnitIcon(0, sx + 0xB, sy, gActiveMap->cursorUnit, gActiveMap->spriteFrame, m3,
                     flag);
    }
  }

  DesignRoomUpdateArmyPanels(sx, sy);

  if (gActiveMap->tilePanelY > 0x9F)
    return;

  sub_08002510(sx, 0x26);

  m4 = gActiveMap->flags & 0x40;
  if (m4 == 0) {
    sub_08002298(sx, gActiveMap->tilePanelY - 10);
    DrawOamObject(0xAA, (sx + 2) & 0x1FF, (gActiveMap->tilePanelY - 1) & 0xFF, m4, 0);
    PutSprite(5, sx, gActiveMap->tilePanelY, gUnknown_08485B2C, 0x3000);
  }

  if (gActiveMap->mode != 4)
    ApplyPaletteExt(gUnknown_084891C0, 0x260, 0x20);
}
asm(".global sub_08001DAC\n.thumb_set sub_08001DAC, DesignRoomUpdateSelectionPanel\n");

void sub_08002298(int a1, int a2) {
  int a;

  a2 &= 0xFF;

  switch (gActiveMap->countPanelState) {
  case 0:
    if (gActiveMap->editMode == 0) {
      if (GetPropertyKindForTerrain(gActiveMap->selectedTerrain) == 0)
        return;
    } else if (gActiveMap->cursorUnit == 0x19) {
      return;
    }
    gActiveMap->countPanelState = gActiveMap->panelSide != 0 ? 0xA : 0x32;
    gActiveMap->countPanelX = 0x1180;
    break;
  case 0xA:
    gActiveMap->countPanelX += (0xBE0 - gActiveMap->countPanelX) >> 3;
    if (gActiveMap->countPanelX <= 0xCC0) {
      gActiveMap->countPanelX = 0xCC0;
      gActiveMap->countPanelState = 0x14;
    }
    break;
  case 0x14:
    if (gActiveMap->editMode == 0) {
      if (gActiveMap->panelSide == 0 ||
          GetPropertyKindForTerrain(gActiveMap->selectedTerrain) == 0)
        gActiveMap->countPanelState = 0x1E;
    } else if (gActiveMap->panelSide == 0 || gActiveMap->cursorUnit == 0x19) {
      gActiveMap->countPanelState = 0x1E;
    }
    break;
  case 0x1E:
    gActiveMap->countPanelX += (0x1180 - gActiveMap->countPanelX) >> 3;
    if (gActiveMap->countPanelX > 0x10DF) {
      gActiveMap->countPanelX = 0x10E0;
      gActiveMap->countPanelState = gActiveMap->panelSide == 0 ? 0x32 : 0;
    }
    break;
  }

  switch (gActiveMap->countPanelState) {
  case 0x32:
    if (gActiveMap->editMode == 0) {
      if (GetPropertyKindForTerrain(gActiveMap->selectedTerrain) == 0)
        return;
    } else if (gActiveMap->cursorUnit == 0x19) {
      return;
    }
    gActiveMap->countPanelState = gActiveMap->panelSide == 0 ? 0x3C : 0;
    gActiveMap->countPanelX = 0xFD80;
    break;
  case 0x3C:
    gActiveMap->countPanelX += (0x320 - gActiveMap->countPanelX) >> 3;
    if (gActiveMap->countPanelX > 0xBF) {
      gActiveMap->countPanelX = 0xC0;
      gActiveMap->countPanelState = 0x46;
    }
    break;
  case 0x46:
    if (gActiveMap->editMode == 0) {
      if (gActiveMap->panelSide != 0 ||
          GetPropertyKindForTerrain(gActiveMap->selectedTerrain) == 0)
        gActiveMap->countPanelState = 0x50;
    } else if (gActiveMap->panelSide != 0 || gActiveMap->cursorUnit == 0x19) {
      gActiveMap->countPanelState = 0x50;
    }
    break;
  case 0x50:
    gActiveMap->countPanelX += (-800 - gActiveMap->countPanelX) >> 3;
    if (gActiveMap->countPanelX <= -640) {
      gActiveMap->countPanelX = -640;
      gActiveMap->countPanelState = gActiveMap->panelSide == 0 ? 0x32 : 0;
    }
    break;
  }

  a = gActiveMap->countPanelX >> 4;
  DrawOamObject(0x54, (a - 0xC) & 0x1FF, a2 + 1, 0, 0);
  if (gActiveMap->editMode == 0)
    DrawSpriteNumberFont2((a + 0x1A) & 0x1FF, a2, 0x3C - (s8)gActiveMap->propertyCount);
  else
    DrawSpriteNumberFont2(
        (a + 0x1A) & 0x1FF, a2,
        0x32 - (s8)((struct ActiveMap *)((u8 *)gActiveMap + gActiveMap->unitArmy))
                   ->propertyCount);
}

void sub_08002510(int a1, int a2) {
  int v;

  v = a2 & 0xFF;

  switch (gActiveMap->sidePanelState) {
  case 0:
    if (sub_0800C9E8() == 0)
      return;
    gActiveMap->sidePanelState = gActiveMap->panelSide != 0 ? 0xA : 0x32;
    gActiveMap->sidePanelX = 0x1180;
    break;
  case 0xA:
    gActiveMap->sidePanelX += (0xB40 - gActiveMap->sidePanelX) >> 3;
    if (gActiveMap->sidePanelX <= 0xD60) {
      gActiveMap->sidePanelX = 0xD60;
      gActiveMap->sidePanelState = 0x14;
    }
    break;
  case 0x14:
    if (sub_0800C9E8() == 0 || gActiveMap->panelSide == 0 || gActiveMap->mode != 1)
      gActiveMap->sidePanelState = 0x1E;
    break;
  case 0x1E:
    gActiveMap->sidePanelX += (0x12A0 - gActiveMap->sidePanelX) >> 3;
    if (gActiveMap->sidePanelX > 0x10DF) {
      gActiveMap->sidePanelX = 0x10E0;
      if (sub_0800C9E8() == 0)
        return;
      if (gActiveMap->mode == 1)
        gActiveMap->sidePanelState = gActiveMap->panelSide == 0 ? 0x32 : 0;
    }
    break;
  }

  switch (gActiveMap->sidePanelState) {
  case 0x32:
    if (sub_0800C9E8() == 0)
      return;
    gActiveMap->sidePanelState = gActiveMap->panelSide == 0 ? 0x3C : 0;
    gActiveMap->sidePanelX = 0xFD80;
    break;
  case 0x3C:
    gActiveMap->sidePanelX += (0x3C0 - gActiveMap->sidePanelX) >> 3;
    if (gActiveMap->sidePanelX > 0x19F) {
      gActiveMap->sidePanelX = 0x1A0;
      gActiveMap->sidePanelState = 0x46;
    }
    break;
  case 0x46:
    if (sub_0800C9E8() == 0 || gActiveMap->panelSide != 0 || gActiveMap->mode != 1)
      gActiveMap->sidePanelState = 0x50;
    break;
  case 0x50:
    gActiveMap->sidePanelX += (-928 - gActiveMap->sidePanelX) >> 3;
    if (gActiveMap->sidePanelX <= -480) {
      gActiveMap->sidePanelX = -480;
      if (sub_0800C9E8() == 0)
        return;
      if (gActiveMap->mode == 1)
        gActiveMap->sidePanelState = gActiveMap->panelSide == 0 ? 0x32 : 0;
    }
    break;
  }

  DrawOamObject(0x8F, ((gActiveMap->sidePanelX >> 4) - 0x18) & 0x1FF, v, 0, 0);
  gActiveMap->sidePanelFrame = (gActiveMap->sidePanelFrame + 1) & 0x3F;
}

void DesignRoomDrawTerrainIcon(int a1, int a2, int a3, int a4, int a5, int a6, int a7) {
  void *dest;

  dest = (void *)(0x06010000 + ((gUnknown_08485C9C[a1] & 0x3FF) << 5));

  if ((a4 & 0x1F) == 8) {
    switch (a4) {
    case 0x08:
    case 0x28:
      LoadTerrainObjTiles(8, 1, dest, a7);
      break;
    case 0x48:
      LoadTerrainObjTiles(8, 2, dest, a7);
      break;
    case 0x68:
      LoadTerrainObjTiles(8, 3, dest, a7);
      break;
    case 0x88:
      LoadTerrainObjTiles(8, 4, dest, a7);
      break;
    }
  } else {
    LoadTerrainObjTiles(a4 & 0x1F, 1, dest, a7);
    a3 += 8;
  }

  if (a5) {
    int attr1;

    attr1 = a2 & 0x1FF;
    if (a6)
      attr1 |= 0x1000;
    a3 = (a3 & 0xFF) | 0x500;
    PutOamHi(attr1, a3, gUnknown_08485CC8[a1], sub_08001D04(a4) << 12);
  } else {
    int attr1;

    attr1 = a2 & 0x1FF;
    if (a6)
      attr1 |= 0x1000;
    a3 = (a3 & 0xFF) | 0x400;
    PutOamHi(attr1, a3, gUnknown_08485CC8[a1], sub_08001D04(a4) << 12);
  }
}
asm(".global sub_0800272C\n.thumb_set sub_0800272C, DesignRoomDrawTerrainIcon\n");

void DesignRoomDrawUnitIcon(int a1, int a2, int a3, int a4, int a5, int a6, int a7) {
  int cls;
  int attr0, attr1;
  int pal;

  pal = 6;
  cls = (s8)gActiveMap->unitArmy;
  if (cls == 0)
    cls = 1;

  a4 &= 0x3F;
  if (a4 != 0x19) {
    if (a7) {
      RegisterDataMove(
          GetUnitSheetGraphics() + ((GetUnitSpriteTile(cls, a4) & 0x3FF) << 5),
          (void *)(0x06010000 + ((gUnknown_08485D20[a1] & 0x3FF) << 5)), 0x80);
    }
    ApplyPalette(
        (u16 *)(gUnknown_0810E6E0 + (gPlayers[cls].teamColor - 1) * 0x20), 22);
  } else {
    CopyGraphicToPoolTiles(0x12, (void *)0x06010000, gUnknown_08485D20[a1]);
    ApplyPalette(gUnknown_081268D8, 30);
  }

  a3 += 8;

  attr1 = a2 & 0x1FF;
  if (a6)
    attr1 |= 0x1000;
  attr0 = ((a3 + 8) & 0xFF) | 0x400;
  if (a5)
    attr0 |= 0x100;
  PutOamHi(attr1, attr0, gUnknown_08485D44[a1],
               a4 != 0x19 ? pal << 12 : 0xE000);
}
asm(".global sub_08002844\n.thumb_set sub_08002844, DesignRoomDrawUnitIcon\n");

void DesignRoomDrawTerrainName(int a1, int a2, int a3, int a4, int a5, int a6) {
  int idx;
  int attr0, attr1;

  GetDesignRoomOption(a4);
  if (a1 != 0)
    idx = a1 * 8 + 0x2B2;
  else
    idx = 0x262;

  if (a6)
    RegisterDataMove((void *)GetTerrainNameGraphic(a4 & 0x1F),
                 (void *)(0x06010000 + (idx << 5)), 0x100);

  attr1 = (a2 - 4) & 0x1FF;
  attr0 = (a3 & 0xFF) | 0x400;
  if (a5)
    attr0 |= 0x100;
  PutOamHi(attr1, attr0, gUnknown_08485CF4[a1], 0x1000);
}
asm(".global sub_08002964\n.thumb_set sub_08002964, DesignRoomDrawTerrainName\n");

void DesignRoomDrawUnitName(int a1, int a2, int a3, int a4, int a5, int a6) {
  u8 t;
  int k;
  void *src;
  int attr0;
  int attr1;
  int idxBase;

  t = gUnknown_02028E40;
  k = a4 & 0x3F;
  idxBase = 0x28C;

  if (a6 != 0) {
    if (a1 == 0) {
      if (k != 0x19)
        src = (void *)GetUnitNameGraphic(k);
      else
        src = gUnknown_08489190[t];

      RegisterDataMove(src, (void *)0x06014BC0, 0x100);
    } else {
      if (k != 0x19)
        src = (void *)GetUnitNameGraphic(k);
      else
        src = gUnknown_08489190[t];

      t = (a1 - 1) * 8;
      RegisterDataMove(
          src, (void *)(0x06010000 + ((((a1 - 1) * 8) + idxBase) << 5)), 0x100);
    }
  }

  attr1 = (a2 - 4) & 0x1FF;
  attr0 = (a3 & 0xFF) | 0x400;
  if (a5)
    attr0 |= 0x100;

  PutOamHi(attr1, attr0, gUnknown_08485D68[a1], 0x1000);
}
asm(".global sub_080029F4\n.thumb_set sub_080029F4, DesignRoomDrawUnitName\n");

void DesignRoomDrawTerrainRing(void) {
  int i;
  int j;
  int flag;
  struct DesignRingEntry *q;

  if (gActiveMap->flags & 0x10) {
    PutOamHi(0x78, 0x48C, (void *)gUnknown_08485B52, 0);
    if (gActiveMap->state == 0x33) {
      flag = gActiveMap->editMode;
      j = gActiveMap->ringIndex + 3;
      if (flag == 0)
        j = gActiveMap->ringIndex + 4;
      if (j > 9)
        j -= 10;
      switch (gDesignRing[j].itemId & 0x1F) {
      case 6:
      case 8:
      case 10:
      case 11:
      case 14:
        if (gActiveMap->spriteId == -1)
          sub_08007B54();
        break;
      }
    } else {
      sub_08007B74();
    }
  }

  j = gActiveMap->ringIndex;
  for (i = 9; i >= 0; i--) {
    q = &gDesignRing[j];
    j++;
    if (j > 9)
      j -= 10;
    if (q->flags & 1) {
      if (!(q->flags & 0x80))
        DesignRoomDrawTerrainName(q->spriteSlot + 1, (q->x >> 8) - 4, (q->y >> 8) + 0x21,
                     q->itemId, q->flags & 0x20, q->flags & 8);
      if (!(q->flags & 0x40))
        DesignRoomDrawTerrainIcon(q->spriteSlot + 1, q->x >> 8, q->y >> 8, q->itemId,
                     q->flags & 0x10, q->flags & 0x100, q->flags & 8);
      q->flags &= ~8;
    }
  }
}
asm(".global sub_08002AB0\n.thumb_set sub_08002AB0, DesignRoomDrawTerrainRing\n");

void DesignRoomDrawUnitRing(void) {
  int i;
  int j;
  struct DesignRingEntry *q;

  if (gActiveMap->flags & 0x10) {
    PutOamHi(0x78, 0x48C, (void *)gUnknown_08485B52, 0);
    if (gActiveMap->state == 0x33) {
      j = gActiveMap->ringIndex + 3;
      if (j > 7)
        j = gActiveMap->ringIndex - 5;
      if (gDesignRing[j].itemId != 0x19) {
        if (gActiveMap->spriteId == -1)
          sub_08007B54();
      } else {
        sub_08007B74();
      }
    }
  }

  j = gActiveMap->ringIndex;
  for (i = 7; i >= 0; i--) {
    q = &gDesignRing[j];
    j++;
    if (j > 7)
      j -= 8;
    if (q->flags & 1) {
      if (!(q->flags & 0x80))
        DesignRoomDrawUnitName(q->spriteSlot + 1, (q->x >> 8) - 4, (q->y >> 8) + 0x21,
                     q->itemId, q->flags & 0x20, q->flags & 8);
      if (!(q->flags & 0x40))
        DesignRoomDrawUnitIcon(q->spriteSlot + 1, q->x >> 8, q->y >> 8, q->itemId,
                     q->flags & 0x10, q->flags & 0x100, q->flags & 8);
      q->flags &= ~8;
    }
  }
}
asm(".global sub_08002C38\n.thumb_set sub_08002C38, DesignRoomDrawUnitRing\n");

void sub_08002D7C(void) {
  sub_0801F114();
  InitTilePool(1, (void *)0x06010000, 0x31C, 0x14);
  LoadTilePoolGraphic(0x3E);
  LoadTilePoolGraphic(0x3F);
  LoadTilePoolGraphic(0x40);
  LoadTilePoolGraphic(0x41);
  InitTilePool(2, (void *)0x06010000, 0x32C, 0x1D);
  LoadTilePoolGraphic(0x54);
  LoadTilePoolGraphic(0x90);
  LoadTilePoolGraphic(0x91);
  LoadTilePoolGraphic(0x8E);
  LoadTilePoolGraphic(0x8F);
  LoadTilePoolGraphic(0xAA);
  sub_08002EF8();
}

void DesignRoomLoadTerrainNamePalettes(void) {
  ApplyPaletteExt((u16 *)GetTerrainNamePalette(3, 0), 0x2E0, 0x20);
  ApplyPaletteExt((u16 *)GetTerrainNamePalette(0xF, 0), 0x2C0, 0x20);
  ApplyPaletteExt((u16 *)GetTerrainNamePalette(1, 0), 0x3C0, 0x20);
  ApplyPaletteExt((u16 *)GetTerrainNamePalette(5, 0), 0x3E0, 0x20);
}
asm(".global sub_08002DEC\n.thumb_set sub_08002DEC, DesignRoomLoadTerrainNamePalettes\n");

void sub_08002E3C(void) {
  RegisterDataMove(gUnknown_0808D8AC, (void *)0x06014D40, 0x460);
  sub_08002EF8();
}

void sub_08002E5C(void) {
  RegisterDataMove(gUnknown_0808D8AC, (void *)0x06014D40, 0x460);
  sub_08002EF8();
  RegisterDataMove(gUnknown_0808DD0C, (void *)0x06016180, 0x200);
  RegisterDataMove(gUnknown_0808DF0C, (void *)0x06016140, 0x20);
  RegisterDataMove(gUnknown_0808DF2C, (void *)0x06016160, 0x20);
}

void DesignRoomLoadGraphics(void) {
  DesignRoomLoadTerrainNamePalettes();
  sub_08002D7C();
  sub_08002E5C();
}
asm(".global sub_08002EB4\n.thumb_set sub_08002EB4, DesignRoomLoadGraphics\n");

void DesignRoomDrawUi(void) {
  DesignRoomUpdateSelectionPanel();

  if ((gActiveMap->flags & 0x20) == 0) {
    if (gActiveMap->editMode == 0)
      DesignRoomDrawTerrainRing();
    else
      DesignRoomDrawUnitRing();
  }
}
asm(".global sub_08002EC8\n.thumb_set sub_08002EC8, DesignRoomDrawUi\n");

void sub_08002EF8(void) {
  int v;

  if (gActiveMap->editMode == 0)
    v = 0xAB;
  else
    v = 0xAA;

  ReloadTilePoolGraphic(0xAA, v);
}

void DesignRoomDrawPropertyCounts(void) {
  u16 buf[0x14];
  int i, j, k, x, y;
  u16 t;

  sub_0808B6E8(buf, gUnknown_0808D728, 0x28);
  y = 0x13;
  k = 0;
  for (j = 0; j <= 4; j++) {
    x = 0x4C;
    for (i = 0; i <= 3; i++) {
      t = buf[k];
      DrawSpriteNumberFont2((x + 0xA) & 0x1FF, (y + 0x10) | 0x400, CountPropertiesOfType(t));
      PutOamHi((x + 2) & 0x1FF, y | 0x400, gUnknown_08485CC8[i + 1],
                   sub_08001D04(t) << 12);
      x += 0x14;
      k++;
    }
    y += 0x19;
  }
}
asm(".global sub_08002F1C\n.thumb_set sub_08002F1C, DesignRoomDrawPropertyCounts\n");

void sub_08002FE4(void) {
  u8 buf[4];
  int i;

  sub_0808B6E8(buf, gUnknown_0808D750, 4);
  DrawWindowBackgroundOnBg2(9, 2, 0xB, 0x11);
  BG_EnableSync(2);
  for (i = 0; i <= 3; i++) {
    LoadTerrainObjTiles(buf[i] & 0x1F, 0,
                 (void *)(0x06010000 + (gUnknown_08485C9C[i + 1] << 5)), 1);
  }
}

void DesignRoomResetArmyPanels(void)
{
    gActiveMap->armyPanelState[0] = 0;
    gActiveMap->armyPanelState[1] = 0;
    gActiveMap->armyPanelState[2] = 0;
    gActiveMap->armyPanelState[3] = 0;
}
asm(".global sub_08003040\n.thumb_set sub_08003040, DesignRoomResetArmyPanels\n");

void sub_08003064(void)
{
    gActiveMap->armyPanelState[0] = 0x32;
    gActiveMap->armyPanelState[1] = 0x32;
    gActiveMap->armyPanelState[2] = 0x32;
    gActiveMap->armyPanelState[3] = 0x32;
}

void DesignRoomUpdateArmyPanels(int a, int b)
{
    DesignRoomUpdateArmyPanel(0, a, b);
    DesignRoomUpdateArmyPanel(1, a, b);
    DesignRoomUpdateArmyPanel(2, a, b);
    DesignRoomUpdateArmyPanel(3, a, b);
}
asm(".global sub_08003088\n.thumb_set sub_08003088, DesignRoomUpdateArmyPanels\n");
