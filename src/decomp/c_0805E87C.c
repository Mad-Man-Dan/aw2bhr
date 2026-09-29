#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805E87C.
 * sub_0805E87C @ 0x0805E87C
 */

/* Wave 52, W52-D.  MATCHED (352/352, relocs resolve to the same addresses).
 *
 * gUnknown_0816DA80 / _0816DA84 are NOT globals -- the ROM words there hold
 * 0x03003F20 and 0x030040D8, so they are agbcc's own -fforce-addr constants for
 * gUnknown_03003F20 and gUnknown_030040D8.  Only the FIRST reference to each
 * gets the force-addr word; the later ones in this function take ordinary
 * inline pool words, and the honest spelling reproduces that split by itself.
 * The promotion must carry the two rodata words:
 *   "rodata": ["0x0816DA80", "0x0816DA84"]
 *
 * `for (i = 0; i < 2; i++)` -- the ROM counts DOWN from 1 because
 * check_dbra_loop reversed it.  The tell is that `movs r1,#1; mov sb,r1` sits
 * AFTER the two LICM hoists in the preheader: a source `for` init is written
 * before them, a strength_reduce biv init after them.
 *
 * THE ONE THING THAT COST ANYTHING: the ROM's `adds r1,r0,#0` before the *12.
 * That copy is CSE substituting the map-cell load into a SECOND local, so the
 * cell expression is written TWICE -- once in the `if` and once as the unit
 * id -- and the id's assignment has to sit INSIDE the `u = gUnits +
 * ...` expression.  Bound as its own preceding statement (`id = cell; u = g +
 * id;`) the copy appears but lands BEFORE gUnits's pool `ldr`
 * instead of after it.  Wave 27 (W27-A) predicted the copy correctly; what is
 * new is that its POSITION is set by which statement the assignment belongs to.
 *
 * Fixing that also fixed the indirect call, which is a second instance of the
 * brief's "a change in a loop can flip an allocation in the tail": with the
 * copy missing, gUnknown_030013EC's pool `ldr` sank BELOW gUnknown_030040D8's
 * and the two pool words swapped.  Binding the function pointer to a local
 * (`f = gUnknown_030013EC; f(...)`) also fixes the pool order but then emits
 * the pointer's LOAD before the arguments, where the ROM sinks it -- so the
 * bound form is NOT the answer here even though it looks closer.
 * `gUnknown_030013EC` is a five-argument indirect call through _call_via_r4;
 * r4 is callee-saved, so the register index is not an arity readout (W46-D).
 *
 * The +0x09 bitfield views are file-local for the reason c_0805DA84.c records:
 * `unk09 &= ~7` on a plain u8 member shortens the mask to 0xf8, while the
 * bitfield gives the ROM's SImode `movs r3,#0x39; rsbs r3,r3,#0`.  The second
 * mask is derived from the first (`adds r3,#0x31` -> -8), which is CSE on the
 * two constants and confirms both writes are bitfield stores on one container.
 * struct Unit's and struct Unk030040D8's own unk09 are left alone so the
 * shared layouts stay shared.
 *
 * struct Unk5A514Cell is repeated here rather than declared in a shared header
 * because src/decomp/c_0805A268.c, c_0805A514.c and c_0805A744.c all define it
 * file-locally, and AiListSuppliersInReach/AiListSuppliersAndSupplyProperties's promoted definitions take it.
 * See the note at AiPickSupplyWard in include/unknown-functions.h. */

struct Unk5A514Cell
{
    /* 0x00 */ u8 x;
    /* 0x01 */ u8 y;
    /* 0x02 */ s16 v;
};
void AiListSuppliersInReach(struct Unk5A514Cell *);
void AiListSuppliersAndSupplyProperties(struct Unk5A514Cell *);
struct CellXY
{
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
};
struct Unk5E87CUnit
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 filler_01[0x08];
    /* 0x09 */ u8 unk09_0 : 6;
    /* 0x09 */ u8 unk09_6 : 2;
    /* 0x0a */ u8 filler_0a[0x02];
};
struct Unk5E87CCmd
{
    /* 0x00 */ u8 filler_00[0x09];
    /* 0x09 */ u8 unk09_0 : 3;
    /* 0x09 */ u8 unk09_3 : 3;
    /* 0x09 */ u8 unk09_6 : 2;
};

void AiSeekSupplier(void)
{
    int i;
    int id;
    struct CellXY pos;
    struct Unk03003338 *list;
    struct Unk5E87CUnit *u;

    list = gUnknown_03003F20;
    GenerateUnitMovementMap(gUnknown_030040D8);
    MapMarkHalo(0x79);
    AiListSuppliersInReach((struct Unk5A514Cell *)list);

    for (i = 0; i < 2; i++)
    {
        pos.x = 0x270F;
        AiPopLastNearestCandidate(list, (u16 *)&pos);

        if (pos.x == 0x270F)
        {
            gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                              gUnknown_030040D8->unk00, 0x78, 1);
            list = gUnknown_03003F20;
            AiListSuppliersAndSupplyProperties((struct Unk5A514Cell *)list);
        }
        else
        {
            if (gMap->unit[gMap->rowOffset[pos.y] + pos.x] != 0)
            {
                u = (struct Unk5E87CUnit *)(gUnits + (id = gMap->unit[gMap->rowOffset[pos.y] + pos.x]));
                if ((s8)gUnknown_03003340[pos.y][pos.x] <= GetUnitMovementBudget((struct Unit *)gUnknown_030040D8))
                {
                    if (u->unk00 == 0x16)
                    {
                        if ((u8)(gUnknown_030040D8->unk00 - 0x13) <= 1)
                        {
                            u->unk09_6++;
                            ((struct Unk5E87CCmd *)gUnknown_030040D8)->unk09_3 = 0;
                            ((struct Unk5E87CCmd *)gUnknown_030040D8)->unk09_0 = 0;
                            AiPublishAction(pos.x, pos.y, 7, 0, 0);
                        }
                    }
                }
            }
            AiAdvanceToward(&pos);
        }
    }
}
asm(".global sub_0805E87C\n.thumb_set sub_0805E87C, AiSeekSupplier\n");
