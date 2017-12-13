#ifndef __HIFU_CAPTURE_H__
#define __HIFU_CAPTURE_H__
//#include <windows.h>

#define HIFU_CAPTURE_CUR_VERSION (1)
#define HIFU_CAPTURE_SIGNATURE "HiFuCapt"

/*
Формат файла сохранения:
HiFu_CaptureHeaderCommon;
<возможен пропуск, см. HiFu_CaptureHeaderCommon.HeaderPos>
HiFu_CaptureHeader
<возможен пропуск, см. HiFu_CaptureHeader.ImagePos>
HiFu_CaptureHeader.NumImages x
	HiFu_CaptureImage
	<возможен пропуск, см. HiFu_CaptureHeader.ImageBitmapOffset>
	BitmapArea
*/


// Без выравнивания - для MS Visual studio
#if defined(_MSC_VER)
#pragma pack (push,1)
#endif

// Общий заголовок для всех версий структур заголовка файла (не должен меняться)
typedef struct {
  char Signature[8];            // Сигнатура файла
  unsigned int Size;						// Размер структуры описателя (sizeof(HiFu_CaptureHeader))
  unsigned int Version;					// Номер версии описателя
  unsigned int HeaderPos;				// Смещение описателя в файле
} HiFu_CaptureHeaderCommon;

// описатель (может меняться)
typedef struct {
  unsigned long	ImageWidth;					// Ширина изображения в пикселях
  unsigned long	ImageHeight;				// Высота изображения в пикселях
  unsigned long	ImageLineSize;			// Длина строки изображения в байтах (для выравнивания)
  unsigned long	ImageSize;					// Полный размер одной картинки, включая заголовок
  unsigned long	ImageBitmapOffset;	// Смещение bitmap внтури области картинки
  unsigned long	ImagePos;           // Начало области картинок в файле
  unsigned int NumImages;           // Число картинок
  unsigned int NumLeft;             // Количество слева от централльного (вспомогательное поле)
  unsigned int NumRight;            // Количество справа от централльного (вспомогательное поле)
  unsigned int CurNo;               // Номер текущей 
} HiFu_CaptureHeaderV1;

// Теущая версия заголовока
typedef HiFu_CaptureHeaderV1 HiFu_CaptureHeader;

// Описатель картинки
typedef struct {
  UR_Coordinate Pos;					// Точка захвата среза
  double CenterShift;					// Смещение от центрального сегмента (отрицательные - левые)
} HiFu_CaptureImageV1;

// Теущая версия заголовока
typedef HiFu_CaptureImageV1 HiFu_CaptureImage;


#if defined(_MSC_VER)
#pragma pack (pop)
#endif

#endif
