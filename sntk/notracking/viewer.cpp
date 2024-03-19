// Ourselves
#include "sntk/notracking/viewer.hpp"

// Bayeux:
#include <bayeux/datatools/exception.h>

namespace sntk {

  namespace notracking {

    viewer::viewer(const datatools::properties & config_)
    {
      if (config_.has_key("display_mode")) {
	std::string display_mode_label = config_.fetch_string("display_mode");
	if (display_mode_label == "text") {
	  _dmode_ = display_mode::text;
	} else if (display_mode_label == "graphics") {
	  _dmode_ = display_mode::graphics;
	} else {
	  DT_THROW(std::logic_error,
		   "Unsupported display mode '" << display_mode_label << "'!");
	}
      }
      return;
    }

    void viewer::display(const no_tracking_driver::working_space & ws_)
    {
      if (_dmode_ == display_mode::text) {
	display_text(ws_);
      } else if (_dmode_ == display_mode::graphics) {
	display_graphics(ws_);
      }
      return;
    }
	
    void viewer::display_text(const no_tracking_driver::working_space & ws_)
    {
      std::ostream & out = std::cout;
      out << "Working space's input data:\n";
      ws_.input.print(out);
      out << "Working space's output data:\n";
      ws_.output.print(out);
      out << std::flush;
      return;
    }
	
    void viewer::display_graphics(const no_tracking_driver::working_space & ws_)
    {
      DT_LOG_NOTICE(datatools::logger::PRIO_NOTICE,
		    "Not implemented yet");
      return;
    }


  } // end of namespace notracking

} // end of namespace sntk

