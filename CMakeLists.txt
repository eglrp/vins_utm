cmake_minimum_required(VERSION 2.8.3)
project(vins_utm)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++11 -fopenmp")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -g")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(Eigen3 REQUIRED)
find_package(Ceres REQUIRED)
find_package(Pangolin REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem program_options system)
find_package(OpenMP REQUIRED)


if(OPENMP_FOUND)
	message("OPENMP FOUND")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

option(USE_OPENMP "Use OpenMP for parallelization" ON)
if(USE_OPENMP)
	message(WARNING "Using OpenMP")
endif()

message(WARNING "OpenCV_VERSION: ${OpenCV_VERSION}")

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${EIGEN3_INCLUDE_DIR}
  ${CERES_INCLUDE_DIRS}
  ${Pangolin_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_library(camera_model SHARED
    src/camera_models/chessboard/Chessboard.cc
    src/camera_models/calib/CameraCalibration.cc
    src/camera_models/camera_models/Camera.cc
    src/camera_models/camera_models/CameraFactory.cc
    src/camera_models/camera_models/CostFunctionFactory.cc
    src/camera_models/camera_models/PinholeCamera.cc
    src/camera_models/camera_models/CataCamera.cc
    src/camera_models/camera_models/EquidistantCamera.cc
    src/camera_models/camera_models/ScaramuzzaCamera.cc
    src/camera_models/sparse_graph/Transform.cc
    src/camera_models/gpl/gpl.cc
    src/camera_models/gpl/EigenQuaternionParameterization.cc)

target_link_libraries(camera_model ${Boost_LIBRARIES} ${OpenCV_LIBS} ${CERES_LIBRARIES})


ADD_LIBRARY(MyVio SHARED
    src/System.cpp
    src/parameters.cpp
    src/estimator.cpp
    src/feature_manager.cpp
    src/feature_tracker.cpp

    src/utility/utility.cpp
    src/initial/solve_5pts.cpp
    src/initial/initial_aligment.cpp
    src/initial/initial_sfm.cpp
    src/initial/initial_ex_rotation.cpp

    src/backend/vertex.cc
    src/backend/edge.cc
    src/backend/problem.cc
    src/backend/vertex_pose.cc
    src/backend/edge_reprojection.cc
    src/backend/edge_imu.cc
    src/backend/edge_prior.cpp
    src/backend/loss_function.cc
    src/backend/imu_integration.cc
    )

target_link_libraries(MyVio  
  ${OpenCV_LIBS}  
  ${CERES_LIBRARIES}
	${Pangolin_LIBRARIES}
  camera_model)

ADD_LIBRARY(utmHTTP SHARED src/HttpReq.cpp)
target_link_libraries(utmHTTP   ${OpenCV_LIBS} ${CERES_LIBRARIES} ${Pangolin_LIBRARIES} )

add_executable(run_euroc test/run_euroc.cpp)
target_link_libraries(run_euroc 
  MyVio utmHTTP
  -lpthread) 
  
add_executable(errorEst test/errorEst.cpp)
target_link_libraries(errorEst
		MyVio utmHTTP
		-lpthread)

add_executable(testCurveFitting test/CurveFitting.cpp)
target_link_libraries(testCurveFitting MyVio utmHTTP)

add_executable(testOpenMP test/testOpenMP.cpp)
target_link_libraries(testOpenMP MyVio utmHTTP)

add_executable(testHTTP test/testHttp.cpp)
target_link_libraries(testHTTP MyVio utmHTTP)

add_executable(testHTTPLoop test/testHttpLoop.cpp)
target_link_libraries(testHTTPLoop MyVio utmHTTP)
