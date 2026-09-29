#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080620FC.
 * sub_080620FC @ 0x080620FC
 */

#include "map.h"
/* MATCHED wave 90 (W90-C), from a +4 / 29.3% park. Two changes:
 *   - the map through the struct members (gMap->rowOffset[j] + i,
 *     ->terrain[off], ->dangerMask[off], &->property[...]). The p/rows/cells
 *     pointer locals were the surplus live value that spilled a1 * 0x20;
 *     without them the frame is the ROM's `sub sp, #0x20`.
 *   - the 0xC00-byte record written as a struct chain,
 *     `((struct Unk620FCBuf *)gUnknown_02029ED8)->blk[g].rec[a1][n].x`.
 *     get_inner_reference builds that offset innermost first, as
 *     (n * 8 + a1 * 0x3e0) + g * 0xc00, which is the ROM's order; the flat
 *     `&gUnknown_02029ED8[n * 8 + a1 * 0x3e0 + g * 0xc00]` cast computes the
 *     g term first. The pointer at +4 is the same chain's `.cell`, which is
 *     why the ROM's pool word is gUnknown_02029ED8 + 0x64 (= gUnknown_02029F3C).
 * The old draft also had rec.x at +0 where the ROM stores at +0x60.
 * Needs "rodata": ["0x0816DB30"] (the force-addr word for gMap). */

struct Unk620FCRec
{
    /* 0x00 */ u8 x;
    /* 0x01 */ u8 y;
    /* 0x02 */ u8 pad[2];
    /* 0x04 */ s8 *cell; /* into gMap->property[] */
};
struct Unk620FCBlk
{
    /* 0x000 */ u8 head[3][0x20];
    /* 0x060 */ struct Unk620FCRec rec[3][0x7c];
};
/* A struct around the array so that `->blk[g]` is a real ARRAY_REF: c-typeck
 * turns a subscript on `*(T (*)[])p` into pointer arithmetic, which would put
 * the g term first. The extent is not proven. */
struct Unk620FCBuf
{
    struct Unk620FCBlk blk[1];
};

void AiFillInterestList(int a1, int a2)
{
    int x;
    int y;
    int n;
    int k;
    int i;
    int j;
    int off;

    n = 0;
    k = 0;
    FillMapPlane(gMap->dangerMask, 0);
    SetWorkingMapPlane((u8 *)gMap->move);
    AiSeedInterestCandidates();

    while ((u8)AiPopNextInterestSeed(&x, &y)) {
        gUnknown_030013EC(x, y, a2, 0x78, 0);
        n = gUnknown_02029ED8[k + a1 * 0x20 + gUnknown_030033EC * 0xc00];
        for (j = 0; j < gMap->height; j++) {
            for (i = 0; i < gMap->width; i++) {
                if ((s8)gUnknown_03003340[j][i] >= 0) {
                    off = gMap->rowOffset[j] + i;
                    if (gUnknown_085767D5[gMap->terrain[off] & 0x1f] != 0) {
                        if (gMap->dangerMask[off] == 0) {
                            ((struct Unk620FCBuf *)gUnknown_02029ED8)->blk[gUnknown_030033EC].rec[a1][n].x = i;
                            ((struct Unk620FCBuf *)gUnknown_02029ED8)->blk[gUnknown_030033EC].rec[a1][n].y = j;
                            ((struct Unk620FCBuf *)gUnknown_02029ED8)->blk[gUnknown_030033EC].rec[a1][n].cell = &gMap->property[gMap->rowOffset[j] + i];
                            gMap->dangerMask[gMap->rowOffset[j] + i] = 0xff;
                            n++;
                            if (n > 0x7a)
                                goto done;
                        }
                    }
                }
            }
        }
    done:
        ((struct Unk620FCBuf *)gUnknown_02029ED8)->blk[gUnknown_030033EC].rec[a1][n].x = 0xfe;
        k++;
        if (k > 0x1e)
            break;
        gUnknown_02029ED8[k + a1 * 0x20 + gUnknown_030033EC * 0xc00] = n + 1;
    }

    ((struct Unk620FCBuf *)gUnknown_02029ED8)->blk[gUnknown_030033EC].rec[a1][n].x = 0xff;
    gUnknown_02029ED8[k + a1 * 0x20 + gUnknown_030033EC * 0xc00] = -1;
}
asm(".global sub_080620FC\n.thumb_set sub_080620FC, AiFillInterestList\n");
