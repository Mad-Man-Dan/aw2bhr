#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08077118.
 * sub_08077118 @ 0x08077118
 */

#include "proc.h"

/* A flat teardown: clear one flag byte, then four calls with no arguments. */
void EndWorldMapDisplayProcs(void)
{
    gUnknown_0202FDFC.unk10 = 0;
    AP_ClearAll();
    EndWorldMapMarkerDrawer();
    Proc_EndEach(gUnknown_086143E0);
    EndWorldMapSelectionFrame();
}
asm(".global sub_08077118\n.thumb_set sub_08077118, EndWorldMapDisplayProcs\n");
