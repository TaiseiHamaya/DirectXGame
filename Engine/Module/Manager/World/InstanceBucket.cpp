#include "InstanceBucket.h"

void InstanceBucket::begin_frame() {
	camera.clear();
	staticMesh.clear();
	skinMesh.clear();
	directionalLightInstance.clear();
	pointLightInstance.clear();
	rect.clear();
}