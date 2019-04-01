#include "FileStat.h"
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


FileStat::FileStat()
    :mp_filename( nullptr )
{

}

FileStat::FileStat(char *filename){
    mp_filename = filename;
}

int FileStat::getFileSize() {

    if( mp_filename != nullptr ){
        struct stat buff;

        lstat( mp_filename, &buff );

        return buff.st_size;
    }

    return -1;
}

bool FileStat::isDirectory() {

    struct stat buff;

    lstat( mp_filename, &buff );

    return S_ISDIR( buff.st_mode );
}

bool FileStat::testAccess( int mode ){

        if( access( mp_filename, mode ) < 0 ){
            return false;
        }

        return true;
}

