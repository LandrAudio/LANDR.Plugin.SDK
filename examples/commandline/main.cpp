#include <iomanip>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "Licenser.h"

std::string wrapText(const std::string& text, int labelWidth) {
    std::ostringstream wrappedText;
    std::istringstream lines(text);
    std::string line;
    std::string indentation(labelWidth, ' ');

    if (std::getline(lines, line))
        wrappedText << line;

    while(std::getline(lines, line))
        wrappedText << "\n" << indentation << line;

    return wrappedText.str();
}

void dumpInfo(const landr::Licenser& licenser) {
    
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

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <license key | -productname>" << std::endl;
        return 1;
    }

    const std::string& arg = argv[1];
    landr::Licenser licenser;

    if (arg == "-productname")
    {
        std::cout << licenser.getProductName() << std::endl;
        return 0;
    }

    licenser.resetActivation();
    licenser.activateWithKey(arg);

    dumpInfo(licenser);

    return 0;
}
