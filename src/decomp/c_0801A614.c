#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801A614.
 * sub_0801A614 @ 0x0801A614, sub_0801A664 @ 0x0801A664
 */

/* The gUnknown_03002F24 cursor is re-read before all but the FIRST of the four
 * byte stores, and that asymmetry is agbcc's aliasing rule rather than
 * anything in the source: a `strb` may alias a pointer global, so the cursor is
 * reloaded after each one; the leading word `str` to unk04 may not, so the
 * cursor loaded for it survives into the byte store that follows.
 *
 * `*(u32 *)&p->unk20` because Unk03001470 models +0x20 and +0x22 as two s16 and
 * the ROM copies the pair as one word -- the same word/halfword duality the
 * unk28 note records, spelled with a cast until a union exists. */
void PushMenu(void)
{
    struct Unk03001470 *p = FindSlotRunningScript(gUnknown_0848A42C);

    if (p != NULL)
    {
        gUnknown_03002F24->unk04 = *(u32 *)&p->unk20;
        gUnknown_03002F24->unk02 = p->unk42;
        gUnknown_03002F24->unk00 = p->unk48;
        gUnknown_03002F24->unk01 = p->unk4a;
        gUnknown_03002F24->unk03 = p->unk4c;
        gUnknown_03002F24++;
    }
}
asm(".global sub_0801A614\n.thumb_set sub_0801A614, PushMenu\n");

/* The pop half of PushMenu's push. Every member read goes through the
 * cursor AFTER the decrement, and agbcc addresses them off the pre-decrement
 * value with negative displacements because it knows what it just stored --
 * that is CSE, not five separate reads of the global.
 *
 * Each of the four `s8` members is widened twice: `lsls #24; asrs #24` reads
 * the signed byte, then `lsls #16; lsrs #16` converts it to CreateMenu's u16
 * parameter. Both pairs are needed; an s16 or u8 member drops one of them. */
void PopMenu(void)
{
    if (gUnknown_03002F24 != gUnknown_03002F50)
    {
        gUnknown_03002F24--;
        CreateMenu((const void *)gUnknown_03002F24->unk04, gUnknown_03002F24->unk00,
                     gUnknown_03002F24->unk01, gUnknown_03002F24->unk03, gUnknown_03002F24->unk02);
    }
}
asm(".global sub_0801A664\n.thumb_set sub_0801A664, PopMenu\n");
