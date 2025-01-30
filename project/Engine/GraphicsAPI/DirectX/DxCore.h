#pragma once

class DxCore final {
private:
	DxCore();

public:
	~DxCore();

private:
	DxCore(const DxCore&) = delete;
	DxCore& operator=(const DxCore&) = delete;

public:
	static void Initialize();
	static void BeginFrame();
	static void EndFrame();
	static void Finalize();

private:
	class Debug final {
	private:
		Debug() = default;

	public:
		~Debug();

	private:
		Debug(const Debug&) = delete;
		Debug& operator=(const Debug&) = delete;

	public:
		static void Initialize();
		static void InfoQueue();

	private:
		static DxCore::Debug& GetInstance();

	private:
	};
};