#include "sntk/datamodels.hpp"

namespace sntk {

  input_data::input_data(const snemo::processing::tracking_precluster_collection_type & tracker_preclusters_,
			 const snemo::processing::base_tracking_driver::calo_hit_collection_type & calo_hits_)
  {
    _tracker_preclusters_ = &tracker_preclusters_;
    _calo_hits_ = &calo_hits_;
    return;
  }
   
  bool input_data::has_data() const
  {
    return _tracker_preclusters_ != nullptr and _calo_hits_ != nullptr;
  }

  const snemo::processing::tracking_precluster_collection_type &
  input_data::tracker_preclusters() const
  {
    return *_tracker_preclusters_;
  }

  const snemo::processing::base_tracking_driver::calo_hit_collection_type &
  input_data::calo_hits() const
  {
    return *_calo_hits_;
  }

  void input_data::clear()
  {
    _tracker_preclusters_ = nullptr;
    _calo_hits_ = nullptr;
    return;
  }

  void input_data::print(std::ostream & out_) const
  {
    out_ << "|-- Number of tracker pre-clusters : ";
    if (_tracker_preclusters_) {
      out_ << _tracker_preclusters_->size();
    } else {
      out_ << "null";
    }
    out_ << '\n';
    out_ << "`-- Number of calorimeter hits : ";
    if (_calo_hits_) {
      out_ << _calo_hits_->size();
    } else {
      out_ << "null";
    }
    out_ << '\n';
    return;
  }

} // end of namespace sntk
