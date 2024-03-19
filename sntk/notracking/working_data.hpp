// Internal working datamodels used by the algorithm
//
// The class/data structures declared in this header
// have voluntarily public internal scope. This is because
// they are intended to be used only from within some private parts
// of the tracking algorithm, typically fom the 'process_tracking'
// method. Thus, noone or nothing else should need to access such
// variable.


#ifndef SNTK_NOTRACKING_WORKING_DATA_HPP
#define SNTK_NOTRACKING_WORKING_DATA_HPP

// Standard library
#include <iostream>
#include <vector>
#include <limits>

// Falaise
#include <falaise/snemo/datamodels/calibrated_tracker_hit.h>

namespace sntk {

  namespace notracking {

    /// Calibrated tracker hit handle
    typedef snemo::datamodel::TrackerHitHdl tracker_hit_handle;

    // Utility functions
    int get_id(const tracker_hit_handle &);
    int get_side(const tracker_hit_handle &);
    int get_row(const tracker_hit_handle &);
    int get_layer(const tracker_hit_handle &);
    double get_x_mm(const tracker_hit_handle &);
    double get_y_mm(const tracker_hit_handle &);
    double get_z_mm(const tracker_hit_handle &);
    double get_sigma_z_mm(const tracker_hit_handle &);
    double get_r_mm(const tracker_hit_handle &);
    double get_sigma_r_mm(const tracker_hit_handle &);
    // ... more utility functions if needed ...
    
    /// Candidate track cluster
    struct cluster
    {
      int id = -1;
      std::vector<tracker_hit_handle> hits;
      /// ... more attributes if needed ...

      bool has_data() const;
      void clear();
      void print(std::ostream & out_) const;
    };

    // Dummy example of a class describing the quality of a fit
    struct fit_info
    {
      double chi2 = std::numeric_limits<double>::quiet_NaN();
      int ndof = -1;
      double pvalue = std::numeric_limits<double>::quiet_NaN();
      /// ... more attributes if needed ...

      bool has_data() const;
      void clear();
    };
    
    // Dummy example of a class describing a fitted line
    struct line_params // 2D version (Hess representation)
    {
      double phi = std::numeric_limits<double>::quiet_NaN();
      double rho = std::numeric_limits<double>::quiet_NaN();
      /// ... more attributes if needed ...

      bool has_data() const;
      void clear();
    };

    // Dummy example of a class describing a fitted helix
    struct helix_params // 2D version
    {
      double radius = std::numeric_limits<double>::quiet_NaN();
      /// ... more attributes if needed ...
      
      bool has_data() const;
      void clear();
    };

    /// Fitted track
    struct track
    {
      int id = -1; ///< Track ID
      const cluster * hcluster = nullptr; ///< Associated cluster handle
      line_params     line; ///< Fitted line
      helix_params    helix; ///< Fitted helix
      fit_info        fit_quality; ///< Fit quality
      /// ... more attributes if needed ...
      
      bool has_data() const;
      void clear();
      bool is_line() const;
      bool is_helix() const;
      void print(std::ostream & out_) const;
    };

    /// Output internal result of the algorithm before conversion to the Falaise TCD/TTD datamodel
    struct output_data
    {
      std::vector<cluster> clusters; ///< Array of candidate cluster
      std::vector<track> tracks; ///< Array of associated tracks (several tracks maybe associated to a same cluster)
      /// ... more attributes if needed ...
      
      void clear();
      void print(std::ostream & out_) const;
    }; 

  } // end of namespace notracking

} // end of namespace sntk

#endif // SNTK_NOTRACKING_WORKING_DATA_HPP
