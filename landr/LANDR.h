#pragma once

#include "Status.h"

#include <string>
#include <memory>

class LANDR
{
public:
    LANDR();
    ~LANDR();
  
    bool licenseIsValid() const;
    bool licenseIsTrial() const;

    void loadLicense();
    void activateWithKey(const std::string& key);

    std::string getUserId() const;
    std::string getTrialType() const;
    std::string getLicenseKey() const;
    std::string getVersion() const;
  
    landr::StatusCode getStatusCode() const;
    landr::StatusInfo getStatusInfo() const;
            
protected:
    class Pimpl;
    std::unique_ptr<Pimpl> m_pimpl;
};
