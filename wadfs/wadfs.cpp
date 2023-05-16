#include "../libWad/Wad.cpp"
#include "fuse.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>
#define FUSE_USE_VERSION 26
using namespace std;

Wad* wad;

static int f_open(const char* path , struct fuse_file_info* f)
{
    if(!wad->isContent(path))
    {
        return -1;
    }
    return 0;
}

static int f_release_dir(const char* path , struct fuse_file_info* f)
{
    //i believe this implementation is the same for f_open_dir in terms of code
    if(wad->isDirectory(path) == false)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static int f_read(const char* path , char* buff , ::size_t size , off_t offset , struct fuse_file_info* f)
{
    int value = wad->getContents(path , buff , size , offset);
    if(value != -1)
    {
        return value;
    }
    else
    {
        return 0;
    }
}

static int f_release(const char* path , struct fuse_file_info* f)
{
    if(wad->isContent(path) == false)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static int f_open_dir(const char* path , struct fuse_file_info* fileInfo)
{
    if(wad->isDirectory(path) == false)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static int f_get(const char* path , struct stat *buff){

    ::memset(buff, 0, sizeof(struct stat));
    string sPath(path);

    if(sPath.length() >= 1 && sPath.substr(sPath.length()-1) != "/")
    {
        sPath = sPath.substr(sPath.length()-1);
    }


    buff->st_gid = getgid();
    buff->st_uid = getuid();
    buff->st_atime = time(NULL);

    if (wad->isDirectory(path)) {
        buff->st_mode = S_IFDIR | 0555;
        buff->st_nlink = 2;
        return 0;
    }

    if (wad->isContent(path)) {
        buff->st_mode = S_IFREG | 0644;
        buff->st_nlink = 1;
        buff->st_size = wad->getSize(path);
        return 0;
    }

    return ~ENOENT;
}



static int f_read_dir(const char* path , void* buff , fuse_fill_dir_t filler , off_t offset , struct fuse_file_info* f)
{
    // if the wad is not a path
    (void) offset;
    (void) f;
    filler(buff , "." , NULL , 0);
    filler(buff , ".." , NULL , 0);
    vector<string> shit;
    string sPath(path);
    //need to put the directory names in here

    if(sPath.length() >= 1 && sPath.substr(sPath.length()-1) != "/")
    {
        sPath = sPath + "/";
    }
    if(sPath.size() == 0)
    {
        sPath = "/";
    }

    wad->getDirectory(sPath , &shit);
    for(string entry : shit)
    {
        filler(buff , entry.c_str() , NULL , 0);
    }
    //convert to void

    return 0;

}

//Used sources
//http://libfuse.github.io/doxygen/
//https://engineering.facile.it/blog/eng/write-filesystem-fuse/
//http://slade.mancubus.net/index.php?page=about



struct fuse_operations fuse_example_operations =
        {
        .getattr = f_get,
        .open = f_open,
        .read = f_read,
        .release = f_release,
        .opendir = f_open_dir,
        .readdir = f_read_dir,
        .releasedir = f_release_dir,
        };


int main(int argc , char* argv[])
{
    wad = Wad::loadWad(argv[1]);

    //argument arithmetic
    argv[argc-2] = argv[argc-1];
    argv[argc-1] = NULL;
    argc = argc -1;

    return fuse_main(2 , argv , &fuse_example_operations , NULL);
    return 0;
}








