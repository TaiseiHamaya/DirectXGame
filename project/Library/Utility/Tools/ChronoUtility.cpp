#include "ChronoUtility.h"

namespace chrono = std::chrono;

ChronoUtility::LocalTimeSeconds ChronoUtility::NowLocalSecond() {
	static const chrono::time_zone* timezone{ chrono::current_zone() };
	chrono::zoned_time nowZT{ timezone, chrono::system_clock::now() };
	LocalTimeSeconds nowZtFloor
		= chrono::floor<chrono::seconds>(nowZT.get_local_time());
	return nowZtFloor;
}
