#include <iomanip>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>
#include "Licenser.h"
#include "UpdateChecker.h"

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

void runUpdateChecker(const std::string& currentVersion)
{
    landr::UpdateChecker updateChecker(currentVersion);

    // Use a short interval so the first check fires immediately and the
    // thread wakes up quickly when we call stopChecking()
    updateChecker.startChecking(100);

    // Poll for a result rather than sleeping for a fixed duration
    constexpr int maxWaitMs = 10000;
    constexpr int pollMs = 100;
    for (int elapsed = 0; !updateChecker.hasNewVersion() && elapsed < maxWaitMs; elapsed += pollMs)
        std::this_thread::sleep_for(std::chrono::milliseconds(pollMs));

    updateChecker.stopChecking();

    nlohmann::json output;
    output["current_version"] = updateChecker.getCurrentVersion();

    if (updateChecker.hasNewVersion()) {
        const auto info = updateChecker.getUpdateInfo();
        if (info) {
            nlohmann::json updateInfo;
            updateInfo["version"] = info->version;
            updateInfo["download_link"] = info->downloadLink;
            updateInfo["release_notes_link"] = info->releaseNotesLink;
            updateInfo["force_update"] = info->forceUpdate;
            output["update_available"] = true;
            output["update_info"] = updateInfo;
        }
    } else {
        output["update_available"] = false;
    }

    std::cout << output.dump(4) << std::endl;
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
        std::cout << "  " << argv[0] << " -versioninfo [plugin version] - Check for updates (defaults to 1.0.0)" << std::endl;
        return 1;
    }

    const std::string& arg = argv[1];
    landr::Licenser licenser;

    if (arg == "-productname") {
        std::cout << licenser.getProductName() << std::endl;
        return 0;
    }

    if (arg == "-versioninfo") {
        const std::string currentVersion = (argc >= 3) ? argv[2] : "1.0.0";
        runUpdateChecker(currentVersion);
        return 0;
    }

    activateLicenseWithKey(arg);
    return 0;
}
