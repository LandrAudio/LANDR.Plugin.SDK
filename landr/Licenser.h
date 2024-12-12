#pragma once

#include "Status.h"

#include <string>
#include <memory>

namespace landr {

/// Class for activating and querying LANDR licenses
class Licenser {
public:

    /// Create a Licenser instance
    /// Expected to be in memory for the lifetime of the application
    Licenser();
    ~Licenser();

    /// @return true if the license is valid
    bool licenseIsValid() const;

    /// @return true if the license is a trial
    bool licenseIsTrial() const;

    /// Load license state
    /// This must be called before calling licenseIsValid()
    void loadLicense();

    /// Activate the license
    /// @param key the license key entered by the user
    void activateWithKey(const std::string& key);
    
    /// Reset the license state on the local machine
    void resetActivation();

    /// @return the user ID of the user to which the license is allocated
    std::string getUserId() const;

    /// @return the license key previously used for activation
    std::string getLicenseKey() const;

    /// @return the version of landr::Licenser
    std::string getVersion() const;

    /// @return status code of the license for which an activation attempt has been made
    /// @see StatusCode
    landr::StatusCode getStatusCode() const;

    /// @return status info that can be used for populating UI for user feedback
    /// @see StatusInfo
    landr::StatusInfo getStatusInfo() const;

protected:
    class Pimpl;
    std::unique_ptr<Pimpl> m_pimpl;
};
}
