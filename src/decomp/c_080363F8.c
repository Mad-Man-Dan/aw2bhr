#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080363F8.
 * sub_080363F8 @ 0x080363F8
 */

/* The eight bytes GetSlotSpriteAttrs returns are an OBJ attribute set (see
 * struct OamData); this view reads attr2's 10-bit tile number from the
 * second word. The ROM's `lsls r5,r1,#22` kept across the call, then
 * `lsrs #22` and `lsrs #17` from it, are three reads of this bitfield, not
 * a masked local. struct OamData's u16 container would load with ldrh; the
 * ROM loads the whole word. */
union Unk363F8Obj
{
    struct UnkVec vec;
    struct
    {
        u32 attr01;
        u32 tileNum : 10;
    } f;
};

/* Advances an object's tile within its 9-frame strip: the frame offset is
 * (current tile - base tile) mod 0x400, reduced mod 9. When it wraps to
 * 0, the 0x120-byte frame graphics are copied to OBJ VRAM at the base
 * tile. Operand order is load-bearing: attributes[2] is read inside the
 * expressions (after the bitfield), and the sum is `& 0xfc00` + remainder
 * + tile. */
void sub_080363F8(s16 index, u16 *attributes)
{
    union Unk363F8Obj obj;
    u16 distance;
    u16 remainder;

    obj.vec = GetSlotSpriteAttrs(index);
    distance = (attributes[2] - obj.f.tileNum) & 0x3ff;
    remainder = distance % 9;
    attributes[2] = (attributes[2] & 0xfc00) + remainder + obj.f.tileNum;
    if (remainder == 0)
    {
        u8 **source = (u8 **)(gUnknown_03001470[index].unk18 + 0x48);
        RegisterDataMove(*source + distance * 0x20,
                      (void *)(0x06010000 + obj.f.tileNum * 0x20), 0x120);
    }
}
