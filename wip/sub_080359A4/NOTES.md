# sub_080359A4 — Wave 65 final park

Configured profile: target 324 bytes; the preserved semantic draft is 16 bytes
short.  The residual is allocation/constant placement rather than missing game
logic.  The ROM keeps the proc in r6, `&unk44` in r4 (and copies it to r5 after
the optional camera call), `&unk42` in r8, and the private force-address word's
address in r9.  The draft carries only r8 and emits neither the same pointer
copies nor the same reload placement.

The final targeted `px = &proc->unk42` binding made the candidate 24 bytes short
and removed more of the ROM's pointer-copy punctuation, so it was reverted.
Changing the definition to a typed proc pointer conflicts with the existing
shared `ProcPtr` contract and did not supply an allocation mechanism.  The body
and local map/proc layouts remain the best semantic reconstruction.

# Wave 92 (W92-A)

    inherited draft                       320  -4  45.06%
    + gMap-> in place of the pool chase   320  -4  49.38%
    + the register pin removed            324  +0  54.63%   <- kept

## The pool word: the brief asked the right question and the answer is gMap

`_08035ACC: .4byte gUnknown_08090EA8` looks like a third level of indirection,
and the inherited draft reproduced it by hand with
`u8 **const *screen = &gUnknown_08090EA8; ... **screen`. It is not a third
level. 0x08090EA8 holds 0x08499590, which is the address of the map pointer, so
the chain is agbcc's ordinary -fforce-addr shape: the text pool word points at
a `.rodata` address word, that word holds the global's address, and the third
load fetches the member. `gMap->scrollY` emits exactly that, and the matched
neighbour src/decomp/c_080358C4.c already proves it with its own word at
0x08090EA4. The neighbouring words 0x08090EA0 / EA4 / EA8 hold 0x08499598,
0x08499590 and 0x08499590 -- one per referring function, not one shared table.

`gMap` and `gUnknown_08499590` are the same 4-byte slot under two names, so
either spells the word; gMap is used here because the matched neighbour uses it
and map.h's member names make the body readable.

## The register pin was a fossil and cost 4 bytes

`register int y asm("r12")` was added in wave 71, when it was worth 12 bytes
against the pool-chasing draft. Against the gMap draft it costs the last four:
removing it makes the function SIZE-EXACT and adds 5 points. It also un-blocks
the permuter, which cannot parse `register ... asm("rN")` at all and had been
reporting "could not score the starting point" on this function -- a syntax
error that reads like a result.

GENERAL: when a pool-word spelling is fixed, RE-TEST every allocation hack the
old draft carried. They were tuned against different code.

## The residual

Size-exact, one instruction's worth of allocation. The ROM keeps `&proc->unk42`
in the HIGH register r8 and pays a `mov rLow,r8` at each of its three reads,
and keeps the `py2` copy in the low r5; our build does the opposite, keeping
`&proc->unk42` low and the `py2` copy in r8. The ROM also reaches
`proc->unk35` as `&proc->unk42 - 13` and makes the py-to-py2 copy on BOTH sides
of the optional camera call where ours makes it once.

Refuted here: writing the body with direct member access instead of the `py` /
`px` / `py2` pointer locals. 300 bytes (-24), 5.25% with the pin, 308 (-16),
7.10% without. The pointer locals are load-bearing.

## Permuter, run 1 (900 s, 4 threads, chained from the size-exact draft)

    54.63% -> 56.17%, still size-exact 324/324, first difference +0x2

The kept candidate is work/sub_080359A4/permuter/output-5100-1 in its SPLICED
form. Diffed against the hand draft, its only change with any meaning is a
zero-trip `do { sub_080358C4(*px, y); } while (0);` around the optional camera
call; everything else is the permuter's parenthesisation. It is left as the
permuter wrote it, per the standing rule about not tidying a permuter win.

WORTH KNOWING FOR THE NEXT RUN: permuter score and byte identity disagreed
badly here, and the divergence was one-directional. The best permuter scores
(3500, 3580) verified at 34.26% and 39.20% and were 8 bytes SHORT, while the
winner sat at score 5100, barely below the 5400 starting point. Anyone reading
`permuter.log` for "found a better score!" lines on this function will pick the
wrong candidates; only the harvest's own trymatch lines mean anything.

