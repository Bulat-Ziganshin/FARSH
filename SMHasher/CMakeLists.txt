project(SMHasher)

cmake_minimum_required(VERSION 2.4)

set(CMAKE_BUILD_TYPE Release)

add_library(
  SMHasherSupport
  AvalancheTest.cpp
  Bitslice.cpp
  Bitvec.cpp
  DifferentialTest.cpp
  FarshTest.cpp
  Hashes.cpp
  KeysetTest.cpp
  Platform.cpp
  Random.cpp
  SpeedTest.cpp
  Stats.cpp
  Types.cpp
)

add_executable(
  SMHasher
  main.cpp
)

target_link_libraries(
  SMHasher
  SMHasherSupport
)
