
#ifndef SNTK_TRACKING_MODULE_HPP
#define SNTK_TRACKING_MODULE_HPP

// Standard library
#include <string>

// This project:

// Interface from Falaise
#include "bayeux/dpp/base_module.h"
#include "bayeux/geomtools/manager.h"
#include "falaise/snemo/processing/module.h"
#include "falaise/snemo/geometry/gg_locator.h"
#include "falaise/snemo/processing/base_tracking_driver.h"
#include <falaise/snemo/processing/detector_description.h>

namespace sntk {

  class tracking_module
    : public dpp::base_module
  {
  public:

    tracking_module();

    // Default destructor
    ~tracking_module() override;

    /// Getting detector description
    const snemo::processing::detector_description &
    get_detector_description() const;
  
    //! Configure the module
    //!
    //! Example of supported configuration parameters:
    //!
    //! General parameters for the module itself
    //! ========================================
    //! 
    //! # Verbosity of the processing module:
    //! logging.priority : string = "debug"
    //!
    //! # Names of the input and output banks used by the tracking module:
    //! EH_tag  : string = "EH"
    //! ECD_tag : string = "CD"
    //! TCD_tag : string = "TCD"
    //! TTD_tag : string = "TTD"
    //!
    //! # Name of the embeded  tracking driver 
    //! driver : string = "notracking"
    //!
    //! General parameters of the tracking driver
    //! =========================================
    //!
    //! # Verbosity of the embeded tracking driver 
    //! notracking.verbosity : string = "debug"
    //!
    //! # Service requirements of the embeded tracking driver 
    //! notracking.services.geometry      : boolean = true
    //! notracking.services.calo_locator  : boolean = false
    //! notracking.services.xcalo_locator : boolean = false
    //! notracking.services.gg_locator    : boolean = true
    //! notracking.services.cell_status   : boolean = false
    //! # # Masking rules for Geiger cells (experts only):
    //! # cell_id_mask_rules : string = "..."
    //!
    //! # Tracker pre-clusterizer preprocessing of the tracking driver:
    //! notracking.TPC.delayed_hit_cluster_time : real as time = 10.0 us
    //! notracking.TPC.processing_prompt_hits   : boolean = true
    //! notracking.TPC.processing_delayed_hits  : boolean = true
    //! notracking.TPC.split_chamber            : boolean = true
    //!
    //! Parameters specific to the tracking driver
    //! ==========================================
    //! 
    //! notracking.visu : boolean = false		
    //! notracking.angular_tolerance : real as angle = 2.5 degree
    //! 
    void initialize(const datatools::properties & config_,
		    datatools::service_manager & services_,
		    dpp::module_handle_dict_type & /* modules_ */) override;

    //! Reset the module
    void reset() override;

    // Process event
    dpp::base_module::process_status process(datatools::things & event_record_) override;
 
  private:

    // Configuration:
    std::string _eh_tag_; ///< Event Header bank name
    std::string _cd_tag_; ///< Calibrated Data bank name
    std::string _tcd_tag_; ///< TrackerClustering Data bank name
    std::string _ttd_tag_; ///< Tracker Trajectory Data bank name

    // Internal working resources:
    snemo::processing::detector_description _detector_desc_;
    snemo::processing::base_tracking_driver * _driver_ = nullptr; ///< Embeded tracking driver 
    
    DPP_MODULE_REGISTRATION_INTERFACE(tracking_module);

  };
  
} // end of namespace sntk

#endif // SNTK_TRACKING_MODULE_HPP
