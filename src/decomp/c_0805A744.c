#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805A744.
 * sub_0805A744 @ 0x0805A744
 */

/* Consumes the cell list AiListEmbarkBoundUnits (src/decomp/c_0805A514.c) produces:
 * seeds the map's 0x3C72 plane to 0x7F, redraws the 0x2852 plane, then for each
 * listed cell drives the gUnknown_030013EC hook and floods the 0x3C72 plane
 * with that cell's threat value wherever the terrain code is 0xd or 0xb and the
 * value beats what is already there.
 *
 * THE RECORD TYPE IS SETTLED BY A PRODUCER/CONSUMER PAIR, not by guesswork --
 * the wave brief's one real oracle for a byte-neutral type. AiListEmbarkBoundUnits writes
 * {u8 x; u8 y; s16 v;} records and terminates the array with `out->v = 0xFFFF`;
 * this function walks the same 4-byte stride and stops on `v == -1`. Same
 * struct, declared here rather than shared because it describes only a pointer
 * parameter.
 * TWO THINGS IN THE INNER BODY WERE MEASURED, NOT GUESSED:
 *  - `cell` is bound as a pointer, and that is what makes the two sign-extended
 *    reads `ldrsb rN,[cell,rZERO]` instead of AiMarkLandingCellsNearEnemyHq's `ldrb; lsl; asr`.
 *    THUMB ldrsb has only a register-offset form, so agbcc will materialise a
 *    zero register for it when the address is a live pointer local and will not
 *    when the address dies at the load. Reading it as evidence that the array
 *    is signed would be wrong: it is the SAME gUnknown_03003340, and the u8
 *    read that supplies the stored value sits right beside it.
 *  - `w` is REUSED for the terrain mask and then for the s8 compare. A fresh
 *    local there is size-exact with r0/r1 swapped through the final block.
 *
 * MATCHED, 1 attempt after 2 probes. */

struct Unk5A514Cell
{
    /* 0x00 */ u8 x;
    /* 0x01 */ u8 y;
    /* 0x02 */ s16 v;
};

void AiFillLanderCostToShoalPlane(struct Unk5A514Cell *p)
{
    int x;
    int y;
    u8 *cell;
    int v;
    int w;
    int off;

    FillMapPlane(gMap->unk3C72, 0x7f);
    SetWorkingMapPlane(gMap->move);

    while (p->v != -1)
    {
        gUnknown_030013EC(p->x, p->y, 0x17, 0x78, -1);

        for (y = 0; y < gMap->height; y++)
        {
            for (x = 0; x < gMap->width; x++)
            {
                cell = gUnknown_03003340[y] + x;
                v = *cell;
                if (*(s8 *)cell >= 0)
                {
                    off = gMap->rowOffset[y] + x;
                    w = gMap->terrain[off] & 0x1f;
                    if (w == 0xd || w == 0xb)
                    {
                        w = *(s8 *)cell;
                        if (w < gMap->unk3C72[off])
                            gMap->unk3C72[off] = v;
                    }
                }
            }
        }

        p++;
    }
}
asm(".global sub_0805A744\n.thumb_set sub_0805A744, AiFillLanderCostToShoalPlane\n");
