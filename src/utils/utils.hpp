#pragma once
#include <iostream>
#include <optional>
#include <str_format/str_format.hpp>

namespace tl {
///
/// \brief fmt-like printf (only {} substitution, no float formatting etc)
///
template <typename... Args>
void print(std::string_view fmt, Args&&... args);

namespace utils {
///
/// \brief Obtain the value corresponding to key in map (if present)
/// \param map associative container of K keys and V values
/// \param key key to search
///
template <typename K, typename V, template <typename, typename, typename...> typename C, typename... Args>
std::optional<V> find(C<K, V, Args...> const& map, K const& key);
///
/// \brief Obtain value corresponding to key in map if present, else fallback
/// \param map associative container of K keys and V values
/// \param key key to search
/// \param fallback default return value
///
template <typename K, typename V, template <typename, typename, typename...> typename C, typename... Args>
V const& valueOr(C<K, V, Args...> const& map, K const& key, V const& fallback);

// impl
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
} // namespace utils

template <typename... Args>
void print(std::string_view fmt, Args&&... args) {
	std::cout << kt::format_str(fmt, std::forward<Args>(args)...);
}
} // namespace tl
