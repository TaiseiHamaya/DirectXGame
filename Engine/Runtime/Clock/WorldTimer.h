#pragma once

class WorldTimer final {
public:
	void ahead();
	void back();
	void set(r32 time);
	r32 time() const;

	bool just_crossed(r32 time) const noexcept;

public:
	operator r32() const noexcept { return timer; };

private:
	r32 timer{ 0 };
};
