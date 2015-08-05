/**
 * @file    FieldI.h
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
 * This file contains the templated method implementations for class
 * Field.
 */

#include "Field.h"
#include "Output.h"

// ************* Constructors and Destructors *************

template<class T>
Field<T>::Field(const HexaFvmMesh &mesh, FieldType type, std::string name)
    :
      mesh_(mesh),
      type_(type),
      name(name)
{
    allocate(mesh_.nCellsI(), mesh_.nCellsJ(), mesh_.nCellsK());
}

template<class T>
Field<T>::Field(const Field &other)
    :
      Field<T>::Field(other.mesh_, other.type_, other.name + "_copy")
{
    operator =(other);
}

// ************* Public Methods *************

template<class T>
Field<T>& Field<T>::operator =(const Field<T>& other)
{
    int i;

    //- Check to make sure this assignment is legal. Fields cannont be equated to other fields that reference a different mesh
    if(&mesh_ != &other.mesh_)
        Output::raiseException("Field", "operator=", "cannot assign a field to another field if the mesh references are different.");

    for(i = 0; i < Array3D<T>::n_; ++i)
        Array3D<T>::data_[i] = other.data_[i];

    if(type_ == CONSERVED && other.type_ == CONSERVED)
    {
        for(i = 0; i < facesI_.size(); ++i)
            facesI_(i) = other.facesI_(i);

        for(i = 0; i < facesJ_.size(); ++i)
            facesJ_(i) = other.facesJ_(i);

        for(i = 0; i < facesK_.size(); ++i)
            facesK_(i) = other.facesK_(i);
    }

    return *this;
}

template<class T>
void Field<T>::allocate(int nCellsI, int nCellsJ, int nCellsK)
{
    Array3D<T>::allocate(nCellsI, nCellsJ, nCellsK);

    if (type_ == CONSERVED)
    {
        facesI_.allocate(nCellsI + 1, nCellsJ, nCellsK);
        facesJ_.allocate(nCellsI, nCellsJ + 1, nCellsK);
        facesK_.allocate(nCellsI, nCellsJ, nCellsK + 1);
    }

    eastBoundaryPatch.allocate(1, mesh_.nCellsJ(), mesh_.nCellsK());
    westBoundaryPatch.allocate(1, mesh_.nCellsJ(), mesh_.nCellsK());
    northBoundaryPatch.allocate(mesh_.nCellsI(), 1, mesh_.nCellsK());
    southBoundaryPatch.allocate(mesh_.nCellsI(), 1, mesh_.nCellsK());
    topBoundaryPatch.allocate(mesh_.nCellsI(), mesh_.nCellsJ(), 1);
    bottomBoundaryPatch.allocate(mesh_.nCellsI(), mesh_.nCellsJ(), 1);
}

template<class T>
T& Field<T>::operator()(int i, int j, int k)
{
    if(i >= 0 && j >= 0 && k >= 0 &&
            i < Array3D<T>::nI_ && j < Array3D<T>::nJ_ && k < Array3D<T>::nK_)
    {
        return Array3D<T>::operator ()(i, j, k);
    }

    // Access to the boundaries
    if(i < 0)
    {
        return westBoundaryPatch(abs(i) - 1, j, k);
    }
    else if (i >= Array3D<T>::nI_)
    {
        return eastBoundaryPatch(i - Array3D<T>::nI_, j, k);
    }

    if(j < 0)
    {
        return southBoundaryPatch(i, abs(j) - 1, k);
    }
    else if (j >= Array3D<T>::nJ_)
    {
        return northBoundaryPatch(i, j - Array3D<T>::nJ_, k);
    }

    if(k < 0)
    {
        return bottomBoundaryPatch(i, j, abs(k) - 1);
    }
    else if (k >= Array3D<T>::nK_)
    {
        return topBoundaryPatch(i, j, k - Array3D<T>::nK_);
    }

    // Just to get rid of the compiler warning
    return Array3D<T>::data_[0];
}

