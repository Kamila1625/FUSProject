#include "Data.h"

Data::~Data()
{
	delete imageData;
}

Data::Data()
{
	imageData = NULL;
}

int Data::ReadData(const char *filePath)
{
	errno_t  errCode = fopen_s(&file, filePath, "rb");
	if (errCode != 0)
	{
		return errCode;
	}
	//узнать размер
	long size;
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	//выделить память
	imageData = new unsigned char[size];
	if (imageData == NULL)
	{
		fclose(file);
		return -100;
	}
	//прочесть
	int res = fread(imageData, 1, size, file);
	if (res != size)
	{
		fclose(file);
		return -200;
	}

	fclose(file);
	return 0;
}

void* Data::GetDataPointer()
{
	return (void*)(imageData);
}