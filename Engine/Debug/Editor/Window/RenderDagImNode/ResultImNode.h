#pragma once

#include <ImNodeFlow/include/ImNodeFlow.h>

class ResultImNode final : public ImFlow::BaseNode {
public:
	ResultImNode();
	~ResultImNode() override = default;

public:
	void draw() override;

private:
};
