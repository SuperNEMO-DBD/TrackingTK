
#ifndef SNTK_GEOMETRY_HPP
#define SNTK_GEOMETRY_HPP

// Standard library
#include <limits>
#include <iostream>
#include <string>

// Falaise:
#include <falaise/snemo/processing/detector_description.h>

namespace sntk {

  struct geometry
  {
    // All dimensions are in mm:
    double tc_radius = std::numeric_limits<double>::quiet_NaN();
    double tc_sizez = std::numeric_limits<double>::quiet_NaN();
    double foil_spacex = std::numeric_limits<double>::quiet_NaN();
    int tc_type = -1;

    void print(std::ostream & out_, const std::string & indent_) const;

    // Factory:
    static geometry make(const snemo::processing::detector_description & dd_);
   
  };

} // end of namespace sntk

#endif // SNTK_GEOMETRY_HPP
