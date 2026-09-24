#pragma once

#include <string>
#include <memory>
#include <optional>
#include <future>

namespace landr {

class UpdateCheckerTests;

/// Class for checking for updates to the plugin
class UpdateChecker {
public:
    /// Information about an available update
    struct UpdateInfo {
        /// @brief The version string of the available update (format: "major.minor.patch")
        std::string version;
        /// @brief URL to download the update
        std::string downloadLink;
        /// @brief URL to the release notes for the update (optional)
        std::string releaseNotesLink;
        /// @brief Whether the update is a forced update (i.e. the current version is no longer supported)
        bool forceUpdate = false;
    };

    /// Create an UpdateChecker instance
    /// @param currentVersion Current version string (format: "major.minor.patch")
    /// @note The version URL is read from the application's Config::getVersionURL()
    UpdateChecker(const std::string& currentVersion);
    ~UpdateChecker();

    /// Start checking for updates at the specified interval
    /// @param intervalMs Check interval in milliseconds (default: 300000ms = 5 minutes)
    void startChecking(int intervalMs = 300000);

    /// Stop checking for updates
    void stopChecking();

    /// Set the check interval while already checking
    /// @param intervalMs Check interval in milliseconds
    void setCheckInterval(int intervalMs);

    /// @brief Get the current version string
    /// @return Current version string (format: "major.minor.patch")
    std::string getCurrentVersion() const;

    /// Check if a new version is available
    /// @return true if an update is available
    bool hasNewVersion() const;

    /// Get information about the available update
    /// @return UpdateInfo if an update is available, or empty optional if none
    std::optional<UpdateInfo> getUpdateInfo() const;

    /// Perform a single check for updates asynchronously
    /// @return Future that resolves to UpdateInfo if an update is available, or empty optional if none
    std::future<std::optional<UpdateInfo>> checkForUpdate();

private:
    friend class UpdateCheckerTests;

    class Pimpl;
    std::unique_ptr<Pimpl> m_pimpl;
};

} // namespace landr
