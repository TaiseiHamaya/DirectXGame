#include "Definition.h"

#include <cassert>

Animation::Animation() {
	distance = 1;
	total_seq = 1;
	is_loop = false;
}

Animation::Animation(int distance_, int total_seq_, bool is_loop_) {
	assert(distance_ >= 1);
	assert(total_seq_ >= 1);
	distance = distance_;
	total_seq = total_seq_;
	is_loop = is_loop_;
}
