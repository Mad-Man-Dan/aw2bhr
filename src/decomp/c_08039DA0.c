#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039DA0.
 * sub_08039DA0 @ 0x08039DA0
 */

#include "proc.h"
/* The same three fields StartSparkleZoom writes when it starts this proc, read
 * back and re-issued with the y bumped by 0xC0. +0x29 and +0x2a each need a
 * runtime `adds` because `ldrb`'s displacement stops at 31; agbcc reuses the
 * first address with `adds r1, #1` for the second, exactly as
 * src/decomp/c_08039BB4.c records for the store side. */
struct Unk39DA0Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ u8 unk29;
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ STRUCT_PAD(0x2b, 0x2c);
    /* 0x2c */ u16 unk2c;
};

void SparkleEffect_StartZoom(struct Unk39DA0Proc *proc)
{
    StartSparkleZoom(proc->unk29, proc->unk2a, proc->unk2c + 0xc0);
}
asm(".global sub_08039DA0\n.thumb_set sub_08039DA0, SparkleEffect_StartZoom\n");