template<class T>
const T& Field<T>::operator()(int i, int j, int k) const
{
    if(i >= 0 && j >= 0 && k >= 0 &&
            i < Array3D<T>::nI_ && j < Array3D<T>::nJ_ && k < Array3D<T>::nK_)
    {
        return Array3D<T>::operator ()(i, j, k);
    }

    // Access to the boundaries
    if(i < 0)
    {
        return westBoundaryPatch(abs(i) - 1, j, k);
    }
    else if (i >= Array3D<T>::nI_)
    {
        return eastBoundaryPatch(i - Array3D<T>::nI_, j, k);
    }

    if(j < 0)
    {
        return southBoundaryPatch(i, abs(j) - 1, k);
    }
    else if (j >= Array3D<T>::nJ_)
    {
        return northBoundaryPatch(i, j - Array3D<T>::nJ_, k);
    }

    if(k < 0)
    {
        return bottomBoundaryPatch(i, j, abs(k) - 1);
    }
    else if (k >= Array3D<T>::nK_)
    {
        return topBoundaryPatch(i, j, k - Array3D<T>::nK_);
    }

    // Just to get rid of the compiler warning
    return Array3D<T>::data_[0];
}

template<class T>
T& Field<T>::operator ()(int i, int j, int k, int faceNo)
{
    switch(faceNo)
    {
    case 0:
        return operator ()(i + 1, j, k);
    case 1:
        return operator ()(i - 1, j, k);
    case 2:
        return operator ()(i, j + 1, k);
    case 3:
        return operator ()(i, j - 1, k);
    case 4:
        return operator ()(i, j, k + 1);
    case 5:
        return operator ()(i, j, k - 1);
    default:
        Output::raiseException("Field", "operator()", "invalid face number specified.");
    };

    return operator ()(i, j, k);
}

template<class T>
const T& Field<T>::operator ()(int i, int j, int k, int faceNo) const
{
    switch(faceNo)
    {
    case 0:
        return operator ()(i + 1, j, k);
    case 1:
        return operator ()(i - 1, j, k);
    case 2:
        return operator ()(i, j + 1, k);
    case 3:
        return operator ()(i, j - 1, k);
    case 4:
        return operator ()(i, j, k + 1);
    case 5:
        return operator ()(i, j, k - 1);
    default:
        Output::raiseException("Field", "operator()", "invalid face number specified.");
    };

    return operator ()(i, j, k);
}

template<class T>
T& Field<T>::operator ()(int k)
{
    if(k < 0 || k >= Array3D<T>::n_)
        Output::raiseException("Field", "operator()", "Attempted to access an element outside the bounds of the field.");

    return Array3D<T>::data_[k];
}

template<class T>
const T& Field<T>::operator ()(int k) const
{
    if(k < 0 || k >= Array3D<T>::n_)
        Output::raiseException("Field", "operator()", "Attempted to access an element outside the bounds of the field.");

    return Array3D<T>::data_[k];
}

template<class T>
void Field<T>::getSubfield(int iLower, int iUpper,
                           int jLower, int jUpper,
                           int kLower, int kUpper,
                           Array3D<T> &subField) const
{
    int i, j, k, ii, jj, kk;

    for(k = kLower, kk = 0; k <= kUpper; ++k, ++kk)
    {
        for(j = jLower, jj = 0; j <= jUpper; ++j, ++jj)
        {
            for(i = iLower, ii = 0; i <= iUpper; ++i, ++ii)
            {
                subField(ii, jj, kk) = operator ()(i, j, k);
            }
        }
    }
}

template<class T>
void Field<T>::setValues(int iLower, int iUpper,
                         int jLower, int jUpper,
                         int kLower, int kUpper,
                         T value)
{
    int i, j, k;

    for(k = kLower; k <= kUpper; ++k)
    {
        for(j = jLower; j <= jUpper; ++j)
        {
            for(i = iLower; i <= iUpper; ++i)
            {
                operator ()(i, j, k) = value;
            }
        }
    }
}

