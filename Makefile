GPU=1
CUDNN=1
OPENMP=1
GRPC=1

ARCH= -gencode arch=compute_52,code=compute_52

VPATH=./source/neutral/:./source/main:./source/rpc:./source/utils:./source/utils/json
EXEC=server
OBJDIR=./obj/

CC=g++
NVCC=nvcc 
AR=ar
ARFLAGS=rcs
OPTS=-Ofast
LDFLAGS= -std=c++11 -lm -pthread
COMMON= -Isource/include/ -Isource/neutral/ -Isource/rpc -Isource/utils
CFLAGS=-Wall -Wno-unknown-pragmas -Wfatal-errors -Wno-write-strings -Wnarrowing -fPIC

ifeq ($(OPENMP), 1) 
CFLAGS+= -fopenmp
endif

ifeq ($(GPU), 1) 
COMMON+= -DGPU -I/usr/local/cuda/include/
CFLAGS+= -DGPU
LDFLAGS+= -L/usr/local/cuda/lib64 -L/usr/local/nvidia/lib64 -lcuda -lcudart -lcublas -lcurand
endif

ifeq ($(CUDNN), 1) 
COMMON+= -DCUDNN 
CFLAGS+= -DCUDNN
LDFLAGS+= -lcudnn
endif

CFLAGS+=$(OPTS)

OBJ=gemm.o utils.o cuda.o deconvolutional_layer.o convolutional_layer.o \
	list.o image.o activations.o im2col.o col2im.o blas.o crop_layer.o \
	dropout_layer.o maxpool_layer.o softmax_layer.o data.o matrix.o network.o \
	connected_layer.o cost_layer.o parser.o option_list.o detection_layer.o \
	route_layer.o box.o normalization_layer.o avgpool_layer.o layer.o \
	local_layer.o shortcut_layer.o activation_layer.o gru_layer.o \
	batchnorm_layer.o region_layer.o reorg_layer.o tree.o darknet.o \
	jsoncpp.o lgy_utils.o log.o server.o

# OBJ=$(patsubst %.cpp,%.o,$(wildcard,source/neutral/*.cpp))
# OBJ+=$(patsubst %.cpp,%.o,$(wildcard,source/main/*.cpp))

EXECOBJA=server.o

ifeq ($(GPU), 1)
LDFLAGS+= -lstdc++ 
OBJ+=convolutional_kernels.o deconvolutional_kernels.o activation_kernels.o im2col_kernels.o \
	 col2im_kernels.o blas_kernels.o crop_layer_kernels.o dropout_layer_kernels.o \
	 maxpool_layer_kernels.o network_kernels.o avgpool_layer_kernels.o
OBJ+=$(patsubst %.cu,%.o,$(wildcard,source/neutral/*.cu))
endif

ifeq ($(GRPC), 1)
CFLAGS+= `pkg-config --cflags protobuf grpc`
LDFLAGS+= `pkg-config --libs protobuf grpc++ grpc` \
          -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed \
          -ldl
OBJ+=darknet.pb.o darknet.grpc.pb.o
# OBJ+=$(patsubst %.cu,%.o,$(wildcard,source/rpc/*.cpp))
endif


EXECOBJ = $(addprefix $(OBJDIR), $(EXECOBJA))
OBJS = $(addprefix $(OBJDIR), $(OBJ))
DEPS = $(wildcard source/neutral/*.h) Makefile source/include/*.h source/rpc/*.h source/utils/*.h source/utils/json/*.h

all: obj $(OBJS) $(EXEC)

$(EXEC): $(EXECOBJ) $(OBJS)
	$(CC) $(COMMON) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJDIR)%.o: %.cpp $(DEPS)
	$(CC) $(COMMON) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(OBJDIR)%.grpc.pb.o: %.grpc.pb.cc $(DEPS)
	$(CC) $(COMMON) $(CFLAGS) -c $< -o $@ $(LDFLAGS)
	
$(OBJDIR)%.pb.o: %.pb.cc $(DEPS)
	$(CC) $(COMMON) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(OBJDIR)%.o: %.cu $(DEPS)
	$(NVCC) $(ARCH) $(COMMON) --compiler-options "$(CFLAGS)" -c $< -o $@

obj:
	mkdir -p obj

.PHONY: clean

clean:
	rm -rf $(OBJS) $(EXEC) $(EXECOBJ)
