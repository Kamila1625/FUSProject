#pragma once
#include <stdio.h>
#include <iostream>
using namespace std;

class Data
{
private:
  FILE *file;
  unsigned char* imageData;
  long dataSize;

public:
  Data();
  ~Data();

  int ReadData(const char *filePath);

  void *GetDataPointer();
  long GetDataSize() { return dataSize; }

};