#pragma once

class DirectXCore final {
private:
	DirectXCore();
	
public:
	~DirectXCore();

private:
	DirectXCore(const DirectXCore&) = delete;
	DirectXCore& operator=(const DirectXCore&) = delete;

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
		static DirectXCore::Debug& GetInstance();

	private:
	};
};