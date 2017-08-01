/*    Copyright (c) 2010-2017, Delft University of Technology
 *    All rigths reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 *
 */

#include "spice.h"

namespace tudat
{

namespace simulation_setup
{

//! Get the set of spice kernels to be used for a SimulationType.
std::vector< boost::filesystem::path > getSpiceKernels( const SimulationType simulationType )
{
    std::vector< std::string > filenames;
    switch ( simulationType ) {
    case singlePerturbedBody:
        filenames = { "pck00009.tpc", "de-403-masses.tpc", "de421.bsp" };
    default:
        throw std::runtime_error( "Could not determine Spice kernels for the specified simulation type." );
    }

    std::vector< boost::filesystem::path > kernels;
    for ( const std::string filename : filenames )
    {
        kernels.push_back( boost::filesystem::path( input_output::getSpiceKernelPath( ) ) / filename );
    }
    return kernels;
}


//! Create a `json` object from a shared pointer to a `SpiceSettings` object.
void to_json( json& jsonObject, const boost::shared_ptr< SpiceSettings >& spiceSettings )
{
    if ( ! spiceSettings )
    {
        return;
    }
    using namespace json_interface;
    using K = Keys::Spice;

    jsonObject[ K::kernels ] = spiceSettings->kernels_;
    jsonObject[ K::preloadKernels ] = spiceSettings->preloadKernels_;
    if ( spiceSettings->preloadKernels_ )
    {
        jsonObject[ K::preloadOffsets ] = spiceSettings->preloadOffsets_;
    }
}

//! Create a shared pointer to a `SpiceSettings` object from a `json` object.
void from_json( const json& jsonObject, boost::shared_ptr< SpiceSettings >& spiceSettings )
{
    using namespace json_interface;
    using K = Keys::Spice;

    const boost::shared_ptr< SimulationType > simulationType
            = getOptional< SimulationType >( jsonObject, SpecialKeys::root / Keys::simulationType );
    if ( simulationType )
    {
        spiceSettings = boost::make_shared< SpiceSettings >( *simulationType );
    }
    else
    {
        spiceSettings = boost::make_shared< SpiceSettings >(
                    getValue< std::vector< path > >( jsonObject, K::kernels ) );
        updateFromJSON( spiceSettings->preloadKernels_, jsonObject, K::preloadKernels, false );
        if ( spiceSettings->preloadKernels_ )
        {
            std::pair< double, double > preloadOffsets =
                    getValue( jsonObject, K::preloadOffsets, spiceSettings->preloadOffsets_ );
            preloadOffsets.first = getValue( jsonObject, K::startPreloadOffset,
                                             - getValue( jsonObject, K::preloadOffset, - preloadOffsets.first ) );
            preloadOffsets.second = getValue( jsonObject, K::endPreloadOffset,
                                              getValue( jsonObject, K::preloadOffset, preloadOffsets.second ) );
            spiceSettings->preloadOffsets_ = preloadOffsets;
        }
    }
}

} // namespace simulation_setup

} // namespace tudat