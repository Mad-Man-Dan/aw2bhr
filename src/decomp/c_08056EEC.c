#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08056EEC.
 * sub_08056EEC @ 0x08056EEC
 */

/* DrawTileBlock5x5B @ 0x08056EEC */

/* Fills a six-halfword record on its own stack and hands the address to
 * DrawTileBlockFlippableB -- the same shape DrawTileBlock4x4A hands to DrawTileBlockA, and the
 * same two ROM tables indexed the same two ways.
 *
 * The second argument is never read; arity 3 comes from the r2 narrowing, and
 * that narrowing is FUSED with the scaling (`lsls #0x10` at entry, `lsrs #0xf`
 * at the use), i.e. `gUnknown_08553864[c]` on a u16 c -- a net left shift of
 * one, not a mask.
 *
 * gUnknown_085D6A48's column 1 is the case DrawTileBlock4x4A is parked on, and the
 * COMMA-OPERATOR anchor closes it. The ROM wants both the `ldrh [r1, #2]`
 * displacement and the three pool words loaded outermost-first (08553878,
 * 085D6A48, 03004580, with 08553878 in the callee-saved r5 that pays for
 * `push {r4, r5, lr}`). `gUnknown_085D6A48[X][1]` gives the pool order and an
 * extra `adds r3, #2`; the struct cast written in place gives the displacement
 * and loads 085D6A48 LAST; binding the base in a statement of its own gives the
 * displacement but hoists 085D6A48 ahead of 08553878. Creating the pseudo
 * inside the subscript with a comma gives all three.
 */
struct Unk85D6A48Row /* 0x18 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u8 filler_08[0x12 - 0x08];
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u8 filler_14[0x18 - 0x14];
};
struct Unk56EEC
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u16 unk0a;
};

void DrawTileBlock5x5B(u16 a, u16 b, u16 c)
{
    struct Unk85D6A48Row *rows;
    struct Unk56EEC s;

    s.unk00 = gUnknown_08553874[a] + gUnknown_08553864[c];
    s.unk02 = 5;
    s.unk04 = 5;
    s.unk06 = gUnknown_08553878[
        (rows = (struct Unk85D6A48Row *)gUnknown_085D6A48,
         rows)[gUnknown_03004580[a][1]].unk02 * 2 + a];
    s.unk08 = gUnknown_08553888[a];
    s.unk0a = gUnknown_08553884[a];
    DrawTileBlockFlippableB(&s);
    gUnknown_03004534 = 1;
}
asm(".global sub_08056EEC\n.thumb_set sub_08056EEC, DrawTileBlock5x5B\n");
