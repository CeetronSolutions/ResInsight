/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2020-     Equinor ASA
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

#include "RimAbstractPlotCollection.h"

#include "cafPdmChildArrayField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"

class RimGridStatisticsPlot;

//==================================================================================================
///
///
//==================================================================================================
class RimGridStatisticsPlotCollection : public caf::PdmObject, public RimPlotCollection
{
    CAF_PDM_HEADER_INIT;

public:
    RimGridStatisticsPlotCollection();

    void addGridStatisticsPlot( RimGridStatisticsPlot* newPlot );

    std::vector<RimGridStatisticsPlot*> gridStatisticsPlots() const;

    void loadDataAndUpdateAllPlots() override;

    void deleteAllPlots() override;

    size_t plotCount() const override;

private:
    caf::PdmChildArrayField<RimGridStatisticsPlot*> m_gridStatisticsPlots;
};
