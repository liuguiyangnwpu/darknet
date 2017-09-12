#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unistd.h>
#include <vector>
#include <exception>

#include <dirent.h>
#include <sys/stat.h>

#include "json/json-forwards.h"
#include "json/json.h"

using namespace std;
using namespace Json;

enum FileMode {
    EXISTS,
    WRITEABLE,
    READABLE,
    RWABLE,
    NOTEXISTS
};

typedef struct _ResultStatus {
    int stauts;
    FileMode mode;
    string desc;
} ResultStatus;


void checkFileInfo(string filePath, ResultStatus& status);
bool isExistsFile(string filePath);

void read_config_Json(string fileName, map<string, string>& argvMap);
std::vector<std::string> split(const std::string &s, char delim);

bool createFolder(string path);
bool checkFilePath(string paths);

#endif // UTILS_HPP

