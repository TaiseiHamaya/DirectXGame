#include "ResultImNode.h"

using namespace szg;

#ifdef DEBUG_FEATURES_ENABLE

ResultImNode::ResultImNode() {
	setTitle("ScreenOut");
	setStyle(ImFlow::NodeStyle::brown());
	ImFlow::BaseNode::addIN<u64>("DisplayOut", 0, ImFlow::ConnectionFilter::SameType());
}

void ResultImNode::draw() {
}

#endif DEBUG_FEATURES_ENABLE
