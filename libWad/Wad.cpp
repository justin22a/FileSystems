#include "Wad.h"

TreeNode* getPath(TreeNode* root, const string &path)
{
    auto* t = root;
    string s;
    bool found = false;
    istringstream str(path);
    //can use getline to attain
    getline(str , s , '/');

    while(getline(str , s , '/'))
    {
        for(unsigned int i = 0; i < t->nodes.size(); i++)
        {
            string current = t->nodes.at(i)->name;
            if(s == current)
            {
                t = t->nodes.at(i);
                found = true;
            }
        }
        if(found == false)
        {
            //didn't find it
            return nullptr;
        }
        found = false;
    }
    return t;

}


TreeNode* create(string nm, char* dt, int length){
    auto* t = new TreeNode();
    t->name = nm;
    t->shit = dt;
    t->len = length;
    return t;
}



bool Wad::isDirectory(const string &path)
{
    TreeNode* t = nullptr;
    bool truth = false;
    //same as get content but path should have children instead
    auto* temp = getPath(this->root,path);
    if(temp != nullptr) {
       truth = getPath(this->root, path)->nodes.empty();
    }
    else
    {
        truth = false;
    }
    if(temp != nullptr && truth == false)
    {
        return true;
    }
    return false;
}

Wad *Wad::loadWad(const string &path) {

    //    {
//        int result = open(path.c_str(),O_RDONLY);
//
//        //create variables for all header and descriptor information
//        int count = 0;
//
//        int size = 4;
//        char* magic = new char[size];
//        int eOffset = 0;
//        int eLength = 0;
//        int nameSize = 8;
//        char* eName = new char[8];
//
//        Wad* wad = new Wad();
//
//        read(result , magic , size);
//        wad->magic = (string)magic;
//
//        read(result, &wad->num_desc ,size);
//        read(result, &wad->offset, size);
//        lseek(result, wad->offset, SEEK_SET);
//
//
//        auto* root = new TreeNode();
//        root->name = "/";
//
//        stack<TreeNode*> container;
//
//        container.push(root);
//
//        int offTemp = 0;
//
//        for(unsigned int i = 0; i < wad->num_desc; i++)
//        {
//            read(result, &eOffset, 4);
//            offTemp += 4;
//            read(result, &eLength, 4);
//            offTemp += 4;
//            read(result, eName, 8);
//            offTemp += 8;
//
//            string _elem = string(eName);
//
//            if(eLength == 0 && _elem.find("_START") != string::npos)
//
//            }
//            else if(eLength == 0 && _elem.find("_END") != string::npos)
//            {
//                container.pop();
//                root = container.top();
//            }
//            else if(i < count)
//            {
//
//            }
//            {
//                char* buff = new char[eLength];
//                lseek(result , eOffset , SEEK_SET);
//                read(result , buff , eLength);
//                TreeNode* t = create(_elem , buff , eLength);
//                root->nodes.push_back(t);
//                lseek(result , wad->offset + offTemp, SEEK_SET);
//                root = container.top();
//            }
//            else
//            {
//
//
//
//
//
//
//
//
//
//
//        }
//
//        close(result);
//        wad->root = container.top();
//        container.pop();
//        return wad;

    //  }


    int result = open(path.c_str(),O_RDONLY);

    //create variables for all header and descriptor information
    int count = 0;
    int eOffset = 0;
    int eLength = 0;
    int nameSize = 8;
    int size = 4;
    char* magic = new char[size];

    char* eName = new char[8];

    Wad* wad = new Wad();

    read(result , magic , size);
    wad->magic = (string)magic;

    read(result, &wad->num_desc ,size);
    read(result, &wad->offset, size);
    lseek(result, wad->offset, SEEK_SET);


    auto* root = new TreeNode();
    root->name = "/";

    stack<TreeNode*> container;

    container.push(root);

    int offTemp = 0;

    for(unsigned int i = 0; i < wad->num_desc; i++)
    {
        read(result, &eOffset, 4);
        offTemp += 4;
        read(result, &eLength, 4);
        offTemp += 4;
        read(result, eName, 8);
        offTemp += 8;

        string _elem = string(eName);

        if(eLength == 0 && _elem.find("_START") != string::npos)
        {
            TreeNode* t = create(_elem.substr(0,_elem.length()-6), nullptr,0);
            root = container.top();
            root->nodes.push_back(t);
            container.push(t);
        }
        else if( _elem.find("_END") != string::npos && eLength == 0)
        {
            container.pop();
            root = container.top();
        }
        else if(_elem.length() == 4 && _elem.substr(0,1) == "E" && _elem.substr(2,1) == "M")
        {
            root = container.top();
            TreeNode* t = create(_elem, nullptr,0);
            root->nodes.push_back(t);
            root = t;
            count=i+10;
        }
        else if(i == count)
        {
            char* buff = new char[eLength];
            lseek(result , eOffset , SEEK_SET);
            read(result , buff , eLength);
            TreeNode* t = create(_elem , buff , eLength);
            root->nodes.push_back(t);
            lseek(result , wad->offset + offTemp, SEEK_SET);
            root = container.top();
        }
        else if(i < count)
        {
            //create a buffer here
            char* buff = new char[eLength];
            lseek(result , eOffset , SEEK_SET);
            read(result , buff, eLength);
            TreeNode* t = create(_elem, buff, eLength);
            root->nodes.push_back(t);
            lseek(result , wad->offset + offTemp, SEEK_SET);
        }
        else
        {
            char* buff = new char[eLength];
            lseek(result , eOffset, SEEK_SET);
            read(result , buff , eLength);
            TreeNode* t = create(_elem, buff , eLength);
            root = container.top();
            root->nodes.push_back(t);
            lseek(result , wad->offset + offTemp , SEEK_SET);

        }

    }

    close(result);
    wad->root = container.top();
    container.pop();
    return wad;

}



