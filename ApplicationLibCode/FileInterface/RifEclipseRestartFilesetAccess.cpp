/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2011-     Statoil ASA
//  Copyright (C) 2013-     Ceetron Solutions AS
//  Copyright (C) 2011-2012 Ceetron AS
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

#include "RifEclipseRestartFilesetAccess.h"
#include "RiaStringEncodingTools.h"
#include "RifEclipseOutputFileTools.h"

#include "cafProgressInfo.h"

#include "ert/ecl/ecl_file.h"
#include "ert/ecl/ecl_nnc_data.h"
#include "ert/ecl/ecl_nnc_geometry.h"

//--------------------------------------------------------------------------------------------------
/// Constructor
//--------------------------------------------------------------------------------------------------
RifEclipseRestartFilesetAccess::RifEclipseRestartFilesetAccess()
    : RifEclipseRestartDataAccess()
{
}

//--------------------------------------------------------------------------------------------------
/// Destructor
//--------------------------------------------------------------------------------------------------
RifEclipseRestartFilesetAccess::~RifEclipseRestartFilesetAccess()
{
    for ( size_t i = 0; i < m_ecl_files.size(); i++ )
    {
        if ( m_ecl_files[i] )
        {
            ecl_file_close( m_ecl_files[i] );
        }

        m_ecl_files[i] = nullptr;
    }
}

