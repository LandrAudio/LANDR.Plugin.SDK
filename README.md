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

# Thread Safety

The LANDR SDK is thread-safe. All methods can be called concurrently from multiple threads without issue.

# Linkage

The LANDR SDK includes two shared libraries that need to be linked.

- libLANDR: the core LANDR SDK
- libLANDRConfig: a library containing product-specific configuration

> [!IMPORTANT]
> These librairies are **not** supplied in this repository. Please contact pluginsdk@landr.com for more information.

## Mac

Link to `LANDR_SDK/lib/macos/libLANDR.a`, `config_<product>/macos/libLANDRConfig.a`

## Windows

Link to `LANDR_SDK/lib/windows/LANDR.lib`, `config_<product>/windows/LANDRConfig.lib`

# Minimum system requirements
- **C++ Standard**: 17
- **CMake**: 3.26
- **macOS**: 10.14
- **Windows**: Windows 10
