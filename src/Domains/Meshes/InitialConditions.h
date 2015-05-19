/**
 * @file    InitialConditions.h
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
 * This file contains the interface for class InitialConditions, which
 * is used for generating initial conditions on fields.
 */

#ifndef INITIAL_CONDITIONS_H
#define INITIAL_CONDITIONS_H

#include <fstream>
#include <string>

#include "Point3D.h"
#include "HexaFvmMesh.h"
#include "Field.h"

class InitialConditions
{
private:

    HexaFvmMesh* meshPtr_;
    std::ifstream inputFile_;

    int nCellsI_, nCellsJ_, nCellsK_;

public:

    InitialConditions();

    void initialize(HexaFvmMesh& mesh);

    void openInputFile(std::string filename, std::string directory);

    void createUniform(double value, Field<double>& phiField);
    void createSphere(double radius, Point3D center, double sphereInnerValue, Field<double>& phiField);
    void createBox(double xLength, double yLength, double zLength, Point3D center, double boxInnerValue, Field<double>& phiField);

};

#endif
