#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801ABF8.
 * sub_0801ABF8 @ 0x0801ABF8
 */

/* Tears down every gUnknown_0200CC38 slot tagged with `id`, then re-runs the
 * link scan. gUnknown_0200CC24 is the installed callback (InitSaveSystem parks
 * an `int` there, so the call needs a cast rather than a retyped global) and
 * gUnknown_0200CC2C is its argument; naming the latter honestly gives the two
 * `ldr r0,[r0]` -- the outer one is agbcc's own -fforce-addr word, the ROM
 * word at 0x0808EF58.
 *
 * `return 1` sits at the BOTTOM with the body nested in the `if`, not as an
 * early `if (id == 0) return 1;`. That is not cosmetic: the early form emits
 * the `movs r0,#1` inline after the guard, the nested form emits it past the
 * literal pools as the ROM does. */
int EraseSaveSlot(u8 id)
{
    int i;
    int r;

    if (id != 0)
    {
        r = ((int (*)(int))gUnknown_0200CC24)(gUnknown_0200CC2C);

        for (i = 0; i < 0x10; i++)
        {
            if (gUnknown_0200CC38.unk00[i] == id)
            {
                gUnknown_0200CC38.unk10[i] |= 0xff;
                gUnknown_0200CC38.unk00[i] |= 0xff;
            }
        }

        return sub_0801A7D8(0, (void *)gUnknown_0200CC2C, r);
    }

    return 1;
}
asm(".global sub_0801ABF8\n.thumb_set sub_0801ABF8, EraseSaveSlot\n");
