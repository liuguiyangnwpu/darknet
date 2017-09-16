#include <iostream>
#include <string>
#include <map>

#include "server.h"

using namespace std;

map<string, string> g_ConfMap;
char **names = NULL;

void service_start(string &ip, string &port) {
    string log_info = "";
    std::string server_address(ip + ":" + port);
    DetectRpcImpl service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    log_info += "Server listening on ";
    log_info += server_address;
    Log::Info(log_info);
    server->Wait();
}

grpc::Status DetectRpcImpl::Detect(grpc::ServerContext* context, const DetectRequest* request, DetectResponse* response) {
    // write your own code
    string req_id = request->req_id();
    string video_name = request->video_name();
    string frame_name = request->frame_name();
    
    response->set_rsp_id(req_id);

    string log_info = "req_id: " + req_id + ", video_name: " + video_name + ", frame_name: " + frame_name;
    Log::Info(log_info);

    string dir_prefix = g_ConfMap["IMAGEDIR"];
    dir_prefix += (dir_prefix.back() == '/' ? "":"/");
    string image_path = dir_prefix + "SRC_" + video_name + "/" + frame_name;
    cout << image_path << endl;
    bool f1 = isExistsFile(image_path);
    if(!f1) {
        response->set_status(false);
        response->set_err(frame_name + " not found !");
        response->set_spend_time(0.0);
        cerr << image_path << " not found !" << endl;
        Log::Error(image_path + " not found !");
    }
    // detect_single_image(char *filename, float thresh, float hier_thresh, char **names);
    float thresh = stof(g_ConfMap["THRESH"]);
    float hier_thresh = stof(g_ConfMap["HIER"]);
    double st_time = what_time_is_it_now();
    map<pair<int, int>, image> crop_images;
    handle_big_image(strdup(image_path.c_str()), crop_images);
    for(auto &iter : crop_images) {
        image im = iter.second;
        cout << im.h << "," << im.w << "," << im.c << endl;
        detect_single_image(im, thresh, hier_thresh, names);
    }
    
    double ed_time = what_time_is_it_now();
    response->set_status(true);
    response->set_err("");
    response->set_spend_time(ed_time - st_time);
    return grpc::Status::OK;
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

    Log::Initialise(g_ConfMap["LOGPATH"]);
    Log::SetThreshold(Log::LOG_TYPE_INFO);
    PUSH_LOG_STACK;

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
    model_start_init(strdup(labelfile.c_str()), strdup(cfgfile.c_str()), strdup(weightfile.c_str()), names);
    // 再启动服务
    string ip = g_ConfMap["SERVERIP"];
    string port = g_ConfMap["PORT"];
    service_start(ip, port);
    return 0;
}
