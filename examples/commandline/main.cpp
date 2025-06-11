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

void dumpInfo(const landr::StatusInfo& statusInfo) {
    const int labelWidth = 16;
    std::ostringstream output;
    output << "\nLicense Status:\n"
        << "-----------------\n"
        << std::setw(labelWidth) << " Title: " << statusInfo.titleText << "\n"
        << std::setw(labelWidth) << " Message: " << wrapText(statusInfo.message, labelWidth) << "\n"
        << std::setw(labelWidth) << " Link Text: " << wrapText(statusInfo.linkText, labelWidth) << "\n"
        << std::setw(labelWidth) << " Link URL: " << statusInfo.linkURL << "\n"
        << std::setw(labelWidth) << " Error Subcode: " << statusInfo.errorSubCode << "\n";

    std::cout << output.str();
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

    dumpInfo(licenser.getStatusInfo());

    return 0;
}
