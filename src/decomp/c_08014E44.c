#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014E44.
 * sub_08014E44 @ 0x08014E44
 */

/* Allocate from the arena, or NULL when none is installed. The NULL block sits
 * PAST the body because the branch goes to it and the body is the fall-through
 * -- the ordinary `if (C) return A; <body>` layout, the same one
 * src/decomp/c_08014D7C.c documents. */
void *HeapMalloc(int size)
{
    if (gUnknown_03000050 == -1)
        return NULL;

    return sub_08014DCC(gUnknown_03000050, size);
}
asm(".global sub_08014E44\n.thumb_set sub_08014E44, HeapMalloc\n");
