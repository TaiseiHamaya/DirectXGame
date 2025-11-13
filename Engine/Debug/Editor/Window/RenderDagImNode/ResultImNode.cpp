#include "ResultImNode.h"

ResultImNode::ResultImNode() {
	setTitle("ScreenOut");
	setStyle(ImFlow::NodeStyle::brown());
	ImFlow::BaseNode::addIN<u64>("DisplayOut", 0, ImFlow::ConnectionFilter::SameType());
}

void ResultImNode::draw() {
}
