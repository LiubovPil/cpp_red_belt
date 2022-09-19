#include "Stats.h"
#include <cctype>

void StatUnit::Add(string_view value) {
	if (auto it = counts.find(value); it != counts.end()) {
		++it->second;
	}
	else {
		++counts[default_key];
	}
}
void Stats::AddMethod(string_view method){
	method_stats.Add(method);
}
void Stats::AddUri(string_view uri) {
	uri_stats.Add(uri);
}
const map<string_view, int>& Stats::GetMethodStats() const {
	return method_stats.GetValues();
}
const map<string_view, int>& Stats::GetUriStats() const {
	return uri_stats.GetValues();
}

void RemoveSpace(string_view& sv) {
	while (!sv.empty() && isspace(sv[0])) {
		sv.remove_prefix(1);
	}
}
string_view ReadToken(string_view& sv) {
	RemoveSpace(sv);
	auto pos = sv.find(' ');
	auto result = sv.substr(0, pos);
	sv.remove_prefix(pos != sv.npos ? pos : sv.size());
	return result;
}
HttpRequest ParseRequest(string_view line) {
	auto method = ReadToken(line);
	auto uri = ReadToken(line);
	return { method, uri, ReadToken(line) };
}