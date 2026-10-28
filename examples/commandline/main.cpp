#include <iomanip>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>
#include "Licenser.h"

std::string wrapText(const std::string& text, int labelWidth)
{
    std::ostringstream wrappedText;
    std::istringstream lines(text);
    std::string line;
    std::string indentation(labelWidth, ' ');

    if (std::getline(lines, line))
        wrappedText << line;

    while (std::getline(lines, line))
        wrappedText << "\n"
                    << indentation << line;

    return wrappedText.str();
}

void dumpLicenserInfo(const landr::Licenser& licenser)
{
    nlohmann::json j;

    const auto& statusInfo = licenser.getStatusInfo();
    j["title"] = statusInfo.titleText;
    j["message"] = statusInfo.message;
    j["link_text"] = statusInfo.linkText;
    j["link_url"] = statusInfo.linkURL;
    j["error_subcode"] = statusInfo.errorSubCode;
    j["valid_license"] = licenser.licenseIsValid();
    j["status_code"] = static_cast<int>(licenser.getStatusCode());

    std::cout << j.dump(4) << std::endl;
}

void activateLicenseWithKey(const std::string& key)
{
    landr::Licenser licenser;
    licenser.activateWithKey(key);
    dumpLicenserInfo(licenser);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage:" << std::endl;
        std::cout << "  " << argv[0] << " <license key>                 - Activate with license key" << std::endl;
        std::cout << "  " << argv[0] << " -productname                  - Print product name" << std::endl;
        return 1;
    }

    const std::string& arg = argv[1];
    landr::Licenser licenser;

    if (arg == "-productname") {
        std::cout << licenser.getProductName() << std::endl;
        return 0;
    }

    activateLicenseWithKey(arg);
    return 0;
}
