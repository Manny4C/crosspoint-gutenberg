#pragma once
#include <cctype>
#include <string>

#include "OpdsServerStore.h"
#include "util/StringUtils.h"

/**
 * Project Gutenberg is a built-in, always-available OPDS catalog. Unlike the
 * user-configured servers in OpdsServerStore (Calibre etc.), it needs no setup:
 * everything Gutenberg serves is DRM-free and public, so there is no auth wall.
 *
 * All Gutenberg endpoints live here as a single config point (per TRD-gutenberg
 * §5) so switching to a mirror later is a one-line edit. Downloads are never
 * hand-built: we follow the acquisition <link> the feed returns.
 *
 * This file is also the extension seam for other DRM-free catalogs: a second
 * OPDS source (e.g. Standard Ebooks) is a copy of this namespace + one home-menu
 * entry, no engine changes. See docs/gutenberg.md "Future sources" for the
 * candidate list — none are implemented yet.
 */
namespace GutenbergCatalog {

// The OPDS root feed. Links out to Popular / Bookshelves / Latest / Search.
constexpr const char* NAME = "Project Gutenberg";
constexpr const char* ROOT_URL = "https://www.gutenberg.org/ebooks.opds/";

// Build the OpdsServer descriptor the browser consumes. No credentials: the
// catalog is public.
inline OpdsServer makeServer() {
  OpdsServer server;
  server.name = NAME;
  server.url = ROOT_URL;
  return server;
}

/**
 * Extract the numeric Gutenberg ebook id from an OPDS entry <id>. Gutenberg
 * uses forms like "urn:gutenberg:1342" or ".../ebooks/1342"; we take the last
 * run of digits in the string. Returns "" when the id carries no number (e.g.
 * a navigation feed), so callers can fall back to title-based naming.
 */
inline std::string extractId(const std::string& entryId) {
  std::string last;
  std::string current;
  for (const char c : entryId) {
    if (std::isdigit(static_cast<unsigned char>(c))) {
      current += c;
    } else {
      if (!current.empty()) last = current;
      current.clear();
    }
  }
  if (!current.empty()) last = current;
  return last;
}

/**
 * Deterministic library filename for a Gutenberg book. Carrying the id makes
 * de-dup a directory check and keeps downloads distinguishable from sideloaded
 * files: "/gutenberg-<id>-<slug>.epub". When no id is available, returns "" so
 * the caller falls back to the generic author-title convention.
 */
inline std::string bookFilename(const std::string& entryId, const std::string& title) {
  const std::string id = extractId(entryId);
  if (id.empty()) return "";
  // Keep the slug short; the id already guarantees uniqueness.
  const std::string slug = StringUtils::sanitizeFilename(title, 60);
  return "/gutenberg-" + id + (slug.empty() ? "" : "-" + slug) + ".epub";
}

}  // namespace GutenbergCatalog
