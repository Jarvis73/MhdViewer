#include "stdafx.h"
#include "myDicomImageReader.h"

#include <iostream>

// 比较函数, 用于排序
bool myCompare(SOPNode &n1, SOPNode &n2)
{
	return atoi(n1.SOPInstanceNumber) < atoi(n2.SOPInstanceNumber);
}

// 字符串分割函数
void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

// 类方法的实现
myDicomImageReader::myDicomImageReader()
{
}

myDicomImageReader::myDicomImageReader(const char *path)
{
	srcDir = path;
}


myDicomImageReader::~myDicomImageReader()
{
}

void myDicomImageReader::init()
{
	series_idx = -1;
	longest_series = -1;
	memset(SOP_counter, 0, MAX_SERIES * sizeof(int));
	files.resize(MAX_SERIES);
}

void myDicomImageReader::SetPath(const char *path)
{
	if (!srcDir.empty())
		srcDir.clear();
	srcDir = path;
}


void myDicomImageReader::ReadDicom()
{
	_finddata_t file;
	long HANDLE;
	HANDLE = _findfirst((srcDir + "\\*.dcm").c_str(), &file);

	OFString seriesNumber[MAX_SERIES];
	OFString series_cur_UID;
	OFString study_base_UID, study_cur_UID;
	OFString InstanceNumber;

	series_idx = 0;
	int series_finder = series_idx;
	bool new_series = false;

	if (getUID((srcDir + "\\" + file.name).c_str(), study_base_UID, seriesNumber[series_idx], InstanceNumber))
	{
		cout << "Load dicom file failed." << endl;
		exit(1);
	}
	SOP_counter[series_finder]++;
	SOPNode new_node;
	new_node.filename = (char *)malloc(sizeof(file.name) + 1);
	strcpy_s(new_node.filename, sizeof(file.name) + 1, file.name);
	strcpy_s(new_node.SOPInstanceNumber, InstanceNumber.c_str());
	files[series_finder].push_back(new_node);

	while (!_findnext(HANDLE, &file)) {
		if (getUID((srcDir + "\\" + file.name).c_str(), study_cur_UID, series_cur_UID, InstanceNumber))
		{
			cout << "Load dicom file failed." << endl;
			exit(1);
		}
		if (study_cur_UID != study_base_UID)
		{
			cout << "Dicom files belong to different studies." << endl;
			exit(1);
		}
		do
		{
			if (series_cur_UID == seriesNumber[series_finder])
			{
				SOP_counter[series_finder]++;
				SOPNode new_node;
				new_node.filename = (char *)malloc(sizeof(file.name) + 1);
				strcpy_s(new_node.filename, sizeof(file.name) + 1, file.name);
				strcpy_s(new_node.SOPInstanceNumber, InstanceNumber.c_str());
				files[series_finder].push_back(new_node);

				new_series = false;
				break;
			}
			series_finder = (series_finder + 1) % (series_idx + 1);
			new_series = true;
		} while (series_finder != 0);

		if (new_series)
		{
			series_idx++;
			series_finder = series_idx;
			seriesNumber[series_finder] = series_cur_UID;
			SOP_counter[series_finder]++;
			SOPNode new_node;
			new_node.filename = (char *)malloc(sizeof(file.name) + 1);
			strcpy_s(new_node.filename, sizeof(file.name) + 1, file.name);
			strcpy_s(new_node.SOPInstanceNumber, InstanceNumber.c_str());
			files[series_finder].push_back(new_node);
		}
	}
	_findclose(HANDLE);

	for (int i(0); i <= series_idx; ++i)
		cout << "Series " << i << ": " << SOP_counter[i] << endl;

	longest_series = 0;
	for (int i(1); i <= series_idx; ++i)
	{
		if (SOP_counter[i] > SOP_counter[longest_series])
			longest_series = i;
	}

	// 获取基本信息
	GetBasicInformation((srcDir + "\\" + files[longest_series][0].filename).c_str());
}

int myDicomImageReader::getUID(const char* path, 
	OFString &StudyInstanceUID, 
	OFString &SeriesNumber, 
	OFString &InstanceNumber)
{
	DcmFileFormat dfile;

	OFCondition result = dfile.loadFile(path);
	if (result.bad())
		return 1;

	DcmDataset *data = dfile.getDataset();

	data->findAndGetOFString(DCM_StudyInstanceUID, StudyInstanceUID);
	data->findAndGetOFString(DCM_SeriesNumber, SeriesNumber);
	data->findAndGetOFString(DCM_InstanceNumber, InstanceNumber);

	return 0;
}

int myDicomImageReader::GetLongestSeriesIdx()
{
	return longest_series;
}

int myDicomImageReader::GetLongestSeriesLength()
{
	return SOP_counter[longest_series];
	return 0;
}

int myDicomImageReader::GetLongestSeriesData(Sint16 *&data)
{
	// 排序
	sort(files[longest_series].begin(), files[longest_series].end(), myCompare);

	for (size_t i(0); i < files[longest_series].size(); ++i)
	{
		GetSliceData((srcDir + "\\" + files[longest_series][i].filename).c_str(), data, i);
	}

	return files[longest_series].size();
}

