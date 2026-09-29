#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803550C.
 * sub_0803550C @ 0x0803550C
 */

/* WAVE 78 (W78-D): MATCHED configured, 44/44. Adding an explicit empty
 * `case 0` changes this from a two-node linear chain into the ROM's three-node
 * case tree. That supplies the missing signed `cmp #1; ble` low-bound test
 * while preserving both out-of-line call blocks and the literal-pool layout.
 *
 * HISTORICAL PARK, wave 28 (W28-A). 40 of 44 bytes; the remaining difference is ONE
 * instruction pair.
 *
 * The ROM dispatches through gcc's balanced case TREE:
 *     cmp r0, #1 ; beq  -> UpdateSnowParticles
 *     cmp r0, #1 ; ble  -> end          <- emit_case_nodes' low-bound test
 *     cmp r0, #2 ; beq  -> UpdateRainParticles
 *     b end
 * i.e. `emit_case_nodes` on a root node 1 with a right child 2 and
 * `node_has_low_bound` FALSE. Every spelling below emits the same code WITHOUT
 * that low-bound pair (the linear `do_jump_if_equal` chain), which is otherwise
 * instruction-for-instruction the ROM including the out-of-line case bodies and
 * the literal pool between them:
 *
 *   switch (gPlaySt.unk2c)   { case 1: ; case 2: ; }   <- this draft
 *   int v = ...;   switch (v)          { case 1: ; case 2: ; }
 *   s8  v = ...;   switch (v)          { ... }  (adds lsl/asr, still no test)
 *   s16 v = ...;   switch (v)          { ... }
 *   switch (...) { case 1: ; case 2: ; case 3: ; case 4: ; }  -> tree, but
 *       balanced onto ROOT 2 (`cmp #2; beq / cmp #2; bgt / cmp #1; bne`)
 *   switch (...) { case 1: ; case 2: ; case 0x80: }            -> same root-2 tree
 *   if (v == 1) A; else if (v > 1) { if (v == 2) B; }           -> `bne` shape,
 *       and the first call block goes inline instead of after the pool
 *
 * So the arm placement, the pool position and the case values are all settled;
 * what is not is which condition makes agbcc take the tree path for a TWO-case
 * switch. The `ble` (signed, against 1) rules out an unsigned index type, and a
 * plain `int` index does not do it either. Ruled out: the index type
 * (u8/s8/s16/int all probed), extra cases, an explicit `default`, and the
 * if/else-if rewrite. */
void UpdateWeatherParticles(void)
{
    switch (gPlaySt.weather)
    {
    case 0:
        break;
    case 1:
        UpdateSnowParticles();
        break;
    case 2:
        UpdateRainParticles();
        break;
    }
}
asm(".global sub_0803550C\n.thumb_set sub_0803550C, UpdateWeatherParticles\n");
