#include <memory.h>

#include "dataloader.h"

DataLoader::DataLoader()
{
  this->isLoaded = false; 
  this->voxelGrid = new VoxelGrid();
  this->dataShift = 0;
}


int DataLoader::LoadHeaderData()
{      
  if (0 != fseek(file, 0, SEEK_SET)) 
    return -1;
  
  if (sizeof(voxelGrid->commonHeaderData) != fread(&voxelGrid->commonHeaderData, 1, sizeof(voxelGrid->commonHeaderData), file))
    return -1;
  
  if (0 != memcmp(voxelGrid->commonHeaderData.Signature, HIFU_CAPTURE_SIGNATURE, sizeof(voxelGrid->commonHeaderData.Signature)))
    return -2;
  
  if (voxelGrid->commonHeaderData.Version > HIFU_CAPTURE_CUR_VERSION)
    return 3;
  
  
  switch(voxelGrid->commonHeaderData.Version)
  {
  case 1:		
    if (sizeof(HiFu_CaptureHeaderV1) != voxelGrid->commonHeaderData.Size) 
      return 5;
    break;
  
  default:	
    return 4;
  }
  
  if (0 != fseek(file, voxelGrid->commonHeaderData.HeaderPos, SEEK_SET))
    return 5;
  
  if (sizeof(voxelGrid->headerData) != fread(&voxelGrid->headerData, 1, sizeof(voxelGrid->headerData), file))
    return 5;
  
  return 0;  
}

int DataLoader::LoadHeaderDataInternal()
{  
  memcpy(&voxelGrid->commonHeaderData, data, sizeof(voxelGrid->commonHeaderData));

  if (0 != memcmp(voxelGrid->commonHeaderData.Signature, HIFU_CAPTURE_SIGNATURE, sizeof(voxelGrid->commonHeaderData.Signature)))
    return -2;

  if (voxelGrid->commonHeaderData.Version > HIFU_CAPTURE_CUR_VERSION)
    return 3;


  switch (voxelGrid->commonHeaderData.Version)
  {
  case 1:
    if (sizeof(HiFu_CaptureHeaderV1) != voxelGrid->commonHeaderData.Size)
      return 5;
    break;

  default:
    return 4;
  }

  dataShift = voxelGrid->commonHeaderData.HeaderPos;
  memcpy(&voxelGrid->headerData, data + dataShift, sizeof(voxelGrid->headerData));

  return 0;
}


int DataLoader::LoadSliceData(int sliceNumber)
{
  HiFu_CaptureImage sliceHeader;
  VoxelSlice *newSlice; 
  int imageSize = voxelGrid->headerData.ImageHeight * voxelGrid->headerData.ImageLineSize;

  // Позиционируемся на начало картинки
  if (0 != fseek(file, voxelGrid->headerData.ImagePos + voxelGrid->headerData.ImageSize * sliceNumber, SEEK_SET))
    return 5;
  
  // Загружаем заголовок
  if (sizeof(sliceHeader) != fread(&sliceHeader, 1, sizeof(sliceHeader), file))
    return 5;
  
  // Описатель среза
  newSlice = new VoxelSlice();
  if (newSlice == NULL)
  {    
    return 2;
  }
  newSlice->dataHeader = sliceHeader;
  
  // Загружаем картинку
  // Переходим на bitmap
  if (0 != fseek(file, voxelGrid->headerData.ImagePos + voxelGrid->headerData.ImageSize * sliceNumber + 
                 voxelGrid->headerData.ImageBitmapOffset, SEEK_SET)) 
  {    
    return 5;
  }
  
  // Захватим память для картинки
  newSlice->sliceVoxels = new unsigned char[imageSize];
  if (newSlice->sliceVoxels == NULL)
  {    
    delete newSlice;
    return 2;
  }  
  
  // Читаем картинку
  if (imageSize != (int)fread(newSlice->sliceVoxels, 1, imageSize, file))
  {
    delete newSlice->sliceVoxels;
    delete newSlice;
    return 5;
  }  
  
  voxelGrid->SetVoxelSlice(sliceNumber, newSlice);      
  
  return 0;
}

