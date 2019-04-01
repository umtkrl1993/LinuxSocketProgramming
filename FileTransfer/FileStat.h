#ifndef _FILESTAT_H
#define _FILESTAT_H
#include <exception>

class FileStat{

public:
    FileStat();

    FileStat( char* filename );

    int getFileSize();

    bool isDirectory();

    bool testAccess( int mode );


private:
    char* mp_filename;

};


class FileException : public std::exception{

    virtual const char* what() const throw(){
        return "file exception";
    }

};



#endif