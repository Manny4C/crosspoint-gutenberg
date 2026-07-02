# Contributing

This is a fork of [CrossPoint Reader](https://github.com/crosspoint-reader/crosspoint-reader)
that adds on-device Project Gutenberg downloads. Contributions to the fork's
feature are welcome; changes to the core reader are usually better sent upstream.

## Ground rules from upstream

The upstream engineering guide ([CLAUDE.md](CLAUDE.md)) and governance
([GOVERNANCE.md](GOVERNANCE.md)) apply here too. The hard constraints worth
repeating:

- **~380 KB RAM, no PSRAM, single framebuffer.** Justify every heap allocation;
  prefer `makeUniqueNoThrow` over bare `new`/`malloc` (bare `new` calls
  `abort()` on OOM with `-fno-exceptions`).
- **The display is a slow terminal** (~500 ms full-frame refresh, no partial
  updates at the app layer). Design for discrete full-screen states, not
  animation or live progress bars.
- **SD and the panel share one SPI bus** — never do SD I/O on the render task if
  it can be avoided; compute file state off the render path.
- All user-facing strings go through `tr(STR_*)` (i18n); logging can be literal.

## Dev loop

```bash
git clone --recurse-submodules https://github.com/Manny4C/crosspoint-gutenberg
cd crosspoint-gutenberg

pio run -e default              # build
pio run -e default -t upload    # flash
pio device monitor              # serial logs

./bin/clang-format-fix          # format (clang-format 21+); CI enforces this
pio check                       # static analysis (cppcheck)
cmake -S test -B build/test -G Ninja && cmake --build build/test && ctest --test-dir build/test  # unit tests
```

## Pull requests

- Branch off `main`; use conventional commit prefixes (`feat:`, `fix:`,
  `docs:`, `ci:`, `refactor:`, `chore:`).
- Keep changes single-concern and small; the codebase is prone to sprawl diffs.
- CI must pass: format, cppcheck, build, and unit tests.
- Test on hardware where you can, and note what you verified (and in which of
  the 4 orientations) — the maintainer can't see your device.

## The Gutenberg feature

The catalog is a thin layer over the existing generic OPDS client. The extension
seam is [`src/GutenbergCatalog.h`](src/GutenbergCatalog.h); see
[docs/gutenberg.md](docs/gutenberg.md) for the design and the list of candidate
future sources.
