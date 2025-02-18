/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017     Statoil ASA
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

#include "RiaColorTables.h"
#include "RiaColorTools.h"

#include "RiuGuiTheme.h"

#include "cvfAssert.h"

#include <QColor>

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::normalPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{ cvf::Color3ub( 0, 0, 255 ),
                                              cvf::Color3ub( 0, 127, 255 ),
                                              cvf::Color3ub( 0, 255, 255 ),
                                              cvf::Color3ub( 0, 255, 0 ),
                                              cvf::Color3ub( 255, 255, 0 ),
                                              cvf::Color3ub( 255, 127, 0 ),
                                              cvf::Color3ub( 255, 0, 0 ) };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::normalPaletteOppositeOrderingColors()
{
    static std::vector<cvf::Color3ub> colors{ cvf::Color3ub( 255, 0, 0 ),
                                              cvf::Color3ub( 255, 127, 0 ),
                                              cvf::Color3ub( 255, 255, 0 ),
                                              cvf::Color3ub( 0, 255, 0 ),
                                              cvf::Color3ub( 0, 255, 255 ),
                                              cvf::Color3ub( 0, 127, 255 ),
                                              cvf::Color3ub( 0, 0, 255 ) };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::blackWhitePaletteColors()
{
    static std::vector<cvf::Color3ub> colors{ cvf::Color3ub::BLACK, cvf::Color3ub::WHITE };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::whiteBlackPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{ cvf::Color3ub::WHITE, cvf::Color3ub::BLACK };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::pinkWhitePaletteColors()
{
    static std::vector<cvf::Color3ub> colors{ cvf::Color3ub::DEEP_PINK, cvf::Color3ub::WHITE };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::whitePinkPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{ cvf::Color3ub::WHITE, cvf::Color3ub::DEEP_PINK };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::blueWhiteRedPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{ cvf::Color3ub::BLUE, cvf::Color3ub::WHITE, cvf::Color3ub::RED };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::redWhiteBluePaletteColors()
{
    static std::vector<cvf::Color3ub> colors{ cvf::Color3ub::RED, cvf::Color3ub::WHITE, cvf::Color3ub::BLUE };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::categoryPaletteColors()
{
    static caf::ColorTable colorTable = caf::ColorTable( categoryColors() );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::contrastCategoryPaletteColors()
{
    static caf::ColorTable colorTable = caf::ColorTable( contrastCategoryColors() );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::tensorWhiteGrayBlackPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub::WHITE,
        cvf::Color3ub::LIGHT_GRAY,
        cvf::Color3ub::BLACK,
    };

    static caf::ColorTable colorTable = caf::ColorTable( colors );
    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::tensorOrangeBlueWhitePaletteColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 236, 118, 0 ), // Orange
        cvf::Color3ub( 56, 56, 255 ), // Vivid Blue
        cvf::Color3ub( 210, 248, 250 ), // White Turquoiseish
    };

    static caf::ColorTable colorTable = caf::ColorTable( colors );
    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::tensorsMagentaBrownGrayPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 248, 0, 170 ), // Magenta
        cvf::Color3ub::BROWN,
        cvf::Color3ub::LIGHT_GRAY,
    };

    static caf::ColorTable colorTable = caf::ColorTable( colors );
    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::angularPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{ cvf::Color3ub( 255, 0, 255 ),
                                              cvf::Color3ub( 0, 0, 255 ),
                                              cvf::Color3ub( 0, 127, 255 ),
                                              cvf::Color3ub( 0, 255, 255 ),
                                              cvf::Color3ub( 0, 255, 0 ),
                                              cvf::Color3ub( 255, 255, 0 ),
                                              cvf::Color3ub( 255, 127, 0 ),
                                              cvf::Color3ub( 255, 0, 0 ),
                                              cvf::Color3ub( 255, 0, 255 ) };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::rainbowPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{ cvf::Color3ub::BLACK,
                                              cvf::Color3ub::MAGENTA,
                                              cvf::Color3ub::BLUE,
                                              cvf::Color3ub::CYAN,
                                              cvf::Color3ub::GREEN,
                                              cvf::Color3ub::RED,
                                              cvf::Color3ub::YELLOW,
                                              cvf::Color3ub::WHITE };

    static caf::ColorTable colorTable = caf::ColorTable( colors );
    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::stimPlanPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 220, 220, 220 ), // Grey
        cvf::Color3ub( 0, 0, 255 ), // Blue
        cvf::Color3ub( 0, 128, 255 ), // Lighter blue
        cvf::Color3ub( 80, 240, 60 ), // Darker green
        cvf::Color3ub( 0, 255, 0 ), // Green
        cvf::Color3ub( 255, 255, 0 ), // Yellow
        cvf::Color3ub( 255, 192, 0 ), // Light orange
        cvf::Color3ub( 255, 128, 0 ), // Orange
        cvf::Color3ub( 255, 64, 0 ), // Red-orange
        cvf::Color3ub( 255, 0, 255 ) // Magenta
    };

    static caf::ColorTable colorTable = caf::ColorTable( colors );
    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::faultsPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 101, 132, 96 ), // Dark green
        cvf::Color3ub( 255, 131, 140 ), // Old pink
        cvf::Color3ub( 210, 176, 112 ), // Light Brown
        cvf::Color3ub( 140, 171, 238 ), // Light gray blue
        cvf::Color3ub( 255, 205, 131 ), // Peach
        cvf::Color3ub( 220, 212, 166 ), // Dark off white
        cvf::Color3ub( 130, 255, 120 ), // Light green
        cvf::Color3ub( 166, 220, 215 ), // Light gray torquise
        cvf::Color3ub( 168, 220, 166 ), // Light gray green
        cvf::Color3ub( 255, 64, 236 ) // Magneta
    };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::wellsPaletteColors()
{
    return categoryPaletteColors();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::summaryCurveDefaultPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 0, 112, 136 ), // Dark Green-Blue
        cvf::Color3ub( 202, 0, 0 ), // Red
        cvf::Color3ub( 78, 204, 0 ), // Clear Green
        cvf::Color3ub( 236, 118, 0 ), // Orange
        cvf::Color3ub( 130, 130, 130 ), // Grey
        cvf::Color3ub( 56, 56, 255 ), // Vivid Blue
        cvf::Color3ub( 248, 0, 170 ), // Magenta
        cvf::Color3ub( 169, 2, 240 ), // Purple
        cvf::Color3ub( 0, 221, 221 ), // Turquoise
        cvf::Color3ub( 201, 168, 206 ), // Light Violet
        cvf::Color3ub( 0, 205, 68 ), // Bluish Green
        cvf::Color3ub( 236, 188, 0 ), // Mid Yellow
        cvf::Color3ub( 51, 204, 255 ), // Bluer Turquoise
        cvf::Color3ub( 164, 193, 0 ), // Mid Yellowish Green
        cvf::Color3ub( 0, 143, 239 ) // Dark Light Blue
    };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::summaryCurveRedPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 202, 0, 0 ), // Off Red
        cvf::Color3ub( 255, 51, 51 ), // Bright Red
        cvf::Color3ub( 255, 102, 102 ) // Light Red
    };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::summaryCurveGreenPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 78, 204, 0 ), // Clear Green
        cvf::Color3ub( 164, 193, 0 ), // Mid Yellowish Green
        cvf::Color3ub( 0, 205, 68 ) // Bluish Green
    };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::summaryCurveBluePaletteColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 56, 56, 255 ), // Vivid Blue
        cvf::Color3ub( 0, 143, 239 ), // Dark Light Blue
        cvf::Color3ub( 153, 153, 255 ) // Off Light Blue
    };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::summaryCurveBrownPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 186, 101, 44 ),
        cvf::Color3ub( 99, 53, 23 ), // Highway Brown
        cvf::Color3ub( 103, 56, 24 ), // Dark Brown
    };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::summaryCurveNoneRedGreenBlueBrownPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 236, 118, 0 ), // Orange
        cvf::Color3ub( 0, 0, 0 ), // Black
        cvf::Color3ub( 248, 0, 170 ), // Magenta
        cvf::Color3ub( 236, 188, 0 ), // Mid Yellow
        cvf::Color3ub( 169, 2, 240 ), // Purple
        cvf::Color3ub( 0, 221, 221 ), // Turquoise
        cvf::Color3ub( 201, 168, 206 ) // Light Violet
    };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::wellLogPlotPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{ caf::ColorTable::fromQColor( QColor( "peru" ) ),
                                              caf::ColorTable::fromQColor( QColor( "blueviolet" ) ),
                                              caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::darkYellow ) ),
                                              caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::darkCyan ) ),
                                              caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::darkMagenta ) ),
                                              caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::darkGray ) ),
                                              caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::yellow ) ),
                                              caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::magenta ) ),
                                              caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::cyan ) ),
                                              caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::gray ) ),
                                              caf::ColorTable::fromQColor( QColor( "yellowgreen" ) ),
                                              caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::black ) ) };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::selectionPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{ caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::magenta ) ),
                                              caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::cyan ) ),
                                              caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::blue ) ),
                                              caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::red ) ),
                                              caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::green ) ),
                                              caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::yellow ) ),
                                              caf::ColorTable::fromQColor( Qt::GlobalColor( Qt::gray ) ) };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::timestepsPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 56, 56, 255 ), // Vivid Blue
        cvf::Color3ub( 0, 143, 239 ), // Dark Light Blue
        cvf::Color3ub( 0, 112, 136 ), // Dark Green-Blue
        cvf::Color3ub( 51, 204, 255 ), // Bluer Turquoise
        cvf::Color3ub( 0, 221, 221 ), // Turquoise
        cvf::Color3ub( 0, 205, 68 ), // Bluish Green
        cvf::Color3ub( 78, 204, 0 ), // Clear Green
        cvf::Color3ub( 164, 193, 0 ), // Mid Yellowish Green
        cvf::Color3ub( 236, 188, 0 ), // Mid Yellow
        cvf::Color3ub( 236, 118, 0 ), // Orange
        cvf::Color3ub( 202, 0, 0 ), // Red
        cvf::Color3ub( 248, 0, 170 ), // Magenta
        cvf::Color3ub( 201, 168, 206 ), // Light Violet
        cvf::Color3ub( 169, 2, 240 ), // Purple
    };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::editableWellPathsPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 201, 145, 201 ), // Gray pink
        cvf::Color3ub( 173, 23, 212 ), // Strong Purple
        cvf::Color3ub( 143, 46, 219 ), // Purple
        cvf::Color3ub( 102, 76, 230 ), // Gray Blue
        cvf::Color3ub( 71, 99, 237 ), // Lighter Gray Blue
        cvf::Color3ub( 31, 130, 247 ), // Strong Blue
        cvf::Color3ub( 0, 153, 255 ), // Dark Turquise
    };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::wellPathsPaletteColors()
{
    // Use inverted category colors to avoid identical colors if we have few sim wells and few well paths

    static caf::ColorTable colorTable = caf::ColorTable( invertedCategoryColors() );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::waterAndRockPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 127, 205, 255 ), // Sea Blue
        cvf::Color3ub( 100, 100, 100 ) // Gray
    };
    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::correlationPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 255, 25, 50 ), // Bluish red
        cvf::Color3ub( 240, 240, 240 ), // Light Gray
        cvf::Color3ub( 255, 100, 50 ), // Dark red Orange
    };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::heatMapPaletteColors()
{
    static std::vector<cvf::Color3ub> colors{ cvf::Color3ub::DARK_BLUE,
                                              cvf::Color3ub( 0, 0, 240 ), // Medium Blue
                                              cvf::Color3ub( 0, 102, 204 ), // Transition Medium Blue to Cyan
                                              cvf::Color3ub::CYAN,
                                              cvf::Color3ub( 75, 255, 47 ), // Green/Yellow more green
                                              cvf::Color3ub::DARK_ORANGE,
                                              cvf::Color3ub::YELLOW };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
cvf::Color3f RiaColorTables::undefinedCellColor()
{
    return cvf::Color3::GRAY;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RiaColorTables::WellPathComponentColors RiaColorTables::wellPathComponentColors()
{
    return { { RiaDefines::WellPathComponentType::WELL_PATH, cvf::Color3::CEETRON },
             { RiaDefines::WellPathComponentType::PERFORATION_INTERVAL, cvf::Color3::DARK_MAGENTA },
             { RiaDefines::WellPathComponentType::FISHBONES, cvf::Color3::DARK_GREEN },
             { RiaDefines::WellPathComponentType::FRACTURE, cvf::Color3::CRIMSON },
             { RiaDefines::WellPathComponentType::ICD, cvf::Color3::DARK_ORANGE },
             { RiaDefines::WellPathComponentType::AICD, cvf::Color3::INDIGO },
             { RiaDefines::WellPathComponentType::ICV, cvf::Color3::ORCHID },
             { RiaDefines::WellPathComponentType::CASING, cvf::Color3::SEA_GREEN },
             { RiaDefines::WellPathComponentType::LINER, cvf::Color3::OLIVE },
             { RiaDefines::WellPathComponentType::PACKER, cvf::Color3::GRAY },
             { RiaDefines::WellPathComponentType::UNDEFINED_COMPONENT, cvf::Color3::MAGENTA } };
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const caf::ColorTable& RiaColorTables::structuralUncertaintyColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 255, 183, 198 ), // Large band
        cvf::Color3ub( 255, 113, 142 ), // Narrow band
        cvf::Color3ub( 255, 18, 67 ), // Mean curve
    };

    static caf::ColorTable colorTable = caf::ColorTable( colors );

    return colorTable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
cvf::Color3f RiaColorTables::defaultGridLineColor()
{
    return cvf::Color3f( 0.92f, 0.92f, 0.92f );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
cvf::Color3f RiaColorTables::defaultFaultLineColor()
{
    return cvf::Color3f( 0.08f, 0.08f, 0.08f );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
cvf::Color3f RiaColorTables::defaultWellLabelColor()
{
    return cvf::Color3f( 0.92f, 0.92f, 0.92f );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
cvf::Color3f RiaColorTables::defaultViewerBackgroundColor()
{
    return cvf::Color3f( 0.69f, 0.77f, 0.87f );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
caf::ColorTable RiaColorTables::createBrightnessBasedColorTable( cvf::Color3ub baseColor, int brightnessLevelCount )
{
    CVF_ASSERT( brightnessLevelCount >= 1 );
    QColor baseRGB( baseColor.r(), baseColor.g(), baseColor.b() );
    float  hueF = baseRGB.hslHueF();
    float  satF = baseRGB.hslSaturationF();

    std::vector<cvf::Color3ub> colors;
    if ( brightnessLevelCount == 1 )
    {
        colors.push_back( cvf::Color3ub( RiaColorTools::fromQColorTo3f( QColor::fromHslF( hueF, satF, 0.5 ) ) ) );
    }
    else
    {
        for ( int i = 0; i < brightnessLevelCount; ++i )
        {
            float brightness = static_cast<float>( i ) / static_cast<float>( brightnessLevelCount - 1 );
            colors.push_back( cvf::Color3ub( RiaColorTools::fromQColorTo3f( QColor::fromHslF( hueF, satF, brightness ) ) ) );
        }
    }
    return caf::ColorTable( colors );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::map<RiaDefines::PhaseType, caf::ColorTable> RiaColorTables::phaseColors()
{
    static std::vector<cvf::Color3ub> waterColors{ cvf::Color3ub( cvf::Color3::DARK_BLUE ), cvf::Color3ub( cvf::Color3::SKY_BLUE ) };
    static std::vector<cvf::Color3ub> gasColors{ cvf::Color3ub( cvf::Color3::DARK_RED ), cvf::Color3ub( cvf::Color3::PINK ) };
    static std::vector<cvf::Color3ub> oilColors{ cvf::Color3ub( cvf::Color3::DARK_GREEN ), cvf::Color3ub( cvf::Color3::YELLOW_GREEN ) };

    return { { RiaDefines::PhaseType::WATER_PHASE, caf::ColorTable( waterColors ) },
             { RiaDefines::PhaseType::GAS_PHASE, caf::ColorTable( gasColors ) },
             { RiaDefines::PhaseType::OIL_PHASE, caf::ColorTable( oilColors ) } };
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
cvf::Color3f RiaColorTables::phaseColor( RiaDefines::PhaseType phase )
{
    cvf::Color3ub col = cvf::Color3::DARK_GRAY;
    QColor        themeCol;

    if ( phase == RiaDefines::PhaseType::GAS_PHASE )
    {
        themeCol = RiuGuiTheme::getColorByVariableName( "curveColorGas2" );
        col      = cvf::Color3ub( 212, 0, 0 );
    }
    else if ( phase == RiaDefines::PhaseType::OIL_PHASE )
    {
        themeCol = RiuGuiTheme::getColorByVariableName( "curveColorOil2" );
        col      = cvf::Color3ub( 0, 204, 0 );
    }
    else if ( phase == RiaDefines::PhaseType::WATER_PHASE )
    {
        themeCol = RiuGuiTheme::getColorByVariableName( "curveColorWater2" );
        col      = cvf::Color3ub( 0, 0, 205 );
    }

    if ( themeCol.isValid() )
    {
        return RiaColorTools::fromQColorTo3f( themeCol );
    }
    else
    {
        return cvf::Color3f( col );
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
cvf::Color3f RiaColorTables::historyCurveContrastColor()
{
    auto unsignedColor = cvf::Color3ub( 248, 0, 170 ); // Magenta

    return cvf::Color3f( unsignedColor );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<cvf::Color3ub> RiaColorTables::categoryColors()
{
    // Based on http://stackoverflow.com/questions/470690/how-to-automatically-generate-n-distinct-colors
    // and Kelly Colors and sorted by hue
    // See also http://www.w3schools.com/colors/ for palettes etc.

    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 128, 62, 117 ), // hwb(310, 24%, 50%) strong_purple
        cvf::Color3ub( 212, 28, 132 ), // hwb(326, 11%, 17%) strong_purplish_red
        cvf::Color3ub( 246, 118, 142 ), // hwb(349, 46%,  4%) strong_purplish_pink
        cvf::Color3ub( 193, 0, 32 ), // hwb(350,  0%, 24%) vivid_red
        cvf::Color3ub( 127, 24, 13 ), // hwb(  6,  5%, 50%) strong_reddish_brown
        cvf::Color3ub( 241, 58, 19 ), // hwb( 11,  7%,  5%) vivid_reddish_orange
        cvf::Color3ub( 255, 122, 92 ), // hwb( 11, 36%,  0%) strong_yellowish_pink
        cvf::Color3ub( 129, 112, 102 ), // hwb( 22, 40%, 49%) medium_gray
        cvf::Color3ub( 255, 104, 0 ), // hwb( 24,  0%,  0%) vivid_orange
        cvf::Color3ub( 89, 51, 21 ), // hwb( 26,  8%, 65%) deep_yellowish_brown
        cvf::Color3ub( 255, 142, 0 ), // hwb( 33,  0%,  0%) vivid_orange_yellow
        cvf::Color3ub( 206, 162, 98 ), // hwb( 36, 38%, 19%) grayish_yellow
        cvf::Color3ub( 244, 200, 0 ), // hwb( 49,  0%,  4%) vivid_greenish_yellow
        cvf::Color3ub( 147, 170, 0 ), // hwb( 68,  0%, 33%) vivid_yellowish_green
        cvf::Color3ub( 59, 84, 23 ), // hwb( 85,  9%, 67%) dark_olive_green
        cvf::Color3ub( 0, 125, 52 ), // hwb(145,  0%, 51%) vivid_green
        cvf::Color3ub( 54, 125, 123 ), // hwb(178, 21%, 51%) vivid_blueish_green
        cvf::Color3ub( 0, 83, 138 ), // hwb(204,  0%, 46%) strong_blue
        cvf::Color3ub( 166, 189, 215 ), // hwb(212, 65%, 16%) very_light_blue
        cvf::Color3ub( 46, 76, 224 ) // hwb(230, 18%, 12%) medium_blue
    };

    return colors;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<cvf::Color3ub> RiaColorTables::contrastCategoryColors()
{
    // Based on http://stackoverflow.com/questions/470690/how-to-automatically-generate-n-distinct-colors
    // and Kelly Colors and sorted by hue
    // See also http://www.w3schools.com/colors/ for palettes etc.

    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 244, 200, 0 ), // hwb( 49,  0%,  4%) vivid_greenish_yellow
        cvf::Color3ub( 128, 62, 117 ), // hwb(310, 24%, 50%) strong_purple
        cvf::Color3ub( 255, 104, 0 ), // hwb( 24,  0%,  0%) vivid_orange
        cvf::Color3ub( 166, 189, 215 ), // hwb(212, 65%, 16%) very_light_blue
        cvf::Color3ub( 193, 0, 32 ), // hwb(350,  0%, 24%) vivid_red
        cvf::Color3ub( 206, 162, 98 ), // hwb( 36, 38%, 19%) grayish_yellow
        cvf::Color3ub( 129, 112, 102 ), // hwb( 22, 40%, 49%) medium_gray
        cvf::Color3ub( 0, 125, 52 ), // hwb(145,  0%, 51%) vivid_green
        cvf::Color3ub( 246, 118, 142 ), // hwb(349, 46%,  4%) strong_purplish_pink
        cvf::Color3ub( 0, 83, 138 ), // hwb(204,  0%, 46%) strong_blue
        cvf::Color3ub( 255, 122, 92 ), // hwb( 11, 36%,  0%) strong_yellowish_pink
        cvf::Color3ub( 212, 28, 132 ), // hwb(326, 11%, 17%) strong_purplish_red
        cvf::Color3ub( 255, 142, 0 ), // hwb( 33,  0%,  0%) vivid_orange_yellow
        cvf::Color3ub( 59, 84, 23 ), // hwb( 85,  9%, 67%) dark_olive_green
        cvf::Color3ub( 127, 24, 13 ), // hwb(  6,  5%, 50%) strong_reddish_brown
        cvf::Color3ub( 54, 125, 123 ), // hwb(178, 21%, 51%) vivid_blueish_green
        cvf::Color3ub( 241, 58, 19 ), // hwb( 11,  7%,  5%) vivid_reddish_orange
        cvf::Color3ub( 147, 170, 0 ), // hwb( 68,  0%, 33%) vivid_yellowish_green
        cvf::Color3ub( 46, 76, 224 ), // hwb(230, 18%, 12%) medium_blue
        cvf::Color3ub( 89, 51, 21 ), // hwb( 26,  8%, 65%) deep_yellowish_brown
        cvf::Color3ub( 0, 0, 0 ) //     hwb(0, 0%, 100%) black
    };

    return colors;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<cvf::Color3ub> RiaColorTables::invertedCategoryColors()
{
    static std::vector<cvf::Color3ub> colors{
        cvf::Color3ub( 46, 76, 224 ), // hwb(230, 18%, 12%) medium_blue
        cvf::Color3ub( 166, 189, 215 ), // hwb(212, 65%, 16%) very_light_blue
        cvf::Color3ub( 0, 83, 138 ), // hwb(204,  0%, 46%) strong_blue
        cvf::Color3ub( 54, 125, 123 ), // hwb(178, 21%, 51%) vivid_blueish_green
        cvf::Color3ub( 0, 125, 52 ), // hwb(145,  0%, 51%) vivid_green
        cvf::Color3ub( 59, 84, 23 ), // hwb( 85,  9%, 67%) dark_olive_green
        cvf::Color3ub( 147, 170, 0 ), // hwb( 68,  0%, 33%) vivid_yellowish_green
        cvf::Color3ub( 244, 200, 0 ), // hwb( 49,  0%,  4%) vivid_greenish_yellow
        cvf::Color3ub( 206, 162, 98 ), // hwb( 36, 38%, 19%) grayish_yellow
        cvf::Color3ub( 255, 142, 0 ), // hwb( 33,  0%,  0%) vivid_orange_yellow
        cvf::Color3ub( 89, 51, 21 ), // hwb( 26,  8%, 65%) deep_yellowish_brown
        cvf::Color3ub( 255, 104, 0 ), // hwb( 24,  0%,  0%) vivid_orange
        cvf::Color3ub( 129, 112, 102 ), // hwb( 22, 40%, 49%) medium_gray
        cvf::Color3ub( 255, 122, 92 ), // hwb( 11, 36%,  0%) strong_yellowish_pink
        cvf::Color3ub( 241, 58, 19 ), // hwb( 11,  7%,  5%) vivid_reddish_orange
        cvf::Color3ub( 127, 24, 13 ), // hwb(  6,  5%, 50%) strong_reddish_brown
        cvf::Color3ub( 193, 0, 32 ), // hwb(350,  0%, 24%) vivid_red
        cvf::Color3ub( 246, 118, 142 ), // hwb(349, 46%,  4%) strong_purplish_pink
        cvf::Color3ub( 212, 28, 132 ), // hwb(326, 11%, 17%) strong_purplish_red
        cvf::Color3ub( 128, 62, 117 ) // hwb(310, 24%, 50%) strong_purple
    };

    return colors;
}
