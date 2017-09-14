GPU=0
CUDNN=1
OPENMP=1
GRPC=1
DEBUG=0

ARCH= -gencode arch=compute_20,code=[sm_20,sm_21] \
      -gencode arch=compute_30,code=sm_30 \
      -gencode arch=compute_35,code=sm_35 \
      -gencode arch=compute_50,code=[sm_50,compute_50] \
      -gencode arch=compute_52,code=[sm_52,compute_52]

# This is what I use, uncomment if you know your arch and want to specify
# ARCH= -gencode arch=compute_52,code=compute_52

VPATH=./source/include:./source/neutral/:./source/main:./source/rpc:./source/utils
EXEC=server
OBJDIR=./obj/

CC=g++
NVCC=nvcc 
AR=ar
ARFLAGS=rcs
OPTS=-Ofast
LDFLAGS= -std=c++11 -lm -pthread
COMMON= -Isource/include/ -Isource/neutral/ -Isource/rpc -Isource/utils -I/usr/local/cuda/include
CFLAGS=-std=c++11 -Wall -Wno-unknown-pragmas -Wfatal-errors -Wno-write-strings -Wnarrowing -fPIC

ifeq ($(OPENMP), 1) 
CFLAGS+= -fopenmp
endif

ifeq ($(DEBUG), 1) 
OPTS=-O0 -g
endif

CFLAGS+=$(OPTS)

OBJ=gemm.o utils.o cuda.o deconvolutional_layer.o convolutional_layer.o \
	list.o image.o activations.o im2col.o col2im.o blas.o crop_layer.o \
	dropout_layer.o maxpool_layer.o softmax_layer.o data.o matrix.o network.o \
	connected_layer.o cost_layer.o parser.o option_list.o detection_layer.o \
	route_layer.o box.o normalization_layer.o avgpool_layer.o layer.o \
	local_layer.o shortcut_layer.o activation_layer.o gru_layer.o \
	batchnorm_layer.o region_layer.o reorg_layer.o tree.o darknet.o

EXECOBJA=server.o

ifeq ($(GPU), 1)
LDFLAGS+= -lstdc++ 
OBJ+=convolutional_kernels.o deconvolutional_kernels.o activation_kernels.o im2col_kernels.o \
	 col2im_kernels.o blas_kernels.o crop_layer_kernels.o dropout_layer_kernels.o \
	 maxpool_layer_kernels.o network_kernels.o avgpool_layer_kernels.o
endif

ifeq ($(GRPC), 1)
CFLAGS+= `pkg-config --cflags protobuf grpc`
LDFLAGS+= -L/root/grpc `pkg-config --libs protobuf grpc++ grpc` \
          -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed \
          -ldl
OBJ+=darknet.pb.o darknet.grpc.pb.o
endif


EXECOBJ = $(addprefix $(OBJDIR), $(EXECOBJA))
OBJS = $(addprefix $(OBJDIR), $(OBJ))
DEPS = $(wildcard source/neutral/*.h) Makefile source/include/*.h

all: obj $(EXEC)

$(EXEC): $(EXECOBJ)
	$(CC) $(COMMON) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJDIR)%.o: %.cpp $(DEPS)
	$(CC) $(COMMON) $(CFLAGS) -c $< -o $@

$(OBJDIR)%.pb.o: %.cpp $(DEPS)
	$(CC) $(COMMON) $(CFLAGS) -c $< -o $@

$(OBJDIR)%.grpc.pb.o: %.cpp $(DEPS)
	$(CC) $(COMMON) $(CFLAGS) -c $< -o $@

$(OBJDIR)%.o: %.cu $(DEPS)
	$(NVCC) $(ARCH) $(COMMON) --compiler-options "$(CFLAGS)" -c $< -o $@

obj:
	mkdir -p obj

.PHONY: clean

clean:
	rm -rf $(OBJS) $(EXEC) $(EXECOBJ)


