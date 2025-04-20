# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appMyWindownApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appMyWindownApp_autogen.dir\\ParseCache.txt"
  "appMyWindownApp_autogen"
  )
endif()
