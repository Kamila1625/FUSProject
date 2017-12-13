#ifndef DATALOADER_H
#define DATALOADER_H

#include "voxelgrid.h"

#include <stdio.h>

class DataLoader
{
private:
  FILE *file;
  const char *data;
  int dataShift;
  bool isLoaded;
  
  VoxelGrid *voxelGrid;
  
  int LoadHeaderData();
  
  int LoadSliceData(int sliceNumber);

  int LoadHeaderDataInternal();

  int LoadSliceDataInternal(int sliceNumber);
  
  
public:  
  DataLoader();
  
  int LoadData(const char *filePath);

  int LoadDataInternal(const char *data, long dataLen);

  bool IsLoaded() { return isLoaded; }
  
  int GetVoxelGridWidth() { return voxelGrid->GetWidth(); }  
  int GetVoxelGridHeight()  { return voxelGrid->GetHeight(); }  
  int GetVoxelGridDepth()  { return voxelGrid->GetDepth(); }
  
  unsigned char *GetAxialSlice() { return voxelGrid->axialData; }  
  unsigned char *GetCoronalSlice()  { return voxelGrid->coronalData; }  
  unsigned char *GetSagittalSlice()  { return voxelGrid->sagittalData; }
  
  unsigned GetAxialSliceSize() { return voxelGrid->axialDataLen; }  
  unsigned GetCoronalSliceSize()  { return voxelGrid->coronalDataLen; }  
  unsigned GetSagittalSliceSize()  { return voxelGrid->sagittalDataLen; }
  
  void UpdateAxialSlice(int sliceNum);  
  void UpdateCoronalSlice(int sliceNum);  
  void UpdateSagittalSlice(int sliceNum);
  
  unsigned char *GenerateDataArray(int *gridW, int *gridH, int *gridD);
};

#endif // DATALOADER_H
