#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BDF8.
 * sub_0803BDF8 @ 0x0803BDF8, sub_0803BE10 @ 0x0803BE10, sub_0803BE28 @ 0x0803BE28
 */

/* F083 -- the three members differ only in the byte they store.
 *
 * The double load (`ldr r0,=gUnknown_0849ECDC; ldr r1,[r0]; strb`) is the
 * global being a POINTER to the record, which is how
 * include/unknown-globals.h already models it, and unk00 is the limit half of
 * its cursor/limit pair -- sub_0803BF70 returns `unk01 == unk00`. So these
 * three are "arm the counter for 2 steps".
 *
 * CloseTopMenu's result is discarded. Nothing here settles its return type;
 * `int` is what unknown-functions.h already declares, and the argument for
 * that is written up there.
 */
void sub_0803BDF8(void)
{
    CloseTopMenu();
    gUnknown_0849ECDC->unk00 = 2;
}

/* F083 -- the three members differ only in the byte they store.
 *
 * The double load (`ldr r0,=gUnknown_0849ECDC; ldr r1,[r0]; strb`) is the
 * global being a POINTER to the record, which is how
 * include/unknown-globals.h already models it, and unk00 is the limit half of
 * its cursor/limit pair -- sub_0803BF70 returns `unk01 == unk00`. So these
 * three are "arm the counter for 3 steps".
 *
 * CloseTopMenu's result is discarded. Nothing here settles its return type;
 * `int` is what unknown-functions.h already declares, and the argument for
 * that is written up there.
 */
void sub_0803BE10(void)
{
    CloseTopMenu();
    gUnknown_0849ECDC->unk00 = 3;
}

/* F083 -- the three members differ only in the byte they store.
 *
 * The double load (`ldr r0,=gUnknown_0849ECDC; ldr r1,[r0]; strb`) is the
 * global being a POINTER to the record, which is how
 * include/unknown-globals.h already models it, and unk00 is the limit half of
 * its cursor/limit pair -- sub_0803BF70 returns `unk01 == unk00`. So these
 * three are "arm the counter for 4 steps".
 *
 * CloseTopMenu's result is discarded. Nothing here settles its return type;
 * `int` is what unknown-functions.h already declares, and the argument for
 * that is written up there.
 */
void sub_0803BE28(void)
{
    CloseTopMenu();
    gUnknown_0849ECDC->unk00 = 4;
}
