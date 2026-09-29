#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804A124.
 * sub_0804A124 @ 0x0804A124
 */

/* gUnknown_0812A178 is NOT an object: the word at 0x0812A178 holds 0x02028E41,
 * i.e. it is agbcc's own -fforce-addr address constant for gUnknown_02028E41 --
 * the same four-byte signature block src/decomp/c_0804A17C.c clears. The
 * header's `u8 *gUnknown_0812A178` made every read THREE loads (pc literal ->
 * .rodata word -> pointer value -> byte) where the ROM has two, and that extra
 * indirection is exactly the "you need one more `*` than the ROM" tell for a
 * force-addr word. The honest spelling emits the pool word by itself. That
 * declaration has been retired from include/unknown-globals.h. */
void StartIntroOrLanguageSelect(void)
{
    gUnknown_02028E40 = gUnknown_0200C420.unk08 >> 6;

    if (gUnknown_02028E41[0] == 0xa5 && gUnknown_02028E41[1] == 0x5a
        && gUnknown_02028E41[2] == 0xc3 && gUnknown_02028E41[3] == 0x3c)
    {
        sub_0806A454();
    }
    else
    {
        gUnknown_02028E41[0] = 0;
        gUnknown_02028E41[1] = 0;
        StopAllMusic();
        sub_080152EC(gUnknown_084C383C, 0);
    }
}
asm(".global sub_0804A124\n.thumb_set sub_0804A124, StartIntroOrLanguageSelect\n");
