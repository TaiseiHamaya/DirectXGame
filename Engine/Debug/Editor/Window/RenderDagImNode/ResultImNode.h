#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IRenderDagImNode.h"

class ResultImNode final : public IRenderDagImNode {
public:
	ResultImNode();
	~ResultImNode() override = default;

public:
	void draw() override;

private:
};

#endif // DEBUG_FEATURES_ENABLE
