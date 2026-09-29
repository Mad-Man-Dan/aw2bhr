#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08062C7C.
 * sub_08062C7C @ 0x08062C7C
 */

/* The `lsls #0x18; lsrs #0x18` is THIS function's own u8 parameter narrowing --
 * agbcc's PROMOTE_MODE re-narrows at entry -- and the copy into r4 is what
 * keeps it alive across the first call. It says nothing about AiAccumulateInfluence's
 * parameter type, which stays unconstrained. */
void AiUpdateInfluence(u8 a)
{
    AiCalcPropertyControlPerBlock();
    AiAccumulateInfluence(a);
}
asm(".global sub_08062C7C\n.thumb_set sub_08062C7C, AiUpdateInfluence\n");
