# vendor/

Third-party tools, not committed. `vendor/` is gitignored: these are upstream
repositories with their own history, and vendoring them here would either bury a
second git tree inside this one or import thousands of files we never edit.
What *is* committed is this file, so the versions are recoverable.

## decomp-permuter

```sh
git clone https://github.com/WhenGryphonsFly/decomp-permuter-agbcc.git \
    vendor/decomp-permuter
```

Pinned at `1f7ef872b12f54db7678ff00e0346abc015410ae` (2025-06-27). This is the
ARMv4T/agbcc fork rather than upstream `simonlindholm/decomp-permuter`; the
difference that matters is `ARM32_SETTINGS` in `src/objdump.py`, without which
the scorer cannot read our objects at all.

Needs three Python packages, none of them installed by default:

```sh
wsl -d Ubuntu -u root -e bash -c \
    "apt-get install -y python3-pycparser python3-toml python3-levenshtein"
```

`python3-levenshtein` is only used by `--algorithm levenshtein`; the default is
difflib. The other two are required.

**If you don't have root** (no sudo password, `apt-get` unavailable), use a venv
under `vendor/` instead -- it survives a reboot same as `vendor/decomp-permuter`
itself, just not committed:

```sh
python3 -m venv vendor/permuter-venv
vendor/permuter-venv/bin/pip install "pycparser==2.22" toml python-Levenshtein
```

`pycparser==2.22` is pinned deliberately: the current release (3.00) dropped the
`pycparser.plyparser` module this permuter fork imports, so a plain
`pip install pycparser` breaks it with `ModuleNotFoundError: No module named
'pycparser.plyparser'`. `apt`'s `python3-pycparser` happens to package an
older version that still has it, which is why the apt route above doesn't need
this pin.

`tools/permute.py` invokes `python3 vendor/decomp-permuter/permuter.py` as a
bare shell command, so whichever `python3` is first on `PATH` is the one that
runs -- the venv is not picked up automatically. Put it first when using this
route:

```sh
PATH="$(pwd)/vendor/permuter-venv/bin:$PATH" python3 tools/permute.py <name> --seconds 300
```

### Required local patch: scorer penalties

`vendor/` is gitignored, so this does **not** survive a re-clone. Re-apply it,
or `tools/permute.py` will warn on every run.

In `src/scorer.py`, upstream ships:

```python
PENALTY_REGALLOC = 5
PENALTY_REORDERING = 60
```

Change `PENALTY_REGALLOC` to **60**. Upstream's weights say a wrong register is
a twelfth of a reordering, which is right for a human reading a diff and wrong
for our verdict: we judge exact bytes, and a wrong register changes its
instruction's encoding just as a reordering does. At 5, trading one reordering
for eleven register differences is a score *win* and a byte disaster — which is
exactly the anomaly `tools/permute.py`'s header records as unexplained
("35 -> 30 while bytes went 80% -> 75%").

Keep it overridable so the weight can be A/B'd without another edit:

```python
PENALTY_REGALLOC = int(__import__("os").environ.get("AW2_PENALTY_REGALLOC", "60"))
```

Second part, added in wave 93: a penalty per instruction of LENGTH difference
from the target. Add beside the other weights:

```python
PENALTY_SIZE = int(__import__("os").environ.get("AW2_PENALTY_SIZE", "1000"))
```

and to `final_score`:

```python
+ abs(len(cand_seq) - len(self.target_seq)) * self.PENALTY_SIZE
```

Without it, a candidate two instructions short pays only 2 x 100 and sheds
many register penalties along with the instructions, so on a size-exact draft
whose residual is register numbering the search walks away from the answer.
Wave 93's run on sub_08070F44 verified 40 candidates that all beat the draft's
score (620-1940 against 2100) and every one matched worse, 4 bytes short at
best. A wrong-length candidate can never be a byte match, so it must never
outrank a right-length one. `AW2_PENALTY_SIZE=0` restores upstream behaviour.

Do not invoke `permuter.py` directly — `tools/permute.py` builds the input
directory, imposes a time limit, and re-checks every result with
`tools/trymatch.py`. The permuter scores by diffing objdump text, which is a
weaker test than byte equality, so its own score is a search signal and not a
verdict.

## Ghidra

This WSL checkout uses Ghidra 12.1.3 and Adoptium JDK 25.0.4.1+1, extracted
under `vendor/`. The archives are available from their official releases:

- `https://github.com/NationalSecurityAgency/ghidra/releases/tag/Ghidra_12.1.3_build`
- `https://github.com/adoptium/temurin25-binaries/releases/tag/jdk-25.0.4.1%2B1`

Verified SHA-256 checksums:

- Ghidra ZIP: `93a5d11a9ad510622acaaf908c556a7b9b764d338e78a7567f3689bf5081fd54`
- JDK tarball: `dbb698396d478e7fa2b1e50f4103324b2a99b90569ee27c33f2261f9215cf41e`

The wrapper sets Java and Ghidra's settings/cache paths without changing the
system Java installation:

```sh
bash tools/ghidra.sh gui
bash tools/ghidra.sh headless <project_location> <project_name> -import <file> \
    -processor ARM:LE:32:v4t
```

For the GBA, choose ARM v4T, and check the Thumb context for each code region.
Ghidra's C is a starting point for understanding a function, not an agbcc
matching candidate. Rewrite it against the repository's types and verify with
`tools/trymatch.py`.

An analyzed project for the current `aw2bhr.elf` is already at
`vendor/ghidra-projects/aw2bhr.gpr`. To print one function's pseudocode from
that project:

```sh
bash tools/ghidra.sh headless "$PWD/vendor/ghidra-projects" aw2bhr \
    -process aw2bhr.elf -noanalysis \
    -postScript DecompileNamedFunction.java sub_0800E9F4 \
    -scriptPath "$PWD/tools/ghidra"
```

The linked ELF has malformed legacy DWARF that Ghidra logs and skips; the
symbol-table import and ARM v4T analysis still complete. Ghidra can also infer
incorrect return types from some agbcc epilogues, so check each result against
`asm/` and the existing headers. Re-import if the linked ELF is rebuilt and its
contents change.
