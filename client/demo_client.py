# Copyright (c) 2009 IW.
# All rights reserved.
#
# Author: liuguiyang <liuguiyangnwpu@gmail.com>
# Date:   2017/9/12

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import os
import time
import hashlib


import cv2
import grpc
import xml.dom.minidom

from rpc import darknet_pb2
from rpc import darknet_pb2_grpc


dir_prefix = "/Volumes/projects/第三方数据下载/JL1ST/"

# 给定一个标记文件，找到对应的目标的位置信息
def extract_airplane_posinfo(filename):
    if not os.path.exists(filename):
    	print(filename + " not exists !")
    	return list()
        # raise IOError(filename + " not exists !")

    # 使用minidom解析器打开 XML 文档
    DOMTree = xml.dom.minidom.parse(filename)
    collection = DOMTree.documentElement
    # 获取集合中所有的目标
    targets = collection.getElementsByTagName("object")
    res = []
    for target in targets:
        target_name = target.getElementsByTagName('name')[0].childNodes[0].data
        bndbox = target.getElementsByTagName("bndbox")[0]
        xmin = bndbox.getElementsByTagName("xmin")[0].childNodes[0].data
        ymin = bndbox.getElementsByTagName("ymin")[0].childNodes[0].data
        xmax = bndbox.getElementsByTagName("xmax")[0].childNodes[0].data
        ymax = bndbox.getElementsByTagName("ymax")[0].childNodes[0].data
        res.append([int(xmin), int(ymin), int(xmax), int(ymax), target_name])
    return res

def show_detect_result(rpc_request, rpc_response, is_show_gt=False):
	src_image_path = dir_prefix + "SRC_" + rpc_request.video_name + "/" + rpc_request.frame_name
	print(src_image_path)
	src_img = cv2.imread(src_image_path)
	for target in rpc_response.targetlist:
		src_img = cv2.rectangle(src_img, 
			(target.x_min, target.y_min), (target.x_max, target.y_max), 
			(0, 255, 0))
		src_img = cv2.putText(src_img, "%s %.2f" % (target.label, target.prob), 
			(target.x_min, target.y_min), 0, 0.5, (0, 255, 0), 1, cv2.LINE_AA)
	if is_show_gt:
		anno_name = ".".join(rpc_request.frame_name.split(".")[:-1] + ["xml"])
		anno_path = dir_prefix + "SRC_" + rpc_request.video_name + "_annotation/" + anno_name
		gt_targets = extract_airplane_posinfo(anno_path)
		for gt in gt_targets:
			src_img = cv2.rectangle(src_img, tuple(gt[:2]), tuple(gt[2:4]), (0, 0, 255), 2)
	cv2.imshow(rpc_request.video_name, src_img)
	cv2.waitKey()

def create_request():
	video_name = "JL101B_MSS_20160904180811_000013363_101_001_L1B_MSS"
	frame_name = "0_1536_0_2048_0672.png"
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
	channel = grpc.insecure_channel("10.2.3.241:9997")
	stub = darknet_pb2_grpc.DetectRpcStub(channel)
	return stub

def demo_run():
	stub = create_channel_stub()
	detectReq = create_request()
	response = stub.Detect(detectReq)
	cv2.namedWindow(detectReq.video_name, cv2.WINDOW_NORMAL)
	show_detect_result(detectReq, response, is_show_gt=True)


if __name__ == "__main__":
	demo_run()