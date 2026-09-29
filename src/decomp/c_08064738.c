#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064738.
 * sub_08064738 @ 0x08064738, sub_08064774 @ 0x08064774
 */

/* A CONDITIONAL EXPRESSION, not an `if` on a pre-initialised local: the ROM
 * loads unk48 FIRST and only then materialises 0xc8, which is do_store_flag's
 * shape -- evaluate the condition, preset the false value, conditionally
 * overwrite. `int id = 0xC8; if (...) id = 0xCB;` puts the `movs #0xc8` ahead
 * of the `ldrb` and is different bytes.
 *
 * unk28 is read `ldrsh` here (the +8 happens before the mask) and `ldrh` in
 * RuleValue_DrawClimate (the mask comes first, so the sign cannot matter) -- same
 * declared s16 member either way. */
void RuleValue_DrawOnOff(struct Unk08580934_Obj *obj)
{
    DrawOamObject(obj->unk48 ? 0xCB : 0xC8, (obj->unk28 + 8) & 0x1FF,
                 (obj->unk2a + 0xC) & 0xFF, 0, 0);
}
asm(".global sub_08064738\n.thumb_set sub_08064738, RuleValue_DrawOnOff\n");

/* The four sprite ids are copied onto the stack before being indexed, which is
 * what an 8-byte ROM image plus an explicit sub_0808B6E8 (memcpy-shaped:
 * dst, src, size, returns dst) produces. gUnknown_0816E0C0 sits in the same
 * 0x0816Exxx .rodata run as gUnknown_0816E1B8, so this is very likely a local
 * array initialiser -- but the ROM's `bl` names the game's own copy routine
 * rather than a compiler helper, so the call is spelled out. */
void RuleValue_DrawClimate(struct Unk08580934_Obj *obj)
{
    u16 ids[4];

    sub_0808B6E8(ids, gUnknown_0816E0C0, 8);

    DrawOamObject(ids[obj->unk48], obj->unk28 & 0x1FF, (obj->unk2a + 0xC) & 0xFF, 0, 0);
}
asm(".global sub_08064774\n.thumb_set sub_08064774, RuleValue_DrawClimate\n");
