#pragma once

class WorldTimer final {
public:
	void ahead();
	void back();
	void set(float time);
	float time() const;

public:
	operator float() const noexcept { return timer; };

private:
	float timer{ 0 };
};
