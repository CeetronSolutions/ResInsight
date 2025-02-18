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

#include "RivReservoirSurfaceIntersectionSourceInfo.h"

#include "RivSurfaceIntersectionGeometryGenerator.h"

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RivReservoirSurfaceIntersectionSourceInfo::RivReservoirSurfaceIntersectionSourceInfo( RivSurfaceIntersectionGeometryGenerator* geometryGenerator )
    : m_intersectionGeometryGenerator( geometryGenerator )
{
    CVF_ASSERT( m_intersectionGeometryGenerator.notNull() );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const std::vector<size_t>& RivReservoirSurfaceIntersectionSourceInfo::triangleToCellIndex() const
{
    CVF_ASSERT( m_intersectionGeometryGenerator.notNull() );

    return m_intersectionGeometryGenerator->triangleToCellIndex();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::array<cvf::Vec3f, 3> RivReservoirSurfaceIntersectionSourceInfo::triangle( int triangleIdx ) const
{
    std::array<cvf::Vec3f, 3> tri;
    tri[0] = ( *m_intersectionGeometryGenerator->triangleVxes() )[triangleIdx * 3];
    tri[1] = ( *m_intersectionGeometryGenerator->triangleVxes() )[triangleIdx * 3 + 1];
    tri[2] = ( *m_intersectionGeometryGenerator->triangleVxes() )[triangleIdx * 3 + 2];

    return tri;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimSurfaceInView* RivReservoirSurfaceIntersectionSourceInfo::intersection() const
{
    return m_intersectionGeometryGenerator->intersection();
}
