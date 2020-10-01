#pragma once
#include <cstdint>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>

namespace utils {
namespace detail {
template <typename Enum>
void initMaps(std::string_view values);

template <typename Enum>
struct helper {
	helper(std::string_view str);
};
} // namespace detail

#define STR_ENUM(name, ...)                                                                                                                \
	enum class name { __VA_ARGS__ };                                                                                                       \
	namespace utils::detail {                                                                                                              \
	inline helper<name> name(#__VA_ARGS__);                                                                                                \
	}

template <typename K, typename V, template <typename, typename, typename...> typename C, typename... Args>
std::optional<V> find(C<K, V, Args...> const& map, K const& key);

template <typename K, typename V, template <typename, typename, typename...> typename C, typename... Args>
V const& valueOr(C<K, V, Args...> const& map, K const& key, V const& fallback);

template <typename Enum>
std::string_view toStr(Enum e) noexcept;

template <typename Enum>
Enum toEnum(std::string const& str, Enum fallback) noexcept;

namespace detail {
template <typename Enum>
helper<Enum>::helper(std::string_view str) {
	initMaps<Enum>(str);
}

template <typename Enum>
struct EnumHasher {
	std::size_t operator()(Enum e) const {
		return std::hash<std::uint64_t>()(static_cast<std::uint64_t>(e));
	}
};

template <typename K, typename V, typename... Args>
using Map = std::unordered_map<K, V, Args...>;
template <typename Enum, typename V>
using EMap = Map<Enum, V, EnumHasher<Enum>>;

template <typename Enum>
struct Maps {
	EMap<Enum, std::string> toStr;
	Map<std::string, Enum> toEnum;
};

template <typename Enum>
Maps<Enum>& maps() {
	static Maps<Enum> maps;
	return maps;
}

inline std::string trim(std::string_view in) {
	std::size_t start = 0;
	std::size_t size = in.size();
	while (start < in.size() && std::isspace(in.at(start++))) {
		--size;
	}
	std::size_t end = in.empty() ? 0 : in.size() - 1;
	while (end > 0 && std::isspace(in.at(end--))) {
		--size;
	}
	return std::string(in.substr(start > 0 ? start - 1 : 0, size));
}

template <typename Enum>
EMap<Enum, std::string> mapStr(std::string_view values) {
	EMap<Enum, std::string> ret;
	std::stringstream str;
	str << values;
	std::size_t idx = 0;
	for (std::string value; std::getline(str, value, ','); ++idx) {
		ret.emplace((Enum)idx, trim(value));
		value.clear();
	}
	return ret;
}

template <typename Enum>
Map<std::string, Enum> strMap(EMap<Enum, std::string> const& map) {
	Map<std::string, Enum> ret;
	for (auto const& [e, s] : map) {
		ret.emplace(s, e);
	}
	return ret;
}

template <typename Enum>
void initMaps(std::string_view values) {
	auto& m = maps<Enum>();
	m.toStr = mapStr<Enum>(values);
	m.toEnum = strMap<Enum>(m.toStr);
}
} // namespace detail

template <typename K, typename V, template <typename, typename, typename...> typename C, typename... Args>
std::optional<V> find(C<K, V, Args...> const& map, K const& key) {
	if (auto search = map.find(key); search != map.end()) {
		return search->second;
	}
	return {};
}

template <typename K, typename V, template <typename, typename, typename...> typename C, typename... Args>
V const& valueOr(C<K, V, Args...> const& map, K const& key, V const& fallback) {
	if (auto search = map.find(key); search != map.end()) {
		return search->second;
	}
	return fallback;
}

template <typename Enum>
std::string_view toStr(Enum e) noexcept {
	return valueOr(detail::maps<Enum>().toStr, e, std::string());
}

template <typename Enum>
Enum toEnum(std::string const& str, Enum fallback) noexcept {
	return valueOr(detail::maps<Enum>().toEnum, str, fallback);
}
} // namespace utils
