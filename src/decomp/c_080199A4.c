#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080199A4.
 * sub_080199A4 @ 0x080199A4, sub_080199B4 @ 0x080199B4
 */

/* Family F068: one call, two literal arguments, nothing else. `pop {r0}` is
 * the void epilogue. The argument types are not recoverable from here --
 * `movs rN,#K` is the same two bytes for every integer width -- so the
 * spelling follows DefeatOtherTeamsAndEndMatch's declaration in unknown-functions.h. */

void DeclareArmy1Winner(void)
{
    DefeatOtherTeamsAndEndMatch(1, 8);
}
asm(".global sub_080199A4\n.thumb_set sub_080199A4, DeclareArmy1Winner\n");

/* Family F068, second member -- same shape as DeclareArmy1Winner, first argument 2
 * instead of 1. */

void DeclareArmy2Winner(void)
{
    DefeatOtherTeamsAndEndMatch(2, 8);
}
asm(".global sub_080199B4\n.thumb_set sub_080199B4, DeclareArmy2Winner\n");