string Wad::getMagic()
{
    return magic;
}

int Wad::getSize(const string &path)
{
    //using isContent is useful
    if(isContent(path) == true)
    {
        return getPath(this->root , path)->len;
    }
    return -1;
}

bool Wad::isContent(const string &path)
{
    //must check if there is data there, and that the path exists by checking for nlptr
    if(getPath(this->root,path) != nullptr && getPath(this->root,path)->len > 0)
    {
        return true;
    }

    return false;
}


int Wad::getDirectory(const string &path, vector<string> *directory)
{
    //get directory at this current path
    bool truth = this->isDirectory(path);
    if(truth)
    {
        //use the vector of directory pointers to store information
        for(auto & i : getPath(this->root , path)->nodes)
        {

            //TODO: must convert before testing
            (*directory).push_back(i->name);
        }
        return getPath(this->root , path)->nodes.size();
    }
    return -1;
}


int Wad::getContents(const string &path, char *buffer, int length, int _offset)
{
     
    _offset = 0;
    bool checkContent = false;
    if(this->isContent(path))
    {
        checkContent = true;
    }
    if(checkContent)
    {
        if(getPath(this->root , path)->len - this->offset < length) {
            length = getPath(this->root, path)->len - _offset;
        }
//
//            //https://en.cppreference.com/w/cpp/string/byte/memcpy/ 
           char *content = getPath(this->root, path)->shit;
            //copy memory
            ::memcpy(buffer, content + _offset, length);

            return length;

    }
return -1;
/*
if(getPath(this->root ,path) != nullptr)
{
    if(getPath(this->root , path)->len > 0)
    {
        if(getPath(this->root , path)->len < _offset + length)
        {
            length = getPath(this->root , path)->len - offset;
        }
        char *content = getPath(this->root, path)->shit;
        ::memcpy(buffer, content + _offset, length);
        return length;
    }
    return -1;
}

    return -1;
*/
}
