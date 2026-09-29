#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018464.
 * sub_08018464 @ 0x08018464, sub_080184A4 @ 0x080184A4
 */

/* The `!= 1` is written the way the ROM branches: with only one arm and no
 * `else`, agbcc emits `beq` over the call, so the natural spelling is the
 * matching one here -- unlike sub_08017CF0, where BOTH arms return. */
bool8 sub_08018464(s16 a)
{
    if (gUnknown_03002514 != 1)
        sub_08018254(a);

    gUnknown_0200C528[a].unk04++;
    return FALSE;
}

/* gUnknown_03002F90 is VOLATILE and that is what this function measures: the
 * ROM reads it `ldrh` and then sign-extends and scales in one `lsls #0x10;
 * asrs #0xe`. A plain `u16` global written `(s16)g` does not produce that --
 * combine folds the load and the sign-extension into `ldrsh` and then needs a
 * separate `lsl #2`. See the note in include/unknown-globals.h. */
void sub_080184A4(void)
{
    sub_08012A54(gUnknown_0848A370[(s16)gUnknown_03002F90]);
    EnableSoundVSync();
}
