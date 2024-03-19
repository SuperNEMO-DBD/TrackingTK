
#ifndef SNTK_DATAMODELS_HPP
#define SNTK_DATAMODELS_HPP

// Standard library
#include <iostream>
#include <limits>
#include <functional>

// Falaise module:
#include "falaise/snemo/processing/base_tracking_driver.h"
#include "falaise/snemo/processing/tracking_utils.h"

namespace sntk {

  /// \brief Data structure referencing input calibrated data prepared for all tracking drivers
  ///
  /// All reference data structures are supposedly extracted from the event record's "CD" bank
  /// and tagged as const (readlony).
  ///
  struct input_data
  {
    input_data() = default;

    input_data(const snemo::processing::tracking_precluster_collection_type & tracker_preclusters_,
	       const snemo::processing::base_tracking_driver::calo_hit_collection_type & calo_hits_);

    /// Check if the object contains intput data
    bool has_data() const;
    
    /// Return a const reference to the array of tracker preclusters
    const snemo::processing::tracking_precluster_collection_type & tracker_preclusters() const; 

    /// Return a const reference to the array of calo gits
    const snemo::processing::base_tracking_driver::calo_hit_collection_type & calo_hits() const;

    /// Basic print
    void print(std::ostream & out_) const;

    /// Clear all referenced input data
    void clear();
    
  private:

    /// Array of tracker preclusters
    const snemo::processing::tracking_precluster_collection_type * _tracker_preclusters_ = nullptr; 

    /// Array of calibrated calo hits (possibly unused)
    const snemo::processing::base_tracking_driver::calo_hit_collection_type * _calo_hits_ = nullptr;
   
  };

} // end of namespace sntk

#endif // SNTK_DATAMODELS_HPP
