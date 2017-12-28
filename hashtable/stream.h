#ifndef __STREAM_H_
#define __STREAM_H_
#include <stdlib.h>

namespace lib
{
class stream
{
public:
    virtual ~stream() {};
    virtual int close() = 0;
    virtual int write(const void *dat, size_t size) = 0;
    virtual int read(void *dat, size_t size) = 0;
    virtual int start_trans() = 0;
    virtual int comit_trans() = 0;
    virtual int drop_trans(bool) = 0;
    virtual int flush() = 0;
};

class wstream : public stream
{
private:
   FILE* _fp;

public:
   wstream(const char* name){_fp=fopen(name,"w+");};
   ~wstream(){if(_fp != NULL) fclose(_fp); _fp = NULL;};
  
    int close(){if(_fp != NULL) fclose(_fp); return 0;};
    int write(const void *dat, size_t size){if(_fp && dat && size> 0) fwrite(dat,size,sizeof(char),_fp); return 0;};
    int read(void *dat, size_t size){if(_fp && dat && size > 0) fread(dat, size, sizeof(char), _fp); return 0;};
    int flush(){if(_fp) fflush(_fp); return 0;};
    int start_trans(){return 0;};
    int comit_trans(){return 0;};
    int  drop_trans(bool){return 0;};
};

class rstream : public stream
{
private:
   FILE* _fp;

public:
   rstream(const char* name){_fp=fopen(name,"r+");};
   ~rstream(){if(_fp != NULL) fclose(_fp); _fp = NULL;};

    int close(){if(_fp != NULL) fclose(_fp); return 0;};
    int write(const void *dat, size_t size){if(_fp && dat && size> 0) fwrite(dat,size,sizeof(char),_fp); return 0;};
    int read(void *dat, size_t size){if(_fp && dat && size > 0) fread(dat, size, sizeof(char), _fp); return 0;};
    int flush(){if(_fp) fflush(_fp); return 0;};
    int start_trans(){return 0;};
    int comit_trans(){return 0;};
    int  drop_trans(bool){return 0;};
};

};

#endif
