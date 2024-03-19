// Ourselves
#include "sntk/notracking/no_tracking_driver.hpp"

// Standard library
#include <stdexcept>

// Bayeux/Falaise:
#include <bayeux/datatools/service_manager.h>
#include <falaise/snemo/datamodels/tracker_clustering_data.h>
#include <falaise/snemo/datamodels/tracker_trajectory_data.h>

// This project
#include "sntk/notracking/viewer.hpp"

namespace sntk {

  namespace notracking {

    void no_tracking_driver::working_space::clear()
    {
      input.clear();
      output.clear();
      return;
    }

    // Destructor's implementation must be provided here after inclusion of the viwer header
    // because of the unique_ptr attribute.
    no_tracking_driver::~no_tracking_driver() = default;

    void no_tracking_driver::config::check() const
    {
      if (angular_tolerance <= 0.0) {
	throw std::domain_error("Invalid angular tolerance");
      }
      return;
    }
 
    no_tracking_driver::no_tracking_driver()
      : base_tracking_driver("notracking")
    {
    }

    // override
    void no_tracking_driver::_at_initialize(const datatools::properties & config_,
					    datatools::service_manager & services_)
    {
      if (datatools::logger::is_debug(get_verbosity())) {
	boost::property_tree::ptree popts;
	popts.put("indent", "[debug] ");
	popts.put("title", "Fake Tracking Driver Configuration");
	config_.print_tree(std::cerr, popts);
      }
      if (datatools::logger::is_debug(get_verbosity())) {
	DT_LOG_DEBUG(get_verbosity(), "Number of accessible services: " << services_.get_local_services().size());
      }
    
      // Extract the config of tracking module:
      falaise::property_set ps{config_};
   
      // Fetch configuration parameters:
      if (config_.has_flag("visu")) {
	_cfg_.visu = true;
      }

      if (config_.has_key("angular_tolerance")) {
	_cfg_.angular_tolerance = config_.fetch_real_with_explicit_dimension("angular_tolerance", "angle");
      }
      _cfg_.check();

      // Populate the geometry info object from the embeded detector description
      _geoinfo_ = geometry::make(get_detector_description());

      if (_cfg_.visu) {
	DT_LOG_DEBUG(get_verbosity(), "Instantiating the visualisation viewer...");
	auto viewerConfig = ps.get<falaise::property_set>("viewer", {});
	_viewer_ = std::make_unique<viewer>(viewerConfig);
      }
      
      return;
    }

    // override
    void no_tracking_driver::_at_reset()
    {
      if (_viewer_) {
	DT_LOG_DEBUG(get_verbosity(), "Destroying the visualisation viewer...");
	_viewer_.reset();
      }
      return;
    }

    // override
    int no_tracking_driver::process_tracking(const snemo::processing::tracking_precluster_collection_type & preclusters_,
					     const base_tracking_driver::calo_hit_collection_type & calo_hits_,
					     snemo::datamodel::tracker_clustering_data & clustering_,
					     snemo::datamodel::tracker_trajectory_data & track_fitting_)
    {

      // From this point, the internal  working space object is usable
      // to host all intermediate informations created and manipulated
      // by the tracking algorithm.

      // First we clear the current content of the working space object 
      _ws_.clear();
     
      // Second, we reference the working space's input data:
      _ws_.input = ::sntk::input_data(preclusters_, calo_hits_);
      
      /*** ... here the tracking algorithm should run... ***/
      
      if (_viewer_) {
	/*** ... here we could run a dedicated viewer ... ***/
	_viewer_->display(_ws_);
      }

      // Finally, we  export the working space  object's output result
      // in the TCD and TTD objects:
      _export_output_(clustering_, track_fitting_);

      return 0;
    }

    void no_tracking_driver::_export_output_(snemo::datamodel::tracker_clustering_data & clustering_,
					     snemo::datamodel::tracker_trajectory_data & track_fitting_)
    {
      // ... do something ...
      return;
    }
 
  } // end of namespace notracking

} // end of namespace sntk
