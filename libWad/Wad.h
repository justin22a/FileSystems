#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <cstring>
#include <stack>
using namespace std;

struct TreeNode{
    vector<TreeNode*> nodes;
    string name;
    char* shit = nullptr;
    int len = 0;
};


class Wad{
public:
    TreeNode* root;
    string magic;
    int num_desc = 0;
    int offset = 0;

    static Wad* loadWad(const string &path);

    string getMagic();
    bool isContent(const string &path);
    bool isDirectory(const string &path);
    int getSize(const string &path);
    int getContents(const string &path, char *buffer, int length, int _offset = 0);
    int getDirectory(const string &path, vector<string> *directory);
};
