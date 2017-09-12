#pragma once

#include <iostream>
#include <string>
#include <map>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <grpc++/grpc++.h>
#include "darknet.h"
#include "../utils/log.h"
#include "../utils/utils.h"
#include "../rpc/darknet.grpc.pb.h"
#include "../rpc/darknet.pb.h"

using namespace std;

using darknet::TargetRect;
using darknet::DetectRequest;
using darknet::DetectResponse;
using darknet::DetectRpc;

void service_start(string &ip, string &port);

class DetectRpcImpl final : public DetectRpc::Service {
    grpc::Status Detect(grpc::ServerContext* context, const DetectRequest* request, DetectResponse* response) override {
        // write your own code
        return grpc::Status::OK;
    }
};
