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

#include "RimPolylinesAnnotationInView.h"
#include "RimAnnotationCollectionBase.h"
#include "RimAnnotationGroupCollection.h"
#include "RimAnnotationInViewCollection.h"
#include "RimPolylinesAnnotation.h"

#include "RigPolyLinesData.h"

CAF_PDM_SOURCE_INIT( RimPolylinesAnnotationInView, "RimPolylinesAnnotationInView" );

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimPolylinesAnnotationInView::RimPolylinesAnnotationInView()
{
    CAF_PDM_InitObject( "PolyLinesAnnotationInView", ":/WellCollection.png" );

    CAF_PDM_InitField( &m_isActive, "IsActive", true, "Is Active" );
    CAF_PDM_InitFieldNoDefault( &m_sourceAnnotation, "SourceAnnotation", "Source Annotation" );

    m_isActive.uiCapability()->setUiHidden( true );
    m_sourceAnnotation.uiCapability()->setUiHidden( true );
    m_sourceAnnotation = nullptr;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimPolylinesAnnotationInView::RimPolylinesAnnotationInView( RimPolylinesAnnotation* sourceAnnotation )
    : RimPolylinesAnnotationInView()
{
    CVF_ASSERT( sourceAnnotation );

    m_isActive         = sourceAnnotation->isActive();
    m_sourceAnnotation = sourceAnnotation;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RimPolylinesAnnotationInView::isActive() const
{
    return m_isActive();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimPolylinesAnnotation* RimPolylinesAnnotationInView::sourceAnnotation() const
{
    return m_sourceAnnotation;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RimPolylinesAnnotationInView::isVisible() const
{
    auto coll = firstAncestorOrThisOfType<RimAnnotationGroupCollection>();

    bool visible = true;
    if ( coll ) visible = coll->isVisible();
    if ( visible && m_sourceAnnotation )
    {
        visible = m_sourceAnnotation->isVisible();

        if ( visible )
        {
            auto globalColl = m_sourceAnnotation->firstAncestorOrThisOfType<RimAnnotationGroupCollection>();
            if ( globalColl ) visible = globalColl->isVisible();
        }
    }
    if ( visible ) visible = m_isActive;
    return visible;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimPolylinesAnnotationInView::fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue )
{
    if ( changedField == &m_isActive )
    {
        auto coll = firstAncestorOrThisOfType<RimAnnotationCollectionBase>();
        if ( coll ) coll->scheduleRedrawOfRelevantViews();
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
caf::PdmFieldHandle* RimPolylinesAnnotationInView::objectToggleField()
{
    return &m_isActive;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
caf::PdmFieldHandle* RimPolylinesAnnotationInView::userDescriptionField()
{
    return m_sourceAnnotation ? m_sourceAnnotation->userDescriptionField() : nullptr;
}

cvf::ref<RigPolyLinesData> RimPolylinesAnnotationInView::polyLinesData() const
{
    auto retval = m_sourceAnnotation->polyLinesData();
    if ( !isVisible() )
    {
        retval->setVisibility( false, false );
    }

    auto coll = firstAncestorOrThisOfType<RimAnnotationInViewCollection>();
    if ( coll )
    {
        retval->setZPlaneLock( coll->snapAnnotations(), coll->annotationPlaneZ() );
    }

    return retval;
}
