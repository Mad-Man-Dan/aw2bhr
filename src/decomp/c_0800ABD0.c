#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800ABD0.
 * sub_0800ABD0 @ 0x0800ABD0
 */

#define MAP0800ABD0 gMap

/* VERIFIED MATCH, 732/732 bytes under configured try_match.  Casting
 * gUnknown_08499590 to the map-header aggregate fixes the older pool-address
 * failure.  The final two instructions are obtained by keeping each y-band's
 * source coordinate in fixed r4 and its post-call copy in fixed r5; the
 * separate result statement leaves r0 live for the following comparison.
 * This is a last-mile allocator constraint, not evidence that the original
 * source used fixed-register syntax.  The accepted relocation names .rodata
 * instead of gUnknown_0808D858 and promotion must carry that pool word. */
void sub_0800ABD0(int x, int y)
{
    if (y > 0)
    {
        register int n asm("r4") = y - 1;
        register int bandY asm("r5");
        int result;

        result = sub_080094EC(x, n);
        bandY = n;
        if (result)
        {
            if (sub_0800A798(x, bandY) < 0
             || CountRiverNeighbours(x, bandY) == 0
             || sub_08009538(x, bandY) == 0)
            {
                SetTerrainAt(x, bandY, 7);
                MakeTileSimple(x, bandY, 0x2a);
                sub_0800A3D4(x, bandY);
            }
        }
        if (x > 0)
        {
            int m = x - 1;
            if (sub_080094EC(m, bandY))
            {
                if (sub_0800A798(m, bandY) <= 0
                 || CountRiverNeighbours(m, bandY) == 0
                 || sub_08009538(m, bandY) == 0)
                {
                    SetTerrainAt(m, bandY, 7);
                    MakeTileSimple(m, bandY, 0x2a);
                    sub_0800A3D4(m, bandY);
                }
            }
        }
        if (x < MAP0800ABD0->width - 1)
        {
            int m = x + 1;
            if (sub_080094EC(m, bandY))
            {
                if (sub_0800A798(m, bandY) <= 0
                 || CountRiverNeighbours(m, bandY) == 0
                 || sub_08009538(m, bandY) == 0)
                {
                    SetTerrainAt(m, bandY, 7);
                    MakeTileSimple(m, bandY, 0x2a);
                    sub_0800A3D4(m, bandY);
                }
            }
        }
    }
    if (y < MAP0800ABD0->height - 1)
    {
        register int n asm("r4") = y + 1;
        register int bandY asm("r5");
        int result;

        result = sub_080094EC(x, n);
        bandY = n;
        if (result)
        {
            if (sub_0800A798(x, bandY) < 0
             || CountRiverNeighbours(x, bandY) == 0
             || sub_08009538(x, bandY) == 0)
            {
                SetTerrainAt(x, bandY, 7);
                MakeTileSimple(x, bandY, 0x2a);
                sub_0800A3D4(x, bandY);
            }
        }
        if (x > 0)
        {
            int m = x - 1;
            if (sub_080094EC(m, bandY))
            {
                if (sub_0800A798(m, bandY) <= 0
                 || CountRiverNeighbours(m, bandY) == 0
                 || sub_08009538(m, bandY) == 0)
                {
                    SetTerrainAt(m, bandY, 7);
                    MakeTileSimple(m, bandY, 0x2a);
                    sub_0800A3D4(m, bandY);
                }
            }
        }
        if (x < MAP0800ABD0->width - 1)
        {
            int m = x + 1;
            if (sub_080094EC(m, bandY))
            {
                if (sub_0800A798(m, bandY) <= 0
                 || CountRiverNeighbours(m, bandY) == 0
                 || sub_08009538(m, bandY) == 0)
                {
                    SetTerrainAt(m, bandY, 7);
                    MakeTileSimple(m, bandY, 0x2a);
                    sub_0800A3D4(m, bandY);
                }
            }
        }
    }
    if (x > 0)
    {
        int m = x - 1;
        if (sub_080094EC(m, y))
        {
            if (sub_0800A798(m, y) < 0
             || CountRiverNeighbours(m, y) == 0
             || sub_08009538(m, y) == 0)
            {
                SetTerrainAt(m, y, 7);
                MakeTileSimple(m, y, 0x2a);
                sub_0800A3D4(m, y);
            }
        }
    }
    if (x < MAP0800ABD0->width - 1)
    {
        int m = x + 1;
        if (sub_080094EC(m, y))
        {
            if (sub_0800A798(m, y) < 0
             || CountRiverNeighbours(m, y) == 0
             || sub_08009538(m, y) == 0)
            {
                SetTerrainAt(m, y, 7);
                MakeTileSimple(m, y, 0x2a);
                sub_0800A3D4(m, y);
            }
        }
    }
}
