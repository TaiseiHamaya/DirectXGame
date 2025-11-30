#pragma once

#include "IRenderDagImNode.h"

class ResultImNode final : public IRenderDagImNode {
public:
	ResultImNode();
	~ResultImNode() override = default;

public:
	void draw() override;

private:
};
