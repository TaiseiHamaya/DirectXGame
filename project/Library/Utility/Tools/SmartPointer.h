#pragma once

#include <memory>

namespace eps {

template<class ConstructT, class... Args>
concept ConstructibleFrom = requires(Args&&... args) {
	ConstructT(std::forward<Args>(args)...); // ArgsでTが初期化可能
};

template<class BaseType, class ...Args>
	requires ConstructibleFrom<BaseType, Args...>
std::unique_ptr<BaseType> CreateUnique(Args&& ...args) {
	return std::make_unique<BaseType>(args...);
}

template<class BaseType, class ...Args>
	requires ConstructibleFrom<BaseType, Args...>
std::shared_ptr<BaseType> CreateShared(Args&& ...args) {
	return std::make_shared<BaseType>(args...);
}

}