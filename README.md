# LANDR SDK

The purpose of the LANDR SDK is to provide software licensing capability through a simple interface. This is achieved via a single `Licenser` class, which is expected to be kept in memory for the lifetime of the plugin.

> [!WARNING]
> This solution provides *licensing only*. It does NOT provide any form of copy protection.

# Example

```cpp
#include "landr/Licenser.h"

landr::Licenser licenser;

licenser.loadLicense();

if (!licenser.licenseIsValid())
{
    // If the license is not valid, attempt to activate it with a user-provided key
    licenser.activateWithKey("1234");
}

if (!licenser.licenseIsValid())
{
    // Optionally provide special handling based on status code
    switch(licenser.getStatusCode())
    {
        case landr::Status::Invalid:
        ...
    }
}
else
{
    // Successfully licensed!
}
```

If the license state changes internally, for example, if the license is revoked during the lifetime of the `Licenser` object, the new state will be reflected in the next call to `licenseIsValid()`.

It is safe to call `activateWithKey()` multiple times. The activation count will only be incremented on the first successful call.


# Advanced Usage

In addition to license activation, and checking status, the SDK provides the option to generate status information for populating a UI dynamically.

This is achieved via the `StatusInfo` struct. All fields in this struct will be populated based on the current license state. It may also be empty.

For example:

```cpp
if (!licenser.licenseIsValid())
{
    const auto info = licenser.getStatusInfo();

    SomePopupDialog dialog;
    dialog.setTitle(info.titleText);
    dialog.setMessage(info.message);
    dialog.setErrorMessage("Error code: " + errorSubCode);

    if (info.linkText)
    {
        dialog.setButtonText(info.linkText);
        dialog.setButtonUrl(info.linkURL);
    }

    dialog.show();
}
```

# Handling Trials
The `Licenser` fully supports trial licenses. In order to check for a trial license, simply call the `licenseIsTrial` method.

For example:

```cpp
if (licenser.licenseIsTrial())
{
    // Apply audio watermark
    // Show banner
}
```

# Checking for Updates

The SDK includes an `UpdateChecker` class that periodically polls a remote JSON file to check for available updates. It runs in a background thread and detects both regular updates and forced updates (when a minimum supported version is specified).

## Basic Usage

```cpp
#include "landr/UpdateChecker.h"

// Create an UpdateChecker with your current version
// The version file URL is obtained from your Config::getVersionURL()
landr::UpdateChecker updateChecker("1.2.3");

// Start checking for updates every 5 minutes (300000 ms)
updateChecker.startChecking(300000);

// Later, check if an update is available
if (updateChecker.hasNewVersion()) {
    const auto info = updateChecker.getUpdateInfo();
    if (info) {
        std::cout << "New version available: " << info->version << std::endl;
        std::cout << "Download: " << info->downloadLink << std::endl;

        if (info->forceUpdate) {
            // A new minimum supported version is required
            // Handle forced update in your UI
        }
    }
}

// Stop checking when done
updateChecker.stopChecking();
```

## version.json Format

The remote JSON file must follow this structure:

```json
{
  "versionUrl": "https://plugins-dev.landr.com/version/sampler",
  "macOS": {
    "version": "1.45.11",
    "url": "https://example.com/downloads/macOS/installer.pkg",
    "releaseNotesUrl": "https://example.com/release-notes",
    "minimumSupportedVersion": "1.20.0"
  },
  "windows": {
    "version": "1.45.11",
    "url": "https://example.com/downloads/windows/installer.msi",
    "releaseNotesUrl": "https://example.com/release-notes",
    "minimumSupportedVersion": "1.20.0"
  }
}
```

### Top-level Fields
- `versionUrl`: Optional field describing the version endpoint (for server documentation; ignored by the SDK)

### Per-OS Fields
- Top-level keys: per-OS sections (`macOS` and `windows`)
- `version`: semantic version string (format: `major.minor.patch`) — **Required**
- `url`: URL to the installer for that OS — **Required**
- `releaseNotesUrl`: URL to release notes — Optional, treated as empty if missing
- `minimumSupportedVersion`: semantic version string. If the current version is lower than this, the update is considered forced — Optional

### Notes
- If `releaseNotesUrl` is missing, it is treated as empty.
- If `minimumSupportedVersion` is missing, `forceUpdate` is false.
- The OS key names are fixed by the platform (automatically detected as `macOS` or `windows`).

## Forced Updates

If `minimumSupportedVersion` is specified in the JSON and the current version is lower, the `forceUpdate` flag in `UpdateInfo` will be set to `true`. This allows you to implement a mandatory update flow in your plugin UI.

The `UpdateChecker` only reports the flag—it does not enforce the update or block functionality. Your plugin must handle the forced update behavior.

# Thread Safety

The LANDR SDK is thread-safe. All methods can be called concurrently from multiple threads without issue.

# Linkage

The LANDR SDK includes two shared libraries that need to be linked.

- libLANDR: the core LANDR SDK
- libLANDRConfig: a library containing product-specific configuration

> [!IMPORTANT]
> These librairies are **not** supplied in this repository. Please contact pluginsdk@landr.com for more information.

## Mac

Link to the following libraries:
- `LANDR_SDK/lib/macos/libLANDR.a`
- `config_<product>/macos/libLANDRConfig.a`
- `SystemConfiguration` framework
- `AppKit` framework

## Windows
Multiple variants of the LANDR SDK are available on windows.
- `LANDR_MT`: Static multithread
- `LANDR_MTd`: Static debug multithread
- `LANDR_MD`: Dynamic multithread
- `LANDR_MTd`: Dynamic debug multithread

These are available to be linked to depending on which runtime library is needed

Link to the following libraries:
- `LANDR_SDK/lib/windows/LANDR_<variant>.lib`
- `config_<product>/windows/LANDRConfig_<variant>.lib`
- `winhttp`
- `Crypt32`
- `wininet.lib`

# Minimum system requirements
- **C++ Standard**: 17
- **CMake**: 3.26
- **macOS**: 10.14
- **Windows**: Windows 10
