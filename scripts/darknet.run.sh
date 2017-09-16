#!/bin/bash
nvidia-docker run -it \
    -v /home/ai-i-liuguiyang/repos_ssd/:/root/repos \
    -p 9997:9997 \
    nvidia/cuda:darknet /bin/bash