int myDicomImageReader::GetSliceData(const char * path, Sint16 *& data_, int slice)
{
	DJDecoderRegistration::registerCodecs();
	DcmFileFormat fileformat;
	if (fileformat.loadFile(path).good()) //filePath为DICOM文件路径 
	{
		//解压缩
		DcmDataset *dataset = fileformat.getDataset();
		E_TransferSyntax xfer = dataset->getOriginalXfer();
		OFCondition sta = dataset->chooseRepresentation(xfer, NULL);
		if (dataset->canWriteXfer(xfer))
		{
			DicomImage* image = new DicomImage(dataset, xfer); //生成DicomImage
			if (image != NULL && image->getStatus() == EIS_Normal)
			{
				IntType *SliceData = (IntType *)(image->getOutputData(BIT /* bits per sample */));
				int leng = GetHeight() * GetWidth();
				int skip = slice * leng;
				Sint16 ofs = (Sint16)GetRescaleOffset();
				int shift = atoi(BitsAllocated.c_str()) - atoi(BitsStored.c_str());
				IntType highbit = 0x0000;
				if (atoi(PixelPaddingValue.c_str()) > 32768)
					highbit = 0x8000;
				for (int i(0); i < leng; ++i)
					*(data_ + i + skip) = ((Sint16)((*(SliceData + i) ^ highbit) >> shift) + ofs);
				
				/*memcpy(data + slice * GetHeight() * GetWidth(), SliceData, 
					sizeof(IntType) * GetHeight() * GetWidth());*/
				if (data_ == NULL)
				{
					cerr << "Error: cannot load DICOM image ("
						<< DicomImage::getString(image->getStatus())
						<< ").\nError code: 3" << endl;
					return 4;
				}
			}
			else
			{
				cerr << "Error: cannot load DICOM image ("
					<< DicomImage::getString(image->getStatus())
					<< ").\nError code: 3" << endl;
				return 3;
			}
			delete image;
		}
		else
		{
			cerr << "Error: cannot load DICOM image.\nError code: 2" << endl;
			return 2;
		}
	}
	else
	{
		cerr << "Error: cannot load DICOM image.\nError code: 1" << endl;
		return 1;
	}
	DJDecoderRegistration::cleanup();

	return 0;
}

int myDicomImageReader::GetBasicInformation(const char *path)
{
	DcmFileFormat dfile;

	OFCondition result = dfile.loadFile(path);
	if (result.bad())
		return 1;

	DcmDataset *data = dfile.getDataset();

	data->findAndGetOFString(DCM_PatientID, PatientID);
	data->findAndGetOFString(DCM_PatientName, PatientName);
	data->findAndGetOFString(DCM_PatientBirthDate, PatientBirthDate);
	data->findAndGetOFStringArray(DCM_PixelSpacing, PixelSpacing);
	data->findAndGetOFString(DCM_SpacingBetweenSlices, SpacingBetweenSlices);
	data->findAndGetOFStringArray(DCM_ImagePositionPatient, ImagePositionPatient);
	data->findAndGetOFString(DCM_Rows, Rows);
	data->findAndGetOFString(DCM_Columns, Columns);
	data->findAndGetOFString(DCM_RescaleIntercept, RescaleIntercept);
	data->findAndGetOFString(DCM_RescaleSlope, RescaleSlope);
	data->findAndGetOFString(DCM_PixelRepresentation, PixelRepresentation);
	data->findAndGetOFString(DCM_BitsAllocated, BitsAllocated);
	data->findAndGetOFString(DCM_BitsStored, BitsStored);
	data->findAndGetOFString(DCM_PixelPaddingValue, PixelPaddingValue);

	return 0;
}

const char * myDicomImageReader::GetPatientID()
{
	return PatientID.c_str();
}

const char * myDicomImageReader::GetDate()
{
	return PatientBirthDate.c_str();
}

double * myDicomImageReader::GetOrigin()
{
	double *origin = new double[3];
	vector<string> ori;
	SplitString(ImagePositionPatient.c_str(), ori, "\\");
	for (int i(0); i < 3; ++i)
		origin[i] = atof(ori[i].c_str());
	return origin;
}

double * myDicomImageReader::GetSpacing()
{
	double *spacing = new double[3];
	vector<string> spa;
	SplitString(PixelSpacing.c_str(), spa, "\\");
	spa.push_back(SpacingBetweenSlices.c_str());
	for (int i(0); i < 3; ++i)
		spacing[i] = atof(spa[i].c_str());
	return spacing;
	return nullptr;
}

const char * myDicomImageReader::GetPatientName()
{
	return PatientName.c_str();
}

int myDicomImageReader::GetHeight()
{
	return atoi(Rows.c_str());
}

int myDicomImageReader::GetWidth()
{
	return atoi(Columns.c_str());
}

double myDicomImageReader::GetRescaleOffset()
{
	return atof(RescaleIntercept.c_str());
}

double myDicomImageReader::GetRescaleSlope()
{
	return atof(RescaleSlope.c_str());
}

int myDicomImageReader::GetPixelRepresentation()
{
	return atoi(PixelRepresentation.c_str());
}