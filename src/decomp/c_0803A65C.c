#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803A65C.
 * sub_0803A65C @ 0x0803A65C
 */

/* gUnknown_0849D89C is named five times and reloaded four -- see its note in
 * include/unknown-globals.h; it is a real ROM pointer word, so each statement
 * re-reads it and the two reads inside the DrawWindowBackgroundOnBg2 argument list share
 * one. */
void UnitClassInfo_Init(void)
{
    gUnknown_0849D89C->unk09 = 0;
    gUnknown_0849D89C->unk0a = 0xff;
    StepMapCursorAndDraw(2);
    sub_080152EC(gUnknown_0849E2C0, 0);
    DrawWindowBackgroundOnBg2(gUnknown_0849D89C->unk02, gUnknown_0849D89C->unk03, 0x10, 0xa);
    sub_08013AD4(2);
}
asm(".global sub_0803A65C\n.thumb_set sub_0803A65C, UnitClassInfo_Init\n");
