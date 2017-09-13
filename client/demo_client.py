# Copyright (c) 2009 IW.
# All rights reserved.
#
# Author: liuguiyang <liuguiyangnwpu@gmail.com>
# Date:   2017/9/12

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import time
import hashlib

import grpc

from rpc import darknet_pb2
from rpc import darknet_pb2_grpc


def create_request():
	video_name = "JL101B_MSS_20160904180811_000013363_101_001_L1B_MSS"
	frame_name = "0_1536_0_2048_0734.png"
	base_key = time.ctime() + video_name + frame_name
	base_key = base_key.encode("utf-8")
	req_id = hashlib.md5(base_key).hexdigest()
	print(req_id)
	detectReq = darknet_pb2.DetectRequest()
	detectReq.req_id = req_id
	detectReq.video_name = video_name
	detectReq.frame_name = frame_name
	return detectReq

def create_channel_stub():
	channel = grpc.insecure_channel("172.17.0.2:9997")
	stub = darknet_pb2_grpc.DetectRpcStub(channel)
	return stub

def demo_run():
	stub = create_channel_stub()
	detectReq = create_request()
	response = stub.Detect(detectReq)
	print("Darknet client received: " + response.message)


if __name__ == "__main__":
	demo_run()