template<class T>
void Field<T>::setValue(T value)
{
    int i;

    for(i = 0; i < Array3D<T>::n_; ++i)
        Array3D<T>::data_[i] = value;
}

template<class T>
void Field<T>::setFixedBoundaryPatches(const T *refValues)
{
    int i;

    for(i = 0; i < eastBoundaryPatch.size(); ++i)
        eastBoundaryPatch(i) = refValues[0];

    for(i = 0; i < westBoundaryPatch.size(); ++i)
        westBoundaryPatch(i) = refValues[1];

    for(i = 0; i < northBoundaryPatch.size(); ++i)
        northBoundaryPatch(i) = refValues[2];

    for(i = 0; i < southBoundaryPatch.size(); ++i)
        southBoundaryPatch(i) = refValues[3];

    for(i = 0; i < topBoundaryPatch.size(); ++i)
        topBoundaryPatch(i) = refValues[4];

    for(i = 0; i < bottomBoundaryPatch.size(); ++i)
        bottomBoundaryPatch(i) = refValues[5];
}

template<class T>
void Field<T>::setFixedBoundaryPatches(T refValue)
{
    T refValues[6] = {refValue};

    setFixedBoundaryPatches(refValues);
}

template<class T>
void Field<T>::setAll(T value)
{
    Array3D<T>::setAll(value);
    facesI_.setAll(value);
    facesJ_.setAll(value);
    facesK_.setAll(value);
    setFixedBoundaryPatches(value);
}

template<class T>
void Field<T>::setEastFacesFromPatch()
{
    int j, k;

    for(k = 0; k < Array3D<T>::sizeK(); ++k)
    {
        for(j = 0; j < Array3D<T>::sizeJ(); ++j)
        {
            facesI_(Array3D<T>::sizeI(), j, k) = eastBoundaryPatch(0, j, k);
        }
    }
}

template<class T>
void Field<T>::setWestFacesFromPatch()
{
    int j, k;

    for(k = 0; k < Array3D<T>::sizeK(); ++k)
    {
        for(j = 0; j < Array3D<T>::sizeJ(); ++j)
        {
            facesI_(0, j, k) = westBoundaryPatch(0, j, k);
        }
    }
}

template<class T>
void Field<T>::setNorthFacesFromPatch()
{
    int i, k;

    for(k = 0; k < Array3D<T>::sizeK(); ++k)
    {
        for(i = 0; i < Array3D<T>::sizeI(); ++i)
        {
            facesJ_(i, Array3D<T>::sizeJ(), k) = northBoundaryPatch(i, 0, k);
        }
    }
}

template<class T>
void Field<T>::setSouthFacesFromPatch()
{
    int i, k;

    for(k = 0; k < Array3D<T>::sizeK(); ++k)
    {
        for(i = 0; i < Array3D<T>::sizeI(); ++i)
        {
            facesJ_(i, 0, k) = southBoundaryPatch(i, 0, k);
        }
    }
}

template<class T>
void Field<T>::setTopFacesFromPatch()
{
    int i, j;

    for(j = 0; j < Array3D<T>::sizeJ(); ++j)
    {
        for(i = 0; i < Array3D<T>::sizeI(); ++i)
        {
            facesK_(i, j, Array3D<T>::sizeK()) = topBoundaryPatch(i, j, 0);
        }
    }
}

template<class T>
void Field<T>::setBottomFacesFromPatch()
{
    int i, j;

    for(j = 0; j < Array3D<T>::sizeJ(); ++j)
    {
        for(i = 0; i < Array3D<T>::sizeI(); ++i)
        {
            facesK_(i, j, 0) = bottomBoundaryPatch(i, j, 0);
        }
    }
}

