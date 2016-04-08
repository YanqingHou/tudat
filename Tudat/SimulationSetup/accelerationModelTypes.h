/*    Copyright (c) 2010-2016, Delft University of Technology
 *    All rigths reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */


#ifndef TUDAT_ACCELERATIONMODELTYPES_H
#define TUDAT_ACCELERATIONMODELTYPES_H

#include "Tudat/Astrodynamics/ElectroMagnetism/cannonBallRadiationPressureAcceleration.h"
#include "Tudat/Astrodynamics/Gravitation/centralGravityModel.h"
#include "Tudat/Astrodynamics/Gravitation/sphericalHarmonicsGravityModel.h"
#include "Tudat/Astrodynamics/Gravitation/thirdBodyPerturbation.h"
#include "Tudat/Astrodynamics/Aerodynamics/aerodynamicAcceleration.h"


namespace tudat
{

namespace simulation_setup
{

//! List of accelerations available in simulations
/*!
 *  List of accelerations available in simulations. Acceleration models not defined by this
 *  given enum cannot be used for automatic acceleration model setup.
 */
enum AvailableAcceleration
{
    undefined_acceleration,
    central_gravity,
    constant_drag_aerodynamic,
    aerodynamic,
    cannon_ball_radiation_pressure,
    spherical_harmonic_gravity,
    third_body_central_gravity,
    third_body_spherical_harmonic_gravity
};

//! Class for providing settings for acceleration model.
/*!
 *  Class for providing settings for acceleration model. This class is a functional (base) class for
 *  settings of acceleration models that  require no information in addition to their type.
 *  Classes defining settings for acceleration models requiring additional information must be
 *  derived from this class.
 *  Bodies exerting and undergong acceleration are set externally from this class.
 *  This class can be used for the easy setup of acceleration models
 *  (see createAccelerationModels.h), but users may also chose to do so manually.
 *  (Derived) Class members are all public, for ease of access and modification.
 */
class AccelerationSettings
{
public:
    //! Constructor, sets type of acceleration.
    /*!
     *  Constructor, sets type of acceleration.
     *  \param accelerationType Type of acceleration from AvailableAcceleration enum.
     */
    AccelerationSettings( const AvailableAcceleration accelerationType ):
        accelerationType_( accelerationType ){ }

    //! Destructor.
    virtual ~AccelerationSettings( ){ }

    //! Type of acceleration from AvailableAcceleration enum.
    AvailableAcceleration accelerationType_;

};

//! Class for providing settings for spherical harmonics acceleration model.
/*!
 *  Class for providing settings for spherical harmonics acceleration model,
 *  specifically the maximum degree and order up to which the field is to be expanded. Note that
 *  the minimum degree and order are currently always set to zero.
 */
class SphericalHarmonicAccelerationSettings: public AccelerationSettings
{
public:
    //! Constructor to set maximum degree and order that is to be taken into account.
    /*!
     *  Constructor to set maximum degree and order that is to be taken into account.
     *  \param maximumDegree Maximum degree
     *  \param maximumOrder Maximum order
     */
    SphericalHarmonicAccelerationSettings( const int maximumDegree,
                                           const int maximumOrder ):
        AccelerationSettings( spherical_harmonic_gravity ), maximumDegree_( maximumDegree ),
        maximumOrder_( maximumOrder ){ }

    //! Maximum degree that is to be used for spherical harmonic acceleration
    int maximumDegree_;

    //! Maximum order that is to be used for spherical harmonic acceleration
    int maximumOrder_;
};

//! Function to identify the derived class type of an acceleration model.
/*!
 *  Function to identify the derived class type of an acceleration model. The type must be defined
 *  in the AvailableAcceleration enum to be recognized by this function.
 *  \param accelerationModel Acceleration model of which the type is to be identified.
 *  \return Type of the accelerationModel, as identified by AvailableAcceleration enum.
 */
AvailableAcceleration getAccelerationModelType(
        const boost::shared_ptr< basic_astrodynamics::AccelerationModel< Eigen::Vector3d > >
        accelerationModel );

//! Typedef defining a list of accelerations acting on a single body, key is the name of each
//! body exerting a acceletation, value is a list of accelerations exerted by that body.
typedef std::map< std::string, std::vector<
boost::shared_ptr< basic_astrodynamics::AccelerationModel< Eigen::Vector3d > > > >
SingleBodyAccelerationMap;

//! Typedef defining a list of accelerations acting on a set of bodies, key is the name of each
//! body undergoing a acceletation, value is SingleBodyAccelerationMap, defining all accelerations
//! acting on it.
typedef std::map< std::string, SingleBodyAccelerationMap > AccelerationMap;

//! Typedef defining a list of acceleration settings, set up in the same manner as the
//! AccelerationMap typedef.
typedef std::map< std::string, std::map< std::string, std::vector< boost::shared_ptr<
AccelerationSettings > > > > SelectedAccelerationMap;

}

}

#endif // TUDAT_ACCELERATIONMODELTYPES_H
