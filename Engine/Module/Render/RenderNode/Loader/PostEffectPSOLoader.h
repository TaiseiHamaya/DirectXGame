#pragma once

#include <memory>

#include <json.hpp>

#include <Library/Utility/Tools/ConstructorMacro.h>
#include <Library/Utility/Template/Reference.h>

class PostEffectPSO;

class PostEffectPSOLoader final {
public:
	PostEffectPSOLoader() = default;
	~PostEffectPSOLoader() = default;

	__CLASS_NON_COPYABLE(PostEffectPSOLoader)

public:
	std::unique_ptr<PostEffectPSO> entry_point(const nlohmann::json& psoJson);

private:
};
