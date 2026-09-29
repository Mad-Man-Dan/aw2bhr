#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D3D8.
 * sub_0803D3D8 @ 0x0803D3D8
 */

void ApplyMapRecord(int a, u8 *b)
{
    CopyMapRecordToGMap(a, b);
    PlaceMapRecordUnits(b);
    RebuildTerrainFromTiles();
}
asm(".global sub_0803D3D8\n.thumb_set sub_0803D3D8, ApplyMapRecord\n");
