/* Typed views of the save-system globals. include/unknown-globals.h declares
 * these symbols with the untyped widths their first users measured, so the
 * draft renames those declarations away and redeclares the same symbols with
 * their real types. On promotion this becomes gMap-style header declarations
 * plus aw2bhr.lds aliases. gUnknown_0200CC88 is already a struct in the header. */
#define gUnknown_0200CC24 gUnknown_0200CC24_untyped
#define gUnknown_0200CC2C gUnknown_0200CC2C_untyped
#include "global.h"
#undef gUnknown_0200CC24
#undef gUnknown_0200CC2C

/* One 4 KiB flash sector, assembled in the gUnknown_02002000 staging buffer
 * before each write attempt. The buffer must stay a cast view: declaring the
 * symbol itself as this struct costs +20 bytes (measured). */
struct SaveSector
{
    /* 0x000 */ u32 signature;
    /* 0x004 */ u8 marker;
    /* 0x005 */ u8 version;
    /* 0x006 */ u8 checksum;
    /* 0x007 */ u8 checksumInverse;
    /* 0x008 */ u32 generation;
    /* 0x00c */ u8 segmentTag;
    /* 0x00d */ u8 saveId;
    /* 0x00e */ u16 dataOffset;
    /* 0x010 */ u32 slotGenerations[16];
    /* 0x050 */ u16 dataLength;
    /* 0x052 */ u8 data[0xf9d];
    /* 0xfef */ u8 slotOwners[16];
    /* 0xfff */ u8 endMarker;
};
#define gSaveSector (*(struct SaveSector *)gUnknown_02002000)

/* How the save data is split across sectors. */
struct SaveSegments
{
    /* 0x00 */ int length[16];
    /* 0x40 */ int offset[16];
};

extern u8 *gUnknown_0200CC2C;           /* the save data being written */
extern int (*gUnknown_0200CC24)(u8 *);  /* returns the size of gSaveData */
#define gSaveData gUnknown_0200CC2C
#define gSaveDataSize gUnknown_0200CC24