int DataLoader::LoadSliceDataInternal(int sliceNumber)
{
  HiFu_CaptureImage sliceHeader;
  VoxelSlice *newSlice;
  int imageSize = voxelGrid->headerData.ImageHeight * voxelGrid->headerData.ImageLineSize;
  
  // Позиционируемся на начало картинки
  dataShift = voxelGrid->headerData.ImagePos + voxelGrid->headerData.ImageSize * sliceNumber;    

  // Загружаем заголовок
  memcpy(&sliceHeader, data + dataShift, sizeof(sliceHeader));  

  // Описатель среза
  newSlice = new VoxelSlice();
  if (newSlice == NULL)
  {
    return 2;
  }
  newSlice->dataHeader = sliceHeader;

  // Загружаем картинку
  // Переходим на bitmap
  dataShift = voxelGrid->headerData.ImagePos + voxelGrid->headerData.ImageSize * sliceNumber + voxelGrid->headerData.ImageBitmapOffset;
  
  // Захватим память для картинки
  newSlice->sliceVoxels = new unsigned char[imageSize];
  if (newSlice->sliceVoxels == NULL)
  {
    delete newSlice;
    return 2;
  }

  // Читаем картинку
  memcpy(newSlice->sliceVoxels, data + dataShift, imageSize);    

  voxelGrid->SetVoxelSlice(sliceNumber, newSlice);

  return 0;
}


int DataLoader::LoadData(const char *filePath)
{
  errno_t errCode = fopen_s(&file, filePath, "rb");
  
  if (file == NULL || errCode != 0) 
  {
    return 1; // file couldn't be opened
  }
  
  if (LoadHeaderData() != 0)
  {
    return 2; // Error in header loading
  }
      
  voxelGrid->SetupVoxelGrid();
  for (unsigned imgIndex = 0; imgIndex < voxelGrid->headerData.NumImages; imgIndex++)
  {
    if (LoadSliceData(imgIndex) != 0)
    {      
      delete voxelGrid;      
      fclose(file);
      
      return 3;      
    }
  }    
  
  fclose(file);
  
  voxelGrid->SetupSlices();
  isLoaded = true;
  
  return 0;  
}


int DataLoader::LoadDataInternal(const char *data, long dataLen)
{  
  if (data == NULL || dataLen == 0)
  {
    return 1; // data couldn't be read
  }
  this->data = data;
  this->dataShift = 0;

  if (LoadHeaderDataInternal() != 0)
  {
    return 2; // Error in header loading
  }

  voxelGrid->SetupVoxelGrid();
  for (unsigned imgIndex = 0; imgIndex < voxelGrid->headerData.NumImages; imgIndex++)
  {
    if (LoadSliceDataInternal(imgIndex) != 0)
    {
      delete voxelGrid;
      this->data = NULL;

      return 3;
    }
  }  

  voxelGrid->SetupSlices();
  isLoaded = true;

  return 0;
}


void DataLoader::UpdateAxialSlice(int sliceNum)
{
  if (isLoaded)
    voxelGrid->UpdateAxialSlice(sliceNum);    
}


void DataLoader::UpdateCoronalSlice(int sliceNum)
{
  if (isLoaded)
    voxelGrid->UpdateCoronalSlice(sliceNum);    
}


void DataLoader::UpdateSagittalSlice(int sliceNum)
{
  if (isLoaded)
    voxelGrid->UpdateSagittalSlice(sliceNum);  
}

unsigned char *DataLoader::GenerateDataArray(int *gridW, int *gridH, int *gridD)
{
  if (isLoaded)
    return voxelGrid->GenerateDataArray(gridW, gridH, gridD);
  else
    return 0;
}
