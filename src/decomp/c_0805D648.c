#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805D648.
 * sub_0805D648 @ 0x0805D648
 */

/* Counts the map cells that hold a unit worth reacting to, uses the count to
 * rewrite the mode byte a3, then publishes the whole call into
 * gUnknown_030046C0 and longjmps out.
 *
 * gUnknown_0816DA44 and gUnknown_0816DA48 are NOT globals: the ROM words there
 * hold 0x030040D8 and 0x08499590, so they are agbcc's own -fforce-addr
 * constants for gUnknown_030040D8 and gUnknown_08499590. The honest spelling
 * reproduces both and the promotion carries the two rodata words.
 *
 * sub_08071910 is the linker's THUMB->ARM veneer for sub_08000554, which is
 * LONGJMP -- and it has to be declared NORETURN, because both call sites here
 * are followed immediately by a literal pool with no epilogue anywhere in the
 * function. See the note at its declaration in include/unknown-functions.h.
 *
 * Two prototypes were corrected against this function, both recorded there:
 * this one's parameters 3 and 5 are u8 rather than int (its own prologue
 * narrows all five), and GenerateBestMovementScript's first two are int rather than u16 (its
 * prologue narrows neither, and a u16 formal makes agbcc zero-extend on top of
 * the sign extension the following comparisons already need).
 *
 * The mode rewrite is a single `if (count && !flag) ... else if (flag) ...`
 * chain, NOT a nested if/else. Nested, agbcc presets 0xb before the flag test
 * and conditionally overwrites; the chain is what makes the count!=0 arm branch
 * straight into the shared 0xc store, which is the `bne` the ROM has.
 *
 * MATCHED. */

/* The file-local view of gUnknown_030046C0 that c_0805D5EC.c's header comment
 * describes: +0x04/+0x05, +0x08 and +0x12 are still filler in the shared
 * struct Unk030046C0 and nothing here discriminates the members it would have
 * to invent, so the view stays in this file rather than reshaping a struct
 * other files read. */
struct Unk0805D648Cmd
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 unk03;
    /* 0x04 */ u8 unk04;
    /* 0x05 */ u8 unk05;
    /* 0x06 */ u8 unk06;
    /* 0x07 */ u8 unk07;
    /* 0x08 */ u32 unk08;
    /* 0x0c */ u8 filler_0c[0x06];
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
};

void AiPublishAction(s16 a1, s16 a2, u8 a3, u8 a4, u8 a5)
{
    int x;
    int y;
    int count;
    struct Unit *e;
    struct Unk0805D648Cmd *d;

    count = 0;

    if (gUnknown_030040D8->unk00 == 0x18 && a3 == 2)
    {
        FillMovementMap(0xff);
        MapSetInRange(a1, a2, 9, 0);

        for (y = 0; y < gMap->height; y++)
        {
            for (x = 0; x < gMap->width; x++)
            {
                if ((s8)gUnknown_03003340[y][x] < 0)
                    continue;
                if (gMap->unit[gMap->rowOffset[y] + x] == 0)
                    continue;
                if (AreUnitsOnSameTeam(gUnknown_03003F38, gMap->unit[gMap->rowOffset[y] + x]))
                    continue;
                e = &gUnits[gMap->unit[gMap->rowOffset[y] + x]];
                if (gPlaySt.fog == 0)
                {
                    if ((u8)(e->type - 0xa) <= 1)
                        continue;
                    if (e->type == 0x11)
                        continue;
                    if (e->type == 0x15)
                        continue;
                    if (e->type == 0x16)
                        continue;
                    if (e->type == 0x18)
                        continue;
                }
                count++;
            }
        }

        if (count != 0 && (gUnknown_030040D8->unk01 & 0x20) == 0)
            a3 = 0xb;
        else if ((gUnknown_030040D8->unk01 & 0x20) != 0)
            a3 = 0xc;
    }

    GenerateUnitMovementMap(gUnknown_030040D8);
    GenerateBestMovementScript(a1, a2, gUnknown_03003110);
    PackPathNibbles(gUnknown_03003110, gUnknown_030046CC);

    if (gUnknown_030040D8->unk02 == a1 && gUnknown_030040D8->unk03 == a2 && a3 == 2)
        sub_08071910(gUnknown_03004680, 1);

    d = (struct Unk0805D648Cmd *)&gUnknown_030046C0;
    d->unk01 = gUnknown_03003F38;
    d->unk08 = gUnknown_03001FD4;
    d->unk04 = gUnknown_03003F24.pos.unk00;
    d->unk05 = gUnknown_03003F24.pos.unk02;
    d->unk02 = a1;
    d->unk03 = a2;
    d->unk00 = a3;
    d->unk06 = a4;
    d->unk07 = a5;
    d->unk12 = gUnknown_030040D8->unk06 - gUnknown_03004074;
    d->unk13 = 1;

    if (gPlaySt.savingEnabled != 0)
        AiSendActionCommand();

    sub_08071910(gUnknown_03004680, 1);
}
asm(".global sub_0805D648\n.thumb_set sub_0805D648, AiPublishAction\n");
