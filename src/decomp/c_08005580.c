#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08005580.
 * sub_08005580 @ 0x08005580
 */

void sub_08005580(void)
{
    InitTextTileCache(0x70);
    PopMenu();
    PlayMusicOrSfx2(0x66);
}
