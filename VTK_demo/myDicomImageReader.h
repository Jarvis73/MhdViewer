#pragma once

// Define DCMTK headers
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>
#include <dcmtk/dcmjpeg/djdecode.h>

#include <vector>

// 默认 Dicom 使用 unsigned short 类型储存数据
#define BIT 16

#define MAX_SERIES 32
#define MAX_UID_LENGTH 256
#define MAX_SERIES_LENGTH 1024
#define _DicomType(Type) Uint##Type
#define DicomType(Type) _DicomType(Type)
#define IntType DicomType(BIT)

using namespace std;

typedef struct {
	char* filename;
	char SOPInstanceNumber[MAX_UID_LENGTH];
} SOPNode, *SOPNodePtr;

/*************************************************
 * 
 * Run init() first before using any methods.
 *
 */
class myDicomImageReader
{
public:
	myDicomImageReader();
	myDicomImageReader(const char *path);
	~myDicomImageReader();

	void init();
	void SetPath(const char *path);
	void ReadDicom();
	int GetLongestSeriesIdx();
	int GetLongestSeriesLength();
	int GetLongestSeriesData(Sint16 *&data);

	const char *GetPatientID();
	const char *GetDate();
	double *GetOrigin();
	double *GetSpacing();
	const char *GetPatientName();
	int GetHeight();
	int GetWidth();
	double GetRescaleOffset();
	double GetRescaleSlope();
	int GetPixelRepresentation();

private:
	int getUID(const char* path, OFString &StudyInstanceUID, OFString &SeriesNumber, OFString &InstanceNumber);
	int GetSliceData(const char *path, Sint16 *&data, int slice);
	int GetBasicInformation(const char *path);

	string srcDir;
	int series_idx;
	int longest_series;
	vector<vector<SOPNode> > files;
	int SOP_counter[MAX_SERIES];

	OFString PatientID,
		PatientName,
		PatientBirthDate,
		ImagePositionPatient, PixelSpacing, SpacingBetweenSlices,
		Rows, Columns,
		RescaleIntercept,
		RescaleSlope,
		PixelRepresentation,
		BitsAllocated, BitsStored, PixelPaddingValue;
};
