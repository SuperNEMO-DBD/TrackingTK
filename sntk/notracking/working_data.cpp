// Ourselves
#include "sntk/notracking/working_data.hpp"

// Bayeux
#include <bayeux/datatools/clhep_units.h>

namespace sntk {

  namespace notracking {

    int get_id(const tracker_hit_handle & hhit_)
    {
      return hhit_->get_id();
    }
    
    int get_side(const tracker_hit_handle & hhit_)
    {
      return hhit_->get_side();
    }

    int get_row(const tracker_hit_handle & hhit_)
    {
      return hhit_->get_row();
    }

    int get_layer(const tracker_hit_handle & hhit_)
    {
      return hhit_->get_layer();
    }
    
    double get_x_mm(const tracker_hit_handle & hhit_)
    {
      return hhit_->get_x() / CLHEP::mm;
    }
    
    double get_y_mm(const tracker_hit_handle & hhit_)
    {
      return hhit_->get_y() / CLHEP::mm;
    }
    
    double get_z_mm(const tracker_hit_handle & hhit_)
    {
      return hhit_->get_z() / CLHEP::mm;
    }
   
    double get_sigma_z_mm(const tracker_hit_handle & hhit_)
    {
      return hhit_->get_sigma_z() / CLHEP::mm;
    }
     
    double get_r_mm(const tracker_hit_handle & hhit_)
    {
      return hhit_->get_r() / CLHEP::mm;
    }
   
    double get_sigma_r_mm(const tracker_hit_handle & hhit_)
    {
      return hhit_->get_sigma_r() / CLHEP::mm;
    }
     
    bool cluster::has_data() const
    {
      return id >= 0 and hits.size() > 0;
    }
 
    void cluster::clear()
    {
      id = -1;
      hits.clear();
      return;
    }
    
    void cluster::print(std::ostream & out_) const
    {
      out_ << "Cluster:\n";
      out_ << "|-- ID : " << id << '\n';
      out_ << "`-- Number of hits : " << hits.size() << '\n';
      return;
    }

    bool fit_info::has_data() const
    {
      return not std::isnan(chi2) and ndof >= 0;
    }
    
    void fit_info::clear()
    {
      chi2 = std::numeric_limits<double>::quiet_NaN();
      ndof = -1;
      pvalue = std::numeric_limits<double>::quiet_NaN();
      return;
    }

    bool line_params::has_data() const
    {
      return not std::isnan(phi) and
	not std::isnan(rho);
    }
      
    void line_params::clear()
    {
      phi = std::numeric_limits<double>::quiet_NaN();
      rho = std::numeric_limits<double>::quiet_NaN();
      return;
     }
  
    bool helix_params::has_data() const
    {
      return not std::isnan(radius);
    }

    void helix_params::clear()
    {
      radius = std::numeric_limits<double>::quiet_NaN();
      return;
    }
     
    bool track::has_data() const
    {
      return id >= 0 and hcluster != nullptr
	and (line.has_data() or helix.has_data())
	and fit_quality.has_data();
    }
 
    void track::clear()
    {
      id = -1;
      hcluster = nullptr;
      line.clear();
      helix.clear();
      fit_quality.clear();
      return;
    }

    bool track::is_line() const
    {
      return not std::isnan(line.phi);
    }

    bool track::is_helix() const
    {
      return not std::isnan(helix.radius);
    }
    
    void track::print(std::ostream & out_) const
    {
      out_ << "Track:\n";
      out_ << "|-- ID : " << id << '\n';
      out_ << "|-- Cluster ID : ";
      if (hcluster) {
	out_ << hcluster->id;
      } else {
	out_ << "none";
      }
      out_ << '\n';	
      out_ << "|-- Fitted as : ";
      if (is_line()) {
	out_ << "'line'";
      } else if (is_helix()) {
	out_ << "'helix'";
      } else {
	out_ << "none";
      }
      out_ << '\n';
      out_ << "`-- Fit quality : " << '\n';
      out_ << "    "
	   << "|-- " << "Chi2 = " << fit_quality.chi2 << '\n';
      out_ << "    "
	   << "|-- " << "ndof = " << fit_quality.ndof << '\n';
      out_ << "    "
	   << "`-- " << "P-value = " << fit_quality.pvalue << '\n';
      out_ << '\n';
      return;
    }

    void output_data::clear()
    {
      clusters.clear();
      tracks.clear();
      return;
    }
     
    void output_data::print(std::ostream & out_) const
    {
      out_ << "|-- " << "Number of reconstructed clusters : " << clusters.size() << '\n';
      out_ << "`-- " << "Number of reconstructed tracks : " << tracks.size() << '\n';
      return;
    }
   
  } // end of namespace notracking

} // end of namespace sntk
