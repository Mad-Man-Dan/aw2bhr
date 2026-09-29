#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072394.
 * sub_08072394 @ 0x08072394, sub_080723A8 @ 0x080723A8
 */

/* Resets the whole 0x10-entry palette-bias run and then clears the fade proc's
 * accumulator. 0x10 is the nominal fade rate this header's StartFadeCore note
 * describes. The constant-argument twin of ExcludePalettesFromFadeCore. */
void ExcludeObjPalettesFromFadeCore(void)
{
    ColFadeSet(0x10, 0x10, 0);
    RemoveFadeCoreCallBack();
}
asm(".global sub_08072394\n.thumb_set sub_08072394, ExcludeObjPalettesFromFadeCore\n");

/* The parameterised twin of ExcludeObjPalettesFromFadeCore. Both arguments narrow with
 * `lsls #16; lsrs #16` at the call, which is the argument conversion into
 * ColFadeSet's u16 parameters (its signature comes from the promoted
 * src/decomp/c_08013780.c) -- so this function's own parameters are `int`. Had
 * they been u16 themselves, PROMOTE_MODE would have delivered them already
 * zero-extended and the pairs would not be here. */
void ExcludePalettesFromFadeCore(int start, int count)
{
    ColFadeSet(start, count, 0);
    RemoveFadeCoreCallBack();
}
asm(".global sub_080723A8\n.thumb_set sub_080723A8, ExcludePalettesFromFadeCore\n");
