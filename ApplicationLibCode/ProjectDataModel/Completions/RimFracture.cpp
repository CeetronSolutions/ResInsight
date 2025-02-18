/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017-2018 Statoil ASA
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

#include "RimFracture.h"

#include "RiaApplication.h"
#include "RiaColorTables.h"
#include "RiaCompletionTypeCalculationScheduler.h"
#include "RiaEclipseUnitTools.h"
#include "RiaLogging.h"

#include "Riu3DMainWindowTools.h"

#include "RigMainGrid.h"

#include "Rim3dView.h"
#include "RimEclipseCase.h"
#include "RimEclipseCellColors.h"
#include "RimEclipseView.h"
#include "RimEllipseFractureTemplate.h"
#include "RimFractureContainment.h"
#include "RimFractureTemplate.h"
#include "RimFractureTemplateCollection.h"
#include "RimOilField.h"
#include "RimProject.h"
#include "RimReservoirCellResultsStorage.h"
#include "RimStimPlanColors.h"
#include "RimStimPlanFractureTemplate.h"

#include "RivWellFracturePartMgr.h"

#include "FractureCommands/RicNewEllipseFractureTemplateFeature.h"
#include "FractureCommands/RicNewStimPlanFractureTemplateFeature.h"

#include "cafHexGridIntersectionTools/cafHexGridIntersectionTools.h"

#include "cafPdmUiDoubleSliderEditor.h"
#include "cafPdmUiPushButtonEditor.h"
#include "cafPdmUiToolButtonEditor.h"
#include "cafPdmUiTreeOrdering.h"

#include "cvfBoundingBox.h"
#include "cvfGeometryTools.h"
#include "cvfMath.h"
#include "cvfPlane.h"

#include <QString>

#include <cmath>

