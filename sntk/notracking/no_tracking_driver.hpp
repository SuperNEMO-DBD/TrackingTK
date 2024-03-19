#ifndef SNTK_NOTRACKING_NO_TRACKING_DRIVER_HPP
#define SNTK_NOTRACKING_NO_TRACKING_DRIVER_HPP

// Standard library
#include <cmath>
#include <memory>

// Falaise module:
#include "falaise/snemo/processing/base_tracking_driver.h"
#include "falaise/snemo/processing/tracking_utils.h"

// This project
#include "sntk/geometry.hpp"
#include "sntk/datamodels.hpp"
#include "sntk/notracking/working_data.hpp"

namespace sntk {

  namespace notracking {

    class viewer; ///< Forward declaration of the debug/development viewer
    
    /// \brief Dummy 'no tracking' algorithm
    ///
    /// This  is only  a demonstrator  class which  performs no  track
    /// reconstruction (no clustering nor track fitting).  However the
    /// structure of this class  illustrates the implementation of any
    /// tracking  driver  to  be  used from  the  TrackingTK package's
    /// tracking module.
    ///
    class no_tracking_driver
      : public snemo::processing::base_tracking_driver
    {
      
    public:
      
      /// \brief Configuration parameters:
      struct config
      {
	bool visu = false; ///< Dummy visualization activation flag
	double angular_tolerance = M_PI / 180.0; ///< Dummy angular tolerance (default: 1 degree)
	// ... more parameters if needed ...
      
	/// Parameters checking (throws if a parametrization error is detected)
	void check() const;
	
      };

      /// \brief Internal work space used by the tracking algorithm
      struct working_space
      {
	input_data  input; ///< Readonly input data
	// ... intermediate data structure if needed ...
	output_data output; ///< Algorithm's final results

	/// Clear all data in working space object
	void clear();
      };

      /// Default constructor       
      no_tracking_driver();

      /// Destructor
      ~no_tracking_driver() override;

    protected:

      /// Initialize the tracking driver (mandatory Falaise interface)
      ///
      /// This method is used to extract configuration parameters from the 'config_' argument
      /// and possibly to access specific services needed by the algorithm.
      ///
      void _at_initialize(const datatools::properties & config_,
			  datatools::service_manager & services_) override;

      /// Reset the tracking driver (mandatory Falaise interface)
      ///
      /// This method is used to properly terminate and dismantle some internal resources
      /// used by the algorithm.
      ///
      void _at_reset() override;

    public:
         
      /// Tracking algorithm (mandatory Falaise interface)
      ///
      /// This method implements the core tracking algorithm.
      ///
      int process_tracking(const snemo::processing::tracking_precluster_collection_type & preclusters_,
			   const base_tracking_driver::calo_hit_collection_type & calo_hits_,
			   snemo::datamodel::tracker_clustering_data & clustering_,
			   snemo::datamodel::tracker_trajectory_data & track_fitting_) override;

    private :

      /// Final export of the tracking algorithm result (working space's output) to the
      /// Falaise data models (TCD+TTD)
      void _export_output_(snemo::datamodel::tracker_clustering_data & clustering_,
			   snemo::datamodel::tracker_trajectory_data & track_fitting_);
      
    private:
     
      config _cfg_; ///< Driver configuration
      geometry _geoinfo_; ///< Geometry information
      working_space _ws_; ///< Working space of the tracking algorithm
      std::unique_ptr<viewer> _viewer_;
      
    };

  } // end of namespace notracking

} // end of namespace sntk

#endif // SNTK_NOTRACKING_NO_TRACKING_DRIVER_HPP
