#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803927C.
 * sub_0803927C @ 0x0803927C, sub_0803929C @ 0x0803929C
 */

#include "proc.h"
struct Unk3927CProc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x2c);
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
};
struct Unk3929CProc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x2c);
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ STRUCT_PAD(0x34, 0x54);
    /* 0x54 */ int unk54;
};

/* The draw call this proc's three script steps all end with: position from
 * +0x2c / +0x30, a fixed 0x41CA tile/palette word, and two zeros. The
 * parameter keeps the declared ProcPtr from unknown-functions.h -- the
 * conversion to the record type is free. */

void CoPowerPortrait_Draw(ProcPtr procPtr)
{
    struct Unk3927CProc *proc = procPtr;

    sub_08043C28(proc->unk2c, proc->unk30, 0x41ca, 0, 0);
}
asm(".global sub_0803927C\n.thumb_set sub_0803927C, CoPowerPortrait_Draw\n");

/* `movs r1, #0xe5; lsls r1, r1, #1` and `movs r0, #0x98; lsls r0, r0, #1` are
 * just the constants 0x1CA and 0x130 -- Thumb immediates stop at 0xFF, so
 * every larger one is built this way. Neither is a shift in the source. */

void CoPowerPortrait_Init(ProcPtr procPtr)
{
    struct Unk3929CProc *proc = procPtr;

    LoadCoFullBodyAndPalette(proc->unk54, 0x1ca, 4);

    proc->unk2c = 0x130;
    proc->unk30 = 0xa0;
}
asm(".global sub_0803929C\n.thumb_set sub_0803929C, CoPowerPortrait_Init\n");
