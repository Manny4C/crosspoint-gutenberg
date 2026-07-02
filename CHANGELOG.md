# Changelog

All notable changes to this fork are documented here. This project follows
[Keep a Changelog](https://keepachangelog.com/en/1.1.0/) and
[Semantic Versioning](https://semver.org/spec/v2.0.0.html).

This fork tracks [CrossPoint Reader](https://github.com/crosspoint-reader/crosspoint-reader)
and adds on-device Project Gutenberg downloads on top of it.

## [Unreleased]

### Added
- **Built-in Project Gutenberg catalog.** An always-available "Project Gutenberg"
  entry on the home screen browses Gutenberg's OPDS catalog (Popular /
  Bookshelves / Latest / Search) and downloads DRM-free EPUBs straight to SD —
  no phone, laptop, or SD-card shuffle, and no configuration.
- **Book detail screen** (title, author, description) before downloading.
- **De-dup by Gutenberg id** (`gutenberg-<id>-<slug>.epub`): a title already in
  the library shows **Read** instead of **Download** and opens into the reader.
- `<summary>`/`<content>` capture in the OPDS parser (bounded to 320 bytes) to
  feed the detail screen.
- `src/GutenbergCatalog.h` as the single config seam for the catalog URL and the
  id/filename helpers, so other DRM-free OPDS sources are a copy + a menu entry.
- Fork tooling: CI (format, cppcheck, build, unit tests), tag-triggered release
  workflow, Dependabot, `CONTRIBUTING.md`, and fork-specific `README.md`.

### Notes
- Networking (Wi-Fi provisioning, CA-verified HTTPS streaming downloader, OPDS
  SAX parser, browse/search/download UI) is reused from upstream unchanged.
- Ownership is computed off the render task to avoid SD I/O on the shared SPI bus
  during a screen refresh.
