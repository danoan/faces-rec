#ifndef __DIRECTORY__
#define __DIRECTORY__

#include <cstdlib>
#include <cstdio>
#include <dirent.h>
#include <cstring>
#include <string>
#include <vector>

bool isFile(std::string path);
int getAllDirectories(std::string path, std::vector<std::string>& dirs);

int getFiles(std::string path, int n, std::vector<std::string>& files);
int getAllFiles(std::string path, std::vector<std::string>& files);


#endif