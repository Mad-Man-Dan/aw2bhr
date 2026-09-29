#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014D7C.
 * sub_08014D7C @ 0x08014D7C
 */

/* Lays a four-word header over a caller-supplied buffer, 16-byte aligned, and
 * returns the aligned base or -1 when the buffer is too small.  `cmp #0x1f;
 * bls` is unsigned, so the size is `u32`; the -1 block sits past the body
 * because the branch goes to it and the body is the fall-through, which is the
 * ordinary `if (C) return A; <body>` layout.
 *
 * The header type describes a pointer, so it lives here rather than in
 * unknown-globals.h. */
struct Unk14D7CHeap
{
    /* 00 */ u32 unk00;
    /* 04 */ u32 unk04;
    /* 08 */ u32 unk08;
    /* 0c */ u32 unk0c;
};

int HeapInitBuffer(void *buf, u32 size)
{
    struct Unk14D7CHeap *h;

    if (size < 0x20)
        return -1;

    h = (struct Unk14D7CHeap *)(((u32)buf + 0xf) & ~0xf);
    size -= (u8 *)h - (u8 *)buf;
    h->unk00 = 0;
    h->unk04 = size - 0x10;
    h->unk08 = 0;
    h->unk0c = size;
    return (int)h;
}
asm(".global sub_08014D7C\n.thumb_set sub_08014D7C, HeapInitBuffer\n");
