#include "Data.h"

Data::~Data()
{
	delete imageData;
}

Data::Data()
{
	imageData = NULL;
  dataSize = -1;
}

int Data::ReadData(const char *filePath)
{
	errno_t  errCode = fopen_s(&file, filePath, "rb");
	if (errCode != 0)
	{
		return errCode;
	}
	//������ ������
	
	fseek(file, 0, SEEK_END);
  dataSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	//�������� ������
	imageData = new unsigned char[dataSize];
	if (imageData == NULL)
	{
		fclose(file);
		return -100;
	}
	//��������
	int res = fread(imageData, 1, dataSize, file);
	if (res != dataSize)
	{
		fclose(file);
		return -200;
	}

	fclose(file);
	return 0;
}

void *Data::GetDataPointer()
{
	return (void*)(imageData);
}