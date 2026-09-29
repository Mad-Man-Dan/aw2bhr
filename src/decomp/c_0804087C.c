#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804087C.
 * sub_0804087C @ 0x0804087C
 */

/* Installs the 0x0849FC64 script and saves the current cursor position.
 *
 * Two `ldrh`/`strh` pairs and not one word copy, so this is member-by-member
 * and not a struct assignment -- agbcc copies a 4-byte struct with a single
 * `ldr`/`str`. Both bases are bound once each. */
void SiloTargetSelect_Init(void)
{
    StartEventScript(gUnknown_0849FC64);

    gUnknown_030033E4.unk00 = gUnknown_03003100.pos.unk00;
    gUnknown_030033E4.unk02 = gUnknown_03003100.pos.unk02;
}
asm(".global sub_0804087C\n.thumb_set sub_0804087C, SiloTargetSelect_Init\n");
