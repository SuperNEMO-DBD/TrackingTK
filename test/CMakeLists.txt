
# 2024-03-17 FM: 
# - Test1
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/test1/simu.conf"       "${CMAKE_CURRENT_BINARY_DIR}/test1/config/simu.conf" @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/test1/variant.profile" "${CMAKE_CURRENT_BINARY_DIR}/test1/config/variant.profile" @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/test1/reco.conf.in"    "${CMAKE_CURRENT_BINARY_DIR}/test1/config/reco.conf" @ONLY)

add_test(NAME trackingtk-test1
  COMMAND
  bash ${CMAKE_CURRENT_SOURCE_DIR}/test1/run.bash
  --work-dir "${CMAKE_CURRENT_BINARY_DIR}/test1/work"
  --cfg-dir "${CMAKE_CURRENT_BINARY_DIR}/test1/config"
  )
set_tests_properties(trackingtk-test1
  PROPERTIES ENVIRONMENT
    "TrackingTK_RESOURCE_DIR=${CMAKE_CURRENT_BINARY_DIR}/test1;PATH=${CMAKE_CURRENT_BINARY_DIR}:$ENV{PATH}"
  )

