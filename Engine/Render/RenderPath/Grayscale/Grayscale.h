#pragma once

#include <memory>
#include <functional>

class RenderingParameters;
class PSOBuilder;

class Grayscale {
public:
	Grayscale() = default;
	~Grayscale() = default;

	Grayscale(const Grayscale&) = delete;
	Grayscale& operator=(const Grayscale&) = delete;
	Grayscale(Grayscale&&) = default;
	Grayscale& operator=(Grayscale&&) = default;

public:
	void initialize();

private:
	void create_builder(std::unique_ptr<PSOBuilder>& builder);

private:
	std::shared_ptr<RenderingParameters> parameter;
	std::function<void(std::uint32_t)> drawFunc;
};
