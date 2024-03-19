// Ourselves:
#include "sntk/tracking_module.hpp"

// Standard library
#include <stdexcept>

// Bayeux/Falaise
#include "bayeux/datatools/exception.h"
#include "falaise/snemo/datamodels/data_model.h"
#include "falaise/snemo/datamodels/event.h"
#include "falaise/snemo/datamodels/event_header.h"
#include "falaise/snemo/datamodels/calibrated_data.h"
#include "falaise/snemo/datamodels/event_header.h"
#include <falaise/snemo/datamodels/tracker_clustering_data.h>
#include <falaise/snemo/datamodels/tracker_trajectory_data.h>

// This project:
#include "sntk/datamodels.hpp"
#include "sntk/notracking/no_tracking_driver.hpp"

namespace sntk {
  
  DPP_MODULE_REGISTRATION_IMPLEMENT(tracking_module, "sntk::tracking_module")
  
  tracking_module::tracking_module() : dpp::base_module() 
  {
    // Default official bank names:
    _eh_tag_ = snedm::labels::event_header();
    _cd_tag_ = snedm::labels::calibrated_data();
    _tcd_tag_ = snedm::labels::tracker_clustering_data();
    _ttd_tag_ = snedm::labels::tracker_trajectory_data();
    return;
  }
  
  tracking_module::~tracking_module() 
  {
    return;
  }
  
  const snemo::processing::detector_description &
  tracking_module::get_detector_description() const
  {
    return _detector_desc_;
  }

  // override
  void tracking_module::initialize(const datatools::properties & config_,
				   datatools::service_manager & services_,
				   dpp::module_handle_dict_type & /* modules_ */) 
  {
    DT_THROW_IF(is_initialized(), std::logic_error,
		"Module '" << get_name() << "' is already initialized ! ");
    dpp::base_module::_common_initialize(config_);
    
    // Extract the config of tracking module:
    falaise::property_set ps{config_};

    // Bank names:
    _eh_tag_ = ps.get<std::string>("EH_tag", snedm::labels::event_header());
    _cd_tag_ = ps.get<std::string>("CD_tag", snedm::labels::calibrated_data());
    _tcd_tag_ = ps.get<std::string>("TCD_tag", snedm::labels::tracker_clustering_data());
    _ttd_tag_ = ps.get<std::string>("TTD_tag", snedm::labels::tracker_trajectory_data());


    // Service management (not used):
    auto servicesPs = ps.get<falaise::property_set>("services", {});
    uint32_t ddFlags= snemo::processing::detector_description::configuration_flags_from(servicesPs);
    _detector_desc_ = snemo::processing::detector_description::make_from_services(services_, ddFlags);

    // // Tracker cell status service :
    // if (not _detector_desc_.has_cell_status_service()) {
    //   snemo::service_handle<snemo::tracker_cell_status_service> cellStatusSVC{services_};
    //   _detector_desc_.set_cell_status_service(*(cellStatusSVC.operator->()));
    // }
    
    // Tracking driver:
    static const std::set<std::string> supportedDriverDrivers = {"notracking"};
    std::string driverName;
    if (config_.has_key("driver")) {
      driverName = config_.fetch_string("driver");
      DT_THROW_IF(not supportedDriverDrivers.count(driverName), std::logic_error,
		  "Unsupported driver name " << driverName << "'");
    }
    if (driverName.empty()) {
      // Default driver:
      driverName == "notracking";
    }

    if (driverName == "notracking") {
      datatools::properties noTrackingConfig;
      config_.export_and_rename_starting_with(noTrackingConfig, "notracking.", "");
      notracking::no_tracking_driver::config cfg;
      _driver_ = new notracking::no_tracking_driver();
      _driver_->initialize(noTrackingConfig, services_);
    } else {
      // ... instantiating other supported drivers...
    }
    
    this->_set_initialized(true);
    return;
  }

  // override
  void tracking_module::reset()
  {
    DT_THROW_IF(not is_initialized(), std::logic_error,
		"Module '" << get_name() << "' is not initialized !");
    this->_set_initialized(false);
    if (_driver_) {
      _driver_->reset();
      delete _driver_;
    }
    return;
  }

  // override
  dpp::base_module::process_status
  tracking_module::process(datatools::things & event_record_)
  {
    DT_THROW_IF(not is_initialized(), std::logic_error,
		"Module '" << get_name() << "' is not initialized !");
    if (not event_record_.has(_eh_tag_)) {
      DT_THROW(std::logic_error, "No '" << _eh_tag_ << "' bank in this event record");
    }
    if (not event_record_.has(_cd_tag_)) {
      DT_THROW(std::logic_error, "No '" << _cd_tag_ << "' bank in this event record");
    }

    // Input Event Header:
    const auto & ehBank = event_record_.get<snemo::datamodel::event_header>(_eh_tag_);
    // 2024-03-18 FM : Should we check for "event_builder" trait from EH bank ?

    // Input Calibrated Data:
    const auto & cdBank = event_record_.get<snemo::datamodel::calibrated_data>(_cd_tag_);

    // Output Tracker Clustering Data:
    auto & tcdBank = ::snedm::addToEvent<snemo::datamodel::tracker_clustering_data>(_tcd_tag_, event_record_);

    // Output Tracker Trajectory Data:
    auto & ttdBank = ::snedm::addToEvent<snemo::datamodel::tracker_trajectory_data>(_ttd_tag_, event_record_);

    int status = _driver_->process(cdBank, tcdBank, ttdBank);
    if (status != 0) {
      return falaise::processing::status::PROCESS_ERROR;
    }
    return falaise::processing::status::PROCESS_OK;
  }

} // end of namespace sntk
