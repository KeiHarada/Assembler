cmake_minimum_required(VERSION 3.8)
project(assembler CXX)
set(CMAKE_CXX_STANDARD 11)

add_executable( ${PROJECT_NAME}
                assembler.hpp
                assembler.cpp
                Sensor.cpp
                SCP.cpp
                input.cpp
                evolvingIntervalExtraction.cpp
                segmentingTimeSeries.cpp
                clusteringSegment.cpp
                scpSearch.cpp
                output.cpp
                )
