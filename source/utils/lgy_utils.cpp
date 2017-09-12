#include "lgy_utils.h"

void checkFileInfo(string filePath, ResultStatus& status) {
    if(access(filePath.c_str(), 0) == 0) {

        status.stauts = 0;
        status.mode = EXISTS;
        status.desc = "EXISTS";

        if(access(filePath.c_str(), 2) == 0) {
            status.stauts = 0;
            status.mode = WRITEABLE;
            status.desc = "WRITEABLE";
        }

        if(access(filePath.c_str(), 4) == 0) {
            status.stauts = 0;
            status.mode = READABLE;
            status.desc = "READABLE";
        }

        if(access(filePath.c_str(), 6) == 0) {
            status.stauts = 0;
            status.mode = RWABLE;
            status.desc = "RWABLE";
        }
    } else {
        status.stauts = -1;
        status.mode = NOTEXISTS;
        status.desc = "NOTEXISTS";
    }
}

bool isExistsFile(string filePath) {
    if(access(filePath.c_str(), 0) == 0) {
        return true;
    }
    return false;
}

void read_config_Json(string fileName, map<string, string> &argvMap) {
    Json::Reader reader;
    Json::Value root;
    ifstream in;
    in.open(fileName.c_str(), std::ios_base::binary);
    if(in.is_open() == false) {
        throw runtime_error("Read Configure Json File Error !");
        cerr << "Read Configure Json File Error !" << endl;
    }
    bool flag = reader.parse(in, root, false);
    if(flag == false) {
        throw runtime_error("Configure Json File Format Error !");
        cerr << "Configure Json File Format Error !" << endl;
    }
    argvMap["SERVERIP"] = root.get("SERVERIP", "127.0.0.1").asString();
    argvMap["PORT"] = root.get("PORT", "9999").asString();
    argvMap["LOGPATH"] = root.get("LOGPATH", "NULL").asString();
    argvMap["GPU"] = root.get("GPU", "0").asString();

    if(root.isMember("DARKNET") == true) {
        argvMap["THRESH"] = root["DARKNET"].get("THRESH", "0.24").asString();
        argvMap["HIER"] = root["DARKNET"].get("HIER", "0.5").asString();
        argvMap["LABELPATH"] = root["DARKNET"].get("LABELPATH", "NULL").asString();
        argvMap["CFGPATH"] = root["DARKNET"].get("CFGPATH", "NULL").asString();
        argvMap["WEIGHTPATH"] = root["DARKNET"].get("WEIGHTPATH", "NULL").asString();
    }

    for(map<string, string>::iterator it=argvMap.begin(); it!=argvMap.end(); it++) {
        if(it->second == "NULL") {
            cerr << it->first << " is " << it->second << endl;
            throw runtime_error("Configure Json File Parameter Error !");
        }
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    elems = split(s, delim);
    return elems;
}

bool createFolder(string path) {
    if(access(path.c_str(), 0) != 0) {
        if (!mkdir(path.c_str(), 0775)) {
            return true;
        } else {
            cerr << "createFolder " << path << "failed" << endl;
        }
    }
    return true;
}

bool checkFilePath(string paths) {
    string tmp;
    size_t end_pos = paths.rfind('/');
    tmp = paths.substr(0, end_pos);
    paths = tmp;
    size_t pos = paths.find('/');

    while(pos = paths.find("/", pos+1), pos != string::npos) {
        tmp = (paths.substr(0, pos));
        if(!createFolder(tmp)) {
            return false;
        }
    }
    if(!createFolder(paths)) {
        return false;
    }
    return true;
}