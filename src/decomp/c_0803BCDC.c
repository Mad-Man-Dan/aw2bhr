#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BCDC.
 * sub_0803BCDC @ 0x0803BCDC
 */

/* MATCHED byte-for-byte (wave 37, W37-Q2).
 * NEEDS ITS POOL WORD PLACED: "rodata": ["0x0809112C"].
 *
 * gUnknown_0809112C is NOT a global. The word at 0x0809112C in baserom.gba is
 * 0x03003FC0, i.e. it is agbcc's own -fforce-addr address-constant pool entry
 * for &gPlaySt, and gen_lds.py invents a symbol name for it. The
 * honest spelling names the struct directly and the build places the word;
 * trymatch reports `relocs: name different symbols that resolve to the same
 * address`, which is the done signal.
 *
 * Copy the first `n` bytes of the caller's buffer into unk3d[1..], where n is
 * sub_0803BD14's count of occupied slots, then clear unk3d[0]. The `+ 1` bias
 * is why the giv init in the preheader is `adds r2,#0x3e` rather than 0x3d:
 * strength_reduce turned `&unk3d[i+1]` into a walking pointer, while the
 * source side stayed `adds r0,r4,r1; ldrb` because `i` is still live for the
 * exit test. Wave 32's W32-C note that sub_0803BF10's "cursor is already
 * biased and addresses unk3d[1..] as unk3e[0..]" is the same idiom.
 *
 * The `ldr r5,=<pool>` sits AHEAD of the guard `cmp r1,r3; bge` because the
 * address is read again after the loop; only the `adds r2,#0x3e` is preheader.
 */

void SetArmyCoIdsFromList(u8 *src)
{
    int n;
    int i;

    n = sub_0803BD14();

    for (i = 0; i < n; i++)
        gPlaySt.co[i + 1] = src[i];

    gPlaySt.co[0] = 0;
}
asm(".global sub_0803BCDC\n.thumb_set sub_0803BCDC, SetArmyCoIdsFromList\n");
