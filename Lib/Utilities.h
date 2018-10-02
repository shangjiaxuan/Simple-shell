#pragma once

template<typename type>
static constexpr size_t find_MAX() {
	size_t start = 0x01;
	for (int i = 0; i < sizeof(type); i++) {
		start <<= 8;
	}
	return start;
}

