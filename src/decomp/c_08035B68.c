#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035B68.
 * GetUnitSpriteFormat @ 0x08035B68
 */

/* Returns `void *` and not the `u32` this was first promoted as: the only
 * caller is CreateMoveSlide, which hands the result straight to Decompress, and
 * unknown-functions.h documents the neighbouring GetMoveSlideGraphicsPointer the same way.
 * Both spellings are one word and compile identically -- re-verified
 * byte-for-byte after the change -- so this is a type-honesty fix, not a
 * codegen one. Reconciled at wave 13 promotion, where the `u32` definition and
 * a `void *` prototype added for the new caller collided at link time; a
 * per-function check cannot see that conflict because it compiles one unit. */
void *GetUnitSpriteFormat(u16 i)
{
    return (void *)gUnknown_0849CD88[i].spriteFormat;
}

asm(".global sub_08035B68\n.thumb_set sub_08035B68, GetUnitSpriteFormat\n");