/* Write a save to flash, using spare slots and retrying failed sectors. */
int sub_0801A7D8(u8 saveId, void *unused, int byteCount)
{
  struct SaveSegments segments;
  int *offsets;
  int zero;
  u8 freeSlots[16];
  u8 writtenSlots[16];
  int segmentCount;
  int lastByte;
  int *segmentOffsets;
  u8 id;
  u8 owner;
  u8 freeCount;
  u8 retryCount;
  int segment;
  int slot;
  int i;
  int j;
  struct SaveSector *new_var;
  int length;
  int total;
  u8 checksum;
  u8 updatedFlags;
  int preservedFlags;
  sub_0801B598(gUnknown_0200CC30, (void (**)(void)) gUnknown_0200CC34);
  id = saveId;
  slot = 0;
  freeCount = 0;
  for (i = 0; i < 16; i++)
  {
    owner = gUnknown_0200CC38.unk00[i];
    if ((owner == saveId) || (owner == 0))
    {
      gUnknown_0200CC38.unk10[i] = 0xff;
    }
    else
      if (gUnknown_0200CC38.unk00[i] == 0xff)
    {
      freeSlots[freeCount] = i;
      freeCount++;
    }
    writtenSlots[i] = 0;
  }

  for (i = 0; i < (freeCount - 1); i++)
  {
    for (j = i + 1; j < freeCount; j++)
    {
      if (gUnknown_0200CC88.slotGeneration[freeSlots[i]] < gUnknown_0200CC88.slotGeneration[freeSlots[j]])
      {
        u8 oldSlot = freeSlots[i];
        freeSlots[i] = freeSlots[j];
        do
        {
          freeSlots[j] = oldSlot;
        }
        while (0);
      }
    }

  }

  for (;;)
  {
    total = 0;
    segmentCount = 0;
    while (byteCount != 0)
    {
      length = 0xfad;
      if ((segmentCount == 0) && (id == 0))
      {
        length -= 0x10;
      }
      if (length > byteCount)
      {
        length = byteCount;
      }
      byteCount -= length;
      segments.length[segmentCount] = length;
      segments.offset[segmentCount] = total;
      total += length;
      segmentCount++;
    }

    retryCount = 0;
    for (segment = segmentCount - 1; segment >= 0;)
    {
      switch (retryCount)
      {
        case 4:
          gUnknown_0200CC38.unk20[slot] |= 2;
          retryCount = 0;

        case 0:
          if (freeCount == 0)
        {
          for (i = 0; i < 16; i++)
          {
            gUnknown_0200CC38.unk10[i] = gUnknown_0200CC38.unk00[i];
          }

          return 1;
        }
          freeCount--;
          slot = freeSlots[freeCount];
          break;

      }

      retryCount++;
      if (gUnknown_0200CC88.slotGeneration[slot] != (-1))
      {
        gUnknown_0200CC88.slotGeneration[slot]++;
      }
      segmentOffsets = segments.offset;
      gUnknown_0200CC38.unk10[slot] = id;
      offsets = segmentOffsets;
      for (i = 0; i <= 0xfff; i++)
      {
        gUnknown_02002000[i] |= 0xff;
      }

      (*((struct SaveSector *) gUnknown_02002000)).signature = 0x73726132;
      if (gUnknown_0200CC38.unk40[slot] == 0x55)
      {
        (*((struct SaveSector *) gUnknown_02002000)).marker = 0x55;
        gUnknown_02002000[0xfff] = 0xaa;
      }
      else
      {
        (*((struct SaveSector *) gUnknown_02002000)).marker = 0xaa;
        gUnknown_02002000[0xfff] = 0x55;
      }
      zero = 0;
      i = 0xf;
      (*((struct SaveSector *) gUnknown_02002000)).version = i;
      (*(new_var = (struct SaveSector *) gUnknown_02002000)).checksum = zero;
      (*((struct SaveSector *) gUnknown_02002000)).checksumInverse = 0xff;
      (*((struct SaveSector *) gUnknown_02002000)).generation = gUnknown_0200CD08;
      (*((struct SaveSector *) gUnknown_02002000)).segmentTag = ((segment * 16) + segmentCount) + (-1);
      (*((struct SaveSector *) gUnknown_02002000)).saveId = id;
      (*((struct SaveSector *) gUnknown_02002000)).dataOffset = offsets[segment];
      for (i = 0; i < 16; i++)
      {
        (*((struct SaveSector *) gUnknown_02002000)).slotGenerations[i] = gUnknown_0200CC88.slotGeneration[i];
      }

      (*((struct SaveSector *) gUnknown_02002000)).dataLength = segments.length[segment];
      for (i = 0; i < segments.length[segment]; i++)
      {
        (*((struct SaveSector *) gUnknown_02002000)).data[i] = gUnknown_0200CC2C[segments.offset[segment] + i];
      }

      if ((segment == 0) && (id == 0))
      {
        for (i = 0; i < 16; i++)
        {
          gUnknown_0200CC38.unk00[i] = gUnknown_0200CC38.unk10[i];
        }

        if (gUnknown_0200CD08 == 0)
        {
          for (i = 0; i < 16; i++)
          {
            total = writtenSlots[i];
            if ((total == 0) && (i != slot))
            {
              (*((struct SaveSector *) gUnknown_02002000)).slotOwners[i] |= 0xff;
            }
            else
            {
              do
              {
                (*((struct SaveSector *) gUnknown_02002000)).slotOwners[i] = gUnknown_0200CC38.unk00[i];
              }
              while (0);
            }
          }

        }
        else
        {
          for (i = 0; i < 16; i++)
          {
            (*((struct SaveSector *) gUnknown_02002000)).slotOwners[i] = gUnknown_0200CC38.unk00[i];
          }

        }
      }
      checksum = 0;
      for (i = 0; i <= 0xfff; i++)
      {
        checksum += gUnknown_02002000[i];
      }

      (*((struct SaveSector *) gUnknown_02002000)).checksum = checksum;
      (*((struct SaveSector *) gUnknown_02002000)).checksumInverse = ~checksum;
      preservedFlags = 0;
      sub_0801B618(slot, (int) (&(*((struct SaveSector *) gUnknown_02002000))));
      lastByte = 0xfff;
      if (sub_0801B648(slot, (int) (&(*((struct SaveSector *) gUnknown_02002000)))) == 0)
      {
        j = slot;
        updatedFlags = (gUnknown_0200CC38.unk20[j] | 8) | preservedFlags;
        gUnknown_0200CC38.unk20[slot] = updatedFlags & 0xfb;
        gUnknown_0200CC38.unk30[slot] = (*((struct SaveSector *) gUnknown_02002000)).segmentTag;
        gUnknown_0200CC38.unk40[slot] = gUnknown_02002000[0xfff];
        gUnknown_0200CC88.slotGeneration[slot] = (*((struct SaveSector *) gUnknown_02002000)).generation;
        retryCount = 0;
        writtenSlots[slot] = 1;
        segment--;
      }
      else
      {
        gUnknown_0200CC38.unk10[slot] = 0xff;
        gUnknown_0200CC38.unk40[slot] = gUnknown_02002000[lastByte];
        retryCount++;
      }
    }

    length = gUnknown_0200CD08;
    if (length <= ((u32) (-2)))
    {
      gUnknown_0200CD08++;
    }
    if (id == 0)
    {
      return 0;
    }
    id = 0;
    byteCount = gUnknown_0200CC24(gUnknown_0200CC2C);
  }

}
