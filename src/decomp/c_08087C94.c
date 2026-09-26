#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08087C94.
 * sub_08087C94 @ 0x08087C94
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"
struct Unk87C94Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
};

/* Wave 53, W53-D. MATCHED.
 *
 * PROMOTION NEEDS A .rodata POOL WORD PLACED:
 *   "rodata": ["0x081D9450"]
 * then re-run tools/split_rodata.py and tools/gen_lds.py.
 *
 * `gUnknown_081D9450` is NOT a global: the ROM word at 0x081D9450 is
 * 0x03001FE8, agbcc's own -fforce-addr address constant for gUnknown_03001FE8,
 * which this function touches four times (size, chr_block, tm_block, chr_block
 * again). Naming the union directly is what reproduces `ldr rN,<pool>;
 * ldr rM,[rN]` followed by byte read-modify-writes through rM.
 *
 * `*(u16 *)&zero = 0;` rather than `zero = 0;` is load-bearing and worth two
 * bytes. A plain `zero = 0` leaves the store's MEM addressed by `sp`, which
 * reload fixes with a scratch; that scratch landed in r0, stayed live, and
 * made the `mov r0, sp` at the CpuSet call redundant, so agbcc deleted it.
 * Taking the address explicitly creates a real pseudo for the store's base
 * instead -- `mov r1, sp` FIRST, then the value -- the pseudo dies at the
 * `strh`, and the call materialises the frame address again exactly as the ROM
 * does. THE TELL IS THE ORDER OF THE TWO `mov`s: address-then-value means an
 * explicit `&` in the source, value-then-address means a reload temp. Neither
 * the local's declaration order nor spelling it as a 1-element array moves
 * this (both probed).
 *
 * The eight-iteration CpuFastSet loop is written ASCENDING even though the ROM
 * counts 7 down to 0 -- check_dbra_loop reverses it, and the two `i * K` terms
 * become the two givs.
 *
 * sub_0807898C's ProcPtr parameter: see work/sub_0808A6CC/sub_0808A6CC.c. Here
 * it is invisible, because r0 already holds proc at the call. */
