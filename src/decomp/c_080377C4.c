#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080377C4.
 * sub_080377C4 @ 0x080377C4
 */

/* MATCHED, wave 43 (W43-I), third attempt.
 *
 * PROMOTION NEEDS A .rodata WORD PLACED: add "rodata": ["0x08090EF8"] to this
 * function's data/promoted.json entry. 0x08090EF8 holds 0x03003F68, i.e.
 * &gUnknown_03003F68 -- it is this unit's own -fforce-addr copy of that address,
 * not an object, and no declaration is needed (verified against baserom.gba).
 * That is the case unknown-globals.h already documents for 0x08090C28 /
 * 0x08090C2C, and this function is a clean example of the two levels coexisting
 * in ONE body: the head reaches gUnknown_03003F68 through the .rodata word
 * (three loads, because the address stays live across the two stores) while the
 * loop and the tail reach it through a plain inline `.4byte gUnknown_03003F68`
 * pool word (two loads). Both are the honest spelling; only the reference count
 * differs, exactly as the W41-C note predicts.
 *
 * Rebuilds the map's render buffer: copy the map's width/height into the buffer
 * header, then for every (x, y) copy the tile halfword at
 * `map->unk0a22[rowOffset[y] + x]` into the buffer at the SAME index, and
 * finally recentre the two scroll shadows on the new size.
 *
 * THREE READOUTS, EACH WORTH AN ATTEMPT.
 *
 * 1. The map's arrays must go through a LOCALLY CAST STRUCT, per the wave-34
 *    (W34-F) rule in unknown-globals.h -- which names this exact expression:
 *    `*(u16 *)(gUnknown_08499590 + 0x417a + y * 2)` reassociates to
 *    `(map + y * 2) + 0x417a`, the wrong order. `->unk417a[y]` keeps the ROM's
 *    `(map + 0x417a) + y * 2`.
 *
 * 2. The BUFFER is the opposite case and must NOT be a struct. The ROM computes
 *    `(buf + idx * 2)` and leaves the +2 in the store's displacement
 *    (`adds r2, r2, r1; strh r0, [r2, #2]`) -- the reassociated form that plain
 *    pointer arithmetic gives. A COMPONENT_REF would spend `adds rD, buf, #2`
 *    as its own instruction. So the two spellings are BOTH needed, in the same
 *    statement, for opposite reasons.
 *
 * 3. THE DESTINATION POINTER NEEDS ITS OWN STATEMENT (`dst = ...;` then
 *    `dst += ...;`), and the reason generalises: a pointer global appearing
 *    inside an ADDRESS expression is expanded under EXPAND_SUM, which does not
 *    load it -- the MEM rides in the address rtx and force_operand emits the
 *    `ldr` afterwards, i.e. after every insn the index expression emitted. The
 *    ROM loads the buffer pointer FIRST, so the original read it into a
 *    variable. Written as a single expression this function is SIZE-EXACT at
 *    89.9% with that two-instruction load sunk two slots and nothing else in
 *    228 bytes different. Read backwards: if your candidate's pointer load is
 *    late and everything around it is right, give the pointer a statement.
 *
 * The tail names gUnknown_03003F68 three times rather than binding it, and the
 * single `ldr` is reused across both `strh`s to gUnknown_03001FF8 /
 * gUnknown_03001418 even though those are `volatile`: a volatile store still
 * goes through true_dependence, and two different SYMBOL_REFs do not alias, so
 * cse keeps the pointer live. Binding it to a local instead loads it before the
 * store's own address and reverses that pool pair. */

void sub_080377C4(void *a1)
{
    u16 *dst;
    int x;
    int y;

    sub_08037B84(sub_08014E44(0xa14));

    ((u8 *)gUnknown_03003F68)[0] = gMap->width;
    ((u8 *)gUnknown_03003F68)[1] = gMap->height;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            dst = (u16 *)gUnknown_03003F68;
            dst += gMap->rowOffset[y] + x;
            dst[1] = gMap->tile[
                gMap->rowOffset[y] + x];
        }
    }

    sub_0801B6EC(a1);
    sub_0801B6FC(a1);

    gUnknown_03001FF8 = (((u8 *)gUnknown_03003F68)[0] * 4 - 0xf0) >> 1;
    gUnknown_03001418 = (((u8 *)gUnknown_03003F68)[1] * 4 - 0xa0) >> 1;
    sub_08014ED4(gUnknown_03003F68);
}
