#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012A54.
 * sub_08012A54 @ 0x08012A54
 */

/* Installs a caller-supplied VBlank handler in slot 1, re-enables interrupt
 * source 2, and registers the HBlank arm sub_08012A24 into the
 * gUnknown_03002FA0 list. The `(void *)` cast on the function symbol is what
 * sub_08011AAC's `void *` parameter forces, exactly as
 * src/decomp/c_08012A74.c and c_080111AC.c spell the same registration.
 *
 * The handler parameter stays `void *`: SetIRQHandler's second argument is
 * opaque and this function does nothing with it but pass it on. */
void sub_08012A54(void *handler)
{
    SetIRQHandler(1, handler);
    sub_0801BB10(2, 2);
    sub_08011AAC((void *)sub_08012A24);
}
