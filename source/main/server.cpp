#include <iostream>
#include <string>
#include <map>

#include "server.h"

using namespace std;

map<string, string> g_ConfMap;

void service_start(string &ip, string &port) {
    std::string server_address(ip + ":" + port);
    DetectRpcImpl service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char **argv) {
    if(argc != 2) {
        cerr << "Input parameter is invalid !" << endl;
        return 0;
    }
    string configPath(argv[1]);
    try {
        read_config_Json(configPath, g_ConfMap);
    } catch (runtime_error &err) {
        cerr << err.what() << endl;
        exit(1);
    }

    gpu_index = stoi(g_ConfMap["GPU"]);

#ifndef GPU
    gpu_index = -1;
#else
    if(gpu_index >= 0){
        cuda_set_device(gpu_index);
    }
#endif
    string labelfile = g_ConfMap["LABELPATH"];
    string cfgfile = g_ConfMap["CFGPATH"];
    string weightfile = g_ConfMap["WEIGHTPATH"];
    bool f1 = isExistsFile(labelfile);
    bool f2 = isExistsFile(cfgfile);
    bool f3 = isExistsFile(weightfile);
    if(!(f1 && f2 && f3)) {
        cerr << "LABELPATH, CFGPATH, WEIGHTPATH not Found !" << endl;
        return 0;
    }
    // 先 load model
    char **names = NULL;
    model_start_init(strdup(labelfile.c_str()), strdup(cfgfile.c_str()), strdup(weightfile.c_str()), names);
    // 再启动服务
    string ip = g_ConfMap["SERVERIP"];
    string port = g_ConfMap["PORT"];
    service_start(ip, port);
    return 0;
}
