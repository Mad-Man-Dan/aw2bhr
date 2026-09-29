#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E160.
 * sub_0806E160 @ 0x0806E160, sub_0806E17C @ 0x0806E17C
 */

/* Family F054, the gUnknown_08581F7C twin of StartMatchSetupScreen -- same shape, same
 * constants, different flag and script. See the note on StartMatchSetupScreen. */

void StartRulesScreen(void)
{
    gUnknown_0202F2C8 = 0;
    sub_080152EC(gUnknown_08581F7C, 2);
}
asm(".global sub_0806E160\n.thumb_set sub_0806E160, StartRulesScreen\n");

/* Family F054, the mode-1 half of the StartRulesScreen pair. See the note on
 * StartMatchSetupScreen. */

void StartRulesScreenViewOnly(void)
{
    gUnknown_0202F2C8 = 1;
    sub_080152EC(gUnknown_08581F7C, 2);
}
asm(".global sub_0806E17C\n.thumb_set sub_0806E17C, StartRulesScreenViewOnly\n");