template<class T>
void Field<T>::setZeroGradientBoundaryEast()
{
    getSubfield(mesh_.uCellI(), mesh_.uCellI(), 0, mesh_.uCellJ(), 0, mesh_.uCellK(), eastBoundaryPatch);
    setEastFacesFromPatch();
}

template<class T>
void Field<T>::setZeroGradientBoundaryWest()
{
    getSubfield(0, 0, 0, mesh_.uCellJ(), 0, mesh_.uCellK(), westBoundaryPatch);
    setWestFacesFromPatch();
}

template<class T>
void Field<T>::setZeroGradientBoundaryNorth()
{
    getSubfield(0, mesh_.uCellI(), mesh_.uCellJ(), mesh_.uCellJ(), 0, mesh_.uCellK(), northBoundaryPatch);
    setNorthFacesFromPatch();
}

template<class T>
void Field<T>::setZeroGradientBoundarySouth()
{
    getSubfield(0, mesh_.uCellI(), 0, 0, 0, mesh_.uCellK(), southBoundaryPatch);
    setSouthFacesFromPatch();
}

template<class T>
void Field<T>::setZeroGradientBoundaryTop()
{
    getSubfield(0, mesh_.uCellI(), 0, mesh_.uCellJ(), mesh_.uCellK(), mesh_.uCellK(), topBoundaryPatch);
    setTopFacesFromPatch();
}

template<class T>
void Field<T>::setZeroGradientBoundaryBottom()
{
    getSubfield(0, mesh_.uCellI(), 0, mesh_.uCellJ(), 0, 0, bottomBoundaryPatch);
    setBottomFacesFromPatch();
}

template<class T>
T Field<T>::maxNeighbour(int i, int j, int k)
{
    using namespace std;

    T maxNb = operator ()(i, j, k);

    maxNb = max(maxNb, operator()(i + 1, j, k));
    maxNb = max(maxNb, operator()(i - 1, j, k));
    maxNb = max(maxNb, operator()(i, j + 1, k));
    maxNb = max(maxNb, operator()(i, j - 1, k));
    maxNb = max(maxNb, operator()(i, j, k + 1));
    maxNb = max(maxNb, operator()(i, j, k - 1));

    return maxNb;
}

template<class T>
T Field<T>::minNeighbour(int i, int j, int k)
{
    using namespace std;

    T minNb = operator ()(i, j, k);

    minNb = min(minNb, operator()(i + 1, j, k));
    minNb = min(minNb, operator()(i - 1, j, k));
    minNb = min(minNb, operator()(i, j + 1, k));
    minNb = min(minNb, operator()(i, j - 1, k));
    minNb = min(minNb, operator()(i, j, k + 1));
    minNb = min(minNb, operator()(i, j, k - 1));

    return minNb;
}

template<class T>
T& Field<T>::face(int i, int j, int k, int faceNo)
{
    switch(faceNo)
    {
    case 0:
        return facesI_(i + 1, j, k);
    case 1:
        return facesI_(i, j, k);
    case 2:
        return facesJ_(i, j + 1, k);
    case 3:
        return facesJ_(i, j, k);
    case 4:
        return facesK_(i, j, k + 1);
    case 5:
        return facesK_(i, j, k);
    default:
        Output::raiseException("Field", "face", "attempted to access a face that does not exist.");
    };
}

template<class T>
void Field<T>::print()
{
    using namespace std;

    int i, j, k, l;

    cout << "Field name = " << "\"" << name << "\"\n";

    for(k = 0, l = 0; k < Array3D<T>::nK_; ++k)
    {
        for(j = 0; j < Array3D<T>::nJ_; ++j)
        {
            for(i = 0; i < Array3D<T>::nI_; ++i, ++l)
            {
                cout << Array3D<T>::data_[l] << " ";
            } // end for i

            cout << endl;
        } // end for j

        cout << endl;
    } // end for k
}
