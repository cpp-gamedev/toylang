#pragma once
#include <cstdint>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utils/utils.hpp>
#include <vector>

///
/// \brief Declare a stringed enum (macro)
///
#define STR_ENUM(name, ...)                                                                                                                \
	enum class name { __VA_ARGS__ };                                                                                                       \
	[[maybe_unused]] inline ::tl::utils::detail::EnumStr<name> g_##name(#__VA_ARGS__);

namespace tl::utils {
///
/// \brief Obtain string representation of Enum e
///
template <typename Enum>
std::string_view toStr(Enum e) noexcept;
///
/// \brief Obtain Enum representation of string str
///
template <typename Enum>
Enum toEnum(std::string_view str, Enum fallback) noexcept;

// impl
namespace detail {
template <typename Enum>
void initLookup(std::string_view values);

template <typename Enum>
struct EnumStr {
	EnumStr(std::string_view str) {
		initLookup<Enum>(str);
	}
};

template <typename Enum>
struct EnumLookup {
	std::vector<std::string> toStr;
	std::unordered_map<std::string_view, Enum> toEnum;
};

template <typename Enum>
EnumLookup<Enum>& lookup() {
	// Pseudo singleton; one such function instantiated for every Enum,
	// thus one static EnumLookup for it as well.
	static EnumLookup<Enum> lookup;
	return lookup;
}

inline std::string trim(std::string_view in) {
	std::size_t start = 0;
	std::size_t size = in.size();
	// Keep incrementing start and decrementing size
	while (start < in.size() && std::isspace(in.at(start++))) {
		--size;
	}
	std::size_t end = in.empty() ? 0 : in.size() - 1;
	// Keep decrementing size and end
	while (end > 0 && std::isspace(in.at(end--))) {
		--size;
	}
	return std::string(in.substr(start > 0 ? start - 1 : 0, size));
}

template <typename Enum>
void initLookup(std::string_view values) {
	auto& l = lookup<Enum>();
	std::stringstream str;
	str << values;
	// First fill up the vector, let it resize etc
	for (std::string value; std::getline(str, value, ',');) {
		l.toStr.push_back(trim(value));
		value.clear();
	}
	// Then add views to its strings in the map
	std::size_t idx = 0;
	for (auto const& s : l.toStr) {
		// s is now stable and can be safely viewed
		l.toEnum.emplace(s, (Enum)idx);
	}
}
} // namespace detail

template <typename Enum>
std::string_view toStr(Enum e) noexcept {
	return detail::lookup<Enum>().toStr.at((std::size_t)e);
}

template <typename Enum>
Enum toEnum(std::string_view str, Enum fallback) noexcept {
	return valueOr(detail::lookup<Enum>().toEnum, str, fallback);
}
} // namespace tl::utils
