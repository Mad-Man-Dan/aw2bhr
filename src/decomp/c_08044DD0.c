#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044DD0.
 * CoPowerDamageHeal_StartAnimation @ 0x08044DD0
 */

#include "proc.h"
/* A proc callback: gUnknown_084A08EC -- the script StartCoPowerDamageHealScript hands to
 * Proc_StartBlocking -- holds 0x08044DD1 as a CALL target, and +0x2c is the
 * first of the five payload bytes that same starter writes into the new proc.
 * +0x2c is past PROC_HEADER's 0x29 bytes, so this is the proc's own state.
 * The `adds r0, #0x2c` ahead of the `ldrb` is just the THUMB displacement
 * limit (0-31 for ldrb), not an address being taken.
 * `pop {r0}; bx r0` -- void.
 */
struct Unk44DD0Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x2c);
    /* 0x2c */ u8 unk2c;
};

void CoPowerDamageHeal_StartAnimation(struct Unk44DD0Proc *proc)
{
    StartCoPowerAnimation(proc->unk2c);
}
asm(".global sub_08044DD0\n.thumb_set sub_08044DD0, CoPowerDamageHeal_StartAnimation\n");
