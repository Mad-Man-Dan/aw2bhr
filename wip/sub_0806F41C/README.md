# sub_0806F41C

0x0806F41C, 308 bytes, THUMB, parked.

Best score so far: 15.4%, +4 bytes.

## What it does

Rebuilds the screen for one entry of a 16-byte screen table, chosen by the proc: it turns the windows off, ends related procs, and either loads the entry's graphics or restarts the gUnknown_08582CAC proc.

## How close it is

Compiles 4 bytes too long (312 against 308; the code alone is 8 over), 15.6% of bytes in place. Every call, constant and field matches. The draft keeps the address of proc->unk38 alive across the first if/else where the ROM computes it again, so it needs one more saved register; that puts `flag` in a high register (its `!= 0` becomes a branch), adds a level of indirection to the table load, and lets the two gUnknown_03002B6C stores merge.

## What is left

First, name the table itself instead of gUnknown_0816E808, which is the compiler's own pointer word holding the table's address 0x085828DC: a quick compile with `extern struct Unk0816E808Entry gUnknown_085828DC[];` gives the ROM's two-step table load, though the extra saved register remains. Then find what makes the compiler recompute the address of proc->unk38 after the if/else; a permuter run has not been tried.

## Already tried

- Binding tbl inside the unk0d test: fixes the order of one load but costs a second high register; worse.
- Two separate pointer locals for the two uses of proc->unk38: the compiler merges them back; byte-identical.
- Dropping tbl and naming the table at all five uses: 12 bytes too long, 8.8%.
- Reaching unk38 through a byte pointer or an integer address: byte-identical.
- Volatile views of unk38 after the branch: force reloads, but in a different form, and the old address is still kept.

## Files

- `sub_0806F41C.c`: the current draft
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 78 at 312/308 (+4 section, +8 code), 15.6%. Raw integer proc+0x38 spelling is byte-identical; volatile post-branch views emit ldrb/shifts and retain the old address instead of ROM recomputation. Residual is post-branch address lifetime.

</details>
