

#ifndef SNTK_NOTRACKING_VIEWER_HPP
#define SNTK_NOTRACKING_VIEWER_HPP

// Bayeux:
#include <bayeux/datatools/properties.h>

// Falaise
#include "sntk/notracking/no_tracking_driver.hpp"

namespace sntk {

  namespace notracking {

    class viewer
    {
    public:

      enum class display_mode {
	text,
	graphics
      };
      
      viewer(const datatools::properties & config_);

      ~viewer() = default;

      void display(const no_tracking_driver::working_space & ws_);
	
      void display_text(const no_tracking_driver::working_space & ws_);
	
      void display_graphics(const no_tracking_driver::working_space & ws_);

    private:

      display_mode _dmode_ = display_mode::text; ///< Display mode (default: text)
      
      // ... some rendering resources...
      
    };

  } // end of namespace notracking

} // end of namespace sntk

#endif // SNTK_NOTRACKING_VIEWER_HPP
