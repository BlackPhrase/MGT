/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2020 BlackPhrase
 *
 * OGSNext Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGSNext Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGSNext Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#pragma once

#include <memory>

#include "engine/IEngine.hpp"

class CHost;

interface IEngineClient;

class CEngine final : public IEngine
{
public:
	bool Init(const InitParams &apInitParams) override;
	void Shutdown() override;
	
	bool Frame() override;
private:
	IEngineClient *LoadClientModule();
	void UnloadClientModule();
private:
	std::unique_ptr<CHost> mpHost;
	
	void *mpClientModule{nullptr};
};