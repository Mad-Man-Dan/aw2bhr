#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800F77C.
 * sub_0800F77C @ 0x0800F77C
 */

#define MAP gMap

int sub_0800F77C(int x, int y, int dir)
{
    int i;
    int count;
    int r;
    int nx, ny;

    r = GetPipeConnectionAt(x, y, dir);
    count = 0;

    nx = x + gUnknown_0848895C[dir];
    if (nx < 0)
        return 2;
    if (nx >= MAP->width)
        return 2;
    ny = y + gUnknown_08488964[dir];
    if (ny < 0)
        return 2;
    if (ny >= MAP->height)
        return 2;

    if (MAP->tile[MAP->rowOffset[ny] + nx] == 0x162
     || MAP->tile[MAP->rowOffset[ny] + nx] == 0x163)
    {
        if (dir == 0 || dir == 1)
        {
            if (MAP->tile[MAP->rowOffset[ny] + nx] == 0x163)
            {
                if (GetPipeConnectionAt(nx, ny, 2) == 2)
                    return 2;
                if (GetPipeConnectionAt(x + gUnknown_0848895C[dir], y + gUnknown_08488964[dir], 3) == 2)
                    return 2;
            }
            return 0;
        }
        else if (dir == 2 || dir == 3)
        {
            if (MAP->tile[MAP->rowOffset[ny] + nx] == 0x162)
            {
                if (GetPipeConnectionAt(nx, ny, 0) == 2)
                    return 2;
                if (GetPipeConnectionAt(x + gUnknown_0848895C[dir], y + gUnknown_08488964[dir], 1) == 2)
                    return 2;
            }
            return 0;
        }
    }

    if (r != 2 && r != 3)
        return 2;

    for (i = 0; i < 4; i++)
    {
        if (gUnknown_0848895C[dir] + gUnknown_0848895C[i] != 0)
        {
            if (GetPipeConnectionAt(x + gUnknown_0848895C[dir], y + gUnknown_08488964[dir], i) == 2)
                count++;
        }
    }
    return count;
}
