#include "InputDevice.h"
#include <iostream>
#include "Game.h"

bool InputDevice::IsKeyDown(unsigned char keycode) const noexcept {
	return keys[keycode];
}

void InputDevice::AddPressedKey(unsigned char keycode) {
	if (keys[keycode] == false) {
		keys[keycode] = true;
		OnKeyDown(keycode);
	}
}

void InputDevice::RemovePressedKey(unsigned char keycode) {
	keys[keycode] = false;
	OnKeyUp(keycode);
}

void InputDevice::OnKeyDown(unsigned char keycode) {
	wchar_t buffer[256];
	wsprintf(buffer, L"Key is down: %d\n", static_cast<unsigned int>(keycode));
	OutputDebugStringW(buffer); 
}

void InputDevice::OnKeyUp(unsigned char keycode) {
	wchar_t buffer[256];
	wsprintf(buffer, L"Key is up: %d\n", static_cast<unsigned int>(keycode));
	OutputDebugStringW(buffer);
}

void InputDevice::MouseRightClick(POINTS pt) {
	wchar_t buffer[256];
	wsprintf(buffer, L"Right mouse at: (%d, %d)\n", pt.x, pt.y);
	OutputDebugStringW(buffer);
}

void InputDevice::MouseLeftClick(POINTS pt) {
	wchar_t buffer[256];
	wsprintf(buffer, L"Left mouse at: (%d, %d)\n", pt.x, pt.y);
	OutputDebugStringW(buffer);
}

void InputDevice::MouseMove(POINTS pt) {
	MousePosition = pt;
}