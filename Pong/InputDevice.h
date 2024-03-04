#pragma once
#include <Windows.h>
#include <bitset>

class InputDevice {

public:
	POINTS MousePosition;

	void AddPressedKey(unsigned char keycode);
	void RemovePressedKey(unsigned char keycode);
	void MouseRightClick(POINTS pt);
	void MouseLeftClick(POINTS pt);
	bool IsKeyDown(unsigned char keycode) const noexcept;
	void MouseMove(POINTS pt);
private:
	static constexpr unsigned int nKeys = 256u;
	std::bitset<nKeys> keys;

	void OnKeyDown(unsigned char keycode);
	void OnKeyUp(unsigned char keycode);
};