#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801E18C.
 * sub_0801E18C @ 0x0801E18C
 */

/* Builds the four OAM affine terms for gUnknown_0200F720[a] out of the
 * SinDegrees / CosDegrees sine and cosine pair and the record's own two
 * scale halfwords: pa = cos*4/sx, pb = -(sin*4)/sy, pc = sin*4/sx,
 * pd = cos*4/sy. The four `__divsi3` calls are the whole 160 bytes.
 *
 * `* 4` and not `<< 2`: the callees return s16, so combine folds the call-site
 * re-narrowing and the scale into one `lsls #0x10; asrs #0xe`, which a shift
 * would not reach.
 *
 * ONE `s16 *` over the record, not three `*(s16 *)&...unk0X` casts. The three
 * reads are `movs rI,#k; ldrsh r0,[r4,rI]` off a SINGLE base -- Thumb has no
 * immediate-offset `ldrsh`, so k is the offset register. Writing each member's
 * address separately makes three address pseudos instead of one, costs sl and
 * r7 and measured +24 bytes.
 *
 * They are read `ldrsh` and not `ldrh; lsls #0x10; asrs #0x10`, so unk00,
 * unk02 and unk04 are SIGNED objects. This is the first function to read any of
 * them -- struct Unk0200F720 types all three `u16` on store width alone and
 * says the signs are unproved -- but the struct is left alone and the sign
 * taken through the pointer, which emits nothing. */
void UpdateObjAffineRecord(int a)
{
    s16 *e = (s16 *)&gUnknown_0200F720[a];

    SetObjAffine(a,
                 CosDegrees(e[2]) * 4 / e[0],
                 -(SinDegrees(e[2]) * 4) / e[1],
                 SinDegrees(e[2]) * 4 / e[0],
                 CosDegrees(e[2]) * 4 / e[1]);
}
asm(".global sub_0801E18C\n.thumb_set sub_0801E18C, UpdateObjAffineRecord\n");
