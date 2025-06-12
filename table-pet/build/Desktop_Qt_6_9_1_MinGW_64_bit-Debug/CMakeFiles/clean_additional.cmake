# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\table-pet_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\table-pet_autogen.dir\\ParseCache.txt"
  "table-pet_autogen"
  )
endif()
