/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017 Statoil ASA
//
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
//
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once

class RicfCommandObject;
class QTextStream;

#include <vector>

namespace caf
{
class PdmObjectFactory;
class PdmScriptIOMessages;
} // namespace caf

//==================================================================================================
//
//
//
//==================================================================================================
class RicfCommandFileReader
{
public:
    static std::vector<RicfCommandObject*>
        readCommands( QTextStream& inputStream, caf::PdmObjectFactory* objectFactory, caf::PdmScriptIOMessages* errorMessageContainer );

    static void writeCommands( QTextStream& outputStream, const std::vector<RicfCommandObject*>& commandsToWrite );
};
