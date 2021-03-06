/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018-2019 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#pragma once

#include <memory>
#include "inputsystem/IInputSystem.hpp"

class CInputEventDispatcher;

class CInputSystem final : public IInputSystem
{
public:
	CInputSystem(IInputSystemImpl *apImpl);
	~CInputSystem();
	
	bool Init(bool abConsoleTextMode) override;
	void Shutdown() override;
	
	void Reset() override;
	
	void Poll() override;
	
	void AttachToWindow(void *apWindow) override;
	void DetachFromWindow() override;
	
	void AddEventListener(IInputEventListener *apListener) override;
	void RemoveEventListener(IInputEventListener *apListener) override;
	
	bool IsKeyDown(eInputKey aeKey) const override;
	
	int GetAnalogValue(AnalogCode aeCode) const override;
	
	int GetGamepadCount() const override;
	void SetGamepadActive(int anGamepad, bool abActive) override;
protected:
	std::unique_ptr<CInputEventDispatcher> mpEventDispatcher;
	
	std::array<bool, eInputKey::SizeOf> mvKeyStates{};
	std::array<bool, eInputKey::SizeOf> mvOldKeyStates{};
private:
	IInputSystemImpl &mImpl;
};