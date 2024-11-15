# LANDR SDK

The purpose of the LANDR SDK is to provide software licensing capability through a simple interface. This is achieved via a single `LANDR` class, which is expected to be kept in memory for the lifetime of the plugin.

**Disclaimer**: This solution provides *licensing only*. It does NOT provide any form of copy protection.
# Example

```cpp
#include "landr/LANDR.h"

LANDR landr;

landr.loadLicense();

if (!landr.licenseIsValid())
{
    // If the license is not valid, attempt to activate with user-provided key
    landr.activateWithKey("1234");
}

if (!landr.licenseIsValid())
{
    // Optionally provide special handling based on status code
    switch(landr.getStatusCode())
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

If license state changes internally, for example the license is revoked during the lifetime of the `LANDR` object, the new state will be reflected in the next call to `licenseIsValid()`.


# Advanced Usage

In addition to license activation, and checking status, the SDK provides the facility to dynamically generate status information to populate a UI. 

This is achieved via the `StatusInfo` struct. All fields in this struct will be populated based on the current license state, and may also be empty.

For example:

```cpp
if (!landr.licenseIsValid())
{
    const auto info = landr.getStatusInfo();

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

The `linkURL` field is populated based on the `pricingUrl` and `helpUrl` fields set in the `ProductInfo` struct passed on construction of the `LANDR` object.

# Thread Safety

The LANDR SDK is thread safe. All methods can be called concurrently from multiple threads without issue.

# Linkage

The LANDR SDK includes two shared libraries that need to be linked.

- libLANDR: the core LANDR SDK
- libLANDRConfig: a library containing product-specific configuration

## Mac

Link to `lib/<config>/libLANDR.a`, `lib/<config>/libLANDRConfig.a`

## Windows

Link to `lib/<config>/LANDR.lib`, `lib/<config>LANDRConfig.lib`

# Minimum system requirements
- **C++ Standard**: 17
- **macOS**: 10.14
- **Windows**: Windows 10
