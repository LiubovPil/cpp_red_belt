#pragma once

#include "http_request.h"

#include <string_view>
#include <array>
#include <map>
using namespace std;

class Stats {
public:
	Stats() = default;

	void AddMethod(string_view method);
	void AddUri(string_view uri);
	const map<string_view, int>& GetMethodStats() const;
	const map<string_view, int>& GetUriStats() const;

private:
	inline static const array<string, 5> methods_name = { "GET" , "PUT" , "POST" , "DELETE" , "UNKNOWN" };
	inline static const array<string, 6> uri_name = { "/", "/order" , "/product" , "/basket" , "/help" , "unknown" };

	inline static const map<string_view, int> method_stats;
	inline static const map<string_view, int> uri_stats;
};

HttpRequest ParseRequest(string_view line);
