# Copyright (c) 2009 IW.
# All rights reserved.
#
# Author: liuguiyang <liuguiyangnwpu@gmail.com>
# Date:   2017/9/12

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import grpc

from rpc import darknet_pb2
from rpc import darknet_pb2_grpc


def create_request():
	pass

def run():
	channel = grpc.insecure_channel("127.0.0.1:9997")
	stub = darknet_pb2_grpc.GreeterStub(channel)
	response = stub.Detect(helloworld_pb2.HelloRequest(name='you'))
  	print("Greeter client received: " + response.message)