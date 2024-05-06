#include "Descriptor.h"

Descriptor::Descriptor(Descriptor&& rhs) noexcept {
	resource = *rhs.resource.ReleaseAndGetAddressOf();
}

Descriptor& Descriptor::operator=(Descriptor&& rhs) noexcept {
	resource = *rhs.resource.ReleaseAndGetAddressOf();
	return *this;
}