CAF_PDM_XML_ABSTRACT_SOURCE_INIT( RimFracture, "Fracture" );

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void setDefaultFractureColorResult()
{
    RiaApplication* app  = RiaApplication::instance();
    RimProject*     proj = app->project();

    for ( RimEclipseCase* const eclCase : proj->eclipseCases() )
    {
        for ( Rim3dView* const view : eclCase->views() )
        {
            std::vector<RimStimPlanColors*> fractureColors = view->descendantsIncludingThisOfType<RimStimPlanColors>();
            for ( RimStimPlanColors* const stimPlanColors : fractureColors )
            {
                stimPlanColors->setDefaultResultName();
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimFracture::RimFracture()
{
    CAF_PDM_InitObject( "Fracture" );

    CAF_PDM_InitFieldNoDefault( &m_fractureTemplate, "FractureDef", "Fracture Template" );
    CAF_PDM_InitField( &m_editFractureTemplate, "EditTemplate", false, "Edit" );
    m_editFractureTemplate.uiCapability()->setUiEditorTypeName( caf::PdmUiToolButtonEditor::uiEditorTypeName() );
    m_editFractureTemplate.uiCapability()->setUiLabelPosition( caf::PdmUiItemInfo::HIDDEN );

    CAF_PDM_InitField( &m_createEllipseFractureTemplate, "CreateEllipseTemplate", false, "No Fracture Templates Found." );
    m_createEllipseFractureTemplate.uiCapability()->setUiEditorTypeName( caf::PdmUiPushButtonEditor::uiEditorTypeName() );
    m_createEllipseFractureTemplate.uiCapability()->setUiLabelPosition( caf::PdmUiItemInfo::TOP );

    CAF_PDM_InitField( &m_createStimPlanFractureTemplate, "CreateStimPlanTemplate", false, "Create New Template?" );
    m_createStimPlanFractureTemplate.uiCapability()->setUiEditorTypeName( caf::PdmUiPushButtonEditor::uiEditorTypeName() );
    m_createStimPlanFractureTemplate.uiCapability()->setUiLabelPosition( caf::PdmUiItemInfo::TOP );

    CAF_PDM_InitField( &m_autoUpdateWellPathDepthAtFractureFromTemplate,
                       "AutoUpdateWellPathDepthAtFractureFromTemplate",
                       true,
                       "Auto-Update From Template" );

    CAF_PDM_InitField( &m_wellPathDepthAtFracture, "WellPathDepthAtFracture", 0.0, "Well/Fracture Intersection Depth" );
    m_wellPathDepthAtFracture.uiCapability()->setUiEditorTypeName( caf::PdmUiDoubleSliderEditor::uiEditorTypeName() );

    CAF_PDM_InitFieldNoDefault( &m_anchorPosition, "AnchorPosition", "Anchor Position" );
    m_anchorPosition.uiCapability()->setUiHidden( true );
    m_anchorPosition.xmlCapability()->disableIO();

    CAF_PDM_InitFieldNoDefault( &m_uiAnchorPosition, "ui_positionAtWellpath", "Fracture Position" );
    m_uiAnchorPosition.registerGetMethod( this, &RimFracture::fracturePositionForUi );
    m_uiAnchorPosition.uiCapability()->setUiReadOnly( true );
    m_uiAnchorPosition.xmlCapability()->disableIO();

    CAF_PDM_InitField( &m_azimuth, "Azimuth", 0.0, "Azimuth" );
    m_azimuth.uiCapability()->setUiEditorTypeName( caf::PdmUiDoubleSliderEditor::uiEditorTypeName() );

    CAF_PDM_InitField( &m_perforationLength, "PerforationLength", 1.0, "Perforation Length" );
    CAF_PDM_InitField( &m_perforationEfficiency, "PerforationEfficiency", 1.0, "Perforation Efficiency" );
    m_perforationEfficiency.uiCapability()->setUiEditorTypeName( caf::PdmUiDoubleSliderEditor::uiEditorTypeName() );

    CAF_PDM_InitField( &m_wellDiameter, "WellDiameter", 0.216, "Well Diameter at Fracture" );
    CAF_PDM_InitField( &m_dip, "Dip", 0.0, "Dip" );
    CAF_PDM_InitField( &m_tilt, "Tilt", 0.0, "Tilt" );

    CAF_PDM_InitField( &m_fractureUnit,
                       "FractureUnit",
                       caf::AppEnum<RiaDefines::EclipseUnitSystem>( RiaDefines::EclipseUnitSystem::UNITS_METRIC ),
                       "Fracture Unit System" );
    m_fractureUnit.uiCapability()->setUiReadOnly( true );

    CAF_PDM_InitField( &m_stimPlanTimeIndexToPlot, "TimeIndexToPlot", 0, "StimPlan Time Step" );

    CAF_PDM_InitFieldNoDefault( &m_uiWellPathAzimuth, "WellPathAzimuth", "Well Path Azimuth" );
    m_uiWellPathAzimuth.registerGetMethod( this, &RimFracture::wellAzimuthAtFracturePositionText );
    m_uiWellPathAzimuth.uiCapability()->setUiReadOnly( true );
    m_uiWellPathAzimuth.xmlCapability()->disableIO();

    CAF_PDM_InitFieldNoDefault( &m_uiWellFractureAzimuthDiff, "WellFractureAzimuthDiff", "Azimuth Difference Between\nFracture and Well" );
    m_uiWellFractureAzimuthDiff.registerGetMethod( this, &RimFracture::wellFractureAzimuthDiffText );
    m_uiWellFractureAzimuthDiff.uiCapability()->setUiReadOnly( true );
    m_uiWellFractureAzimuthDiff.xmlCapability()->disableIO();

    CAF_PDM_InitField( &m_wellFractureAzimuthAngleWarning,
                       "WellFractureAzimithAngleWarning",
                       QString( "Difference is below 10 degrees. Consider longitudinal fracture" ),
                       "" );
    m_wellFractureAzimuthAngleWarning.uiCapability()->setUiReadOnly( true );
    m_wellFractureAzimuthAngleWarning.xmlCapability()->disableIO();

    m_fracturePartMgr = new RivWellFracturePartMgr( this );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimFracture::~RimFracture()
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
double RimFracture::perforationLength() const
{
    return m_perforationLength();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
double RimFracture::perforationEfficiency() const
{
    return m_perforationEfficiency();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::setStimPlanTimeIndexToPlot( int timeIndex )
{
    m_stimPlanTimeIndexToPlot = timeIndex;
    if ( m_autoUpdateWellPathDepthAtFractureFromTemplate )
    {
        placeUsingTemplateData();
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<size_t> RimFracture::getPotentiallyFracturedCells( const RigMainGrid* mainGrid ) const
{
    if ( !mainGrid ) return {};

    cvf::BoundingBox fractureBBox = boundingBoxInDomainCoords();
    return mainGrid->findIntersectingCells( fractureBBox );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue )
{
    if ( changedField == &m_fractureTemplate )
    {
        if ( fractureUnit() != m_fractureTemplate->fractureTemplateUnit() )
        {
            QString fractureUnitText = caf::AppEnum<RiaDefines::EclipseUnitSystem>::uiText( fractureUnit() );

            QString warningText = QString( "Using a fracture template defined in a different unit is not supported.\n\nPlease select a "
                                           "fracture template of unit '%1'" )
                                      .arg( fractureUnitText );

            RiaLogging::errorInMessageBox( nullptr, "Fracture Template Selection", warningText );

            PdmObjectHandle* prevValue    = oldValue.value<caf::PdmPointer<PdmObjectHandle>>().rawPtr();
            auto             prevTemplate = dynamic_cast<RimFractureTemplate*>( prevValue );

            m_fractureTemplate = prevTemplate;
        }

        setFractureTemplate( m_fractureTemplate );
    }
    else if ( changedField == &m_editFractureTemplate )
    {
        m_editFractureTemplate = false;
        if ( m_fractureTemplate != nullptr )
        {
            Riu3DMainWindowTools::selectAsCurrentItem( m_fractureTemplate() );
        }
    }
    else if ( changedField == &m_createEllipseFractureTemplate )
    {
        m_createEllipseFractureTemplate = false;
        RicNewEllipseFractureTemplateFeature::createNewTemplateForFractureAndUpdate( this );
    }
    else if ( changedField == &m_createStimPlanFractureTemplate )
    {
        RicNewStimPlanFractureTemplateFeature::createNewTemplateForFractureAndUpdate( this );
    }

    else if ( changedField == &m_autoUpdateWellPathDepthAtFractureFromTemplate )
    {
        if ( m_autoUpdateWellPathDepthAtFractureFromTemplate && m_fractureTemplate() )
        {
            m_wellPathDepthAtFracture = m_fractureTemplate->wellPathDepthAtFracture();
            placeUsingTemplateData();
        }
        updateFractureGrid();
        RimProject::current()->scheduleCreateDisplayModelAndRedrawAllViews();
    }

    else if ( changedField == &m_wellPathDepthAtFracture )
    {
        updateFractureGrid();
        RimProject::current()->scheduleCreateDisplayModelAndRedrawAllViews();
    }

    if ( changedField == &m_stimPlanTimeIndexToPlot )
    {
        if ( m_autoUpdateWellPathDepthAtFractureFromTemplate() ) placeUsingTemplateData();
    }

    if ( changedField == &m_azimuth || changedField == &m_fractureTemplate || changedField == &m_stimPlanTimeIndexToPlot ||
         changedField == objectToggleField() || changedField == &m_dip || changedField == &m_tilt || changedField == &m_perforationLength )
    {
        clearCachedNonDarcyProperties();

        auto eclipseCase = firstAncestorOrThisOfType<RimEclipseCase>();
        if ( eclipseCase )
        {
            RiaCompletionTypeCalculationScheduler::instance()->scheduleRecalculateCompletionTypeAndRedrawAllViews( { eclipseCase } );
        }
        else
        {
            RiaCompletionTypeCalculationScheduler::instance()->scheduleRecalculateCompletionTypeAndRedrawAllViews();
        }

        RimProject::current()->scheduleCreateDisplayModelAndRedrawAllViews();
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
cvf::Vec3d RimFracture::fracturePosition() const
{
    return m_anchorPosition;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const NonDarcyData& RimFracture::nonDarcyProperties() const
{
    CVF_ASSERT( !m_cachedFractureProperties.isDirty() );

    return m_cachedFractureProperties;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::ensureValidNonDarcyProperties()
{
    if ( m_cachedFractureProperties.isDirty() )
    {
        NonDarcyData props;

        if ( m_fractureTemplate )
        {
            props.width                 = m_fractureTemplate->computeFractureWidth( this );
            props.conductivity          = m_fractureTemplate->computeKh( this );
            props.dFactor               = m_fractureTemplate->computeDFactor( this );
            props.effectivePermeability = m_fractureTemplate->computeEffectivePermeability( this );
            props.eqWellRadius          = m_fractureTemplate->computeWellRadiusForDFactorCalculation( this );
            props.betaFactor            = m_fractureTemplate->getOrComputeBetaFactor( this );

            props.isDataDirty = false;
        }
        m_cachedFractureProperties = props;
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::clearCachedNonDarcyProperties()
{
    m_cachedFractureProperties = NonDarcyData();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RiaDefines::WellPathComponentType RimFracture::componentType() const
{
    return RiaDefines::WellPathComponentType::FRACTURE;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QString RimFracture::componentLabel() const
{
    return name();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QString RimFracture::componentTypeLabel() const
{
    return "Fracture";
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
cvf::Color3f RimFracture::defaultComponentColor() const
{
    return RiaColorTables::wellPathComponentColors()[componentType()];
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
double RimFracture::startMD() const
{
    if ( fractureTemplate()->orientationType() == RimFractureTemplate::ALONG_WELL_PATH )
    {
        return fractureMD() - 0.5 * perforationLength();
    }
    else
    {
        return fractureMD();
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
double RimFracture::endMD() const
{
    if ( fractureTemplate()->orientationType() == RimFractureTemplate::ALONG_WELL_PATH )
    {
        return startMD() + perforationLength();
    }
    else
    {
        return startMD() + fractureTemplate()->computeFractureWidth( this );
    }
}

//--------------------------------------------------------------------------------------------------
/// https://stackoverflow.com/a/52432897
//--------------------------------------------------------------------------------------------------
double getAbsoluteDiff2Angles( const double x, const double y, const double c )
{
    // c can be PI (for radians) or 180.0 (for degrees);
    return c - fabs( fmod( fabs( x - y ), 2 * c ) - c );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
double RimFracture::wellFractureAzimuthDiff() const
{
    // Compute the relative difference between two lines
    // See https://github.com/OPM/ResInsight/issues/5899

    double angle1 = wellAzimuthAtFracturePosition();
    double angle2 = m_azimuth;

    double diffDegrees        = getAbsoluteDiff2Angles( angle1, angle2, 180.0 );
    double smallesDiffDegrees = std::min( 180.0 - diffDegrees, diffDegrees );

    return smallesDiffDegrees;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QString RimFracture::wellFractureAzimuthDiffText() const
{
    double wellDifference = wellFractureAzimuthDiff();
    return QString::number( wellDifference, 'f', 2 );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QString RimFracture::wellAzimuthAtFracturePositionText() const
{
    double wellAzimuth = wellAzimuthAtFracturePosition();
    return QString::number( wellAzimuth, 'f', 2 );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
cvf::BoundingBox RimFracture::boundingBoxInDomainCoords() const
{
    std::vector<cvf::Vec3f> nodeCoordVec;
    std::vector<cvf::uint>  triangleIndices;

    triangleGeometryTransformed( &triangleIndices, &nodeCoordVec, true );

    cvf::BoundingBox fractureBBox;
    for ( const auto& nodeCoord : nodeCoordVec )
        fractureBBox.add( nodeCoord );

    return fractureBBox;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
double RimFracture::wellRadius() const
{
    if ( m_fractureUnit == RiaDefines::EclipseUnitSystem::UNITS_METRIC )
    {
        return m_wellDiameter / 2.0;
    }
    else if ( m_fractureUnit == RiaDefines::EclipseUnitSystem::UNITS_FIELD )
    {
        return RiaEclipseUnitTools::inchToFeet( m_wellDiameter / 2.0 );
    }

    return cvf::UNDEFINED_DOUBLE;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
cvf::Vec3d RimFracture::anchorPosition() const
{
    return m_anchorPosition();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
cvf::Mat4d RimFracture::transformMatrix() const
{
    cvf::Vec3d center = anchorPosition();

    // Dip (in XY plane)
    cvf::Mat4d dipRotation = cvf::Mat4d::fromRotation( cvf::Vec3d::Z_AXIS, cvf::Math::toRadians( m_dip() ) );

    // Dip (out of XY plane)
    cvf::Mat4d tiltRotation = cvf::Mat4d::fromRotation( cvf::Vec3d::X_AXIS, cvf::Math::toRadians( m_tilt() ) );

    // Ellipsis geometry is produced in XY-plane, rotate 90 deg around X to get zero azimuth along Y
    cvf::Mat4d rotationFromTesselator = cvf::Mat4d::fromRotation( cvf::Vec3d::X_AXIS, cvf::Math::toRadians( 90.0f ) );

    // Azimuth rotation
    cvf::Mat4d azimuthRotation = cvf::Mat4d::fromRotation( cvf::Vec3d::Z_AXIS, cvf::Math::toRadians( -m_azimuth() - 90 ) );

    cvf::Mat4d m = azimuthRotation * rotationFromTesselator * dipRotation * tiltRotation;
    m.setTranslation( center );

    return m;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
double RimFracture::dip() const
{
    return m_dip();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::setDip( double dip )
{
    m_dip = dip;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
double RimFracture::tilt() const
{
    return m_tilt();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::setTilt( double tilt )
{
    m_tilt = tilt;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::setAzimuth( double azimuth )
{
    m_azimuth = azimuth;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::setFractureTemplateNoUpdate( RimFractureTemplate* fractureTemplate )
{
    if ( fractureTemplate && fractureTemplate->fractureTemplateUnit() != fractureUnit() )
    {
        QString fractureUnitText = caf::AppEnum<RiaDefines::EclipseUnitSystem>::uiText( fractureUnit() );

        QString warningText = QString( "Using a fracture template defined in a different unit is not supported.\n\nPlease select a "
                                       "fracture template of unit '%1'" )
                                  .arg( fractureUnitText );

        RiaLogging::errorInMessageBox( nullptr, "Fracture Template Selection", warningText );

        return;
    }

    if ( m_fractureTemplate )
    {
        m_fractureTemplate->wellPathDepthAtFractureChanged.disconnect( this );
    }

    m_fractureTemplate = fractureTemplate;

    if ( m_fractureTemplate )
    {
        m_fractureTemplate->wellPathDepthAtFractureChanged.connect( this, &RimFracture::onWellPathDepthAtFractureInTemplateChanged );
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::triangleGeometry( std::vector<cvf::Vec3f>* nodeCoords, std::vector<cvf::uint>* triangleIndices ) const
{
    triangleGeometryTransformed( triangleIndices, nodeCoords, false );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::triangleGeometryTransformed( std::vector<cvf::uint>* triangleIndices, std::vector<cvf::Vec3f>* nodeCoords, bool transform ) const
{
    RimFractureTemplate* fractureDef = fractureTemplate();
    if ( fractureDef )
    {
        fractureDef->fractureTriangleGeometry( nodeCoords, triangleIndices, m_wellPathDepthAtFracture );
    }

    if ( transform )
    {
        cvf::Mat4d m = transformMatrix();

        for ( cvf::Vec3f& v : *nodeCoords )
        {
            cvf::Vec3d vd( v );

            vd.transformPoint( m );

            v = cvf::Vec3f( vd );
        }
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
cvf::Vec3d RimFracture::fracturePositionForUi() const
{
    cvf::Vec3d v = m_anchorPosition;

    v.z() = -v.z();

    return v;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QList<caf::PdmOptionItemInfo> RimFracture::calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions )
{
    QList<caf::PdmOptionItemInfo> options;

    RimProject* proj = RimProject::current();
    CVF_ASSERT( proj );

    if ( fieldNeedingOptions == &m_fractureTemplate )
    {
        RimOilField* oilField = proj->activeOilField();
        if ( oilField && oilField->fractureDefinitionCollection() )
        {
            RimFractureTemplateCollection* fracDefColl = oilField->fractureDefinitionCollection();

            for ( RimFractureTemplate* fracDef : fracDefColl->fractureTemplates() )
            {
                QString displayText = fracDef->nameAndUnit();
                if ( fracDef->fractureTemplateUnit() != fractureUnit() )
                {
                    displayText += " (non-matching unit)";
                }

                options.push_back( caf::PdmOptionItemInfo( displayText, fracDef ) );
            }
        }
    }
    else if ( fieldNeedingOptions == &m_stimPlanTimeIndexToPlot )
    {
        if ( fractureTemplate() )
        {
            RimFractureTemplate* fracTemplate = fractureTemplate();
            if ( dynamic_cast<RimMeshFractureTemplate*>( fracTemplate ) )
            {
                RimMeshFractureTemplate* fracTemplateStimPlan = dynamic_cast<RimMeshFractureTemplate*>( fracTemplate );
                std::vector<double>      timeValues           = fracTemplateStimPlan->timeSteps();
                int                      index                = 0;
                for ( double value : timeValues )
                {
                    options.push_back( caf::PdmOptionItemInfo( QString::number( value ), index ) );
                    index++;
                }
            }
        }
    }

    return options;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering )
{
    if ( m_fractureUnit() == RiaDefines::EclipseUnitSystem::UNITS_METRIC )
    {
        m_wellDiameter.uiCapability()->setUiName( "Well Diameter [m]" );
        m_perforationLength.uiCapability()->setUiName( "Perforation Length [m]" );
    }
    else if ( m_fractureUnit() == RiaDefines::EclipseUnitSystem::UNITS_FIELD )
    {
        m_wellDiameter.uiCapability()->setUiName( "Well Diameter [inches]" );
        m_perforationLength.uiCapability()->setUiName( "Perforation Length [ft]" );
    }

    if ( fractureTemplate() )
    {
        if ( fractureTemplate()->orientationType() == RimFractureTemplate::ALONG_WELL_PATH ||
             fractureTemplate()->orientationType() == RimFractureTemplate::TRANSVERSE_WELL_PATH )
        {
            m_uiWellPathAzimuth.uiCapability()->setUiHidden( true );
            m_uiWellFractureAzimuthDiff.uiCapability()->setUiHidden( true );
            m_wellFractureAzimuthAngleWarning.uiCapability()->setUiHidden( true );
        }

        else if ( fractureTemplate()->orientationType() == RimFractureTemplate::AZIMUTH )
        {
            m_uiWellPathAzimuth.uiCapability()->setUiHidden( false );
            m_uiWellFractureAzimuthDiff.uiCapability()->setUiHidden( false );
            if ( wellFractureAzimuthDiff() < 10 || ( wellFractureAzimuthDiff() > 170 && wellFractureAzimuthDiff() < 190 ) ||
                 wellFractureAzimuthDiff() > 350 )
            {
                m_wellFractureAzimuthAngleWarning.uiCapability()->setUiHidden( false );
            }
            else
            {
                m_wellFractureAzimuthAngleWarning.uiCapability()->setUiHidden( true );
            }
        }

        m_wellPathDepthAtFracture.uiCapability()->setUiName( fractureTemplate()->wellPathDepthAtFractureUiName() );

        if ( fractureTemplate()->orientationType() == RimFractureTemplate::ALONG_WELL_PATH ||
             fractureTemplate()->orientationType() == RimFractureTemplate::TRANSVERSE_WELL_PATH )
        {
            m_azimuth.uiCapability()->setUiReadOnly( true );
        }
        else if ( fractureTemplate()->orientationType() == RimFractureTemplate::AZIMUTH )
        {
            m_azimuth.uiCapability()->setUiReadOnly( false );
        }

        fractureTemplate()->useUserDefinedPerforationLength();

        if ( fractureTemplate()->orientationType() == RimFractureTemplate::ALONG_WELL_PATH ||
             ( fractureTemplate()->orientationType() == RimFractureTemplate::AZIMUTH && fractureTemplate()->useUserDefinedPerforationLength() ) )
        {
            m_perforationEfficiency.uiCapability()->setUiHidden( false );
            m_perforationLength.uiCapability()->setUiHidden( false );
        }
        else
        {
            m_perforationEfficiency.uiCapability()->setUiHidden( true );
            m_perforationLength.uiCapability()->setUiHidden( true );
        }

        if ( fractureTemplate()->useFiniteConductivityInFracture() )
        {
            m_wellDiameter.uiCapability()->setUiHidden( false );
        }
        else if ( fractureTemplate()->conductivityType() == RimFractureTemplate::INFINITE_CONDUCTIVITY )
        {
            m_wellDiameter.uiCapability()->setUiHidden( true );
        }

        RimFractureTemplate* fracTemplate = fractureTemplate();
        if ( dynamic_cast<RimMeshFractureTemplate*>( fracTemplate ) )
        {
            m_stimPlanTimeIndexToPlot.uiCapability()->setUiHidden( false );

            m_stimPlanTimeIndexToPlot.uiCapability()->setUiReadOnly( true );
        }
        else
        {
            m_stimPlanTimeIndexToPlot.uiCapability()->setUiHidden( true );
        }
    }
    else
    {
        m_stimPlanTimeIndexToPlot.uiCapability()->setUiHidden( true );
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute )
{
    if ( field == &m_azimuth )
    {
        caf::PdmUiDoubleSliderEditorAttribute* myAttr = dynamic_cast<caf::PdmUiDoubleSliderEditorAttribute*>( attribute );
        if ( myAttr )
        {
            myAttr->m_minimum = 0;
            myAttr->m_maximum = 360;
        }
    }

    if ( field == &m_perforationEfficiency )
    {
        caf::PdmUiDoubleSliderEditorAttribute* myAttr = dynamic_cast<caf::PdmUiDoubleSliderEditorAttribute*>( attribute );
        if ( myAttr )
        {
            myAttr->m_minimum = 0;
            myAttr->m_maximum = 1.0;
        }
    }

    if ( field == &m_wellPathDepthAtFracture )
    {
        caf::PdmUiDoubleSliderEditorAttribute* myAttr = dynamic_cast<caf::PdmUiDoubleSliderEditorAttribute*>( attribute );
        if ( myAttr )
        {
            if ( fractureTemplate() )
            {
                auto [minimum, maximum] = fractureTemplate()->wellPathDepthAtFractureRange();
                myAttr->m_minimum       = minimum;
                myAttr->m_maximum       = maximum;
            }
        }
    }

    if ( field == &m_createEllipseFractureTemplate )
    {
        auto myAttr          = dynamic_cast<caf::PdmUiPushButtonEditorAttribute*>( attribute );
        myAttr->m_buttonText = "Ellipse Template";
    }

    if ( field == &m_createStimPlanFractureTemplate )
    {
        auto myAttr          = dynamic_cast<caf::PdmUiPushButtonEditorAttribute*>( attribute );
        myAttr->m_buttonText = "StimPlan Template";
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::setAnchorPosition( const cvf::Vec3d& pos )
{
    m_anchorPosition = pos;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RiaDefines::EclipseUnitSystem RimFracture::fractureUnit() const
{
    return m_fractureUnit();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::setFractureUnit( RiaDefines::EclipseUnitSystem unitSystem )
{
    m_fractureUnit = unitSystem;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RimFracture::isEclipseCellOpenForFlow( const RigMainGrid*      mainGrid,
                                            const std::set<size_t>& reservoirCellIndicesOpenForFlow,
                                            size_t                  globalCellIndex ) const
{
    CVF_ASSERT( fractureTemplate() );
    if ( !fractureTemplate()->fractureContainment()->isEnabled() ) return true;

    return fractureTemplate()->fractureContainment()->isEclipseCellOpenForFlow( mainGrid, globalCellIndex, reservoirCellIndicesOpenForFlow );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::setFractureTemplate( RimFractureTemplate* fractureTemplate )
{
    setFractureTemplateNoUpdate( fractureTemplate );

    if ( !fractureTemplate )
    {
        return;
    }

    RimStimPlanFractureTemplate* stimPlanFracTemplate = dynamic_cast<RimStimPlanFractureTemplate*>( fractureTemplate );
    if ( stimPlanFracTemplate )
    {
        m_stimPlanTimeIndexToPlot   = stimPlanFracTemplate->activeTimeStepIndex();
        m_wellPathDepthAtFracture   = stimPlanFracTemplate->wellPathDepthAtFracture();
        double templateFormationDip = stimPlanFracTemplate->formationDip();
        if ( templateFormationDip != HUGE_VAL ) m_dip = templateFormationDip;
    }
    else
    {
        m_wellPathDepthAtFracture = fractureTemplate->wellPathDepthAtFracture();
    }

    if ( fractureTemplate->orientationType() == RimFractureTemplate::AZIMUTH )
    {
        m_azimuth = fractureTemplate->azimuthAngle();
    }
    else
    {
        updateAzimuthBasedOnWellAzimuthAngle();
    }
    m_wellDiameter      = fractureTemplate->wellDiameter();
    m_perforationLength = fractureTemplate->perforationLength();

    clearCachedNonDarcyProperties();

    setDefaultFractureColorResult();
    updateFractureGrid();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimFractureTemplate* RimFracture::fractureTemplate() const
{
    return m_fractureTemplate();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RivWellFracturePartMgr* RimFracture::fracturePartManager()
{
    CVF_ASSERT( m_fracturePartMgr.notNull() );

    return m_fracturePartMgr.p();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::updateFractureGrid()
{
    m_fractureGrid = nullptr;

    if ( m_fractureTemplate() )
    {
        m_fractureGrid = m_fractureTemplate->createFractureGrid( m_wellPathDepthAtFracture );
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const RigFractureGrid* RimFracture::fractureGrid() const
{
    return m_fractureGrid.p();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::initAfterRead()
{
    if ( RimProject::current()->isProjectFileVersionEqualOrOlderThan( "2020.10.2" ) )
    {
        if ( m_fractureTemplate() )
        {
            RimStimPlanFractureTemplate* stimPlanFracTemplate = dynamic_cast<RimStimPlanFractureTemplate*>( m_fractureTemplate() );
            RimEllipseFractureTemplate*  ellipseFracTemplate  = dynamic_cast<RimEllipseFractureTemplate*>( m_fractureTemplate() );

            if ( stimPlanFracTemplate )
            {
                m_wellPathDepthAtFracture = stimPlanFracTemplate->wellPathDepthAtFracture();
            }
            else if ( ellipseFracTemplate )
            {
                // This is a bit awkward, but initAfterRead for the templates
                // happens after initAfterRead for the fracture. The value
                // has not been corrected in the template at this point, so we
                // have to calculate it explicitly.
                m_wellPathDepthAtFracture = ellipseFracTemplate->computeLegacyWellDepthAtFracture();
            }
        }
    }

    if ( m_fractureTemplate() )
    {
        m_fractureTemplate->wellPathDepthAtFractureChanged.connect( this, &RimFracture::onWellPathDepthAtFractureInTemplateChanged );
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::onWellPathDepthAtFractureInTemplateChanged( const caf::SignalEmitter* emitter, double newDepth )
{
    if ( m_autoUpdateWellPathDepthAtFractureFromTemplate )
    {
        m_wellPathDepthAtFracture = newDepth;
        updateFractureGrid();
        RimProject::current()->scheduleCreateDisplayModelAndRedrawAllViews();
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFracture::placeUsingTemplateData()
{
}
