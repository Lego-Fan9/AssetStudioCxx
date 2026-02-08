#ifndef UNITY_VERSION_HPP
#define UNITY_VERSION_HPP

#include <cstdint>
#include <ostream>
#include <string>
#include <tuple>

struct BuildTypes {
    inline static const std::string Alpha = "a";
    inline static const std::string Beta = "b";
    inline static const std::string Final = "f";
    inline static const std::string Patch = "p";
    inline static const std::string Tuanjie = "t";
};

class UnityVersion {
public:
    int Major{0};
    int Minor{0};
    int Patch{0};
    int Build{0};
    std::string BuildType;
    std::string FullVersion;

    UnityVersion() = default;
    explicit UnityVersion(const std::string &version);
    UnityVersion(int major, int minor = 0, int patch = 0);

    bool IsStripped() const;
    bool IsAlpha() const;
    bool IsBeta() const;
    bool IsPatch() const;
    bool IsTuanjie() const;

    static bool TryParse(const std::string &versionStr, UnityVersion &version);

    bool operator==(const UnityVersion &other) const;
    bool operator!=(const UnityVersion &other) const;
    bool operator<(const UnityVersion &other) const;
    bool operator<=(const UnityVersion &other) const;
    bool operator>(const UnityVersion &other) const;
    bool operator>=(const UnityVersion &other) const;

    bool operator==(int major) const;
    bool operator!=(int major) const;
    bool operator<(int major) const;
    bool operator<=(int major) const;
    bool operator>(int major) const;
    bool operator>=(int major) const;

    bool operator==(const std::tuple<int, int> &tuple) const;
    bool operator!=(const std::tuple<int, int> &tuple) const;
    bool operator<(const std::tuple<int, int> &tuple) const;
    bool operator<=(const std::tuple<int, int> &tuple) const;
    bool operator>(const std::tuple<int, int> &tuple) const;
    bool operator>=(const std::tuple<int, int> &tuple) const;

    bool operator==(const std::tuple<int, int, int> &tuple) const;
    bool operator!=(const std::tuple<int, int, int> &tuple) const;
    bool operator<(const std::tuple<int, int, int> &tuple) const;
    bool operator<=(const std::tuple<int, int, int> &tuple) const;
    bool operator>(const std::tuple<int, int, int> &tuple) const;
    bool operator>=(const std::tuple<int, int, int> &tuple) const;

    std::tuple<int, int, int> ToTuple() const;
    std::array<int, 3> ToArray() const;

    std::string ToString() const;

private:
    int CompareTo(const UnityVersion &other) const;
    int CompareTo(const std::tuple<int, int> &tuple) const;
    int CompareTo(const std::tuple<int, int, int> &tuple) const;
};

#endif
