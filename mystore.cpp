#include "mystore.h"


int crypt_add(int c, int count, char* crypt)
{
    return (c + crypt[count]) % 256;
}

int crypt_rm(int c, int count, char* crypt)
{
    return ((c - crypt[count]) < 0) ? (c - crypt[count] + 256) : (c - crypt[count]);
}

std::string read_store(char *crypt, std::string namef)
{
    //Read
    std::string note;
    int f_fd = open(namef.c_str(),O_RDONLY);
    int count = 0;
    char buf[4096];
    ssize_t len;
    if (f_fd < 0)
    {
        //fprintf(stderr,"Error open file for reading\n");
        return "Error open file for reading\n";
    }
    while ((len = read(f_fd,buf,sizeof(buf))) > 0)
    {
        for (int i = 0; i < len; ++i)
        {
            buf[i] = (char) crypt_rm(buf[i],count, crypt);
            count = (count + 1) % 5;
        }
        //write(1, buf, len);
        std::string str(buf, len);
        note.append(str);
    }
    close(f_fd);

    /*
    std::string note;
    std::ifstream f;
    f.open(namef);
    while (f >> c)
    {
        c = (char) crypt_rm(c, count, crypt);
        std::cout << c;
        note.push_back(c);
        count = (count + 1) % 5;
    }
    f.close();
    */

    return note;
}

int write_store(char *crypt, std::string notef, std::string namef)
{
    int f_fd, newf_fd;
    int count;
    char buf[4096];
    ssize_t len;
    
    f_fd = open(notef.c_str(), O_RDONLY);
    if (f_fd < 0)
    {
        return 1;
    }

    newf_fd = open(namef.c_str(), O_WRONLY|O_CREAT|O_APPEND, 0666);
    if (newf_fd < 0) 
    {
        return 1;
    }

    //Read, write and crypt
    count = 0;
    while ((len=read(f_fd, buf, sizeof(buf))) > 0)
    {
        for(int i = 0; i < len; ++i)
        {
            buf[i] = (char) crypt_add(buf[i], count, crypt);
            count = (count + 1) % 5;
        }
        write(newf_fd,buf,len);
    }
    close(f_fd);
    close(newf_fd);

    return 0;
}