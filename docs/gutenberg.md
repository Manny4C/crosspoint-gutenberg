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

## Future sources (candidates — NOT yet supported)

The catalog is a generic OPDS client with a single config point
([`src/GutenbergCatalog.h`](../src/GutenbergCatalog.h)). Adding another DRM-free
source is, in the easy case, a copy of that file plus one home-menu entry — no
engine changes. This is a wishlist to guide that; none are implemented and none
are commitments. URLs and auth should be re-verified before use.

**Tier A — drop-in OPDS (same shape as Gutenberg):**

- **Standard Ebooks** — meticulously typeset public-domain classics. OPDS at
  `https://standardebooks.org/opds`. Caveat: the feed may require a free
  patron/account token now (they gate bulk/OPDS access to fund the project), so
  it likely needs the username/password fields the store already has.
- **Feedbooks — Public Domain** — classic literature OPDS. The endpoint has
  moved over the years (Feedbooks pivoted to a paid store), so verify the
  current public-domain catalog URL before wiring it.

**Tier B — OPDS, but a mixed catalog (must filter to free/public-domain):**

- **Internet Archive / Open Library (BookServer)** — enormous OPDS catalog
  (`https://bookserver.archive.org`). Only openly-downloadable public-domain
  scans are DRM-free; lending titles are ACSM/Adobe-DRM and are explicitly out
  of scope. Would need format/rights filtering in link selection.

**Tier C — great content, needs an adapter (not OPDS → new parser/shim first):**

- **Gutendex** — JSON API over Gutenberg. Useful as a *fallback* if Gutenberg's
  OPDS ever throttles, but it's JSON, so it needs a small JSON adapter feeding
  the same entry model instead of the SAX path.
- **Digital Public Library of America (DPLA)** — REST/JSON aggregator.
- **Project Gutenberg Australia**, **Runeberg** (Nordic public domain),
  **Wikisource** (via a `ws-export`-style gateway) — HTML or bespoke; each needs
  scraping or a conversion gateway before it can look like an OPDS feed.

**Integration seam:** Tier A entries are a new `SomeCatalog::makeServer()` + a
home-menu item, mirroring the Gutenberg wiring exactly. Tier B/C need work in
the parser/link-selection layer before the browser can consume them.

## Notes / not-yet

- One book at a time, foreground only (politeness + battery), per the PRD.
- No cover thumbnails on the detail screen (grayscale render cost — deferred).
- Format selection prefers EPUB (the parser already favors `application/epub+zip`);
  TXT fallback and download resume are v2 items.
