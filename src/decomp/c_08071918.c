#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071918.
 * sub_08071918 @ 0x08071918
 */

#include "hardware.h"
/*
 * sub_08071918 -- debug hook: with L held, a fresh press of B calls StartDebugArmyEditor.
 *
 * gpKeySt->held is the held-key mask (0x200 is L) and gpKeySt->pressed the
 * newly-pressed mask (2 is B). The four parameters are never read; the callers
 * pass four arguments, so the prototype keeps them.
 *
 * The entry point is 8 bytes before the code: four no-op halfwords (left over
 * from the alignment of the veneer table in front of it) sit between the entry
 * label and the function's `push {lr}`, and callers branch to the label, so the
 * padding is executed as no-ops. The C compiler cannot emit bytes before a
 * function's first instruction, so:
 *   - the first asm() statement emits the four no-op halfwords;
 *   - the function itself is named for where its code really starts
 *     (sub_08071920);
 *   - the second asm() statement makes sub_08071918 an alias for that address
 *     minus 8.
 * Keep the two statements directly above the function: the padding has to land
 * immediately before the function's first instruction.
 */
asm(".text\n\t.code 16\n\t.align 2, 0\n\t.2byte 0, 0, 0, 0\n");
asm(".global sub_08071918\n.thumb_set sub_08071918, sub_08071920 - 8\n");

void sub_08071920(void *a, int b, int c, int d)
{
    if ((gpKeySt->held & 0x200) && (gpKeySt->pressed & 2))
        StartDebugArmyEditor();
}
