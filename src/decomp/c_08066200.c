#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066200.
 * sub_08066200 @ 0x08066200, sub_08066210 @ 0x08066210
 */

/* Family F024: `push {lr}; ldr r1,=g; bl f; pop {r0}; bx r0`. r0 is never
 * written, so the incoming parameter passes through as argument ONE and the
 * pool word is argument TWO. Arity is invisible in a pass-through wrapper; the
 * reading comes from sub_08063A30's own body, which uses r0 as a base
 * (`str r1,[r0,#4]`, then zeroes +0x08 and +0x10) and r1 as the stored value.
 * The object in r0 gets no struct type: nothing here or in the callee
 * constrains anything but those three offsets, so `void *` is the weakest
 * model and keeps this file free of a struct it cannot name. */
void sub_08066200(void *a)
{
    sub_08063A30(a, gUnknown_08580B18);
}

/* Family F024: `push {lr}; ldr r1,=g; bl f; pop {r0}; bx r0`. r0 is never
 * written, so the incoming parameter passes through as argument ONE and the
 * pool word is argument TWO. Arity is invisible in a pass-through wrapper; the
 * reading comes from sub_08063A30's own body, which uses r0 as a base
 * (`str r1,[r0,#4]`, then zeroes +0x08 and +0x10) and r1 as the stored value.
 * The object in r0 gets no struct type: nothing here or in the callee
 * constrains anything but those three offsets, so `void *` is the weakest
 * model and keeps this file free of a struct it cannot name. */
void TeamBadge_StartExitDown(void *a)
{
    sub_08063A30(a, gUnknown_08580A68);
}
asm(".global sub_08066210\n.thumb_set sub_08066210, TeamBadge_StartExitDown\n");
