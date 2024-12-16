#include <iomanip>
#include <iostream>
#include <sstream>
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

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <license key>" << std::endl;
        return 1;
    }
    
    const std::string& licenseKey = argv[1];
    
    landr::Licenser licenser;
    licenser.resetActivation();
    
    licenser.activateWithKey(licenseKey);
    const auto statusInfo = licenser.getStatusInfo();
    
    constexpr int labelWidth = 16;
    std::ostringstream output;
    output << "\nLicense Status:\n"
        << "-----------------\n"
        << std::setw(labelWidth) << " Title: " << statusInfo.titleText << "\n"
        << std::setw(labelWidth) << " Message: " << wrapText(statusInfo.message, labelWidth) << "\n"
        << std::setw(labelWidth) << " Link Text: " << wrapText(statusInfo.linkText, labelWidth) << "\n"
        << std::setw(labelWidth) << " Link URL: " << statusInfo.linkURL << "\n"
        << std::setw(labelWidth) << " Error Subcode: " << statusInfo.errorSubCode << "\n";
    
    std::cout << output.str();

    return 0;
}
