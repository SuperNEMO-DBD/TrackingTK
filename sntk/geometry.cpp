#include "sntk/geometry.hpp"

// Bayeux:
#include <bayeux/datatools/exception.h>

namespace sntk {

  void geometry::print(std::ostream & out_, const std::string & indent_) const
  {
    out_ << indent_ << "|-- Tracker cell radius : " << tc_radius << " mm" << '\n';
    out_ << indent_ << "|-- Tracker cell length : " << tc_sizez << " mm" << '\n';
    out_ << indent_ << "|-- Foil frame width    : " << foil_spacex << " mm" << '\n';
    out_ << indent_ << "`-- Tracker cell type   : " << tc_type << '\n';
    return;
  }

  // static
  geometry geometry::make(const snemo::processing::detector_description & dd_)
  {
    DT_THROW_IF(not dd_.has_gg_locator(), std::logic_error,
		"No Geiger cell locator");
    // DT_THROW_IF(not dd_.has_cell_status_service(), std::logic_error,
    // 		"No Geiger cell status service");
    const auto & ggLocator = dd_.get_gg_locator();
    const auto & ggStatusService = dd_.get_cell_status_service();
    
    geometry geo;
    geo.tc_radius = ggLocator.cellRadius() / CLHEP::mm;
    geo.tc_sizez = ggLocator.anodeWireLength() / CLHEP::mm;
    geo.tc_type = ggLocator.cellGIDType();
    // int drift_cell_cathodic_ring_type = dd_.get_geometry_manager();
    geomtools::geom_id first_layer_gid(geo.tc_type, 0, 0, 0, 0);
    geomtools::vector_3d first_layer_cell_pos = ggLocator.getCellPosition(first_layer_gid);
    double x_cell_pos = first_layer_cell_pos.x();
    geo.foil_spacex = 2 * (std::abs(x_cell_pos) - geo.tc_radius);
    geo.print(std::cerr, "[debug] ");
    return geo;
  }
    
} // end of namespace sntk
