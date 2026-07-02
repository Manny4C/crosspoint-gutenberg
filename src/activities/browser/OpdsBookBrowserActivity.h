#pragma once
#include <OpdsParser.h>

#include <string>
#include <utility>
#include <vector>

#include "OpdsServerStore.h"
#include "activities/Activity.h"
#include "util/ButtonNavigator.h"

/**
 * Activity for browsing and downloading books from an OPDS server.
 * Supports navigation through catalog hierarchy and downloading EPUBs.
 */
class OpdsBookBrowserActivity final : public Activity {
 public:
  enum class BrowserState { CHECK_WIFI, WIFI_SELECTION, LOADING, BROWSING, DETAIL, DOWNLOADING, ERROR, SEARCH_INPUT };

  explicit OpdsBookBrowserActivity(GfxRenderer& renderer, MappedInputManager& mappedInput, OpdsServer server,
                                   bool gutenberg = false)
      : Activity("OpdsBookBrowser", renderer, mappedInput),
        buttonNavigator(),
        server(std::move(server)),
        gutenberg(gutenberg) {}

  void onEnter() override;
  void onExit() override;
  void loop() override;
  void render(RenderLock&&) override;

 private:
  ButtonNavigator buttonNavigator;
  BrowserState state = BrowserState::LOADING;
  std::vector<OpdsEntry> entries;
  std::vector<std::string> navigationHistory;
  std::string currentPath;
  std::string searchTemplate;
  bool consumeConfirm = false;
  bool consumeBack = false;  // Added missing member
  int selectorIndex = 0;
  std::string errorMessage;
  std::string statusMessage;
  size_t downloadProgress = 0;
  size_t downloadTotal = 0;
  int detailIndex = 0;  // entries[] index shown on the DETAIL screen

  OpdsServer server;       // Copied at construction — safe even if the store changes during browsing
  bool gutenberg = false;  // Built-in Project Gutenberg catalog: id de-dup + detail screen

  void checkAndConnectWifi();
  void launchWifiSelection();
  void onWifiSelectionComplete(bool connected);
  void fetchFeed(const std::string& path);
  void navigateToEntry(const OpdsEntry& entry);
  void navigateBack();
  void showDetail(int index);
  void downloadBook(const OpdsEntry& book);
  void openBook(const std::string& path);
  void launchSearch();
  void performSearch(const std::string& query);
  bool preventAutoSleep() override { return true; }

  // Local library path a book would occupy once downloaded, and whether it is
  // already there (de-dup by Gutenberg id, per PRD F7).
  std::string libraryPathFor(const OpdsEntry& book) const;
  bool isInLibrary(const OpdsEntry& book) const;
  void renderDetail();
};
