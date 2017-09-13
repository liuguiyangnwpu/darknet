#!/bin/bash
#!/bin/bash
docker run -it \
    -v /home/ai-i-liuguiyang/repos_ssd/:/root/repos \
    -p 9997:9997 \
    ubuntu:darknet /bin/bash
