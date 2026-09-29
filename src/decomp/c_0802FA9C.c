#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802FA9C.
 * sub_0802FA9C @ 0x0802FA9C
 */

/* Copy an 8-byte ROM table onto the stack and play the entry the argument
 * selects. The copy is real, not an artefact -- it is what sub sp, #8 pays
 * for, and the ROM indexes the STACK buffer, not the table.
 *
 * The ldrsh fixes the element as s16; the lsls #0x18; lsrs #0x18 prologue
 * fixes the parameter as u8. PlayMusicOrSfx2 takes int, so no narrowing happens
 * at the call. */
void SioPlayPingSound(u8 a)
{
    s16 tab[4];

    sub_0808B6E8(tab, gUnknown_08090CA0, 8);
    PlayMusicOrSfx2(tab[a]);
}
asm(".global sub_0802FA9C\n.thumb_set sub_0802FA9C, SioPlayPingSound\n");
