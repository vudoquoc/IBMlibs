/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "IBstaticFvMesh.H"

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::IBdynamicFvMesh> Foam::IBdynamicFvMesh::New(const IOobject& io)
{
    // Note: - do not register the dictionary since IBdynamicFvMeshes themselves
    // do this.
    // - defaultRegion (region0) gets loaded from constant, other ones
    //   get loaded from constant/<regionname>. Normally we'd use
    //   polyMesh::dbDir() but we haven't got a polyMesh yet ...
    IOobject dictHeader
    (
        "dynamicMeshDict",
        io.time().constant(),
        (io.name() == polyMesh::defaultRegion ? "" : io.name()),
        io.db(),
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
    );

    if (dictHeader.typeHeaderOk<IOdictionary>(true))
    {
        IOdictionary dict(dictHeader);

        const word IBdynamicFvMeshTypeName(dict.lookup("IBdynamicFvMesh"));

        Info<< "Selecting IBdynamicFvMesh " << IBdynamicFvMeshTypeName << endl;

        const_cast<Time&>(io.time()).libs().open
        (
            dict,
            "IBdynamicFvMeshLibs",
            IOobjectConstructorTablePtr_
        );

        if (!IOobjectConstructorTablePtr_)
        {
            FatalErrorInFunction
                << "IBdynamicFvMesh table is empty"
                << exit(FatalError);
        }

        IOobjectConstructorTable::iterator cstrIter =
            IOobjectConstructorTablePtr_->find(IBdynamicFvMeshTypeName);

        if (cstrIter == IOobjectConstructorTablePtr_->end())
        {
            FatalErrorInFunction
                << "Unknown IBdynamicFvMesh type "
                << IBdynamicFvMeshTypeName << nl << nl
                << "Valid IBdynamicFvMesh types are :" << endl
                << IOobjectConstructorTablePtr_->sortedToc()
                << exit(FatalError);
        }

        return autoPtr<IBdynamicFvMesh>(cstrIter()(io));
    }
    else
    {
        return autoPtr<IBdynamicFvMesh>(new IBstaticFvMesh(io));
    }
}


// ************************************************************************* //
