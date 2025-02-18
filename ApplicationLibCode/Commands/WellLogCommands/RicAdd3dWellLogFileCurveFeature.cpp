/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018-     Equinor ASA
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

#include "RicAdd3dWellLogFileCurveFeature.h"

#include "RicWellLogTools.h"

#include "Rim3dWellLogFileCurve.h"
#include "RimProject.h"
#include "RimWellPath.h"

#include "Riu3DMainWindowTools.h"

#include <QAction>

CAF_CMD_SOURCE_INIT( RicAdd3dWellLogFileCurveFeature, "RicAdd3dWellLogFileCurveFeature" );

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RicAdd3dWellLogFileCurveFeature::isCommandEnabled() const
{
    std::vector<RimCase*> cases = RimProject::current()->allGridCases();
    if ( cases.empty() ) return false;

    return ( RicWellLogTools::findWellPathWithLogFileFromSelection() != nullptr );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicAdd3dWellLogFileCurveFeature::onActionTriggered( bool isChecked )
{
    RimWellPath* selectedWellPath = RicWellLogTools::findWellPathWithLogFileFromSelection();
    if ( !selectedWellPath ) return;

    Rim3dWellLogFileCurve* rim3dWellLogFileCurve = new Rim3dWellLogFileCurve();
    selectedWellPath->add3dWellLogCurve( rim3dWellLogFileCurve );

    rim3dWellLogFileCurve->setDefaultFileCurveDataInfo();

    RimProject::current()->scheduleCreateDisplayModelAndRedrawAllViews();

    Riu3DMainWindowTools::selectAsCurrentItem( rim3dWellLogFileCurve );
    Riu3DMainWindowTools::setExpanded( selectedWellPath );

    selectedWellPath->updateConnectedEditors();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicAdd3dWellLogFileCurveFeature::setupActionLook( QAction* actionToSetup )
{
    actionToSetup->setIcon( QIcon( ":/WellLogCurve16x16.png" ) );
    actionToSetup->setText( "Create 3D Well Log LAS Curve" );
}
