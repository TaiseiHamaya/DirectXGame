#include "string_hashed.h"

#ifdef DEBUG_FEATURES_ENABLE

eps::string_hashed::string_hashed(std::string_view sv) {
	str = sv;
	val = eps::string_hash(sv);
}

eps::string_hashed& eps::string_hashed::operator=(std::string_view sv) {
	str = sv;
	val = eps::string_hash(sv);
	return *this;
}

eps::string_hashed::string_hashed(string_literal sv) {
	str = sv;
	val = eps::string_hash(sv);
}

eps::string_hashed& eps::string_hashed::operator=(string_literal sv) {
	str = sv;
	val = eps::string_hash(sv);
	return *this;
}

u64 eps::string_hashed::value() const {
	return val;
}

std::string_view eps::string_hashed::view_debug() const {
	return str;
}

bool eps::string_hashed::operator==(const string_hashed& rhs) const {
	return val == rhs.val;
}

std::strong_ordering eps::string_hashed::operator<=>(const string_hashed& rhs) const {
	return val <=> rhs.val;
}

#endif //DEBUG_FEATURES_ENABLE
