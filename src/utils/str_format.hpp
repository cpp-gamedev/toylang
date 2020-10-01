#pragma once
#include <iostream>
#include <sstream>
#include <string_view>

namespace tl {
inline constexpr std::string_view fmt_token = "{}";

inline std::ostream& format(std::ostream& out, std::string_view fmt) {
	out << fmt;
	return out;
}

template <typename Arg, typename... Args>
std::ostream& format(std::ostream& out, std::string_view fmt, Arg&& arg, Args&&... args) {
	if (auto search = fmt.find(fmt_token); search != std::string::npos) {
		std::string_view text(fmt.data(), search);
		out << text << std::forward<Arg>(arg);
		return format(out, fmt.substr(search + fmt_token.size()), std::forward<Args>(args)...);
	}
	return format(out, fmt);
}

template <typename... Args>
std::string format(std::string_view fmt, Args&&... args) {
	std::stringstream str;
	format(str, fmt, std::forward<Args>(args)...);
	return str.str();
}

template <typename... Args>
void print(std::string_view fmt, Args&&... args) {
	std::cout << format(fmt, std::forward<Args>(args)...);
}
} // namespace tl
