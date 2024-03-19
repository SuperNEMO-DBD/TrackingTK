=========================================
SuperNEMO TrackingTK

=========================================

:description: Falaise tracking plugin for SuperNEMO data reconstruction
:date: 2024-03-18
:authors: Tomáš Križák, Maros Petro, Miroslav Macko, François Mauger


Introduction
=============
          
The TrackingTK package is a  Falaise plugin which provides a *tracking
module* able  to run a  specific tracking algorithm  (called *tracking
driver*) to  identify clusters  of Geiger cell  hits in  the SuperNEMO
detector and fit associated charged particles' trajectories.

This package only works with Falaise version >= 5.1.1. It makes use of
the new  ``snemo::processing::base_tracking_driver`` class  from which
any tracking driver class should inherit.

A ``sntk::notracking::no_tracking_driver`` dummy  class is provided to
illustrate  the  integration  of  a   tracking  driver  class  in  the
``sntk::tracking_module``.   It  does  **not**  operate  any  tracking
reconstruction and lets the **TCD** and **TTD** banks empty.  However,
it implements three mandatory following virtual methods in order to be
usable as an embeded driver in the ``sntk::tracking_module`` :

- .. code:: c++

     void _at_initialize(const   datatools::properties   &   config_,
                         datatools::service_manager  & services_) override;
  ..

  This method configures and initializes the internal working space of
  the tracking driver.

- .. code:: c++

     void _at_reset() override;
  ..
 
  This method terminates and resets  the internal working space of the
  tracking driver.

- .. code:: c++
            
     int process_tracking(const std::vector<snemo::processing::tracking_precluster> & preclusters_,
                          const base_tracking_driver::calo_hit_collection_type & calo_hits_,
                          snemo::datamodel::tracker_clustering_data & clustering_,
                          snemo::datamodel::tracker_trajectory_data & track_fitting_) override;
  ..
  
  This methods operates  the tracking algorithm. It takes  a vector of
  preclusters of  Geiger hits  as input  data and  possibly a  list of
  calorimeter hits.   Tracker preclusters as well  as calorimeter hits
  are automatically extracted from the  input **CD** bank.  The method
  aims  to fill  the **TCD**  and  **TTD** banks  with identified  and
  measured   tracks  of   charged  particles   (clusters  and   fitted
  trajectories).

The   ``snemo::processing::tracking_precluster``  data   structure  is
described in the Falaise ``snemo/processing/tracking_utils.h`` header.
It corresponds to the tracker's input data used by any tracking driver
to build **TCD** and **TTD** banks.  These clusters are build from the
**CD**  bank  using a  *pre-clustering  algorithm*  which is  able  to
identify sets of Geiger hits on  each side of the tracking chamber and
also to separate candidate *prompt* and *delayed* clusters.
  
This  design  allows people  to  enrich  the TrackingTK  package  with
additional   tracking  algorithms/drivers   that  implement   specific
reconstruction   approaches   (Legendre   transform   based,   maximum
likelihood based...).


Class layout
=================


.. code::

     Falaise                       Falaise
     +------------------+          +------------------------+
     | dpp::base_module |          | snemo::processing::    |
     +------------------+          |   base_tracking_driver |
               ^ [inherit]         +------------------------+
               |                                ^      ^
               |                       [inherit]|      |
               |                                |      |
   TrackingTK  |                                |      |
   +-----------+-----------+                    |      |
   | sntk::tracking_module |                    |      |    
   +-----------------------+       TrackingTK   |      `--------.
   | - driver              |--+-<> +------------+------------+  |
   +-----------------------+  |    |  some_tracking_driver   |  |
                              |    +-------------------------+  |
                              |    | - _at_initialize(...)   |  |
                         [own]|    | - _at_reset()           |  |
                              |    | - process_tracking(...) |  |
                              |    +-------------------------+  |
                              |                   _____________.'
                              |                 .'              
                              |    TrackingTK   |
                              +-<> +------------+------------+
                                   |  other_tracking_driver  |
                                   +-------------------------+
                                   | - _at_initialize(...)   |
                                   | - _at_reset()           |
                                   | - process_tracking(...) |
                                   +-------------------------+
..




.. end
   
