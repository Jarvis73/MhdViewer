//// ITKdemo.cpp: 定义控制台应用程序的入口点。
////
//
#include "stdafx.h"
//#include <ctime>
//
//// Define VTK headers
//#include <vtkActor2D.h>
//#include <vtkAssemblyPath.h>
//#include <vtkAutoInit.h>
//#include <vtkCamera.h>
//#include <vtkCommand.h>
//#include <vtkCornerAnnotation.h>
//#include <vtkDICOMImageReader.h>
//#include <vtkImageActor.h>
//#include <vtkImageData.h>
//#include <vtkImageViewer2.h>
//#include <vtkInteractorStyleImage.h>
//#include <vtkMath.h>
//#include <vtkMetaImageReader.h>
//#include <vtkPointData.h>
//#include <vtkPropPicker.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkRenderer.h>
//#include <vtkSmartPointer.h>
//#include <vtkTextMapper.h>
//#include <vtkTextProperty.h>
//
//
//// Define ITK/VTK libraries
//#include "vtk_lib.h"
//
//#define JDEBUG
//
//#define CONSTSTR "Location:\t ( %d,  %d,  %d/%d )\nValue:       "
//
//VTK_MODULE_INIT(vtkRenderingOpenGL2)
//VTK_MODULE_INIT(vtkInteractionStyle)
//VTK_MODULE_INIT(vtkRenderingFreeType)
//
//const unsigned int							Dimension = 3;
//bool										FileFlag = false;
//char										PatientName[512];
//ofstream									fout;
//char										buffer[256];
//char										name[1024];
//
//typedef signed short						PixelType;
//typedef float								FloatPixelType;
//typedef std::vector< std::string >			SeriesIdContainer;
//typedef std::vector< std::string >			FileNamesContainer;
//
//
//// 图像像素值读取模板
//template<typename T>
//void vtkValueMessageTemplate(vtkImageData* image, int* position, std::string &message)
//{
//	T* tuple = ((T*)image->GetScalarPointer(position));
//	int components = image->GetNumberOfScalarComponents();
//	for (int c = 0; c < components; ++c)
//	{
//		sprintf_s(buffer, 256, "%d", tuple[c]);
//		message += buffer;
//		if (c != (components - 1))
//		{
//			message += ", ";
//		}
//	}
//}
//
//void CreateFileName(char *filename, int capacity)
//{
//	assert(capacity >= 512);
//	time_t now;
//	time(&now);
//	struct tm ptime;
//	localtime_s(&ptime, &now);
//	char nowtime[512];
//	strftime(nowtime, sizeof(nowtime), "%y%m%d%H%M%S", &ptime);
//	strcpy_s(filename, capacity, "./Output/");
//	strcat_s(filename, capacity, PatientName);
//	strcat_s(filename, capacity, "_");
//	strcat_s(filename, capacity, nowtime);
//	strcat_s(filename, capacity, ".csv");
//}
//
//class vtkImageInteractionCallback : public vtkCommand
//{
//private:
//
//	vtkImageViewer2*				Viewer;      // Pointer to the viewer
//	vtkPropPicker*					Picker;      // Pointer to the picker
//	vtkTextMapper*					StatusMapper;
//	vtkImageData*					image;
//	int _Slice;
//	int _MinSlice;
//	int _MaxSlice;
//	double *origin;
//	double *spacing;
//	int image_coordinate[3];
//	char msg[200];
//	char msg2[200];
//	bool scrolling;
//	int orientation;
//	bool leftButtonDown;
//	int timerId;
//	unsigned int NumberOfClicks;
//	int PreviousPosition[3];
//	int ResetPixelDistance;
//
//public:
//	static vtkImageInteractionCallback *New()
//	{
//		return new vtkImageInteractionCallback;
//	}
//
//	vtkImageInteractionCallback() : Viewer(0), Picker(0), StatusMapper(0),
//		scrolling(false), origin(0), spacing(0), orientation(2), leftButtonDown(false),
//		NumberOfClicks(0), ResetPixelDistance(5)
//	{
//	}
//
//	~vtkImageInteractionCallback()
//	{
//		this->Viewer = NULL;
//		this->Picker = NULL;
//		this->StatusMapper = NULL;
//	}
//
//	void SetPicker(vtkPropPicker *picker)
//	{
//		this->Picker = picker;
//	}
//
//	void SetViewer(vtkImageViewer2 *viewer)
//	{
//		Viewer = viewer;
//		_MinSlice = Viewer->GetSliceMin();
//		_MaxSlice = Viewer->GetSliceMax();
//		_Slice = _MinSlice;
//		//std::cout << "Slicer: Min = " << _MinSlice << ", Max = " << _MaxSlice << std::endl;
//
//		image = Viewer->GetInput();
//		spacing = image->GetSpacing();
//		origin = image->GetOrigin();
//	}
//
//	void SetStatusMapper(vtkTextMapper* statusMapper)
//	{
//		StatusMapper = statusMapper;
//	}
//
//protected:
//
//	void MoveSliceForward() {
//		if (_Slice > _MinSlice)
//			_Slice -= 1;
//		else
//			_Slice = Viewer->GetSliceMax();
//
//		Viewer->SetSlice(_Slice);
//		image_coordinate[orientation] = _Slice;
//		sprintf_s(msg, 200, CONSTSTR, image_coordinate[0], image_coordinate[1], image_coordinate[2], Viewer->GetSliceMax());
//		std::string message(msg);
//
//		switch (image->GetScalarType())
//		{
//			vtkTemplateMacro((vtkValueMessageTemplate<VTK_TT>(image, image_coordinate, message)));
//		default:
//			return;
//		}
//
//		StatusMapper->SetInput(message.c_str());
//		Viewer->Render();
//	}
//
//	void MoveSliceBackward() {
//		if (_Slice < _MaxSlice)
//			_Slice += 1;
//		else
//			_Slice = 0;
//
//		Viewer->SetSlice(_Slice);
//		image_coordinate[orientation] = _Slice;
//		sprintf_s(msg, 200, CONSTSTR, image_coordinate[0], image_coordinate[1], image_coordinate[2], Viewer->GetSliceMax());
//		std::string message(msg);
//
//		switch (image->GetScalarType())
//		{
//			vtkTemplateMacro((vtkValueMessageTemplate<VTK_TT>(image, image_coordinate, message)));
//		default:
//			return;
//		}
//
//		StatusMapper->SetInput(message.c_str());
//		Viewer->Render();
//	}
//
//public:
//
//	virtual void Execute(vtkObject * vtkNotUsed(caller), unsigned long event, void * vtkNotUsed(callData))
//	{
//		vtkRenderWindowInteractor *interactor = this->Viewer->GetRenderWindow()->GetInteractor();
//		vtkRenderer* renderer = this->Viewer->GetRenderer();
//		vtkImageActor* actor = this->Viewer->GetImageActor();
//		vtkImageData* image = this->Viewer->GetInput();
//		vtkInteractorStyle *style = vtkInteractorStyle::SafeDownCast(interactor->GetInteractorStyle());
//
//#if VTK_MAJOR_VERSION <= 5
//		image->Update();
//#endif
//
//		switch (event)
//		{
//		case vtkCommand::MouseMoveEvent:
//		{
//			// 拾取交互器提供的鼠标坐标
//			Picker->Pick(interactor->GetEventPosition()[0], interactor->GetEventPosition()[1], 0.0, renderer);
//
//			// 有可能把其他的对象分配给拾取器, 所以要在对象集中检查我们是否拾取到了图像区域
//			vtkAssemblyPath* path = Picker->GetPath();
//			bool validPick = false;
//			if (path)
//			{
//				vtkCollectionSimpleIterator sit;
//				path->InitTraversal(sit);
//				vtkAssemblyNode *node;
//				for (int i = 0; i < path->GetNumberOfItems() && !validPick; ++i)
//				{
//					node = path->GetNextNode(sit);
//					if (actor == vtkImageActor::SafeDownCast(node->GetViewProp()))
//					{
//						validPick = true;
//					}
//				}
//			}
//			// 鼠标不在图像区域
//			if (!validPick)
//			{
//				StatusMapper->SetInput("Off Image");
//				interactor->Render();
//				return;
//			}
//
//			// 获取此次拾取点的世界坐标并转为图像坐标
//			double pos[3];
//			Picker->GetPickPosition(pos);
//			for (int i(0); i < 3; ++i)
//				pos[i] = (pos[i] - origin[i]) / spacing[i];
//
//			int axis = Viewer->GetSliceOrientation();
//			switch (axis)
//			{
//			case vtkImageViewer2::SLICE_ORIENTATION_XZ:
//				image_coordinate[0] = vtkMath::Round(pos[0]);
//				image_coordinate[1] = Viewer->GetSlice();
//				image_coordinate[2] = vtkMath::Round(pos[2]);
//				break;
//			case vtkImageViewer2::SLICE_ORIENTATION_YZ:
//				image_coordinate[0] = Viewer->GetSlice();
//				image_coordinate[1] = vtkMath::Round(pos[1]);
//				image_coordinate[2] = vtkMath::Round(pos[2]);
//				break;
//			default:  // vtkImageViewer2::SLICE_ORIENTATION_XY
//				image_coordinate[0] = vtkMath::Round(pos[0]);
//				image_coordinate[1] = vtkMath::Round(pos[1]);
//				image_coordinate[2] = Viewer->GetSlice();
//				break;
//			}
//
//			sprintf_s(msg, 200, CONSTSTR, image_coordinate[0], image_coordinate[1], image_coordinate[2], Viewer->GetSliceMax());
//			std::string message(msg);
//			// 提取该点的像素值, 并加入message
//			switch (image->GetScalarType())
//			{
//				vtkTemplateMacro((vtkValueMessageTemplate<VTK_TT>(image, image_coordinate, message)));
//			default:
//				return;
//			}
//			StatusMapper->SetInput(message.c_str());
//			interactor->Render();
//
//			style->OnMouseMove();
//			break;
//		}
//		case vtkCommand::MouseWheelForwardEvent:
//			MoveSliceForward();
//			break;
//		case vtkCommand::MouseWheelBackwardEvent:
//			MoveSliceBackward();
//			break;
//		case vtkCommand::TimerEvent:
//			MoveSliceBackward();
//			break;
//		case vtkCommand::MiddleButtonPressEvent:
//			scrolling = !scrolling;
//			if (scrolling)
//				timerId = interactor->CreateRepeatingTimer(200);
//			else
//				interactor->DestroyTimer(timerId);
//			break;
//		case vtkCommand::KeyPressEvent:
//		{
//			std::string key = interactor->GetKeySym();
//			if (key.compare("Up") == 0) {
//				MoveSliceForward();
//			}
//			else if (key.compare("Down") == 0) {
//				MoveSliceBackward();
//			}
//			style->OnKeyPress();
//			break;
//		}
//		case vtkCommand::LeftButtonPressEvent:
//		{
//			NumberOfClicks++;
//			int pickPosition[2];
//			interactor->GetEventPosition(pickPosition);
//
//			int xdist = pickPosition[0] - PreviousPosition[0];
//			int ydist = pickPosition[1] - PreviousPosition[1];
//
//			PreviousPosition[0] = pickPosition[0];
//			PreviousPosition[1] = pickPosition[1];
//
//			int moveDistance = (int)sqrt((double)(xdist*xdist + ydist*ydist));
//
//			// Reset numClicks - If mouse moved further than resetPixelDistance
//			if (moveDistance > ResetPixelDistance)
//			{
//				NumberOfClicks = 1;
//			}
//
//			if (NumberOfClicks == 2)
//			{
//				if (!FileFlag)
//				{
//					char filename[512];
//					CreateFileName(filename, sizeof(filename));
//					if (_access("Output", 0))
//						if (_mkdir("Output"))
//						{
//							std::cerr << "Create directory failed!!!" << std::endl;
//						}
//					fout.open(filename);
//					assert(fout.is_open());
//					std::cout << "File " << filename << " has been created." << std::endl;
//					FileFlag = true;
//					fout << "x, y, z\n";
//				}
//				fout << image_coordinate[0] << ", " <<
//					image_coordinate[1] << ", " <<
//					image_coordinate[2] << "\n";
//				NumberOfClicks = 0;
//			}
//			style->OnLeftButtonDown();
//			break;
//		}
//		default:
//			break;
//		}
//	}
//};
//
//void showMessage(vtkSmartPointer<vtkDICOMImageReader> reader, std::string &info)
//{
//	char buffer[2048];
//	char *ID = "";
//	char *BirthDate = "";
//	vtkImageData* image = reader->GetOutput();
//	double* origin = image->GetOrigin();
//	double* spacing = image->GetSpacing();
//	strcpy_s(PatientName, sizeof(PatientName), reader->GetPatientName());
//
//	sprintf_s(buffer, sizeof(buffer),
//		"Name: %s\n\
//		Image ID: %s\n\
//		Birth Date: %s\n\
//		Origin: ( %.3f, %.3f, %.3f )\n\
//		Spacing: ( %f, %f, %f )\n\
//		Height: %d\n\
//		Width: %d\n\
//		Rescale Intercept: %d\n\
//		Rescale Slope: %d",
//		PatientName, ID, BirthDate, 
//		origin[0], origin[1], origin[2], spacing[0], spacing[1], spacing[2],
//		reader->GetHeight(), reader->GetWidth(),
//		(int)(reader->GetRescaleOffset()), (int)(reader->GetRescaleSlope()));
//	
//	info += buffer;
//
//}
//
//void showMessage(vtkSmartPointer<vtkMetaImageReader> reader, std::string &info)
//{
//	char buffer[2048];
//	char *ID = reader->GetPatientID();
//	char *BirthDate = reader->GetDate();
//	vtkImageData* image = reader->GetOutput();
//	double* origin = image->GetOrigin();
//	double* spacing = image->GetSpacing();
//	strcpy_s(PatientName, sizeof(PatientName), reader->GetPatientName());
//	if (strcmp(PatientName, "?") == 0)
//	{
//		char *shortname = strrchr(name, 92) + 1;
//		int len = strnlen_s(name, sizeof(name)) - (shortname - name);
//		shortname[len - 4] = '\0';
//		strcpy_s(PatientName, sizeof(PatientName), shortname);
//	}
//
//	sprintf_s(buffer, sizeof(buffer),
//		"Name: %s\n\
//		Image ID: %s\n\
//		Birth Date: %s\n\
//		Origin: ( %.3f, %.3f, %.3f )\n\
//		Spacing: ( %f, %f, %f )\n\
//		Height: %d\n\
//		Width: %d\n\
//		Rescale Intercept: %d\n\
//		Rescale Slope: %d",
//		PatientName, ID, BirthDate,
//		origin[0], origin[1], origin[2], spacing[0], spacing[1], spacing[2],
//		reader->GetHeight(), reader->GetWidth(),
//		(int)(reader->GetRescaleOffset()), (int)(reader->GetRescaleSlope()));
//
//	info += buffer;
//
//}
//
//int main(int argc, char* argv[])
//{
//	// Verify input arguments
//	bool IsMeta = false;
//	if (argc < 2)
//	{
//		//TCHAR szBuffer[256] = { 0 };
//		//OPENFILENAME file = { 0 };
//		//file.hwndOwner = NULL;
//		//file.lStructSize = sizeof(file);
//		//file.lpstrFilter = _T("所有文件(*.*)\0*.*\0Dicom文件(*.dcm)\0*.dcm\0mhd文件(*.mhd)\0*.mhd\0");//要选择的文件后缀 
//		//file.lpstrFile = szBuffer;//存放文件的缓冲区 
//		//file.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
//		//file.nFilterIndex = 0;
//		//file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//标志如果是多选要加上OFN_ALLOWMULTISELECT
//		//BOOL bSel = GetOpenFileName(&file);
//
//		TCHAR szBuffer[MAX_PATH] = { 0 };
//		BROWSEINFO bi = { 0 };
//		bi.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄  
//		bi.pszDisplayName = szBuffer;//接收文件夹的缓冲区  
//		bi.lpszTitle = TEXT("选择一个文件或文件夹");//标题  
//		bi.ulFlags = BIF_USENEWUI | BIF_BROWSEINCLUDEFILES;
//		LPITEMIDLIST idl = SHBrowseForFolder(&bi);
//		SHGetPathFromIDList(idl, szBuffer);
//
//		int iLength;
//		iLength = WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, NULL, 0, NULL, NULL);
//		WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, name, iLength, NULL, NULL);
//
//		if (_access(name, 0))
//			return EXIT_FAILURE;
//
//	}
//	else
//	{
//		strcpy_s(name, sizeof(name), argv[1]);
//	}
//	int nameLen = strnlen_s(name, sizeof(name));
//	char *extent = &name[nameLen - 3];
//	if (!strcmp(extent, "mhd"))
//	{
//		IsMeta = true;
//	}
//	
//	// 创建图像数据读取器
//	vtkSmartPointer<vtkMetaImageReader> metaReader = vtkSmartPointer<vtkMetaImageReader>::New();
//	vtkSmartPointer<vtkDICOMImageReader> DicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
//	if (IsMeta)
//	{
//		metaReader->SetFileName(name);
//		metaReader->Update();
//	}
//	else
//	{
//		DicomReader->SetDirectoryName(name);
//		DicomReader->Update();
//	}
//
//	// 创建拾取器
//	vtkSmartPointer<vtkPropPicker> propPicker = vtkSmartPointer<vtkPropPicker>::New();
//	propPicker->PickFromListOn();
//	// 创建显示器
//	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
//	if (IsMeta)
//		imageViewer->SetInputConnection(metaReader->GetOutputPort());
//	else
//		imageViewer->SetInputConnection(DicomReader->GetOutputPort());
//	// 把交互器添加到拾取列表
//	propPicker->AddPickList(imageViewer->GetImageActor());
//
//	// 显示交互信息
//	vtkSmartPointer<vtkTextProperty> coordTextProp = vtkSmartPointer<vtkTextProperty>::New();
//	coordTextProp->SetFontSize(20);
//	coordTextProp->SetVerticalJustificationToBottom();
//	coordTextProp->SetJustificationToLeft();
//	vtkSmartPointer<vtkTextMapper> coordTextMapper = vtkSmartPointer<vtkTextMapper>::New();
//	coordTextMapper->SetInput("Location: ( 0, 0, 0 )\nValue: ");
//	coordTextMapper->SetTextProperty(coordTextProp);
//	vtkSmartPointer<vtkActor2D> coordTextActor = vtkSmartPointer<vtkActor2D>::New();
//	coordTextActor->SetMapper(coordTextMapper);
//	coordTextActor->SetPosition(15, 10);
//
//	// 显示病人信息
//	std::string info;
//	if (IsMeta)
//		showMessage(metaReader, info);
//	else
//		showMessage(DicomReader, info);
//	
//	vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
//	cornerAnnotation->SetLinearFontScaleFactor(2);
//	cornerAnnotation->SetNonlinearFontScaleFactor(1);
//	cornerAnnotation->SetMaximumFontSize(20);
//	cornerAnnotation->SetText(2, info.c_str());
//	cornerAnnotation->GetTextProperty()->SetColor(1, 1, 1);
//
//	imageViewer->GetRenderer()->AddViewProp(cornerAnnotation);
//
//	// 显示选中点
//	vtkSmartPointer<vtkTextProperty> pointTextProp = vtkSmartPointer<vtkTextProperty>::New();
//	pointTextProp->SetFontSize(20);
//	pointTextProp->SetVerticalJustificationToTop();
//	pointTextProp->SetJustificationToRight();
//	vtkSmartPointer<vtkTextMapper> pointTextMapper = vtkSmartPointer<vtkTextMapper>::New();
//	pointTextMapper->SetInput("");
//	pointTextMapper->SetTextProperty(pointTextProp);
//	vtkSmartPointer<vtkActor2D> pointTextActor = vtkSmartPointer<vtkActor2D>::New();
//	pointTextActor->SetMapper(pointTextMapper);
//	pointTextActor->SetPosition(1285, 690);
//
//	// 创建交互器
//	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//	imageViewer->SetupInteractor(renderWindowInteractor);
//	
//	// 定义回调函数
//	vtkSmartPointer<vtkImageInteractionCallback> callback = vtkSmartPointer<vtkImageInteractionCallback>::New();
//	callback->SetViewer(imageViewer);
//	callback->SetStatusMapper(coordTextMapper);
//	callback->SetPicker(propPicker);
//
//	// 定义交互方式
//	vtkInteractorStyleImage* imageStyle = imageViewer->GetInteractorStyle();
//	imageStyle->AddObserver(vtkCommand::MouseMoveEvent, callback);
//	imageStyle->AddObserver(vtkCommand::MouseWheelForwardEvent, callback);
//	imageStyle->AddObserver(vtkCommand::MouseWheelBackwardEvent, callback);
//	imageStyle->AddObserver(vtkCommand::KeyPressEvent, callback);
//	imageStyle->AddObserver(vtkCommand::TimerEvent, callback);
//	imageStyle->AddObserver(vtkCommand::MiddleButtonPressEvent, callback);
//	imageStyle->AddObserver(vtkCommand::LeftButtonPressEvent, callback);
//
//	
//	imageViewer->SetColorLevel(0);
//	imageViewer->SetColorWindow(2000);
//	imageViewer->SetSliceOrientationToXY();
//	imageViewer->GetRenderer()->AddActor2D(coordTextActor);
//	imageViewer->GetRenderer()->AddActor2D(pointTextActor);
//	vtkRenderer* renderer = imageViewer->GetRenderer();
//	imageViewer->GetRenderWindow()->SetSize(1300, 700);
//	//imageViewer->GetRenderWindow()->FullScreenOn();
//	//imageViewer->GetRenderWindow()->BordersOn();
//	renderer->ResetCamera();
//	if (IsMeta)
//		renderer->GetActiveCamera()->SetViewUp(0, -1, 0);
//	imageViewer->Render();
//	renderer->SetBackground(0, 0, 0);
//	imageViewer->Render();
//
//	renderWindowInteractor->Initialize();
//	renderWindowInteractor->Start();
//	// 释放数据
//	imageViewer->GetRenderer()->RemoveViewProp(imageViewer->GetImageActor());
//
//	if (fout.is_open())
//	{
//		fout.close();
//		std::cout << "File has been closed." << std::endl;
//	}
//	
//	return EXIT_SUCCESS;
//}
//
//// C:\DataSet\DSB2017\stage1\00cba091fa4ad62cc3200a657aeb957e
//// C:\DataSet\DSB2017\stage1\4d7df08f074b221eec6311c2617a5ba8
//// C:\DataSet\Luna16\data_set\1.3.6.1.4.1.14519.5.2.1.6279.6001.100225287222365663678666836860.mhd
