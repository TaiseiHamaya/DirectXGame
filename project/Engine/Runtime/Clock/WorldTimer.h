#pragma once

class WorldTimer final {
public:
	void ahead();
	void back();
	void set(float time);
	float time() const;

private:
	float timer{ 0 };
};
