#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800CB30.
 * sub_0800CB30 @ 0x0800CB30
 */

/*
 * sub_0800CB30 -- remap the army that owns every property on the map, and put
 * it back again.
 *
 * Called in a pair around something that needs the map in its stored form:
 * (0, 0) applies the mapping and returns a token, and (1, <that token>) undoes
 * it. Each 0x0848_88xx table below holds both directions, and a1 picks the row.
 *
 * With a1 == 0, sub_0800C958 is asked about each of the four army slots and
 * `result` collects one bit per slot that answers (8, 4, 2, 1). That number
 * indexes each army's table, and gPlaySt.armyColor[1] to [4] are set from the
 * colours it names. With a1 != 0, `result` is the token passed in as a2 and the
 * four players simply get team colours 1 to 4 back.
 *
 * Every cell of the map is then visited. Bits 5 to 7 of the terrain byte say
 * which army owns a property there (0x20, 0x40, 0x60 and 0x80 for armies 1 to
 * 4); the new owner is that army's table entry at [result + a1 * 0x10] and is
 * written back into those bits. Property kinds 8, 6, 14, 10 and 11 also get a
 * new tile, from that kind's table indexed by (new owner - 2) / 2. `result` is
 * returned.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The four outer arms are the same block of code four times over and all
 *     end with the same store. Write them out: the original's single shared
 *     block is the compiler merging the identical tails, not the source sharing
 *     them.
 *   - The token is the local `result`, never the a2 parameter written back to.
 *     The original copies a2 elsewhere and re-reads a1 to test it, which only
 *     happens when the two are separate objects.
 *   - The owner byte is read a second time in each arm, after the terrain
 *     store. That reload is the reason the tables must not be `const`.
 */

#define MAP gMap
/* Each of the 0x0848_88xx tables is two rows of 0x10 bytes; the loop below
 * indexes it as [result + a1 * 0x10], so a1 chooses the row. Row 1 is also read
 * on its own for the army colours, and it has to be reached through this struct
 * rather than as `base[k + 0x10]` or `(base + 0x10)[k]`: the first shares the
 * added index between all four reads and the second folds the whole address
 * into one constant, and neither is what the original does. */
struct Unk88Rows
{
    /* 0x00 */ u8 row0[0x10];
    /* 0x10 */ u8 row1[0x10];
};
#define ROW1(sym) (((struct Unk88Rows *)(sym))->row1)

int sub_0800CB30(int a1, int a2)
{
    int x, y;
    int c;
    int v;
    int result;

    if (a1 == 0)
    {
        result = (sub_0800C958(0x28) >= 0) ? 8 : 0;
        if (sub_0800C958(0x48) >= 0)
            result |= 4;
        if (sub_0800C958(0x68) >= 0)
            result |= 2;
        if (sub_0800C958(0x88) >= 0)
            result |= 1;
        gPlaySt.armyColor[1] = gUnknown_08488948[(ROW1(gUnknown_084888A0)[result] >> 1) - 1];
        gPlaySt.armyColor[2] = gUnknown_08488948[(ROW1(gUnknown_084888C0)[result] >> 1) - 1];
        gPlaySt.armyColor[3] = gUnknown_08488948[(ROW1(gUnknown_084888E0)[result] >> 1) - 1];
        gPlaySt.armyColor[4] = gUnknown_08488948[(ROW1(gUnknown_08488900)[result] >> 1) - 1];
    }
    else
    {
        result = a2;
        gPlayers[1].teamColor = 1;
        gPlayers[2].teamColor = 2;
        gPlayers[3].teamColor = 3;
        gPlayers[4].teamColor = 4;
    }

    for (y = 0; y < MAP->height; y++)
    {
        for (x = 0; x < MAP->width; x++)
        {
            c = MAP->terrain[MAP->rowOffset[y] + x];
            switch (c & 0xE0)
            {
            case 0x20:
                c = (c & 0x1F) | (gUnknown_084888A0[result + a1 * 0x10] << 4);
                MAP->terrain[MAP->rowOffset[y] + x] = c;
                switch (c & 0x1F)
                {
                case 8:
                    v = gUnknown_08488920[(gUnknown_084888A0[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 6:
                    v = gUnknown_08488928[(gUnknown_084888A0[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 14:
                    v = gUnknown_08488930[(gUnknown_084888A0[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 10:
                    v = gUnknown_08488938[(gUnknown_084888A0[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 11:
                    v = gUnknown_08488940[(gUnknown_084888A0[result + a1 * 0x10] - 2) >> 1];
                    break;
                default:
                    v = -1;
                    break;
                }
                if (v >= 0)
                    MAP->tile[MAP->rowOffset[y] + x] = v;
                break;

            case 0x40:
                c = (c & 0x1F) | (gUnknown_084888C0[result + a1 * 0x10] << 4);
                MAP->terrain[MAP->rowOffset[y] + x] = c;
                switch (c & 0x1F)
                {
                case 8:
                    v = gUnknown_08488920[(gUnknown_084888C0[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 6:
                    v = gUnknown_08488928[(gUnknown_084888C0[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 14:
                    v = gUnknown_08488930[(gUnknown_084888C0[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 10:
                    v = gUnknown_08488938[(gUnknown_084888C0[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 11:
                    v = gUnknown_08488940[(gUnknown_084888C0[result + a1 * 0x10] - 2) >> 1];
                    break;
                default:
                    v = -1;
                    break;
                }
                if (v >= 0)
                    MAP->tile[MAP->rowOffset[y] + x] = v;
                break;

            case 0x60:
                c = (c & 0x1F) | (gUnknown_084888E0[result + a1 * 0x10] << 4);
                MAP->terrain[MAP->rowOffset[y] + x] = c;
                switch (c & 0x1F)
                {
                case 8:
                    v = gUnknown_08488920[(gUnknown_084888E0[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 6:
                    v = gUnknown_08488928[(gUnknown_084888E0[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 14:
                    v = gUnknown_08488930[(gUnknown_084888E0[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 10:
                    v = gUnknown_08488938[(gUnknown_084888E0[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 11:
                    v = gUnknown_08488940[(gUnknown_084888E0[result + a1 * 0x10] - 2) >> 1];
                    break;
                default:
                    v = -1;
                    break;
                }
                if (v >= 0)
                    MAP->tile[MAP->rowOffset[y] + x] = v;
                break;

            case 0x80:
                c = (c & 0x1F) | (gUnknown_08488900[result + a1 * 0x10] << 4);
                MAP->terrain[MAP->rowOffset[y] + x] = c;
                switch (c & 0x1F)
                {
                case 8:
                    v = gUnknown_08488920[(gUnknown_08488900[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 6:
                    v = gUnknown_08488928[(gUnknown_08488900[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 14:
                    v = gUnknown_08488930[(gUnknown_08488900[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 10:
                    v = gUnknown_08488938[(gUnknown_08488900[result + a1 * 0x10] - 2) >> 1];
                    break;
                case 11:
                    v = gUnknown_08488940[(gUnknown_08488900[result + a1 * 0x10] - 2) >> 1];
                    break;
                default:
                    v = -1;
                    break;
                }
                if (v >= 0)
                    MAP->tile[MAP->rowOffset[y] + x] = v;
                break;

            }
        }
    }

    return result;
}
