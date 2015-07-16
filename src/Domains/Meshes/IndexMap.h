/**
 * @file    IndexMap.h
 * @author  Adam O'Brien <obrienadam89@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This file contains the interface for class IndexMap, which is used
 * to produce a map functor that converts between i,j,k indices of a mesh
 * to a global index to be used for assembling matrices.
 */

#ifndef INDEX_MAP_H
#define INDEX_MAP_H

#include "Array3D.h"

class IndexMap
{
public:

    enum CellStatus{ACTIVE, GHOST, INACTIVE};

    IndexMap();

    void initialize(int nCellsI, int nCellsJ, int nCellsK);
    int operator()(int i, int j, int k, int varSetNo);

    int nActive(){ return nActive_; }
    bool isActive(int i, int j, int k);
    bool isGhost(int i, int j, int k);
    bool isInactive(int i, int j, int k);

    void setActive(int i, int j, int k);
    void setGhost(int i, int j, int k);
    void setInactive(int i, int j, int k);
    void generateGlobalIndices();

private:

    int nCellsI_, nCellsJ_, nCellsK_, nActive_;
    Array3D<int> globalIndices_;
    Array3D<CellStatus> cellStatuses_;
};

#endif
