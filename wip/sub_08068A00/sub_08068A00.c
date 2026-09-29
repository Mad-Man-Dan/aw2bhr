#include "global.h"
#include "proc.h"
#include "hardware.h"

/* PARKED at 25.5%, size 176 vs 196 (-20), first difference at +0x6, i.e. inside
 * the switch's comparison tree. Wave 36 (W36-K), re-measured wave 59 (W59-E).
 *
 * EVERY BODY IS RIGHT. Below the tree the diff is nothing but branch-target
 * offsets: all five arms and the tail assemble instruction-for-instruction,
 * including the gDispIo mask chain and both Decompress destinations.
 *
 * THE WHOLE REMAINING DIFF IS FOUR MISSING CASE PIVOTS.
 *
 * The ROM's tree tests nine values -- 0, 0x26, 0x4e, 0x58, 0x60, 0x62, 0x80,
 * 0xb4, 0xc7 -- and four of them (0x26, 0x4e, 0x58, 0x62) branch to the
 * after-switch label, which is also the default target. Written as empty
 * `break;` arms, agbcc emits only SIX pivots (0, 0x26, 0x60, 0x80, 0xb4, 0xc7)
 * and roots the tree at 0x60 instead of 0x4e -- that is the entire 20 bytes.
 *
 * ================= WAVE 59 (W59-E): TWO NEW MEASUREMENTS =================
 *
 * (1) THE CASE ORDER IN THIS FILE IS NOW CONFIRMED, NOT ASSUMED. W59-A derived
 * "case bodies are laid out in SOURCE order" on a JUMP TABLE this wave; this
 * function is the other dispatch kind (a binary tree of compares), and the rule
 * HOLDS here too. Two compile_probes of the same nine arms differing only in
 * label order leave the comparison tree byte-identical and move ONLY the body
 * blocks, to exactly the source order. The ROM's descending body order plus its
 * literal pool sitting immediately after the 0xc7 body (agbcc drops a pool only
 * after the FIRST body emitted) fix the original's order as DESCENDING.
 * Written up in docs/agbcc-codegen.md beside W59-A's section.
 *
 * (2) THE STRUCTURAL RESULT, AND IT SHOULD STOP FURTHER SPELLING ATTEMPTS.
 * agbcc's tree root is a deterministic function of the case-node COUNT alone.
 * Measured on this function, (nodes) -> (0-based index of the root node):
 *     2 -> 0 (chain)   3 -> 0   4 -> 1   6 -> 2   9 -> 4
 * The ROM emits NINE pivots but roots at index 2 (0x4e); nine live nodes root
 * at index 4 (0x60) every time. The ROM's right sub-tree (6 nodes -> 0x62) and
 * left (2 nodes -> 0x26 over 0) both obey that table exactly -- only the top
 * split does not. So NO flat nine-case switch compiled by this toolchain can
 * produce this tree, whatever the empty arms are spelled as. The open question
 * is what OTHER construct puts a case node's `beq default` at the root; it is
 * no longer "which spelling keeps the four dead nodes alive".
 *
 * RULED OUT, each by measurement -- do not re-test any of these:
 *   1. (w36) an explicit empty `default: break;`. Identical six-pivot tree.
 *   2. (w36) a statement live at RTL-expansion time and deleted later (a store
 *      to an unused int local) in each dead arm, aimed at group_case_nodes.
 *      Identical six-pivot tree, which rules out label-grouping as the cause.
 *   3. (w59) the four values grouped as consecutive labels on ONE `break;`.
 *      Byte-identical to four separate empty arms.
 *   4. (w59) arms that survive expansion and cross-jump onto the tail rather
 *      than being deleted (`case X: proc->unk2c--; return;`) -- wave 36's own
 *      stated next thing to try. It DOES keep all nine nodes and emits all nine
 *      pivots, but roots at 0x60, not 0x4e, and each arm costs a live `movs`.
 *   5. (w59) THE TOOLCHAIN. A throwaway data/compiler-overrides.json entry with
 *      cc1 old_agbcc and -fprologue-bugfix removed gives the SAME six-pivot
 *      tree at the same 25.5%, and additionally gets the gDispIo chain wrong
 *      (`movs r0,#1` before `ldrb r1,[r2,#1]`, where the ROM and default agbcc
 *      both load first). Entry reverted.
 *
 * WAVE 87 (W87-A). Unchanged. 0 try_match. See W87-notes.md.
 *   6. (w87) THE SHARED `default:` LABEL, all three spellings -- `case 0x4e:
 *      default: break;`, `default: case 0x4e: break;`, and all four dead
 *      values plus `default:` on one break. ALL THREE give the SAME six-pivot
 *      tree rooted at 0x60 as this draft. `default:` is not a case node, so it
 *      cannot keep a dead node alive. This axis is now closed five ways.
 *   NEW AND USEFUL (w87): the ROM's LEFT SUBTREE refutes "nine flat nodes" a
 *   second way, independently of W59-E's root-index table. The ROM emits
 *   `cmp 0x26/beq; cmp 0x26/bgt; cmp 0/beq` -- 0x26 rooted with 0 as its LEFT
 *   child. balance_case_nodes only splits a sublist when i > 2, so a two-node
 *   sublist stays a CHAIN and is emitted head-first, i.e. 0 would be tested
 *   first with a `blt`. So the sublist below 0x4e held THREE OR MORE case
 *   nodes. Both tells now say the same thing: the original switch had more
 *   case VALUES than the nine its tree tests. Look for a tenth value in
 *   (0, 0x26) or a negative one or a `case A ... B:` range -- not for another
 *   spelling of the four dead arms.
 *   Also checked: estimate_case_costs' cost-table split (which would move the
 *   root) is unreachable here -- it needs every case value in [-1,127] and
 *   0x80/0xb4/0xc7 are not.
 *
 * The four dead cases are left in below: they are the correct READING of the
 * ROM, just not a spelling that survives codegen. */
struct Unk68A00Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
};

void sub_08068A00(struct Unk68A00Proc *proc)
{
    switch (proc->unk2c)
    {
    case 0xc7:
        SetDispEnable(1, 1, 1, 0, 1);
        Decompress(gUnknown_0817DE24, (void *)0x06009400);
        break;
    case 0xb4:
        Decompress(gUnknown_0818E364, (void *)0x06010000);
        break;
    case 0x80:
        sub_080678D4(-1);
        break;
    case 0x62:
        proc->unk2c += 0; break;
    case 0x60:
        sub_080673D0(0x40, 1, proc);
        break;
    case 0x58:
        proc->unk2c += 0; break;
    case 0x4e:
        proc->unk2c += 0; break;
    case 0x26:
        proc->unk2c += 0; break;
    case 0:
        Proc_EndEach(gUnknown_08580FF4);
        Proc_EndEach(gUnknown_08580FCC);
        Proc_Break(proc);
        break;
    }

done:
    proc->unk2c--;
}
