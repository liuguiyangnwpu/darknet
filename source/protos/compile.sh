protoc -I . --grpc_out=../rpc --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` darknet.proto
protoc -I . --cpp_out=../rpc darknet.proto


