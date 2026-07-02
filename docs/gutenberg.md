# On-Device Project Gutenberg Downloads

This fork adds a built-in **Project Gutenberg** catalog so an X3/X4 can browse
and download free, DRM-free books directly over Wi-Fi — no phone, laptop, or
SD-card shuffle. See [PRD-gutenberg](../../xteink%20x3/PRD-gutenberg.md) and
[TRD-gutenberg](../../xteink%20x3/TRD-gutenberg.md) for the product/technical
background.

## What it does

- **Home → Project Gutenberg** — always present, no configuration. Opens
  Gutenberg's OPDS catalog root (Popular / Bookshelves / Latest / Search).
- **Browse** the catalog with the nav buttons; **Search** by title/author with
  the on-screen keyboard (one query per *Go*, paged results).
- **Book detail** screen shows title, author, and description, then
  **Download** (or **Read**, if the book is already in the library).
- **Download → SD**, chunked and power-safe (the existing HTTPS downloader:
  streamed to a temp file, verified, renamed; CA-verified TLS; polite
  `User-Agent`).
- **De-dup by Gutenberg id** — downloaded books are named
  `gutenberg-<id>-<slug>.epub`, so re-opening a title you already have offers
  **Read** instead of re-downloading.
- Downloaded books join the normal library and read fully offline.

## How it was built (reuse over new code)

Almost the entire pipeline already existed as a generic OPDS client. This
feature is a thin product layer on top:

| Piece | Status |
|---|---|
| Wi-Fi provisioning, HTTPS+CA downloader, OPDS SAX parser, browse/search/download activity | reused as-is |
| `src/GutenbergCatalog.h` | **new** — single config point for the catalog URL + id/filename helpers |
| Built-in home-menu entry + `ActivityManager::goToGutenberg()` | **new** |
| `gutenberg` mode in `OpdsBookBrowserActivity` (detail screen, id de-dup, open-if-owned) | **new** |
| `<summary>`/`<content>` capture in `OpdsParser` (for the detail screen) | **new**, bounded to 320 bytes |

To point at a mirror later, edit the one constant in
[`src/GutenbergCatalog.h`](../src/GutenbergCatalog.h) (`ROOT_URL`).

## Build & flash

```bash
pio run -e default            # or: gh_release / slim
pio run -e default -t upload  # flash over USB (Web Serial flasher also works)
```

## Try it

1. Boot, select **Project Gutenberg** on the home screen.
2. First run: pick your Wi-Fi network and enter the password (stored in NVS,
   asked only once).
3. Browse **Popular** (or **Search** a title), open a book, press **Download**.
4. Back out; the book is in your library and opens like any sideloaded EPUB.

## Notes / not-yet

- One book at a time, foreground only (politeness + battery), per the PRD.
- No cover thumbnails on the detail screen (grayscale render cost — deferred).
- Format selection prefers EPUB (the parser already favors `application/epub+zip`);
  TXT fallback and download resume are v2 items.
