#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E11C.
 * sub_0806E11C @ 0x0806E11C, sub_0806E14C @ 0x0806E14C
 */

/* A teardown that picks one of two sound/effect ids off the mode flag and then
 * runs the same three shutdown calls either way.
 *
 * Both arms JOIN, so the out-of-line block past the literal pool is the ELSE:
 * `beq` reaches the 0x71 arm, which makes 0x66 the `if` body and the condition
 * `!= 0`. That is the opposite of the reading a returning `if/else` would get --
 * see the arm-placement note in docs/agbcc-codegen.md. Both spellings are 48
 * bytes, so nothing but the branch sense separates them. */
void RulesScreen_Close(void)
{
    if (gUnknown_0202F2C8 != 0)
        PlayMusicOrSfx2(0x66);
    else
        PlayMusicOrSfx2(0x71);

    RulesScreenDismissRuleOptions();
    RulesScreenDismissArmyColumns();
    sub_0806D840();
}
asm(".global sub_0806E11C\n.thumb_set sub_0806E11C, RulesScreen_Close\n");

/* Two sequential calls and no nesting: sub_0801537C's argument is set up with a
 * fresh pool `ldr` into r0, which overwrites anything sub_080733B8 returned. */
void RulesScreen_Finish(void)
{
    sub_080733B8();
    sub_0801537C(gUnknown_08580CC4);
}
asm(".global sub_0806E14C\n.thumb_set sub_0806E14C, RulesScreen_Finish\n");
