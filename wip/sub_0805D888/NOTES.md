
## Wave 91 (W91-B)

Member form: the draft already reads the map through
`((struct Map *)gUnknown_08499590)->`. The lever has nothing to act on (the
gMap spelling was not separately needed). NEGATIVE by construction.

1. The park's "CSE merge" of the found block's (s16)x/(s16)y with the body
   call is really GCSE (PRE). Under a temporary `-O2 -fno-gcse` profile
   the barrier-free draft (w91-nogcse.c, the draft without the asm barrier)
   is SIZE-EXACT at 64.76%. No giv, mask in sl, frame 20, and every block
   matches except ONE 2-byte residual: the y-loop guard. The ROM has
   `ldr r3,[sp,#8]; cmp r3,r0`, which compares bestY's slot, known 0. The
   candidate has `cmp r6,r0` (y).
2. The guard's owner is CSE2 (the -da trace of insn 676 in rtl-n1/: bestY
   through .cse/.loop, y from .cse2 on). make_regs_eqv makes the NEW reg
   the class head when its REGNO_LAST_UID is later than the head's. y's last
   use is the found block's `(ashift y 16)` at the textual end, which is after
   bestY's last use in the `bestX != -1` call. cse1 forwards `fx = x; fy = y`
   copies at the goto site into the found block, so the copies do not help
   (n1, identical). The inline found call (b4) fixes the guard but
   brings the y<<16 giv back (+4).
3. Under the CONFIGURED flags, a found FLAG (`fnd = 1; break;` + `if (fnd)
   break;`, then post-loop `if (fnd) A; ...`) removes the giv and puts the
   loop body on the ROM's allocation (first diff +0xa -> +0x33). It is still
   +8, from the dead `movs r2,#0; cmp r2,#0; bne` flag tests and a block
   order. w91-flag.c. Worse than the barrier draft, which stays as the draft.
Open: a source layout where y's last use precedes bestY's while the found
block stays after the loop. Or settle -fno-gcse for this region (see
sub_0805D438's NOTES).

## wave 96

Base: unchanged draft (52.17% size+0, first +0xa; still has the empty asm barrier).
Lead's un-binding / barrier-deletion probe measured in a one-unit harness at configured:
- barrier deleted: 542 bytes (+36 incl. pad), `sub sp,#28` (ROM #20, draft #16) -- the barrier is not masking a correct layout; deleting it makes the frame wrong in the other direction.
- barrier deleted AND sx/sy un-bound (call with x,y directly): identical to the previous line (cse folds the copies).
- same, plus the found block calling with x,y directly: identical.
Under `-fno-gcse` (a diagnostic profile only) the barrier-free source is size-exact 508 at 64.76%, first diff +0x7a (the y-loop guard);
`for (y = bestY; ...)` is byte-identical to `for (y = 0; ...)` there (bestY is the constant 0 and cse folds the copy).
So the earlier wave-91 reading stands: the barrier is a stand-in for the gcse/PRE merge, and the last 2 bytes are which register class head cse2 picks for y.
Proposed summary: left: y-loop guard compares y's register where the ROM compares bestY's spill slot; frame is 16 vs 20 with the barrier. tried += barrier removal (frame 28), un-binding sx/sy, y seeded from bestY (all folded).
