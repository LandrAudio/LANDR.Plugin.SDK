#pragma once

#include <string>

/// LANDR data structures
namespace landr {

/// License status codes
/// @see Licenser::getStatusCode()
enum class StatusCode {
    Active, ///< The license is valid and active
    Offline, ///< A network-related error occurred
    Invalid, ///< The license is invalid e.g. wrong key entered
    Limited, ///< License received too many activations
    Deactivated, ///< License was previously active, but has been revoked, suspended, or expired
    Inactive, ///< License has not yet been activated
    Missing, ///< License is missing
    Closed, ///< The license is permanently valid and no longer requires activation, e.g. lease paid
    Other ///< An unhandled error has occurred, see StatusInfo::errorSubCode for details
};

/// License status information
/// @see Licenser::getStatusInfo()
struct StatusInfo {
    std::string message; ///< A descriptive message corresponding to the license status
    std::string titleText; ///< An optional title for the message
    std::string linkText; ///< Text for a link that may be used in relation to the status
    std::string linkURL; ///< URL for a link that may be used in relation to the status
    std::string errorSubCode; ///< A subcode corresponding to an error or status from the license provider
};
}
