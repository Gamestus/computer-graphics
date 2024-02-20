#pragma once
#include <bitset>

class InputDevice {

public:
	void AddPressedKey(unsigned char keycode);
	void RemovePressedKey(unsigned char keycode);
	bool IsKeyDown(unsigned char keycode) const noexcept;
private:
	static constexpr unsigned int nKeys = 256u;
	std::bitset<nKeys> keys;

	void OnKeyDown(unsigned char keycode);
	void OnKeyUp(unsigned char keycode);
	void OnMouseMove();
};