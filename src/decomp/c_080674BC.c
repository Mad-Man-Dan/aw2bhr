#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080674BC.
 * sub_080674BC @ 0x080674BC
 */

#include "proc.h"
struct Unk674BCProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ int unk58;
};

/* The sibling of PaletteFadeFromWhite_Init four instructions shorter: same 0x20-frame arm,
 * but the palette call takes a plain 1 so there is no `rsbs`. */
void PaletteFadeToWhite_Init(struct Unk674BCProc *proc)
{
    proc->unk58 = 0x20;
    ColFadeToWhite(1);
    EnablePaletteSync();
}
asm(".global sub_080674BC\n.thumb_set sub_080674BC, PaletteFadeToWhite_Init\n");
