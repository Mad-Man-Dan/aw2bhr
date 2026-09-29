#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B8E0.
 * sub_0803B8E0 @ 0x0803B8E0, sub_0803B904 @ 0x0803B904
 */

/* gUnknown_03003F30 is already typed `s8 []`, and this is the tell: the byte is
 * read `ldrb` then re-signed with `lsls #24; asrs #24` before the zero test,
 * which is what a signed char element costs and what a u8 would not emit. */
void sub_0803B8E0(void)
{
    if (gUnknown_03003F30[3] != 0)
        sub_08017688(4);
    else
        sub_0803B8C4();
}

/* Same guard as sub_0803B8E0 one element down, but the argument is computed
 * rather than literal, and the three-shift run `lsls #0x18; asrs #8; lsrs #0x10`
 * is exactly the two conversions folded together: sub_08016D04 returns s8, so
 * its result is re-narrowed at the call site (`lsls #24; asrs #24`), and
 * sub_08017688's parameter is u16, so it is then zero-extended (`lsls #16;
 * lsrs #16`). agbcc collapses the adjacent `asrs #24; lsls #16` into `asrs #8`.
 * Nothing here is a mask plus a shift. */
void sub_0803B904(void)
{
    if (gUnknown_03003F30[2] != 0)
        sub_08017688(sub_08016D04(2));
    else
        StartWarRoom();
}
