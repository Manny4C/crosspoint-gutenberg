# CrossPoint Reader — Project Gutenberg Edition

A fork of [**CrossPoint Reader**](https://github.com/crosspoint-reader/crosspoint-reader)
(open-source e-reader firmware for the ESP32-C3-based Xteink **X3 / X4**) that
adds **on-device Project Gutenberg downloads** — browse the catalog and grab
free, DRM-free books straight to your reader over Wi-Fi, with no phone, laptop,
or SD-card shuffle.

> This repo tracks upstream CrossPoint and layers one feature on top. For the
> full firmware docs (hardware, all features, flashing details, contributing),
> see the preserved upstream readme: [README.upstream.md](README.upstream.md).

## What this fork adds

- A built-in, always-available **Project Gutenberg** entry on the home screen —
  zero configuration (the catalog is public and DRM-free).
- **Browse** (Popular / Bookshelves / Latest) and **Search** by title/author.
- A **book detail** screen (title, author, description) before you download.
- **Download EPUB → SD**, streamed and power-safe over CA-verified HTTPS.
- **De-dup by Gutenberg id** — a title you already have shows **Read** instead
  of **Download**, and opens straight into the reader.

Full write-up, design notes, and a wishlist of other DRM-free sources that could
reuse the same OPDS engine (Standard Ebooks, Feedbooks, etc.):
[**docs/gutenberg.md**](docs/gutenberg.md).

The feature is a thin product layer over CrossPoint's existing generic OPDS
client; the extension seam is a single config file,
[`src/GutenbergCatalog.h`](src/GutenbergCatalog.h).

## Build & flash

Requires [PlatformIO](https://platformio.org/). Clone with submodules:

```bash
git clone --recurse-submodules <this-repo-url>
cd crosspoint-gutenberg

pio run -e default              # build (also: gh_release / slim)
pio run -e default -t upload    # flash over USB
```

A browser-based Web Serial flasher works too — write `.pio/build/default/firmware.bin`.

## Try it

1. Boot, select **Project Gutenberg** on the home screen.
2. First run: pick your Wi-Fi network and enter the password (stored once, in NVS).
3. Browse or search, open a book, press **Download** — it lands in your library
   and reads fully offline like any sideloaded EPUB.

## Credits & license

Fork of **CrossPoint Reader** by Dave Allie and the CrossPoint community
([crosspoint-reader/crosspoint-reader](https://github.com/crosspoint-reader/crosspoint-reader)),
which builds on the [FreeInk SDK](https://github.com/Free-Ink/freeink-sdk). All
of the heavy lifting — the reader, rendering, OPDS client, Wi-Fi, and downloader
— is their work.

Licensed under the **MIT License**, the same as upstream. The original copyright
notice is preserved in [LICENSE](LICENSE); the Project Gutenberg additions in
this fork are released under the same terms.
