#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804ADFC.
 * sub_0804ADFC @ 0x0804ADFC
 */

void NameEntry_StartSpriteScript(void)
{
    sub_080152EC(gUnknown_084C3D6C, 1);
}
asm(".global sub_0804ADFC\n.thumb_set sub_0804ADFC, NameEntry_StartSpriteScript\n");
