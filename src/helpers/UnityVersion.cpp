#include <array>
#include <cstdint>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>

#include "helpers/UnityVersion.hpp"

UnityVersion::UnityVersion(const std::string &version) {
    if (version.empty())
        throw std::invalid_argument("Unity version cannot be empty.");

    try {
        std::regex digits(R"(\d+)");
        std::regex nondigits(R"(\D+)");
        std::sregex_iterator it(version.begin(), version.end(), digits);
        std::sregex_iterator end;

        std::vector<int> ver;
        while (it != end) {
            ver.push_back(std::stoi(it->str()));
            ++it;
        }
        if (ver.size() < 3)
            throw std::invalid_argument("Invalid Unity version format");

        Major = ver[0];
        Minor = ver[1];
        Patch = ver[2];
        Build = ver.size() >= 4 ? ver[3] : 0;
        FullVersion = version;

        std::sregex_iterator it2(version.begin(), version.end(), nondigits);
        std::vector<std::string> buildMatches;
        while (it2 != end) {
            buildMatches.push_back(it2->str());
            ++it2;
        }
        if (buildMatches.size() > 2) {
            BuildType = buildMatches[2];
        }
    } catch (...) {
        throw std::invalid_argument("Failed to parse Unity version: \"" + version + "\"");
    }
}

UnityVersion::UnityVersion(int major, int minor, int patch)
    : Major(major), Minor(minor), Patch(patch) {
    FullVersion = std::to_string(Major) + "." + std::to_string(Minor) + "." + std::to_string(Patch);
    if (!IsStripped()) {
        Build = 1;
        BuildType = BuildTypes::Final;
        FullVersion += BuildType + std::to_string(Build);
    }
}

bool UnityVersion::IsStripped() const {
    return Major == 0 && Minor == 0 && Patch == 0;
}
bool UnityVersion::IsAlpha() const {
    return BuildType == BuildTypes::Alpha;
}
bool UnityVersion::IsBeta() const {
    return BuildType == BuildTypes::Beta;
}
bool UnityVersion::IsPatch() const {
    return BuildType == BuildTypes::Patch;
}
bool UnityVersion::IsTuanjie() const {
    return BuildType == BuildTypes::Tuanjie && *this >= std::make_tuple(2022, 3, 2);
}

bool UnityVersion::TryParse(const std::string &versionStr, UnityVersion &version) {
    try {
        version = UnityVersion(versionStr);
        return true;
    } catch (...) {
        return false;
    }
}

int UnityVersion::CompareTo(const UnityVersion &other) const {
    return CompareTo(std::make_tuple(other.Major, other.Minor, other.Patch));
}
int UnityVersion::CompareTo(const std::tuple<int, int> &tuple) const {
    if (Major != std::get<0>(tuple))
        return Major - std::get<0>(tuple);
    if (Minor != std::get<1>(tuple))
        return Minor - std::get<1>(tuple);
    return 0;
}
int UnityVersion::CompareTo(const std::tuple<int, int, int> &tuple) const {
    int res = CompareTo(std::make_tuple(std::get<0>(tuple), std::get<1>(tuple)));
    if (res != 0)
        return res;
    return Patch - std::get<2>(tuple);
}

bool UnityVersion::operator==(const UnityVersion &other) const {
    return CompareTo(other) == 0;
}
bool UnityVersion::operator!=(const UnityVersion &other) const {
    return !(*this == other);
}
bool UnityVersion::operator<(const UnityVersion &other) const {
    return CompareTo(other) < 0;
}
bool UnityVersion::operator<=(const UnityVersion &other) const {
    return CompareTo(other) <= 0;
}
bool UnityVersion::operator>(const UnityVersion &other) const {
    return CompareTo(other) > 0;
}
bool UnityVersion::operator>=(const UnityVersion &other) const {
    return CompareTo(other) >= 0;
}

bool UnityVersion::operator==(int major) const {
    return Major == major;
}
bool UnityVersion::operator!=(int major) const {
    return Major != major;
}
bool UnityVersion::operator<(int major) const {
    return Major < major;
}
bool UnityVersion::operator<=(int major) const {
    return Major <= major;
}
bool UnityVersion::operator>(int major) const {
    return Major > major;
}
bool UnityVersion::operator>=(int major) const {
    return Major >= major;
}

bool UnityVersion::operator==(const std::tuple<int, int> &t) const {
    return std::tie(Major, Minor) == t;
}
bool UnityVersion::operator!=(const std::tuple<int, int> &t) const {
    return std::tie(Major, Minor) != t;
}
bool UnityVersion::operator<(const std::tuple<int, int> &t) const {
    return CompareTo(t) < 0;
}
bool UnityVersion::operator<=(const std::tuple<int, int> &t) const {
    return CompareTo(t) <= 0;
}
bool UnityVersion::operator>(const std::tuple<int, int> &t) const {
    return CompareTo(t) > 0;
}
bool UnityVersion::operator>=(const std::tuple<int, int> &t) const {
    return CompareTo(t) >= 0;
}

bool UnityVersion::operator==(const std::tuple<int, int, int> &t) const {
    return std::tie(Major, Minor, Patch) == t;
}
bool UnityVersion::operator!=(const std::tuple<int, int, int> &t) const {
    return std::tie(Major, Minor, Patch) != t;
}
bool UnityVersion::operator<(const std::tuple<int, int, int> &t) const {
    return CompareTo(t) < 0;
}
bool UnityVersion::operator<=(const std::tuple<int, int, int> &t) const {
    return CompareTo(t) <= 0;
}
bool UnityVersion::operator>(const std::tuple<int, int, int> &t) const {
    return CompareTo(t) > 0;
}
bool UnityVersion::operator>=(const std::tuple<int, int, int> &t) const {
    return CompareTo(t) >= 0;
}

std::tuple<int, int, int> UnityVersion::ToTuple() const {
    return {Major, Minor, Patch};
}
std::array<int, 3> UnityVersion::ToArray() const {
    return {Major, Minor, Patch};
}

std::string UnityVersion::ToString() const {
    return FullVersion;
}
