# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "stm32f103_aht20_baremetal.bin"
  "stm32f103_aht20_baremetal.hex"
  "stm32f103_aht20_baremetal.map"
  )
endif()
