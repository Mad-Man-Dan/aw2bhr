# sub_080726E8 — wave 93 (W93-B)

## New base: 56.94% size-exact, up from 17.54% at +12

The wave-start scan named `best.c`/`recovered.c` (56.94%, size-exact 216). It was
diffed against the draft and it IS equivalent C. Its only change is a
`volatile` temporary in the flipped arm's store:

    volatile int col;          /* was new_var */
    ...
    col = ix;
    *((map + (x + col)) + ((y + iy) * 0x20)) = (p[iy*0x20 + w - ix - 1] + base) ^ 0x400;

`col` is set before it is read, holds `ix` unchanged, and the source index still
uses `ix`, so the same map cell gets the same value. `volatile` on a local that
nothing else touches adds no observable behaviour. Adopted (renamed from
`new_var`), draft backed up as `sub_080726E8.w93-start.c`. Re-measured after the
rename: **56.94%, size-exact, first difference +0xa** — identical to the
pre-rename score.

## The volatile is the right MECHANISM and the wrong CONSTRUCT — and the frame proves it

The parked residual was: agbcc strength-reduces both the source and the
destination pointer in the flipped arm, where the ROM recomputes both addresses
every iteration and reduces nothing. The `volatile` read forces the value to the
stack and back, which defeats exactly that reduction — this is the "volatile
splits VALUE cse" splitter, and it is the first thing that has moved this
function. It takes the size from +12 to exact and triples the matching bytes.

But it cannot be what the original wrote, and the stack frame says so
outright:

    ROM        add sp, #8      (map at [sp], p at [sp,#4])
    candidate  add sp, #12

A `volatile` local is an addressable object, so it costs a third frame slot. The
ROM has **no third stack object at all**. So the construct the original used
defeats the strength reduction *without creating an addressable local*. That is
a sharper target than "keep one more value live inside the loop", which is what
the parked entry proposed.

The size still comes out exact because the trade is even: the volatile adds one
slot and removes the two slots the two hoisted giv initial values needed.

The extra slot also perturbs the register map globally, which is why the
previously byte-exact UNFLIPPED arm now differs too (r2/r4, r3/r2, sl/r9, r9/r8
swap roles at the tail). That is a consequence of the frame, not a second
problem.

## Six spellings measured against the frame hypothesis � all negative

The hypothesis was: find a flipped-arm-only CSE split that allocates no stack
object, and the frame should come out at the ROM's 8 bytes. Six variants were
compiled together with `tools/drafts.py bases`. Frames read off the prologue:

    ROM                                          sub sp, #8   (map, p)
    volatile temp (the adopted base)  56.94% +0  sub sp, #12  (map, p, col)
    plain `int col` temp              17.54% +12 -- byte-identical to no temp
    `u16 *d` set in its own statement 17.54% +12 -- byte-identical to no temp
    8-bit fold mask on the dest index 18.64% +4  no local
    the same mask on BOTH indices     19.64% +8  no local
    `u16 ix`                          10.19% +0  sub sp, #4   (map only)
    `u16 ix` and `u16 iy`             11.82% +4

Four things worth keeping from that:

- **Only `volatile` defeats the reduction.** A plain temporary and a pointer
  bound in its own statement are byte-identical to writing neither: agbcc
  coalesces both away and strength-reduces exactly as before. The wave-89
  splitter list is right that the splitter must be `volatile`, a statement join,
  a `static inline`, or a fold-proof mask � an ordinary local is not a splitter.
- **The fold-proof mask is a PARTIAL splitter here, and it needs no local.** On
  the destination index alone it takes +12 to +4, so it kills one of the two
  strength reductions. Applied to the source index as well it goes back to +8,
  so masking the source index costs 4 bytes rather than saving them. (It also
  truncates to 8 bits, which is only provably identity while `x >= 0`.)
- **Size-exact is reachable two ways and neither is structurally right.**
  `u16 ix` hits 216 bytes exactly with no stack object, but its frame is 4 bytes:
  it drops `p` off the stack, where the ROM keeps `map` at [sp] and `p` at
  [sp,#4]. The volatile form keeps both and adds a third slot. One frame is a
  slot short, the other a slot long, and the volatile form matches 123 bytes
  against 22, so it is much the better base.
- The remaining search space is therefore a splitter that defeats the
  destination reduction with no frame object (the fold mask does) AND leaves the
  source reduction alone AND does not disturb `p`'s slot. The fold mask at +4 is
  the nearest miss and is where the next attempt should start, not from the
  volatile form.

`-fno-rerun-loop-opt` puts the OLD draft at 41.20% size+0 (against its
configured 17.54% at +12). That is the same fact from the flag side: the ROM's
flipped loop carries fewer strength-reduced induction variables than our compile
makes. It is a hint about the loop's source shape, not an override.

`-fno-rerun-loop-opt` puts the OLD draft at 41.20% size+0 (against its
configured 17.54% at +12). That is the same fact from the flag side: the ROM's
flipped loop carries fewer strength-reduced induction variables than our compile
makes. It is a hint about the loop's source shape, not an override.

## Residual

216/216, 123 of 216 bytes differ, first difference at +0xa. One extra frame slot
(12 against 8) and the register renumbering that follows from it.
