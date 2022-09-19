#pragma once

#include "http_request.h"

#include <string_view>
#include <array>
#include <map>
using namespace std;

class StatUnit {
public:
	template <typename Container>
	StatUnit(const Container& known_keys, string_view default_key)
		: default_key(default_key)
	{
		counts[default_key] = 0;
		for (string_view key : known_keys) {
			counts[key] = 0;
		}
	}

	void Add(string_view value);

	const map<string_view, int>& GetValues() const {
		return counts;
	}
private:
	const string_view default_key;
	map<string_view, int> counts;
};

class Stats {
public:
	Stats() = default;

	void AddMethod(string_view method);
	void AddUri(string_view uri);
	const map<string_view, int>& GetMethodStats() const;
	const map<string_view, int>& GetUriStats() const;

private:
	inline static const array<string, 4> methods_name = { "GET" , "PUT" , "POST" , "DELETE" };
	inline static const string default_method_name = "UNKNOWN";

	inline static const array<string, 5> uri_name = { "/", "/order" , "/product" , "/basket" , "/help"};
	inline static const string default_uri_name = "unknown";

	StatUnit method_stats{ methods_name, default_method_name };
	StatUnit uri_stats{ uri_name, default_uri_name };
};

HttpRequest ParseRequest(string_view line);