void CoDesignC1_08087C95(struct Unk87C94Proc *proc)
{
    int i;
    u16 zero;

    sub_0807898C(proc);
    sub_08013B0C();

    gUnknown_0300251C.bits.tm_block = 0x1b;

    gUnknown_03001FE8.bits.size = 2;
    gUnknown_03001FE8.bits.chr_block = 1;

    gUnknown_030030A4.bits.win1_enable_bg0 = TRUE;
    gUnknown_030030A4.bits.win1_enable_bg1 = FALSE;
    gUnknown_030030A4.bits.win1_enable_bg2 = FALSE;
    gUnknown_030030A4.bits.win1_enable_bg3 = FALSE;
    gUnknown_030030A4.bits.win1_enable_obj = FALSE;

    gUnknown_030030DC.bits.win0_enable_bg0 = TRUE;
    gUnknown_030030DC.bits.win0_enable_bg1 = TRUE;
    gUnknown_030030DC.bits.win0_enable_bg2 = TRUE;
    gUnknown_030030DC.bits.win0_enable_bg3 = TRUE;
    gUnknown_030030DC.bits.win0_enable_obj = TRUE;

    gUnknown_03002B68 = 0;
    gUnknown_03002B30 = 0x90;
    gUnknown_030024E4 = 0xf0;
    gUnknown_030020B8 = 0xa0;

    SetWinEnable(0, 1, 0);

    sub_08013AEC();

    Decompress(gUnknown_0823A3D4,
        (void *)(gUnknown_0300251C.bits.chr_block * 0x4000 + 0x06000000));
    Decompress(gUnknown_08239FA4, gBG3TilemapBuffer);

    for (i = 0; i < 0x400; i++)
        gBG3TilemapBuffer[i] += 0x2000;

    ApplyPaletteExt(gUnknown_0823BE20, 0x40, 0x20);
    sub_08013B1C();

    gUnknown_03002F18 = 0xff28;

    *(u16 *)&zero = 0;
    CpuSet(&zero, (void *)(gUnknown_03001FE8.bits.tm_block * 0x800 + 0x06000000),
           0x01000800);
    Decompress(gUnknown_0823468C,
        (void *)(gUnknown_03001FE8.bits.chr_block * 0x4000 + 0x06000000));
    Decompress(gUnknown_0823456C, gBG1TilemapBuffer);
    sub_08013AFC();
    sub_0802D5CC(0, 0);

    Decompress(gUnknown_082346D0, gUnknown_0200FC50);

    for (i = 0; i < 8; i++)
        CpuFastSet(gUnknown_0200FC50 + i * 0x400, (void *)(0x06010000 + i * 0x100), 0x40);

    ApplyPaletteExt(gUnknown_08234AD0, 0x200, 0x20);

    proc->unk58 = 0;
    proc->unk5c = 0;

    sub_08043BA4(gUnknown_030058E0[DivRem(0, gUnknown_03005948[proc->unk58]) + proc->unk5c], 0x40, 1);
    sub_08043B14(gUnknown_030058E0[0], 0x2cc);
    sub_08043E3C(gUnknown_030058E0[DivRem(0, gUnknown_03005948[proc->unk58]) + proc->unk5c], (void *)0x06013000, 0x12);
    sub_08043E3C(gUnknown_030058E0[DivRem(1, gUnknown_03005948[proc->unk58]) + proc->unk5c], (void *)0x06013480, 0x13);
    sub_08043E3C(gUnknown_030058E0[DivRem(2, gUnknown_03005948[proc->unk58]) + proc->unk5c], (void *)0x06013900, 0x14);
    sub_08043E3C(gUnknown_030058E0[DivRem(0, gUnknown_03005948[proc->unk58]) + proc->unk5c], (void *)0x06014200, 0x16);
    sub_08043E3C(gUnknown_030058E0[DivRem(1, gUnknown_03005948[proc->unk58]) + proc->unk5c], (void *)0x06014680, 0x17);
    sub_08043E3C(gUnknown_030058E0[DivRem(2, gUnknown_03005948[proc->unk58]) + proc->unk5c], (void *)0x06014B00, 0x18);

    sub_0801F114();
    sub_0801F150(1, (void *)0x06010000, 0x2d8, 0x1b);
    sub_0801F150(2, (void *)0x06010000, 0x2ec, 0x1c);

    sub_0801F234(0x3e);
    sub_0801F234(0x3f);
    sub_0801F234(0x40);
    sub_0801F234(0x41);
    sub_0801F234(0x42);
    sub_0801F234(0x43);
    sub_0801F234(0x44);
    sub_0801F234(0x48);
    sub_0801F234(0x49);
    sub_0801F234(0x4f);
    sub_0801F234(0x83);
    sub_0801F234(0x84);
    sub_0801F234(0x85);
    sub_0801F234(0x86);
    sub_0801F234(0x87);
    sub_0801F234(0x88);
    sub_0801F234(0x89);
    sub_0801F234(0x8a);
    sub_0801F234(0x8b);

    Proc_Start(ProcScr_PutFace, proc);
}

asm(".global sub_08087C94\n.thumb_set sub_08087C94, CoDesignC1_08087C95\n");

extern void CoDesignC1_08088005(void);
extern void CoDesignC1_IDLE_08088041(void);

struct ProcCmd CONST_DATA ProcScr_CoDesignC1[] =
{
    PROC_1D(30),
    PROC_CALL(CoDesignC1_08087C95),
    PROC_1E(30),
    PROC_CALL(CoDesignC1_08088005),
    PROC_REPEAT(CoDesignC1_IDLE_08088041),
    PROC_END,
};

asm(".global gUnknown_08616DFC\n.set gUnknown_08616DFC, ProcScr_CoDesignC1\n");