The permuter could not run on this function at all before this wave, because
pycparser rejects the `register int y asm("r12")` the draft had carried since
wave 71 and permute.py reports that as "could not score the starting point".

## Permuter, run 2 -- REACHED 79.32% AND WAS REJECTED, AND THE REASON IS USEFUL

    56.17% size-exact  ->  79.32% size-exact, from output-4380-1

Rejected. Kept for inspection at `work/sub_080359A4/w92-perm2-uninit.c`.

WHAT IT DOES: it DELETES the unconditional `py2 = py;` and puts the assignment
INSIDE the do/while that only runs when the camera call runs. On the other path
`py2` is never assigned, and it is then read twice -- in the occupancy test and
in sub_080255F4's third argument. That is a read of an uninitialized local.
The emitted code happens to be correct because the allocator gives `py` and
`py2` the same register, but the C is undefined and this is the same class of
candidate wave 74 rejected twice (see sub_0802F6A0's entry, where adopting one
cost several waves of scores that measured the wrong program).

TWO SAFE SPELLINGS OF THE SAME IDEA WERE MEASURED AND BOTH ARE WORSE. Do not
re-run them:

    py2 = py; before the if AND again inside it   328  +4  18.90%
      (with either `*py` or `y` as the call's second argument -- identical)
    delete py2 entirely and use *py everywhere    316  -8  14.51%

WHAT IT TELLS US ANYWAY, and this is the part to build on. The ROM emits
`adds r5,r4,#0` TWICE -- once before the `bne` that skips the camera call and
once after the call returns. Writing that copy twice in the source costs four
bytes, so the ROM's two copies are ONE source statement that the compiler
duplicated across the join, not two statements. The permuter got the
duplication by making the copy conditional, which is not available to us. The
legitimate lever is whatever makes agbcc duplicate one unconditional copy into
both predecessors of that join; that is the question for the next wave, and it
is a much narrower one than "the pointer allocnos are rotated".

## wave 96

Base: `sub_080359A4.w96-start.c` (56.2%, first diff +0x2). `best.c` (79.3%) is wrong C: it assigns `py2 = py` only inside
the `if` and reads it after. Its content is that the copy `py2` must be created on the call path AFTER the call. The legal
spelling assigns `py2 = py` on BOTH paths (`if (...) { call; py2 = py; } else { py2 = py; }`): 75.3%, first diff still +0x2
(the prologue register order). Transferred as a general trick: an "uninitialised on one path" permuter form has a legal
twin that assigns the copy on every path. Then one 800 s permuter run: 75.3% -> 77.2%, kept change is a write-only temp
around the first call argument (`sub_080358C4(pxVal = *px, y)`) and `(*py)` instead of `(*py2)` in the turnState read.
Residual: register roles. ROM: proc r6, py r4, y in ip, px address r8, `*px` value r7, py2 r5, and
`proc->unk35` computed as `px - 13` off the r8 copy of `&proc->unk42`; ours: proc r5, py r6, y r4, px address sl.
Reading `unk35` through `((u8 *)px)[-13]` is byte-identical (cse already derives it). Not solved.
Proposed summary: 77.2% size-exact; left: register roles for proc/py/y/px, first diff in the prologue.

## wave 97 (W97-U)

Base `sub_080359A4.c` (77.16%). New fact from reading the ROM: after the do-while every later read of y goes through the
COPY (r5 = py2), including `((*py2)+8)/16` and the final call's y argument; py (r4) is used only to make the copies, and
`y` (first read, kept in ip) is a separate pseudo held in a high register. Two live pointer copies + a high-register y is
why the ROM pushes only {r6,r7} for hi regs: spelling BOTH later reads as `*py2` makes the push list match
(`{r6,r7}`) but copy propagation deletes py2 entirely (316 B, -8, 15.1%). Reading only the final call's y through py2:
316 B, 13.0%. `py2 = py;` before the `if` and again after the call: the second copy is deleted as redundant (320 B, -4);
with both later reads `*py2` that is 59.6%, push list matches, size -4 (the ROM's second `adds r5,r4,#0` after the call
is what is missing). `x = *px` as a real s16/int local used in the range checks: 53.4% +4, frame differs.
Mechanism: the ROM keeps two copies (before and after the call); cse removes the second because the first already
makes py2 == py. No spelling tried keeps both; whatever the source does, the copy after the call must not be provably
redundant (the C for that is unfound).
