#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037B90.
 * sub_08037B90 @ 0x08037B90, sub_08037C34 @ 0x08037C34
 */

/* Recounts the four terrain tallies in gUnknown_02027F74 by walking the map's
 * tile plane and classifying each cell through gUnknown_0849959C, the same
 * tile-id -> terrain byte table c_0801759C.c and c_08021750.c use.
 *
 * gUnknown_03003F68 is declared `void *`; the byte fields at +0/+1 are the
 * width and height, as c_08037CF8.c and c_08086D98.c already read them. The
 * u16 plane at +2 gets a file-local tag so the reference stays a COMPONENT_REF
 * and keeps the ROM's `ldrh [rN, #2]`, the spelling c_0801759C.c records.
 *
 * Two orderings the diff insisted on, and they agree with each other:
 *  - `base = 0` is the FIRST statement. The ROM materialises ONE zero and
 *    reuses it as the store value for all four counters; written after them,
 *    agbcc emits a separate constant.
 *  - The switch arms are emitted in SOURCE order, so unk01 comes first and
 *    unk03 last -- the same unk01/unk00/unk02/unk03 order as the four zero
 *    stores above. Written 6/0xa/0xb/0xe the four bodies come out reversed
 *    and the `case 0xe` test flips from `bne` to `beq`+`b`. */
/* Clears the whole 0x400-entry buffer gBG1TilemapBuffer points at, then lays a
 * fresh 2x2-metatile grid over it and kicks the consumer.
 *
 * The two clamped extents are `u8` LOCALS, not `int`: the ROM compares them
 * `bls` (unsigned) but shifts them `asrs` (signed). An `int` local gives
 * `ble`+`asrs` and a `u32` local gives `bls`+`lsrs`; only a `u8` -- promoted to
 * int for the shift, but known zero-extended at the compare -- gives the ROM's
 * pairing. Measured with all three side by side.
 *
 * The inner counter steps 16 with bound `((h + 1) >> 1) * 16`, and the stored
 * value uses `j` unscaled. Writing it as a 0..(h+1)/2 counter with `j * 16`
 * inside emits `lsl #5`/`lsl #4` per iteration instead.
 *
 * `j + 1 + i + 0x1000` must keep the 1 and the 0x1000 apart: agbcc's fold only
 * merges a constant with an ADJACENT one, so this spelling hoists 0x1000 into
 * a register (two instructions, no pool word) and leaves `adds #1` inline.
 * Spelled `... + i + 1 + 0x1000` the two fold to 0x1001 and need a pool word.
 *
 * gBG1TilemapBuffer is reloaded inside the loop because it is a POINTER
 * global, not an array -- do not bind it to a local. */
void sub_08037B90(void)
{
    u16 i;
    u16 j;
    u8 w;
    u8 h;

    for (i = 0; i <= 0x3ff; i++)
        gBG1TilemapBuffer[i] = 0;

    w = ((u8 *)gUnknown_03003F68)[0];
    h = ((u8 *)gUnknown_03003F68)[1];
    if (w > 0x20)
        w = 0x20;
    if (h > 0x20)
        h = 0x20;

    for (i = 0; i < (w + 1) >> 1; i++)
        for (j = 0; j < ((h + 1) >> 1) * 16; j += 16)
            gBG1TilemapBuffer[j * 2 + i] = j + 1 + i + 0x1000;

    sub_08013AFC();
}

void sub_08037C34(void)
{
    u16 base;
    u8 i;
    u8 j;

    base = 0;
    gUnknown_02027F74.unk01 = 0;
    gUnknown_02027F74.unk00 = 0;
    gUnknown_02027F74.unk02 = 0;
    gUnknown_02027F74.unk03 = 0;

    for (i = 0; i < gUnknown_03003F68->height; i++)
    {
        for (j = 0; j < gUnknown_03003F68->width; j++)
        {
            switch (gUnknown_0849959C[gUnknown_03003F68->tile[base + j]] & 0x1f)
            {
            case 0xe:
                gUnknown_02027F74.unk01++;
                break;
            case 6:
                gUnknown_02027F74.unk00++;
                break;
            case 0xa:
                gUnknown_02027F74.unk02++;
                break;
            case 0xb:
                gUnknown_02027F74.unk03++;
                break;
            }
        }
        base += gUnknown_03003F68->width;
    }
}
