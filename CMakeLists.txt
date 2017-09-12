cmake_minimum_required(VERSION 3.5)
project(darknet)

set(CMAKE_CXX_STANDARD 11)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -Wno-write-strings")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnarrowing -Wmisleading-indentation -fPIC")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

option (USE_GPU "Use GPU nvcc" OFF)
option (USE_OPENMP "Use openmp" ON)
option (USE_GRPC "Use grpc" ON)

if (USE_OPENMP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif(USE_OPENMP)

if (USE_GRPC)
    set(EXTRA_LIBS "protobuf pthread grpc++ grpc")
endif(USE_GRPC)

include_directories("/usr/local/include")
include_directories("source/include")
include_directories("source/neutral")
LINK_DIRECTORIES("/usr/local/lib")

set(SOURCE_FILES
        source/neutral/activation_layer.c
        source/neutral/activation_layer.h
        source/neutral/activations.c
        source/neutral/activations.h
        source/neutral/avgpool_layer.c
        source/neutral/avgpool_layer.h
        source/neutral/batchnorm_layer.c
        source/neutral/batchnorm_layer.h
        source/neutral/blas.c
        source/neutral/blas.h
        source/neutral/box.c
        source/neutral/box.h
        source/neutral/col2im.c
        source/neutral/col2im.h
        source/neutral/compare.c
        source/neutral/connected_layer.c
        source/neutral/connected_layer.h
        source/neutral/convolutional_layer.c
        source/neutral/convolutional_layer.h
        source/neutral/cost_layer.c
        source/neutral/cost_layer.h
        source/neutral/crop_layer.c
        source/neutral/crop_layer.h
        source/neutral/cuda.c
        source/neutral/cuda.h
        source/neutral/data.c
        source/neutral/data.h
        source/neutral/deconvolutional_layer.c
        source/neutral/deconvolutional_layer.h
        source/neutral/detection_layer.c
        source/neutral/detection_layer.h
        source/neutral/dropout_layer.c
        source/neutral/dropout_layer.h
        source/neutral/gemm.c
        source/neutral/gemm.h
        source/neutral/gru_layer.c
        source/neutral/gru_layer.h
        source/neutral/im2col.c
        source/neutral/im2col.h
        source/neutral/image.c
        source/neutral/image.h
        source/neutral/layer.c
        source/neutral/layer.h
        source/neutral/list.c
        source/neutral/list.h
        source/neutral/local_layer.c
        source/neutral/local_layer.h
        source/neutral/matrix.c
        source/neutral/matrix.h
        source/neutral/maxpool_layer.c
        source/neutral/maxpool_layer.h
        source/neutral/network.c
        source/neutral/network.h
        source/neutral/normalization_layer.c
        source/neutral/normalization_layer.h
        source/neutral/option_list.c
        source/neutral/option_list.h
        source/neutral/parser.c
        source/neutral/parser.h
        source/neutral/region_layer.c
        source/neutral/region_layer.h
        source/neutral/reorg_layer.c
        source/neutral/reorg_layer.h
        source/neutral/route_layer.c
        source/neutral/route_layer.h
        source/neutral/shortcut_layer.c
        source/neutral/shortcut_layer.h
        source/neutral/softmax_layer.c
        source/neutral/softmax_layer.h
        source/neutral/stb_image.h
        source/neutral/stb_image_write.h
        source/neutral/tree.c
        source/neutral/tree.h
        source/neutral/utils.c
        source/neutral/utils.h
        source/utils/json/json-forwards.h
        source/utils/json/json.h
        source/utils/jsoncpp.cpp
        source/utils/log.cpp
        source/utils/log.h
        source/utils/utils.cpp
        source/utils/utils.h
        source/rpc/darknet.grpc.pb.cc
        source/rpc/darknet.grpc.pb.h
        source/rpc/darknet.pb.cc
        source/rpc/darknet.pb.h
        source/include/darknet.h
        source/include/server.h
        source/main/darknet.cpp
        source/main/server.cpp)

add_executable(darknet ${SOURCE_FILES})
target_link_libraries(darknet protobuf pthread grpc++ grpc)