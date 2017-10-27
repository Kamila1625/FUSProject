#ifndef VOXELGRID_H
#define VOXELGRID_H

typedef struct {
  double Coord[6];  
} UR_Coordinate;

#include "Hifu_capture.h"


typedef struct {
  HiFu_CaptureImage dataHeader;
  unsigned char *sliceVoxels;
} VoxelSlice;


class VoxelGrid
{
private:    
  
  int width;
  int height;
  int depth;
  
  VoxelSlice *voxels; // Each slice <=> WxH plane    
  
public:
  HiFu_CaptureHeaderCommon commonHeaderData;
  HiFu_CaptureHeader headerData;
  
  unsigned char *axialData;
  unsigned char *coronalData;
  unsigned char *sagittalData;
  
  unsigned axialDataLen;
  unsigned coronalDataLen;
  unsigned sagittalDataLen;
  
  VoxelGrid();
  
  void SetupSlices();
  void SetupVoxelGrid();
  void SetupVoxelGrid(int gridDepth);
  void SetVoxelSlice(int sliceNumber, VoxelSlice *slice);
  
  ~VoxelGrid();
  
  int GetWidth() { return width; }
  int GetHeight() { return height; }
  int GetDepth() { return depth; }
  
  void UpdateAxialSlice(int sliceNum);
  void UpdateCoronalSlice(int sliceNum);
  void UpdateSagittalSlice(int sliceNum);
  
  unsigned char *GenerateDataArray(int *gridW, int *gridH, int *gridD);
};

#endif // VOXELGRID_H
