#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B7C0.
 * sub_0801B7C0 @ 0x0801B7C0, sub_0801B8A8 @ 0x0801B8A8, sub_0801B8D0 @ 0x0801B8D0
 */

#include "hardware.h"
/* The gUnknown_03002B80 text table: a 0x258-byte arena of NUL-separated
 * strings terminated by a 1 byte, plus a parallel record array at 0x258 and a
 * halfword VRAM tile cursor at 0x358. Same block, same layout as the struct in
 * src/decomp/c_0801B9C8.c, which reads unk00 and unk02; the byte at +0x03 that
 * it spells `filler_03` is this function's `kind` key. */
struct Unk1B7C0Ent
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 unk03;
};
struct Unk1B7C0Blk
{
    /* 0x000 */ u8 unk000[0x258];
    /* 0x258 */ struct Unk1B7C0Ent unk258[0x40];
    /* 0x358 */ u16 unk358;
};

int CacheTextString(const char *str, int kind)
{
    struct Unk1B7C0Blk *p = (struct Unk1B7C0Blk *)&gUnknown_03002B80;
    int cursor;
    int i;
    int n;

    if (*str == 0)
        return -1;

    cursor = 0;
    i = 0;
    while (p->unk000[cursor] != 1)
    {
        if (p->unk258[i].unk03 == kind
            && sub_0808B694(&p->unk000[cursor], str) == 0)
            return i;
        cursor += sub_0808B6B0((const char *)&p->unk000[cursor]);
        cursor += 1;
        i++;
    }

    sub_0808B678((char *)&p->unk000[cursor], str);
    n = RenderTextStringToTiles((const u8 *)str,
                     (int)((u8 *)(gUnknown_03002B6C.bits.chr_block * 0x4000)
                           + ((p->unk358 & 0x3FF) * 32 + 0x06000000)),
                     kind);
    p->unk258[i].unk00 = p->unk358;
    p->unk258[i].unk02 = n >> 1;
    p->unk258[i].unk03 = kind;
    p->unk358 = p->unk358 + n;
    n = sub_0808B6B0(str);
    p->unk000[cursor + n + 1] = 1;
    return i;
}
asm(".global sub_0801B7C0\n.thumb_set sub_0801B7C0, CacheTextString\n");

void PreloadTextStrings(const u8 *p, int b)
{
    while (*p != 1)
    {
        CacheTextString((const char *)p, b);
        p += sub_0808B6B0((const char *)p);
        p += 1;
    }
}
asm(".global sub_0801B8A8\n.thumb_set sub_0801B8A8, PreloadTextStrings\n");

/* Renders a NUL-terminated string one glyph at a time through the IWRAM
 * overlay entry DrawGlyphRam and returns its width in half-tiles.
 *
 * The three word cells are reached through agbcc -fforce-addr .rodata words at
 * 0x0808EF84/_88/_8C, which baserom.gba shows hold &0x03000054, &0x03000058 and
 * &0x0300005C -- see the note in include/unknown-globals.h. 0x03000054 has no
 * linkable symbol of its own (aw2bhr.lds jumps 0x50 -> 0x58), so it is spelled
 * as the second word of gUnknown_03000050, exactly as work/UpdateInterruptEnable spells
 * 0x03000060 off gUnknown_0300005C.
 *
 * `(u8)` on AdvanceTextPixelCursor's result is a cast at the call site and not a narrow
 * return type: the test is a bare `lsls #0x18` with no `lsrs`, and the promoted
 * definition in src/decomp/c_0801B964.c returns int. */
int RenderTextStringToTiles(const u8 *str, int x, int c)
{
    (&gUnknown_03000050)[1] = 0;
    gUnknown_03000058 = 0;
    gUnknown_0300005C = 0;

    while (*str != 0)
    {
        int w;

        if (gUnknown_0300005C != 0 && (u8)AdvanceTextPixelCursor(1))
            x += 0x40;

        w = DrawGlyphRam(*str++, x, gUnknown_03000058, c);
        (&gUnknown_03000050)[1] = w;
        if ((u8)AdvanceTextPixelCursor(w))
            x += 0x40;
    }

    return Div(gUnknown_0300005C + 7, 8) * 2;
}
asm(".global sub_0801B8D0\n.thumb_set sub_0801B8D0, RenderTextStringToTiles\n");
