#include <memory.h>

#include "voxelgrid.h"

VoxelGrid::VoxelGrid()
{    
  voxels = 0;
}

void VoxelGrid::SetupSlices()
{
  this->width = headerData.ImageWidth;
  this->height = headerData.ImageHeight;
  this->depth = headerData.NumImages;
  
  this->axialDataLen = 4 * this->width * this->height;
  this->coronalDataLen = 4 * this->depth * this->width;
  this->sagittalDataLen = 4 * this->depth * this->height;
  
  this->axialData = new unsigned char[this->axialDataLen];
  this->coronalData = new unsigned char[this->coronalDataLen];
  this->sagittalData = new unsigned char[this->sagittalDataLen];  
  
  memset(this->axialData, 0, this->axialDataLen);
  memset(this->coronalData, 0, this->coronalDataLen);
  memset(this->sagittalData, 0, this->sagittalDataLen);
}


void VoxelGrid::SetupVoxelGrid()
{
  voxels = new VoxelSlice[headerData.NumImages];
}


void VoxelGrid::SetupVoxelGrid(int gridDepth)
{
  voxels = new VoxelSlice[gridDepth];
}


void VoxelGrid::SetVoxelSlice(int sliceNumber, VoxelSlice *slice)
{
  if (sliceNumber >= 0 && sliceNumber < (int)headerData.NumImages)
    voxels[sliceNumber] = *slice;
}

VoxelGrid::~VoxelGrid()
{
  delete this->axialData;
  delete this->coronalData;
  delete this->sagittalData;
  
  for (int i = 0; i < this->depth; i++)
    delete voxels[i].sliceVoxels;    
  delete voxels;
}

void VoxelGrid::UpdateAxialSlice(int sliceNum)
{  
  for (int h = 0; h < height; h++)
    for (int w = 0; w < width; w++)
    {
      axialData[4 * (h * width + w) + 0] = voxels[sliceNum].sliceVoxels[4 * (h * width + w) + 0];  
      axialData[4 * (h * width + w) + 1] = voxels[sliceNum].sliceVoxels[4 * (h * width + w) + 1];  
      axialData[4 * (h * width + w) + 2] = voxels[sliceNum].sliceVoxels[4 * (h * width + w) + 2];  
      axialData[4 * (h * width + w) + 3] = voxels[sliceNum].sliceVoxels[4 * (h * width + w) + 3];  
    }
}


void VoxelGrid::UpdateCoronalSlice(int sliceNum)
{
  for (int d = 0; d < depth; d++)
    for (int w = 0; w < width; w++)
    {
      coronalData[4 * (d * width + w) + 0] = voxels[d].sliceVoxels[4 * (sliceNum * width + w) + 0];  
      coronalData[4 * (d * width + w) + 1] = voxels[d].sliceVoxels[4 * (sliceNum * width + w) + 1];  
      coronalData[4 * (d * width + w) + 2] = voxels[d].sliceVoxels[4 * (sliceNum * width + w) + 2];  
      coronalData[4 * (d * width + w) + 3] = voxels[d].sliceVoxels[4 * (sliceNum * width + w) + 3];  
    }        
}


void VoxelGrid::UpdateSagittalSlice(int sliceNum)
{
  for (int h = 0; h < height; h++)
    for (int d = 0; d < depth; d++)
    {
      sagittalData[4 * (d * height + h) + 0] = voxels[d].sliceVoxels[4 * (h * width + sliceNum) + 0];  
      sagittalData[4 * (d * height + h) + 1] = voxels[d].sliceVoxels[4 * (h * width + sliceNum) + 1];  
      sagittalData[4 * (d * height + h) + 2] = voxels[d].sliceVoxels[4 * (h * width + sliceNum) + 2];  
      sagittalData[4 * (d * height + h) + 3] = voxels[d].sliceVoxels[4 * (h * width + sliceNum) + 3];  
    }      
}

unsigned char *VoxelGrid::GenerateDataArray(int *gridW, int *gridH, int *gridD)
{
  unsigned char *data;
  int multX = 2, multY = 2, multZ = 1,  
      newW = width / multX + 1,
      newH = height / multY + 1,
      newD = depth * multZ;
  
  data = new unsigned char[newW * newH * newD];
  memset(data, 0, newW * newH * newD);
  for (int k = 0, kReal = 0; k < newD; k++, kReal = k / multZ)
  {
    for (int j = 0, jReal = 0; j < height; j += multY, jReal++)
    {
      for (int i = 0, iReal = 0; i < width; i += multX, iReal++)
      {
        data[k * newW * newH + jReal * newW + iReal] = //(unsigned char)(((float)j / height) * 256);
            voxels[kReal].sliceVoxels[4 * (j * width + i) + 0];
      }  
    }  
  }  
  
  *gridW = newW;
  *gridH = newH;
  *gridD = newD;
  
  return data;
}
