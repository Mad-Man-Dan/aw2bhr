#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08050B70.
 * sub_08050B70 @ 0x08050B70
 */

#include "hardware.h"

/* The same "rebuild this slot's OBJ attributes" skeleton as
 * src/decomp/c_08051DE0.c and AirBlastEffect_Init -- read that file's header first --
 * ending in an indirect DISPATCH instead of SetEffectScreenPosition. The tail is the whole
 * of what is new, and it needs three separate levers that are each already
 * documented but have not had to combine before:
 *
 *  - `int prio`, for the same reason c_08051DE0.c needs `int tile`. Assigning
 *    the gUnknown_0855239C element straight into the 2-bit priority bitfield
 *    lets combine fold the field mask into the LOAD and emit `ldrb`; the ROM
 *    keeps `ldrh` and masks afterwards, which only the int temp produces.
 *
 *  - `row`, for the reason src/decomp/c_08051F4C.c records: `gUnknown_085D6A48
 *    [w][2]` hoists the constant 4 onto the base (`adds rB, #4`) where the ROM
 *    wants it in the `ldrh` displacement, and binding the row to a `u16 *`
 *    fixes it.
 *
 *  - `fn`. With the call written directly, agbcc sinks the `ldr` of the
 *    function pointer past the second argument's load; binding it to a local
 *    puts it where the ROM has it, immediately after its own address
 *    arithmetic.
 *
 * The last two collide, and the comma is what resolves it. Bound as its own
 * statement, `row` makes gUnknown_085D6A48's pool word precede
 * gUnknown_085535B0's, because agbcc creates address pseudos in source order
 * and the ROM's order is outermost-first. Assigning `row` INSIDE the subscript
 * keeps the whole dispatch one expression -- so the table's address is still
 * created first -- while still giving the row a real local to fold the 4 into.
 * Same lever as the `(meta = gUnknown_03004580, ...)` in c_0804D290.c. This was
 * very likely a macro in the original.
 *
 * `oam.hFlip = gUnknown_0300453C ^ 1` needs no explicit `& 1`: the `ands` with
 * 1 is the 1-bit bitfield's own truncation, and the `movs #0x11; rsbs` after it
 * is ~0x10, the QImode clear mask for bit 4 of byte 3.
 *
 * gUnknown_0855239C stays FLAT and indexed `[c * 2 + gUnknown_0300450C]`; wave
 * 17 confirmed that spelling byte-exact against the `[][2]` alternative. */
void ThirdEffect_Init(void)
{
    struct OamData oam;
    int tile;
    int prio;
    u16 *row;
    void (*fn)(u16, u16);

    CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *)&oam);

    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;

    oam.hFlip = gUnknown_0300453C ^ 1;
    oam.paletteNum = 8;
    tile = gUnknown_020296B0[gUnknown_0300453C].unk00;
    oam.tileNum = tile;
    prio = gUnknown_0855239C[gUnknown_0300453C * 2 + gUnknown_0300450C];
    oam.priority = prio;

    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);

    fn = gUnknown_085535B0[(row = gUnknown_085D6A48[gUnknown_03004580[gUnknown_0300453C][1]],
                            row[2])];
    fn(gUnknown_0300453C, gUnknown_0300451C);
}
asm(".global sub_08050B70\n.thumb_set sub_08050B70, ThirdEffect_Init\n");
