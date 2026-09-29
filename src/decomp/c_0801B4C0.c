#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B4C0.
 * sub_0801B4C0 @ 0x0801B4C0
 */

/*
 * FormatSaveSectors -- format the sixteen save sectors.
 *
 * Returns 1 without doing anything unless gUnknown_0200CD0C is 1. Otherwise
 * SetFlashTimerIntrIfPresent is handed gUnknown_0200CC30 and gUnknown_0200CC34 (the second as
 * a table of function pointers), and then for each of the sixteen sectors:
 *
 *   - the byte tables .unk00 and .unk10 are set to 0xff and .unk20 and .unk30
 *     to 0, and both generation counters are zeroed;
 *   - FillBytesWithFF fills the 0x1000-byte buffer at gUnknown_02002000;
 *   - the sector is written with ProgramFlashSectorIfPresent and read back with VerifyFlashSectorIfPresent,
 *     up to four times. Four failed attempts abandon the whole job and return
 *     1;
 *   - the buffer's last byte is kept in that sector's .unk40.
 *
 * Returns 0 once all sixteen sectors are done.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The two read-modify-writes are one statement, `unk00[i] = (unk10[i] |=
 *     0xff)`, with a plain `=` on the outer half. Written `unk00[i] |=
 *     unk10[i] |= 0xff` the compiler works out that both bytes end up 0xff and
 *     just stores the constant, losing the load-or-store the original has, and
 *     binding the outer address to a local first does the same. As two separate
 *     statements the two addresses are computed in the other order.
 *   - The `return 1` for a gUnknown_0200CD0C that is not 1 sits at the end of
 *     the function. As an early-return guard at the top it compiles to the same
 *     instructions with the blocks the other way round.
 */
int FormatSaveSectors(void)
{
  int i;
  int j;
  if (gUnknown_0200CD0C == 1)
  {
    SetFlashTimerIntrIfPresent(gUnknown_0200CC30, (void (**)(void)) gUnknown_0200CC34);
    for (i = 0; i < 0x10; i++)
    {
      gUnknown_0200CC38.unk00[i] = (gUnknown_0200CC38.unk10[i] |= 0xff);
      gUnknown_0200CC38.unk20[i] = 0;
      gUnknown_0200CC38.unk30[i] = 0;
      gUnknown_0200CC88.sectorGeneration[i] = gUnknown_0200CC88.slotGeneration[i] = 0;
      FillBytesWithFF(gUnknown_02002000, 0x1000);
      for (j = 0; j < 4; j++)
      {
        ProgramFlashSectorIfPresent(i, (int) gUnknown_02002000);
        if (VerifyFlashSectorIfPresent(i, (int) gUnknown_02002000) == 0)
        {
          break;
        }
      }

      if (j == 4)
      {
        return 1;
      }
      gUnknown_0200CC38.unk40[i] = gUnknown_02002000[0xfff];
    }

    return 0;
  }
  return 1;
}
asm(".global sub_0801B4C0\n.thumb_set sub_0801B4C0, FormatSaveSectors\n");
