cmake_minimum_required(VERSION 3.5)
project(DarkNetServer)

set(CMAKE_CXX_STANDARD 11)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -Wno-write-strings)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wnarrowing -fPIC)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

include_directories("/usr/local/include")
include_directories("source/include")
include_directories("source/neutral")
link_directories("/usr/local/lib")


option (USE_GPU "Use GPU nvcc" ON)
option (USE_OPENMP "Use openmp" ON)
option (USE_GRPC "Use grpc" ON)

if (USE_OPENMP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif(USE_OPENMP)

if (USE_GPU)
    add_definitions(-DGPU)
    find_package(CUDA REQUIRED)
    include_directories(/usr/local/cuda/include)
    link_directories(/usr/local/cuda/lib64)
    set(CUDA_HOST_COMPILATION_CPP ON)
    #set(CUDA_NVCC_FLAGS -v -lstdc++ -Xcompiler -fexceptions -Xcompiler -fPIC)
    set(CUDA_NVCC_FLAGS -lstdc++ -lcudadevrt -Xcompiler -fPIC)
    #set(CUDA_NVCC_FLAGS  "${CUDA_NVCC_FLAGS} -O3 -std=c++11")
    #set(CUDA_NVCC_FLAGS  "${CUDA_NVCC_FLAGS} -gencode arch=compute_20,code=[sm_20,sm_21]")
    #set(CUDA_NVCC_FLAGS  "${CUDA_NVCC_FLAGS} -gencode arch=compute_30,code=sm_30")
    #set(CUDA_NVCC_FLAGS  "${CUDA_NVCC_FLAGS} -gencode arch=compute_35,code=sm_35")
    #set(CUDA_NVCC_FLAGS  "${CUDA_NVCC_FLAGS} -gencode arch=compute_50,code=[sm_50,compute_50]")
    set(CUDA_NVCC_FLAGS  "${CUDA_NVCC_FLAGS} -gencode arch=compute_52,code=compute_52")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    set(CUDA_SEPARABLE_COMPILATION ON)
    set(CUDA_PROPAGATE_HOST_FLAGS OFF)
endif(USE_GPU)

set(SOURCE_FILES
        source/include/data_struct.h
        source/neutral/activation_layer.cpp
        source/neutral/activation_layer.h
        source/neutral/activations.cpp
        source/neutral/activations.h
        source/neutral/avgpool_layer.cpp
        source/neutral/avgpool_layer.h
        source/neutral/batchnorm_layer.cpp
        source/neutral/batchnorm_layer.h
        source/neutral/blas.cpp
        source/neutral/blas.h
        source/neutral/box.cpp
        source/neutral/box.h
        source/neutral/col2im.cpp
        source/neutral/col2im.h
        source/neutral/compare.cpp
        source/neutral/connected_layer.cpp
        source/neutral/connected_layer.h
        source/neutral/convolutional_layer.cpp
        source/neutral/convolutional_layer.h
        source/neutral/cost_layer.cpp
        source/neutral/cost_layer.h
        source/neutral/crop_layer.cpp
        source/neutral/crop_layer.h
        source/neutral/cuda.cpp
        source/neutral/cuda.h
        source/neutral/data.cpp
        source/neutral/data.h
        source/neutral/deconvolutional_layer.cpp
        source/neutral/deconvolutional_layer.h
        source/neutral/detection_layer.cpp
        source/neutral/detection_layer.h
        source/neutral/dropout_layer.cpp
        source/neutral/dropout_layer.h
        source/neutral/gemm.cpp
        source/neutral/gemm.h
        source/neutral/gru_layer.cpp
        source/neutral/gru_layer.h
        source/neutral/im2col.cpp
        source/neutral/im2col.h
        source/neutral/image.cpp
        source/neutral/image.h
        source/neutral/layer.cpp
        source/neutral/layer.h
        source/neutral/list.cpp
        source/neutral/list.h
        source/neutral/local_layer.cpp
        source/neutral/local_layer.h
        source/neutral/matrix.cpp
        source/neutral/matrix.h
        source/neutral/maxpool_layer.cpp
        source/neutral/maxpool_layer.h
        source/neutral/network.cpp
        source/neutral/network.h
        source/neutral/normalization_layer.cpp
        source/neutral/normalization_layer.h
        source/neutral/option_list.cpp
        source/neutral/option_list.h
        source/neutral/parser.cpp
        source/neutral/parser.h
        source/neutral/region_layer.cpp
        source/neutral/region_layer.h
        source/neutral/reorg_layer.cpp
        source/neutral/reorg_layer.h
        source/neutral/route_layer.cpp
        source/neutral/route_layer.h
        source/neutral/shortcut_layer.cpp
        source/neutral/shortcut_layer.h
        source/neutral/softmax_layer.cpp
        source/neutral/softmax_layer.h
        source/neutral/stb_image.h
        source/neutral/stb_image_write.h
        source/neutral/tree.cpp
        source/neutral/tree.h
        source/neutral/utils.cpp
        source/neutral/utils.h
        source/neutral/activation_kernels.cu 
        source/neutral/avgpool_layer_kernels.cu 
        source/neutral/blas_kernels.cu 
        source/neutral/col2im_kernels.cu 
        source/neutral/convolutional_kernels.cu 
        source/neutral/crop_layer_kernels.cu 
        source/neutral/deconvolutional_kernels.cu 
        source/neutral/dropout_layer_kernels.cu 
        source/neutral/im2col_kernels.cu 
        source/neutral/maxpool_layer_kernels.cu 
        source/neutral/network_kernels.cu 
        source/utils/json/json-forwards.h
        source/utils/json/json.h
        source/utils/jsoncpp.cpp
        source/utils/log.cpp
        source/utils/log.h
        source/utils/lgy_utils.cpp
        source/utils/lgy_utils.h
        source/rpc/darknet.grpc.pb.cc
        source/rpc/darknet.grpc.pb.h
        source/rpc/darknet.pb.cc
        source/rpc/darknet.pb.h
        source/include/darknet.h
        source/include/server.h
        source/main/darknet.cpp
        source/main/server.cpp)

if (USE_GPU)
    cuda_add_executable(DarkNetServerGPU ${SOURCE_FILES})
endif(USE_GPU)

if (USE_GRPC)
    set(GRPC_LIBS protobuf pthread grpc++ grpc)
    message("${GRPC_LIBS}")
endif(USE_GRPC)

if (USE_GPU)
    target_link_libraries(DarkNetServerGPU ${CUDA_LIBRARIES} ${GRPC_LIBS})
endif(USE_GPU)
