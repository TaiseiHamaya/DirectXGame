#ifdef DEBUG_FEATURES_ENABLE

#include "IRenderDagImNode.h"

void IRenderDagImNode::set_node_id(u64 nodeId_) {
	nodeId = nodeId_;
}

u64 IRenderDagImNode::node_id() const {
	return nodeId;
}

#endif // DEBUG_FEATURES_ENABLE