//--------------------------------------------------------------------------------------------------
/// Open files
//--------------------------------------------------------------------------------------------------
bool RifEclipseRestartFilesetAccess::open()
{
    if ( !m_fileNames.empty() )
    {
        caf::ProgressInfo progInfo( m_fileNames.size(), "" );

        int i;
        for ( i = 0; i < m_fileNames.size(); i++ )
        {
            progInfo.setProgressDescription( m_fileNames[i] );

            openTimeStep( i );

            progInfo.incrementProgress();
        }
    }

    return true;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RifEclipseRestartFilesetAccess::setRestartFiles( const QStringList& fileSet )
{
    close();
    m_ecl_files.clear();

    m_fileNames = fileSet;
    m_fileNames.sort(); // To make sure they are sorted in increasing *.X000N order. Hack. Should probably be actual
                        // time stored on file.

    for ( int i = 0; i < m_fileNames.size(); i++ )
    {
        m_ecl_files.push_back( nullptr );
    }

    CVF_ASSERT( m_fileNames.size() == static_cast<int>( m_ecl_files.size() ) );
}

//--------------------------------------------------------------------------------------------------
/// Close files
//--------------------------------------------------------------------------------------------------
void RifEclipseRestartFilesetAccess::close()
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RifEclipseRestartFilesetAccess::setTimeSteps( const std::vector<QDateTime>& timeSteps )
{
    CVF_ASSERT( (size_t)m_fileNames.size() == timeSteps.size() );
    m_timeSteps = timeSteps;
}

//--------------------------------------------------------------------------------------------------
/// Get the number of time steps
//--------------------------------------------------------------------------------------------------
size_t RifEclipseRestartFilesetAccess::timeStepCount()
{
    return m_timeSteps.size();
}

//--------------------------------------------------------------------------------------------------
/// Get the time steps
//--------------------------------------------------------------------------------------------------
void RifEclipseRestartFilesetAccess::timeSteps( std::vector<QDateTime>* timeSteps, std::vector<double>* daysSinceSimulationStart )
{
    if ( m_timeSteps.empty() )
    {
        size_t numSteps = m_fileNames.size();
        size_t i;
        for ( i = 0; i < numSteps; i++ )
        {
            std::vector<QDateTime> stepTime;
            std::vector<double>    stepDays;

            openTimeStep( i );

            size_t perTimeStepEmptyKeywords = 0;
            RifEclipseOutputFileTools::timeSteps( m_ecl_files[i], &stepTime, &stepDays, &perTimeStepEmptyKeywords );

            if ( stepTime.size() == 1 )
            {
                m_timeSteps.push_back( stepTime[0] );
                m_daysSinceSimulationStart.push_back( stepDays[0] );
            }
            else
            {
                m_timeSteps.push_back( QDateTime() );
                m_daysSinceSimulationStart.push_back( 0.0 );
            }
        }
    }

    *timeSteps                = m_timeSteps;
    *daysSinceSimulationStart = m_daysSinceSimulationStart;
}

//--------------------------------------------------------------------------------------------------
/// Get list of result names
//--------------------------------------------------------------------------------------------------
std::vector<RifKeywordValueCount> RifEclipseRestartFilesetAccess::keywordValueCounts()
{
    CVF_ASSERT( timeStepCount() > 0 );

    for ( int i = 0; i < m_fileNames.size(); i++ )
    {
        openTimeStep( i );
    }

    return RifEclipseOutputFileTools::keywordValueCounts( m_ecl_files );
}

//--------------------------------------------------------------------------------------------------
/// Get result values for given time step
//--------------------------------------------------------------------------------------------------
bool RifEclipseRestartFilesetAccess::results( const QString& resultName, size_t timeStep, size_t gridCount, std::vector<double>* values )
{
    if ( timeStep >= timeStepCount() )
    {
        return false;
    }

    openTimeStep( timeStep );

    if ( !m_ecl_files[timeStep] )
    {
        return false;
    }

    size_t fileGridCount = ecl_file_get_num_named_kw( m_ecl_files[timeStep], resultName.toLatin1().data() );

    // No results for this result variable for current time step found
    if ( fileGridCount == 0 ) return true;

    // Result handling depends on presents of result values for all grids
    if ( fileGridCount < gridCount )
    {
        return false;
    }

    // NB : 6X simulator can report multiple keywords for one time step. The additional keywords do not contain and
    // data imported by ResInsight. We read all blocks of data, also empty to simplify the logic.
    // See https://github.com/OPM/ResInsight/issues/5763

    for ( size_t i = 0; i < fileGridCount; i++ )
    {
        std::vector<double> gridValues;

        if ( !RifEclipseOutputFileTools::keywordData( m_ecl_files[timeStep], resultName, i, &gridValues ) )
        {
            return false;
        }

        values->insert( values->end(), gridValues.begin(), gridValues.end() );
    }

    return true;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RifEclipseRestartFilesetAccess::dynamicNNCResults( const ecl_grid_type* grid,
                                                        size_t               timeStep,
                                                        std::vector<double>* waterFlux,
                                                        std::vector<double>* oilFlux,
                                                        std::vector<double>* gasFlux )
{
    if ( timeStep > timeStepCount() )
    {
        return false;
    }

    openTimeStep( timeStep );

    if ( !m_ecl_files[timeStep] )
    {
        return false;
    }

    ecl_file_view_type* summaryView = ecl_file_get_global_view( m_ecl_files[timeStep] );

    RifEclipseOutputFileTools::transferNncFluxData( grid, summaryView, waterFlux, oilFlux, gasFlux );

    return true;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RifEclipseRestartFilesetAccess::readWellData( well_info_type* well_info, bool importCompleteMswData )
{
    if ( !well_info ) return;

    for ( size_t i = 0; i < m_ecl_files.size(); i++ )
    {
        openTimeStep( i );

        if ( m_ecl_files[i] )
        {
            int reportNumber = RifEclipseRestartFilesetAccess::reportNumber( m_ecl_files[i] );
            if ( reportNumber != -1 )
            {
                well_info_add_wells( well_info, m_ecl_files[i], reportNumber, importCompleteMswData );
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RifEclipseRestartFilesetAccess::openTimeStep( size_t timeStep )
{
    CVF_ASSERT( timeStep < m_ecl_files.size() );

    if ( m_ecl_files[timeStep] == nullptr )
    {
        int index = static_cast<int>( timeStep );
        ecl_file_type* ecl_file = ecl_file_open( RiaStringEncodingTools::toNativeEncoded( m_fileNames[index] ).data(), ECL_FILE_CLOSE_STREAM );

        m_ecl_files[timeStep] = ecl_file;

        if ( ecl_file )
        {
            auto phases = RifEclipseOutputFileTools::findAvailablePhases( ecl_file );

            m_availablePhases.insert( phases.begin(), phases.end() );
        }
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
int RifEclipseRestartFilesetAccess::reportNumber( const ecl_file_type* ecl_file )
{
    if ( !ecl_file ) return -1;

    const char* eclFileName = ecl_file_get_src_file( ecl_file );
    QString     fileNameUpper( eclFileName );
    fileNameUpper = fileNameUpper.toUpper();

    // Convert to upper case, as ecl_util_filename_report_nr does not handle lower case file extensions
    int reportNumber = ecl_util_filename_report_nr( RiaStringEncodingTools::toNativeEncoded( fileNameUpper ).data() );

    return reportNumber;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
int RifEclipseRestartFilesetAccess::readUnitsType()
{
    ecl_file_type* ecl_file = nullptr;

    if ( !m_ecl_files.empty() )
    {
        openTimeStep( 0 );
        ecl_file = m_ecl_files[0];
    }

    return RifEclipseOutputFileTools::readUnitsType( ecl_file );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::set<RiaDefines::PhaseType> RifEclipseRestartFilesetAccess::availablePhases() const
{
    return m_availablePhases;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<int> RifEclipseRestartFilesetAccess::reportNumbers()
{
    std::vector<int> reportNr;

    for ( const auto* ecl_file : m_ecl_files )
    {
        if ( ecl_file )
        {
            int reportNumber = RifEclipseRestartFilesetAccess::reportNumber( ecl_file );
            if ( reportNumber != -1 )
            {
                reportNr.push_back( reportNumber );
            }
        }
    }

    return reportNr;
}
