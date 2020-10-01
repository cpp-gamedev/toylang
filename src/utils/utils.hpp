#pragma once
#include <optional>

namespace tl::utils {
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
} // namespace tl::utils
