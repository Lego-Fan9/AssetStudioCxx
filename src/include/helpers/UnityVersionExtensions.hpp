#ifndef UNITY_VERSION_EXTENSIONS_H
#define UNITY_VERSION_EXTENSIONS_H

#include <tuple>

#include "helpers/UnityVersion.hpp"

namespace UnityVersionExtensions {

inline bool IsInRange(const UnityVersion& ver, const UnityVersion& lower, const UnityVersion& upper) {
    return ver >= lower && ver < upper;
}

inline bool IsInRange(const UnityVersion& ver, int lower, const UnityVersion& upper) {
    return ver.Major >= lower && ver < upper;
}

inline bool IsInRange(const UnityVersion& ver, const std::tuple<int,int>& lower, const UnityVersion& upper) {
    return ver >= lower && ver < upper;
}

inline bool IsInRange(const UnityVersion& ver, const std::tuple<int,int,int>& lower, const UnityVersion& upper) {
    return ver >= lower && ver < upper;
}

inline bool IsInRange(const UnityVersion& ver, const UnityVersion& lower, int upper) {
    return ver >= lower && ver.Major < upper;
}

inline bool IsInRange(const UnityVersion& ver, const UnityVersion& lower, const std::tuple<int,int>& upper) {
    return ver >= lower && ver < upper;
}

inline bool IsInRange(const UnityVersion& ver, const UnityVersion& lower, const std::tuple<int,int,int>& upper) {
    return ver >= lower && ver < upper;
}

inline bool IsInRange(const UnityVersion& ver, int lower, int upper) {
    return ver.Major >= lower && ver.Major < upper;
}

inline bool IsInRange(const UnityVersion& ver, int lower, const std::tuple<int,int>& upper) {
    return ver.Major >= lower && ver < upper;
}

inline bool IsInRange(const UnityVersion& ver, int lower, const std::tuple<int,int,int>& upper) {
    return ver.Major >= lower && ver < upper;
}

inline bool IsInRange(const UnityVersion& ver, const std::tuple<int,int>& lower, int upper) {
    return ver >= lower && ver.Major < upper;
}

inline bool IsInRange(const UnityVersion& ver, const std::tuple<int,int,int>& lower, int upper) {
    return ver >= lower && ver.Major < upper;
}

inline bool IsInRange(const UnityVersion& ver, const std::tuple<int,int>& lower, const std::tuple<int,int>& upper) {
    return ver >= lower && ver < upper;
}

inline bool IsInRange(const UnityVersion& ver, const std::tuple<int,int>& lower, const std::tuple<int,int,int>& upper) {
    return ver >= lower && ver < upper;
}

inline bool IsInRange(const UnityVersion& ver, const std::tuple<int,int,int>& lower, const std::tuple<int,int>& upper) {
    return ver >= lower && ver < upper;
}

inline bool IsInRange(const UnityVersion& ver, const std::tuple<int,int,int>& lower, const std::tuple<int,int,int>& upper) {
    return ver >= lower && ver < upper;
}

} // namespace UnityVersionExtensions

#endif