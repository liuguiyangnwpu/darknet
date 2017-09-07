import codecs
import json
import os
import cv2

dir_prefix = "/Volumes/projects/第三方数据下载/JL1ST/SRC_JL101B_MSS_20160904180811_000013363_101_001_L1B_MSS_YOLOv2"
src_img_prefix = dir_prefix + "/src/"
src_annotation_prefix = dir_prefix + "/annotation/"
src_yolov2_prefix = dir_prefix + "/yolov2/"

def convert(size, box):
    dw = 1./(size[0])
    dh = 1./(size[1])
    x = (box[0] + box[1])/2.0 - 1
    y = (box[2] + box[3])/2.0 - 1
    w = box[1] - box[0]
    h = box[3] - box[2]
    x = x*dw
    w = w*dw
    y = y*dh
    h = h*dh
    return (x,y,w,h)


def convert_annotation(src_img_path, src_anno_path):
    yolov2_name = ".".join(src_anno_path.split("/")[-1].split(".")[:-1] + ["txt"])
    img = cv2.imread(src_img_path)
    h, w = img.shape[:2]
    with codecs.open(src_anno_path, "r", "utf8") as handler:
        anno_info = json.load(handler)
        out_file = open(src_yolov2_prefix + yolov2_name, 'w')
        for b in anno_info:
            # b [xmin, ymin, xmax, ymax] --> [xmin, xmax, ymin, ymax]
            tmp_b = [b[0], b[2], b[1], b[3]]
            bb = convert((w,h), tmp_b)
            out_file.write("0 " + " ".join([str(a) for a in bb]) + '\n')
        out_file.close()
    print("save to ", yolov2_name)

def create_yolov2_samples():
    anno_file_list = os.listdir(src_annotation_prefix)
    for anno_file in anno_file_list:
        if anno_file.startswith("._"):
            continue
        img_name = ".".join(anno_file.strip().split(".")[:-1] + ["png"])
        abs_img_path = src_img_prefix + img_name
        abs_anno_path = src_annotation_prefix + anno_file
        convert_annotation(abs_img_path, abs_anno_path)


# os.system("cat 2007_train.txt 2007_val.txt 2012_train.txt 2012_val.txt > train.txt")
# os.system("cat 2007_train.txt 2007_val.txt 2007_test.txt 2012_train.txt 2012_val.txt > train.all.txt")

