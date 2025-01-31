/****************************************************************************/
//    Copyright (C) 2009 Aali132                                            //
//    Copyright (C) 2018 quantumpencil                                      //
//    Copyright (C) 2018 Maxime Bacoux                                      //
//    Copyright (C) 2020 myst6re                                            //
//    Copyright (C) 2020 Chris Rizzitello                                   //
//    Copyright (C) 2020 John Pritchard                                     //
//    Copyright (C) 2024 Julian Xhokaxhiu                                   //
//                                                                          //
//    This file is part of FFNx                                             //
//                                                                          //
//    FFNx is free software: you can redistribute it and/or modify          //
//    it under the terms of the GNU General Public License as published by  //
//    the Free Software Foundation, either version 3 of the License         //
//                                                                          //
//    FFNx is distributed in the hope that it will be useful,               //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of        //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         //
//    GNU General Public License for more details.                          //
/****************************************************************************/

/*
	HEXT Specification by DLPB

	See https://forums.qhimm.com/index.php?topic=13574.0

	This implementation may differ from the original specification as new required functionalities will be implemented,
	in order to ease modders life.
*/

#pragma once

#include <string>
#include <vector>

class Hext {
private:
	int inGlobalOffset = 0;
	bool isMultilineComment = false;

	int GetAddress(const std::string& token) const;
    static std::vector<char> GetBytes(const std::string& token);

    static bool HasCheckpoint(const std::string& token);
    static bool ParseCheckpoint(const std::string& token, const std::string& checkpoint);
    static bool ParseCommands(std::string token);
	bool ParseComment(const std::string& token);
	bool ParseGlobalOffset(const std::string& token);
	bool ParseMemoryPermission(const std::string& token) const;
	bool ParseMemoryPatch(const std::string& token) const;

public:
	void Apply(const std::string& filename);
	void ApplyDelayed(const std::string& filename, const std::string& checkpoint);
	void ApplyAll(const std::string& checkpoint = std::string());
};

extern Hext hextPatcher;