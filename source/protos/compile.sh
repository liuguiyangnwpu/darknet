# for server in cpp language
protoc -I . --grpc_out=../rpc --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` darknet.proto
protoc -I . --cpp_out=../rpc darknet.proto


# for client for pyhton language
protoc -I=. --python_out=../../client/rpc darknet.proto
python -m grpc_tools.protoc -I . --python_out=../../client/rpc --grpc_python_out=../../client/rpc darknet.proto
