
#ifndef _MY_STORE_TOOLS_
#define _MY_STORE_TOOLS_

#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <string>
#include <iostream>
#include <fstream>

static const std::string STOR_PATH("/home/andr/mstor/");

int crypt_add(int c, int count, char* crypt);

int crypt_rm(int c, int count, char* crypt);

std::string read_store(char *crypt, std::string namef);

int write_store(char *crypt, std::string notef, std::string namef);

#endif