#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <ImNodeFlow/include/ImNodeFlow.h>

class IRenderDagImNode : public ImFlow::BaseNode {
public:
	virtual ~IRenderDagImNode() override = default;

public:
	void set_node_id(u64 nodeId_);
	u64 node_id() const;

protected:
	u64 nodeId;
};

#endif // DEBUG_FEATURES_ENABLE
