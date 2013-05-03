
#define CINTERFACE

#include "d3d9.h"
#include "d3dx9.h"
#define NOMINMAX 
#include <windows.h>
#include <string>
#include <sstream>
#include <tchar.h>
#include <fstream>
#include <algorithm>
#include <shlwapi.h>


#include <Python.h>
#include <commctrl.h>
#include <richedit.h>

#include <process.h>

#include "resource.h"
#include "MMDExport.h"
#define M_PI 3.1415926535897932384626433832795

#ifdef WITH_ALEMBIC
#pragma comment(lib, "AlembicAbc.lib")
#pragma comment(lib, "AlembicAbcCoreAbstract.lib")
#pragma comment(lib, "AlembicAbcCoreHDF5.lib")
#pragma comment(lib, "AlembicAbcGeom.lib")
#pragma comment(lib, "AlembicUtil.lib")

#include <Alembic/Abc/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreHDF5/All.h>

namespace AbcA = Alembic::AbcCoreAbstract;
//using namespace Alembic::AbcGeom;

class AlembicArchive {
public:
	
	static AlembicArchive& instance() { 
		static AlembicArchive instance;
		return instance; 
	}

	Alembic::Abc::OArchive * archive;
	AbcA::uint32_t timeindex;

	AbcA::TimeSamplingPtr timesampling;
	

	typedef std::map<int, Alembic::AbcGeom::OXform > XformMap;
	XformMap xformMap;

	typedef std::map<int, Alembic::AbcGeom::OXformSchema > XformSchemaMap;
	XformSchemaMap xformSchemaMap;

	typedef std::map<int, Alembic::AbcGeom::OPolyMesh > MeshMap;
	MeshMap meshMap;

	typedef std::map<int, Alembic::AbcGeom::OPolyMeshSchema > SchemaMap;
	SchemaMap schemaMap;
	
	typedef std::map<int, int > SurfaceSizeMap;
	SurfaceSizeMap surfaceSizeMap;
	
	typedef std::map<int, Alembic::AbcGeom::OCamera > CameraMap;
	CameraMap cameraMap;
	
	typedef std::map<int, Alembic::AbcGeom::OCameraSchema  > CameraSchemaMap;
	CameraSchemaMap cameraSchemaMap;

	typedef std::map<int, int> FiToVi;
	typedef std::map<int, FiToVi> FiToViMap;
	FiToViMap fiToViMap;

	bool isExportNormals;
	bool isExportUvs;
	int exportMode;

	void end() { 
		xformMap.clear();
		xformSchemaMap.clear();
		meshMap.clear();
		schemaMap.clear();
		cameraMap.clear();
		cameraSchemaMap.clear();
		fiToViMap.clear();
		surfaceSizeMap.clear();
		{
			delete archive; archive = NULL;
		}
	}
private:
	AlembicArchive() : archive(NULL), timeindex(0), exportMode(0) {}
};

#endif //WITH_ALEMBIC

template <class T> std::wstring to_wstring(T value)
{
    std::wstringstream converter;
    std::wstring  wstr;
    converter << value;
    converter >> wstr;
    return wstr;
}

template <class T> std::string to_string(T value)
{
    std::stringstream converter;
    std::string  str;
    converter << value;
    converter >> str;
    return str;
}

//ワイド文字列からマルチバイト文字列
//ロケール依存
static void to_string(std::string &dest, const std::wstring &src) {
	char *mbs = new char[src.length() * MB_CUR_MAX + 1];
	wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
	dest = mbs;
	delete [] mbs;
}

//マルチバイト文字列からワイド文字列
//ロケール依存
static void to_wstring(std::wstring &dest, const std::string &src) {
	wchar_t *wcs = new wchar_t[src.length() + 1];
	mbstowcs(wcs, src.c_str(), src.length() + 1);
	dest = wcs;
	delete [] wcs;
}

static void messagebox(std::string title, std::string message)
{
	::MessageBoxA(NULL, message.c_str(), title.c_str(), MB_OK);
}

static void messagebox_float4(float v[4], const char *title)
{
	::MessageBoxA(NULL, std::string(
		to_string(v[0]) + " "
		+ to_string(v[1]) + " "
		+ to_string(v[2]) + " "
		+ to_string(v[3]) + "\n").c_str(), title, MB_OK);
}

static void messagebox_matrix(D3DXMATRIX& mat, const char *title)
{
	::MessageBoxA(NULL,
		std::string(
		to_string(mat._11)+" "+to_string(mat._12)+" "+to_string(mat._13)+" "+to_string(mat._14)+"\n"
		+to_string(mat._21)+" "+to_string(mat._22)+" "+to_string(mat._23)+" "+to_string(mat._24)+"\n"
		+to_string(mat._31)+" "+to_string(mat._32)+" "+to_string(mat._33)+" "+to_string(mat._34)+"\n"
		+to_string(mat._41)+" "+to_string(mat._42)+" "+to_string(mat._43)+" "+to_string(mat._44)+"\n").c_str(), title, MB_OK);
}

// IDirect3DDevice9のフック関数
void hookDevice(void);
void originalDevice(void);
// フックしたデバイス
IDirect3DDevice9 *p_device;

// pythonマルチスレッド
unsigned int WINAPI runScript(void* data);

RenderData renderData;

int primitiveCounter = 0;

typedef std::vector<IDirect3DVertexBuffer9* > VertexBufferList;
VertexBufferList finishBuffers;
VertexBufferList finishBuffersCopy;
std::vector<std::pair<IDirect3DTexture9*, bool> > finishTextureBuffers;

std::map<IDirect3DVertexBuffer9*, RenderedBuffer> renderedBuffers;
std::map<IDirect3DTexture9*, RenderedTexture> renderedTextures;
std::map<int, std::map<int , RenderedMaterial*> > renderedMaterials;
//-----------------------------------------------------------------------------------------------------------------

static bool writeTextureToFile(std::string &texturePath, IDirect3DTexture9 * texture, D3DXIMAGE_FILEFORMAT fileFormat);

static bool writeTextureToFiles(std::string &texturePath, std::string &textureType, bool uncopied = false);

static bool copyTextureToFiles(std::string &texturePath);

static bool writeTextureToMemory(std::string &textureName, IDirect3DTexture9 * texture, bool copied);



//------------------------------------------Python呼び出し--------------------------------------------------------
CRITICAL_SECTION criticalSection;
HANDLE	hMutex; //ミューテックスのハンドル

static float preFrameTime = 0.0f;
static int preFrame = 0;
static int presentCount = 0;
static int currentFrame = 0;

// 行列で3Dベクトルをトランスフォームする
// D3DXVec3Transformとほぼ同じ
static void d3d_vector3_dir_transform(
	D3DXVECTOR3 &dst, 
	const D3DXVECTOR3 &src, 
	const D3DXMATRIX &matrix)
{
	const float tmp[] = {
		src.x*matrix.m[0][0] + src.y*matrix.m[1][0] + src.z*matrix.m[2][0],
		src.x*matrix.m[0][1] + src.y*matrix.m[1][1] + src.z*matrix.m[2][1],
		src.x*matrix.m[0][2] + src.y*matrix.m[1][2] + src.z*matrix.m[2][2]
	};
	dst.x = tmp[0];
	dst.y = tmp[1];
	dst.z = tmp[2];
}

static void d3d_vector3_transform(
	D3DXVECTOR3 &dst, 
	const D3DXVECTOR3 &src, 
	const D3DXMATRIX &matrix)
{
	const float tmp[] = {
		src.x*matrix.m[0][0] + src.y*matrix.m[1][0] + src.z*matrix.m[2][0] + 1.0f*matrix.m[3][0],
		src.x*matrix.m[0][1] + src.y*matrix.m[1][1] + src.z*matrix.m[2][1] + 1.0f*matrix.m[3][1],
		src.x*matrix.m[0][2] + src.y*matrix.m[1][2] + src.z*matrix.m[2][2] + 1.0f*matrix.m[3][2]
	};
	dst.x = tmp[0];
	dst.y = tmp[1];
	dst.z = tmp[2];
}

// python
extern "C" {
	static std::string basePath; //ベースディレクトリ
	static std::wstring wBasePath; //ベースディレクトリ
	static std::string mmdbridge_python_script; //pythonスクリプト
	static std::wstring pythonScript; // スクリプトパス
	static std::wstring pythonName; // スクリプト名
	static std::vector<std::wstring> pythonNames; //スクリプト名
	static std::vector<std::wstring> pythonScripts; //スクリプトパス
	static std::string python_error_string; //pythonから出たエラー
	static int scriptCallSetting = 2; // スクリプト呼び出し設定
	static int startFrame = 5;
	static int endFrame = 100;
	std::map<int, int> exportedFrames;
	bool isExportedFrame = false;
	static int frameWidth = 800;
	static int frameHeight = 450;
	static int exportFPS = 30;
	static bool is_texture_buffer_enabled = false;
	static RenderedMaterial* currentRenderedMaterial = NULL;
	static std::map<int, int> py_int_map;
	static std::map<int, float> py_float_map;

	static HANDLE pythonThreadHandle = NULL;

	// 非エクスポート	
	static bool relaod_python_script()
	{
		mmdbridge_python_script = "";
		std::ifstream ifs(pythonScript.c_str());
		if (!ifs) return false;
		char buf[2048];
		while (ifs.getline( buf, sizeof(buf))) {
			mmdbridge_python_script.append(buf);
			mmdbridge_python_script.append("\r\n");
		}
		ifs.close();
		return true;
	}

	
	// 非エクスポート
	static void reload_python_file_paths()
	{
		TCHAR app_full_path[1024];	// アプリフルパス
		
		GetModuleFileName(NULL, app_full_path, sizeof(app_full_path) / sizeof(TCHAR));

		std::wstring searchPath = std::wstring(app_full_path);
		searchPath = searchPath.substr(0, searchPath.rfind(_T("MikuMikuDance.exe")));

		std::wstring searchStr(searchPath + _T("\\*.py"));

		// pythonファイル検索
		WIN32_FIND_DATA find;
		HANDLE hFind = FindFirstFile(searchStr.c_str(), &find);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if(! (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					std::wstring name( find.cFileName);
					std::wstring path(searchPath + find.cFileName);
					// ファイルだった
					if (pythonName.empty()) { 
						pythonName = name;
						pythonScript = path;
					}
					pythonNames.push_back(name);
					pythonScripts.push_back(path);
				}
			} while(FindNextFile(hFind, &find));
			FindClose(hFind);
		}
	}

	// Get a reference to the main module.
	static PyObject* main_module = NULL; 

	// Get the main module's dictionary
	// and make a copy of it.
	static PyObject* main_dict = NULL;

	static PyObject * get_vertex_buffer_size(PyObject *self, PyObject *args)
	{
		return Py_BuildValue("i", finishBuffers.size());
	}

	static PyObject * get_vertex_size(PyObject *self, PyObject *args)
	{
		int at;
		if (!PyArg_ParseTuple(args, "i", &at)) { return NULL; }
		return Py_BuildValue("i", renderedBuffers[finishBuffers[at]].vertecies.size());
	}

	static PyObject * get_vertex(PyObject *self, PyObject *args)
	{
		int at;
		int vpos;
		if (!PyArg_ParseTuple(args, "ii", &at, &vpos)) { return NULL; } 
		float x = renderedBuffers[finishBuffers[at]].vertecies[vpos].x;
		float y = renderedBuffers[finishBuffers[at]].vertecies[vpos].y;
		float z = renderedBuffers[finishBuffers[at]].vertecies[vpos].z;

		return Py_BuildValue("[f,f,f]", x, y, z);
	}

	static PyObject * get_normal_size(PyObject *self, PyObject *args)
	{
		int at;
		if (!PyArg_ParseTuple(args, "i", &at)) { return NULL; }
		return Py_BuildValue("i", renderedBuffers[finishBuffers[at]].normals.size());
	}

	static PyObject * get_normal(PyObject *self, PyObject *args)
	{
		int at;
		int vpos;
		if (!PyArg_ParseTuple(args, "ii", &at, &vpos)) { return NULL; } 
		float x = renderedBuffers[finishBuffers[at]].normals[vpos].x;
		float y = renderedBuffers[finishBuffers[at]].normals[vpos].y;
		float z = renderedBuffers[finishBuffers[at]].normals[vpos].z;

		return Py_BuildValue("[f,f,f]", x, y, z);
	}

	static PyObject * get_uv_size(PyObject *self, PyObject *args)
	{
		int at;
		if (!PyArg_ParseTuple(args, "i", &at)) { return NULL; }
		return Py_BuildValue("i", renderedBuffers[finishBuffers[at]].uvs.size());
	}

	static PyObject * get_uv(PyObject *self, PyObject *args)
	{
		int at;
		int vpos;
		if (!PyArg_ParseTuple(args, "ii", &at, &vpos)) { return NULL; } 
		float u = renderedBuffers[finishBuffers[at]].uvs[vpos].x;
		float v = renderedBuffers[finishBuffers[at]].uvs[vpos].y;

		return Py_BuildValue("[f, f]", u, v);
	}

	static PyObject * get_material_size(PyObject *self, PyObject *args)
	{
		int at;
		if (!PyArg_ParseTuple(args, "i", &at)) { return NULL; } 
		return Py_BuildValue("i", renderedBuffers[finishBuffers[at]].materials.size());
	}

	static PyObject * is_accessory(PyObject *self, PyObject *args)
	{
		int at;
		if (!PyArg_ParseTuple(args, "i", &at)) { return NULL; } 
		int result = 0;
		if (renderedBuffers[finishBuffers[at]].isAccessory)
		{
			result = 1;
		}
		return Py_BuildValue("i", result);
	}

	static PyObject * get_diffuse(PyObject *self, PyObject *args)
	{
		int at;
		int mpos;
		if (!PyArg_ParseTuple(args, "ii", &at, &mpos)) { return NULL; } 
		RenderedMaterial* mat = renderedBuffers[finishBuffers[at]].materials[mpos];
		return Py_BuildValue("[f,f,f,f]", mat->diffuse.x, mat->diffuse.y, mat->diffuse.z, mat->diffuse.w);
	}

	static PyObject * get_ambient(PyObject *self, PyObject *args)
	{
		int at;
		int mpos;
		if (!PyArg_ParseTuple(args, "ii", &at, &mpos)) { return NULL; } 
		RenderedMaterial* mat = renderedBuffers[finishBuffers[at]].materials[mpos];
		return Py_BuildValue("[f,f,f]", mat->ambient.x, mat->ambient.y, mat->ambient.z);
	}

	static PyObject * get_specular(PyObject *self, PyObject *args)
	{
		int at;
		int mpos;
		if (!PyArg_ParseTuple(args, "ii", &at, &mpos)) { return NULL; } 
		RenderedMaterial* mat = renderedBuffers[finishBuffers[at]].materials[mpos];
		return Py_BuildValue("[f,f,f]", mat->specular.x, mat->specular.y, mat->specular.z);
	}

	static PyObject * get_emissive(PyObject *self, PyObject *args)
	{
		int at;
		int mpos;
		if (!PyArg_ParseTuple(args, "ii", &at, &mpos)) { return NULL; } 
		RenderedMaterial* mat = renderedBuffers[finishBuffers[at]].materials[mpos];
		return Py_BuildValue("[f,f,f]", mat->emissive.x, mat->emissive.y, mat->emissive.z);
	}

	static PyObject * get_power(PyObject *self, PyObject *args)
	{
		int at;
		int mpos;
		if (!PyArg_ParseTuple(args, "ii", &at, &mpos)) { return NULL; } 
		RenderedMaterial* mat = renderedBuffers[finishBuffers[at]].materials[mpos];
		return Py_BuildValue("f", mat->power);
	}

	static PyObject * get_texture(PyObject *self, PyObject *args)
	{
		int at;
		int mpos;
		if (!PyArg_ParseTuple(args, "ii", &at, &mpos)) { return NULL; } 
		RenderedMaterial* mat = renderedBuffers[finishBuffers[at]].materials[mpos];
		return Py_BuildValue("s", mat->texture.c_str());
	}

	static PyObject * get_exported_texture(PyObject *self, PyObject *args)
	{
		int at;
		int mpos;
		if (!PyArg_ParseTuple(args, "ii", &at, &mpos)) { return NULL; } 
		RenderedMaterial* mat = renderedBuffers[finishBuffers[at]].materials[mpos];
		return Py_BuildValue("s", mat->memoryTexture.c_str());
	}

	static PyObject * get_face_size(PyObject *self, PyObject *args)
	{
		int at;
		int mpos;
		if (!PyArg_ParseTuple(args, "ii", &at, &mpos)) { return NULL; } 
		return Py_BuildValue("i", renderedBuffers[finishBuffers[at]].materials[mpos]->surface.faces.size());
	}

	static PyObject * get_face(PyObject *self, PyObject *args)
	{
		int at;
		int mpos;
		int fpos;
		if (!PyArg_ParseTuple(args, "iii", &at, &mpos, &fpos)) { return NULL; }
		RenderedSurface &surface = renderedBuffers[finishBuffers[at]].materials[mpos]->surface;
		int v1 = surface.faces[fpos].ix;
		int v2 = surface.faces[fpos].iy;
		int v3 = surface.faces[fpos].iz;
		return Py_BuildValue("[i,i,i]", v1, v2, v3);
	}

	static PyObject * get_texture_buffer_size(PyObject *self, PyObject *args)
	{
		return Py_BuildValue("i", finishTextureBuffers.size());
	}

	static PyObject * get_texture_size(PyObject *self, PyObject *args)
	{
		int at;
		if (!PyArg_ParseTuple(args, "i", &at)) { return NULL; }
		return Py_BuildValue("[i, i]", 
			renderedTextures[finishTextureBuffers[at].first].size.x,
			renderedTextures[finishTextureBuffers[at].first].size.y);
	}

	static PyObject * get_texture_name(PyObject *self, PyObject *args)
	{
		int at;
		if (!PyArg_ParseTuple(args, "i", &at)) { return NULL; }
		return Py_BuildValue("s", 
			renderedTextures[finishTextureBuffers[at].first].name.c_str());
	}

	static PyObject * get_texture_pixel(PyObject *self, PyObject *args)
	{
		int at;
		int tpos;
		if (!PyArg_ParseTuple(args, "ii", &at, &tpos)) { return NULL; }
		um_vector4 &rgba = renderedTextures[finishTextureBuffers[at].first].texture[tpos];
		return Py_BuildValue("[f, f, f, f]", rgba.x, rgba.y, rgba.z, rgba.w);
	}

	static PyObject * export_texture(PyObject *self, PyObject *args)
	{
		int at;
		int mpos;
		char *dst;
		if (!PyArg_ParseTuple(args, "iis", &at, &mpos, &dst)) { return NULL; } 
		RenderedMaterial* mat = renderedBuffers[finishBuffers[at]].materials[mpos];
		std::string path(dst);

		std::string textureType = path.substr(path.size() - 3, 3);

		D3DXIMAGE_FILEFORMAT fileFormat;
		if (textureType == "bmp" || textureType == "BMP") { fileFormat = D3DXIFF_BMP; }
		else if (textureType == "png" || textureType == "PNG") { fileFormat = D3DXIFF_PNG; }
		else if (textureType == "jpg" || textureType == "JPG") { fileFormat = D3DXIFF_JPG; }
		else if (textureType == "tga" || textureType == "TGA") { fileFormat = D3DXIFF_TGA; }
		else if (textureType == "dds" || textureType == "DDS") { fileFormat = D3DXIFF_DDS; }
		else if (textureType == "ppm" || textureType == "PPM") { fileFormat = D3DXIFF_PPM; }
		else if (textureType == "dib" || textureType == "DIB") { fileFormat = D3DXIFF_DIB; }
		else if (textureType == "hdr" || textureType == "HDR") { fileFormat = D3DXIFF_HDR; }
		else if (textureType == "pfm" || textureType == "PFM") { fileFormat = D3DXIFF_PFM; }
		else { return NULL; }

		int res = 0;
		if (mat->tex)
		{
			if (writeTextureToFile(std::string(path), mat->tex, fileFormat)) { res = 1; }
		}

		return Py_BuildValue("i", res);
	}

	static PyObject * export_textures(PyObject *self, PyObject *args)
	{
		char *s;
		char *t;
		if (!PyArg_ParseTuple(args, "ss", &s, &t)) { return NULL; }
		std::string path(s);
		std::string type(t);

		int res = 0;
		if (PathFileExistsA(path.c_str()) && PathFileExistsA(path.c_str())) {
			res = writeTextureToFiles(path, type) ? 1 : 0;
		}		 
		return Py_BuildValue("i", res);
	}

	static PyObject * export_uncopied_textures(PyObject *self, PyObject *args)
	{
		char *s;
		char *t;
		if (!PyArg_ParseTuple(args, "ss", &s, &t)) { return NULL; }
		std::string path(s);
		std::string type(t);

		int res = 0;
		if (PathFileExistsA(path.c_str()) && PathFileExistsA(path.c_str())) {
			res = writeTextureToFiles(path, type, true) ? 1 : 0;
		}		 
		return Py_BuildValue("i", res);
	}

	static PyObject * copy_textures(PyObject *self, PyObject *args)
	{
		char *s;
		if (!PyArg_ParseTuple(args, "s", &s)) { return NULL; }
		std::string path(s);

		int res = 0;
		if (PathFileExistsA(path.c_str()) && PathFileExistsA(path.c_str())) {
			res = copyTextureToFiles(path) ? 1 : 0;
		}		 
		return Py_BuildValue("i", res);
	}

	static  PyObject * get_base_path(PyObject *self, PyObject *args)
	{
		return Py_BuildValue("s", basePath.c_str());
	}

	static PyObject * get_camera_up(PyObject *self, PyObject *args)
	{
		D3DXVECTOR3 v;
		D3DXVECTOR3 dst;
		UMGetCameraUp(&v);
		d3d_vector3_dir_transform(dst, v,  renderedBuffers.begin()->second.world_inv);
		return Py_BuildValue("[f, f, f]",  dst.x, dst.y, dst.z);
	}

	static PyObject * get_camera_up_org(PyObject *self, PyObject *args)
	{
		D3DXVECTOR3 v;
		UMGetCameraUp(&v);
		return Py_BuildValue("[f, f, f]",  v.x, v.y, v.z);
	}
	
	static PyObject * get_camera_at(PyObject *self, PyObject *args)
	{
		D3DXVECTOR3 v;
		D3DXVECTOR3 dst;
		UMGetCameraAt(&v);
		d3d_vector3_transform(dst, v,  renderedBuffers.begin()->second.world_inv);
		return Py_BuildValue("[f, f, f]", dst.x, dst.y, dst.z);
	}

	static PyObject * get_camera_eye(PyObject *self, PyObject *args)
	{
		D3DXVECTOR3 v;
		D3DXVECTOR3 dst;
		UMGetCameraEye(&v);
		d3d_vector3_transform(dst, v, renderedBuffers.begin()->second.world_inv);

		return Py_BuildValue("[f, f, f]", dst.x, dst.y, dst.z);
	}

	static PyObject * get_camera_eye_org(PyObject *self, PyObject *args)
	{
		D3DXVECTOR3 v;
		UMGetCameraEye(&v);
		return Py_BuildValue("[f, f, f]", v.x, v.y, v.z);
	}

	static PyObject * get_camera_fovy(PyObject *self, PyObject *args)
	{
		D3DXVECTOR4 v;
		UMGetCameraFovLH(&v);
		return Py_BuildValue("f", v.x);
	}

	static PyObject * get_camera_aspect(PyObject *self, PyObject *args)
	{
		D3DXVECTOR4 v;
		UMGetCameraFovLH(&v);
		return Py_BuildValue("f", v.y);
	}

	static PyObject * get_camera_near(PyObject *self, PyObject *args)
	{
		D3DXVECTOR4 v;
		UMGetCameraFovLH(&v);
		return Py_BuildValue("f", v.y);
	}

	static PyObject * get_camera_far(PyObject *self, PyObject *args)
	{
		D3DXVECTOR4 v;
		UMGetCameraFovLH(&v);
		return Py_BuildValue("f", v.w);
	}
	
	static PyObject * get_frame_number(PyObject *self, PyObject *args)
	{
		static int preFrameNumber = -1;
		float time = ExpGetFrameTime();
		int frame = static_cast<int>(time * exportFPS);
		if (scriptCallSetting == 1 && preFrameNumber == frame)
		{
			++frame;
		}
		preFrameNumber = frame;

		return Py_BuildValue("i", frame);
	}

	static PyObject * get_start_frame(PyObject *self, PyObject *args)
	{
		return Py_BuildValue("i", startFrame);
	}
	
	static PyObject * get_end_frame(PyObject *self, PyObject *args)
	{
		return Py_BuildValue("i", endFrame);
	}

	static PyObject * get_frame_width(PyObject *self, PyObject *args)
	{
		return Py_BuildValue("i", frameWidth);
	}

	static PyObject * get_frame_height(PyObject *self, PyObject *args)
	{
		return Py_BuildValue("i", frameHeight);
	}

	static PyObject * get_light(PyObject *self, PyObject *args)
	{
		int at;
		if (!PyArg_ParseTuple(args, "i", &at)) { return NULL; }
		um_vector3 &light = renderedBuffers[finishBuffers[at]].light;
		return Py_BuildValue("[f, f, f]", light.x, light.y, light.z);
	}

	static PyObject * messagebox(PyObject *self, PyObject *args)
	{
		const char *str = "";
		const char *title = "info";
		if(!PyArg_ParseTuple(args, "s", &str, &title)) return NULL;
		::MessageBoxA(NULL, str, title, MB_OK);
		return Py_BuildValue("");
	}

	static PyObject * get_world(PyObject *self, PyObject* args)
	{
		int at;
		if (!PyArg_ParseTuple(args, "i", &at)) { return NULL; }
		D3DXMATRIX& world = renderedBuffers[finishBuffers[at]].world;
		return Py_BuildValue("[f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f]", 
			world.m[0][0], world.m[0][1], world.m[0][2], world.m[0][3],
			world.m[1][0], world.m[1][1], world.m[1][2], world.m[1][3],
			world.m[2][0], world.m[2][1], world.m[2][2], world.m[2][3],
			world.m[3][0], world.m[3][1], world.m[3][2], world.m[3][3]);
	}

	static PyObject * get_world_inv(PyObject *self, PyObject* args)
	{
		int at;
		if (!PyArg_ParseTuple(args, "i", &at)) { return NULL; }
		D3DXMATRIX& world_inv = renderedBuffers[finishBuffers[at]].world_inv;
		return Py_BuildValue("[f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f]", 
			world_inv.m[0][0], world_inv.m[0][1], world_inv.m[0][2], world_inv.m[0][3],
			world_inv.m[1][0], world_inv.m[1][1], world_inv.m[1][2], world_inv.m[1][3],
			world_inv.m[2][0], world_inv.m[2][1], world_inv.m[2][2], world_inv.m[2][3],
			world_inv.m[3][0], world_inv.m[3][1], world_inv.m[3][2], world_inv.m[3][3]);
	}

	static PyObject * get_view(PyObject *self, PyObject* args)
	{
		int at;
		if (!PyArg_ParseTuple(args, "i", &at)) { return NULL; }
		D3DXMATRIX& view = renderedBuffers[finishBuffers[at]].view;
		return Py_BuildValue("[f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f]", 
			view.m[0][0], view.m[0][1], view.m[0][2], view.m[0][3],
			view.m[1][0], view.m[1][1], view.m[1][2], view.m[1][3],
			view.m[2][0], view.m[2][1], view.m[2][2], view.m[2][3],
			view.m[3][0], view.m[3][1], view.m[3][2], view.m[3][3]);
	}

	static PyObject * get_projection(PyObject *self, PyObject* args)
	{
		int at;
		if (!PyArg_ParseTuple(args, "i", &at)) { return NULL; }
		D3DXMATRIX& projection = renderedBuffers[finishBuffers[at]].projection;
		return Py_BuildValue("[f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f]", 
			projection.m[0][0], projection.m[0][1], projection.m[0][2], projection.m[0][3],
			projection.m[1][0], projection.m[1][1], projection.m[1][2], projection.m[1][3],
			projection.m[2][0], projection.m[2][1], projection.m[2][2], projection.m[2][3],
			projection.m[3][0], projection.m[3][1], projection.m[3][2], projection.m[3][3]);
	}

	static PyObject * set_texture_buffer_enabled(PyObject *self, PyObject* args)
	{
		int enabled;
		if (!PyArg_ParseTuple(args, "i", &enabled)) { return NULL; }

		is_texture_buffer_enabled = (enabled == 1);
		
		return Py_BuildValue("i", 1); //success
	}

	static PyObject * set_current_material(PyObject *self, PyObject* args)
	{
		int at;
		int mpos;
		if (!PyArg_ParseTuple(args, "ii", &at, &mpos)) { return NULL; } 

		WaitForSingleObject(hMutex,INFINITE);
		const int bufferSize = static_cast<int>(renderedBuffers[finishBuffersCopy[at]].materials.size());
		const int totalSize = static_cast<int>(finishBuffersCopy.size());
		if (totalSize > at && bufferSize > mpos)
		{
			if (renderedBuffers.find(finishBuffersCopy[at]) != renderedBuffers.end())
			{
				currentRenderedMaterial = renderedBuffers[finishBuffersCopy[at]].materials[mpos];
			}
		}
		ReleaseMutex(hMutex);

		return Py_BuildValue("i", 1); //success
	}

	static PyObject * set_int_value(PyObject *self, PyObject* args)
	{
		int pos;
		int value;
		if (!PyArg_ParseTuple(args, "ii", &pos, &value)) { return Py_BuildValue(""); } 
		py_int_map[pos] = value;
		return Py_BuildValue("i", 1); //success
	}

	static PyObject * set_float_value(PyObject *self, PyObject* args)
	{
		int pos;
		float value;
		if (!PyArg_ParseTuple(args, "if", &pos, &value)) { return Py_BuildValue(""); } 
		py_float_map[pos] = value;
		return Py_BuildValue("i", 1); //success
	}

	static PyObject * get_int_value(PyObject *self, PyObject* args)
	{
		int pos;
		if (!PyArg_ParseTuple(args, "i", &pos)) { return NULL; } 
		if (py_int_map.find(pos) != py_int_map.end())
		{
			return Py_BuildValue("i", py_int_map[pos]);
		}
		return  Py_BuildValue(""); //None
	}

	static PyObject * get_float_value(PyObject *self, PyObject* args)
	{
		int pos;
		if (!PyArg_ParseTuple(args, "i", &pos)) { return NULL; } 
		if (py_float_map.find(pos) != py_float_map.end())
		{
			return Py_BuildValue("i", py_float_map[pos]);
		}
		return Py_BuildValue(""); //None
	}

	static PyObject * d3dx_invert_matrix(PyObject *self, PyObject* args)
	{
		D3DXMATRIX src;		
		if (!PyArg_ParseTuple(args, "(ffffffffffffffff)", 			
			&src.m[0][0], &src.m[0][1], &src.m[0][2], &src.m[0][3],
			&src.m[1][0], &src.m[1][1], &src.m[1][2], &src.m[1][3],
			&src.m[2][0], &src.m[2][1], &src.m[2][2], &src.m[2][3],
			&src.m[3][0], &src.m[3][1], &src.m[3][2], &src.m[3][3])) { return NULL; }

		D3DXMATRIX dst;
		::D3DXMatrixInverse(&dst, NULL, &src);

		return Py_BuildValue("[f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f]", 
			dst.m[0][0], dst.m[0][1], dst.m[0][2], dst.m[0][3],
			dst.m[1][0], dst.m[1][1], dst.m[1][2], dst.m[1][3],
			dst.m[2][0], dst.m[2][1], dst.m[2][2], dst.m[2][3],
			dst.m[3][0], dst.m[3][1], dst.m[3][2], dst.m[3][3]);

	}

	static PyObject * d3dx_vec3_normalize(PyObject *self, PyObject* args)
	{
		D3DXVECTOR3 vec;
		if (!PyArg_ParseTuple(args, "fff", &vec.x, &vec.y, &vec.z)) { return NULL; }
		::D3DXVec3Normalize(&vec, &vec);
		return Py_BuildValue("[f, f, f]", vec.x, vec.y, vec.z);
	}

#ifdef WITH_ALEMBIC
	
	static PyObject * start_alembic_export(PyObject *self, PyObject* args)
	{
		if (!AlembicArchive::instance().archive)
		{
			int isExportNomals = 0;
			int isExportUvs = 0;
			int exportMode = 0;
			const char *filepath = "";
			if (!PyArg_ParseTuple(args, "siii", &filepath, &exportMode, &isExportNomals, &isExportUvs)) { return NULL; }

			std::string output_path(filepath);
			if (output_path.empty()) 
			{
				output_path = std::string(basePath + "out\\alembic_file.abc");
			}

			AlembicArchive::instance().archive =
				new Alembic::Abc::OArchive(Alembic::AbcCoreHDF5::WriteArchive(),
				output_path.c_str());

			AlembicArchive &archive = AlembicArchive::instance();
				
			const double dt = 1.0 / 30.0;
			archive.timesampling = AbcA::TimeSamplingPtr(new AbcA::TimeSampling(dt, 0.0));
			archive.archive->addTimeSampling(*archive.timesampling);
			archive.isExportNormals = (isExportNomals != 0);
			archive.isExportUvs = (isExportUvs != 0);
			archive.exportMode = exportMode;

			return Py_BuildValue("i", 1); //success
		}
		return Py_BuildValue(""); //None 
	}

	static PyObject * end_alembic_export(PyObject *self, PyObject* args)
	{
		if (AlembicArchive::instance().archive)
		{
			AlembicArchive::instance().end();
			return Py_BuildValue("i", 1); //success
		}
		
		return Py_BuildValue("");// None
	}

	static void export_alembic_xform_by_material(AlembicArchive &archive, RenderedBuffer & renderedBuffer, int renderedBufferIndex)
	{
		Alembic::AbcGeom::OObject topObj(*archive.archive, Alembic::AbcGeom::kTop);

		for (int k = 0, ksize = static_cast<int>(renderedBuffer.materials.size()); k < ksize; ++k)
		{
			Alembic::AbcGeom::OPolyMesh polyMesh;
			const int key = renderedBufferIndex * 10000 + k;
				
			Alembic::AbcGeom::OXform xform;
			if (archive.xformMap.find(key) != archive.xformMap.end())
			{
				xform = archive.xformMap[key];
			}
			else
			{
				xform = Alembic::AbcGeom::OXform(topObj, "xform_" + to_string(renderedBufferIndex) + "_material_" + to_string(k) , archive.timesampling);
				archive.xformMap[key] = xform;
			}

			bool isFirstMesh = false;
			if (archive.meshMap.find(key) != archive.meshMap.end())
			{
				polyMesh = archive.meshMap[key];
			}
			else
			{
				polyMesh = Alembic::AbcGeom::OPolyMesh(xform, "mesh_" + to_string(renderedBufferIndex) + "_material_" + to_string(k), archive.timesampling);
				archive.meshMap[key] = polyMesh;
				isFirstMesh = true;

				Alembic::AbcGeom::OPolyMeshSchema &meshSchema = polyMesh.getSchema();
				archive.schemaMap[key] = meshSchema;
			}

			if (archive.surfaceSizeMap.find(key) == archive.surfaceSizeMap.end())
			{
				archive.surfaceSizeMap[key] = 0;
			}

			if (archive.fiToViMap.find(key) == archive.fiToViMap.end())
			{
				AlembicArchive::FiToVi fiToVi;
				archive.fiToViMap[key] = fiToVi;
			}
			
			Alembic::AbcGeom::OPolyMeshSchema &meshSchema = archive.schemaMap[key];
			meshSchema.setTimeSampling(archive.timesampling);

			Alembic::AbcGeom::OPolyMeshSchema::Sample empty;
			
			std::vector<Alembic::Util::int32_t> faceList;
			std::vector<Alembic::Util::int32_t> faceCountList;

			RenderedBuffer::UVList &uvList = renderedBuffer.uvs;
			RenderedBuffer::VertexList &vertexList = renderedBuffer.vertecies;
			RenderedBuffer::NormalList &normalList =  renderedBuffer.normals;

			RenderedBuffer::VertexList vertexListByMaterial;
			RenderedBuffer::UVList uvListByMaterial;
			RenderedBuffer::NormalList normalListByMaterial;

			RenderedMaterial* material = renderedBuffer.materials.at(k);
			const int materialSurfaceSize = static_cast<int>(material->surface.faces.size());
			vertexListByMaterial.resize(materialSurfaceSize * 3);
			faceList.resize(materialSurfaceSize * 3);
			faceCountList.resize(materialSurfaceSize);

			if (!uvList.empty())
			{
				uvListByMaterial.resize(materialSurfaceSize * 3);
			}
			if (!normalList.empty())
			{
				normalListByMaterial.resize(materialSurfaceSize * 3);
			}
			
			AlembicArchive::FiToVi& fiToVi = archive.fiToViMap[key];
			int& preSurfaceSize = archive.surfaceSizeMap[key];
			bool isFirstSurface = material->surface.faces.size() != preSurfaceSize;
			if (!isFirstMesh && isFirstSurface)
			{
				continue;
			}

			// re assign par material
			int lastIndex = 0;

			for (int n = 0; n < materialSurfaceSize; ++n)
			{
				um_vector3 face = material->surface.faces[n];

				const int f1 = face.ix - 1;
				const int f2 = face.iy - 1;
				const int f3 = face.iz - 1;
				int vi1 = 0;
				int vi2 = 0;
				int vi3 = 0;

				if (isFirstMesh)
				{
					if (fiToVi.find(f1) == fiToVi.end()) {
						vi1 = lastIndex;
						fiToVi[f1] = vi1;
						++lastIndex;
					} else {
						vi1 = fiToVi[f1];
					}
				
					if (fiToVi.find(f2) == fiToVi.end()) {
						vi2 = lastIndex;
						fiToVi[f2] = vi2;
						++lastIndex;
					} else {
						vi2 = fiToVi[f2];
					}

					if (fiToVi.find(f3) == fiToVi.end()) {
						vi3 = lastIndex;
						fiToVi[f3] = vi3;
						++lastIndex;
					} else {
						vi3 = fiToVi[f3];
					}
				}
				else
				{
					vi1 = fiToVi[f1];
					vi2 = fiToVi[f2];
					vi3 = fiToVi[f3];
				}

				vertexListByMaterial[vi1] = vertexList.at(f1);
				vertexListByMaterial[vi2] = vertexList.at(f2);
				vertexListByMaterial[vi3] = vertexList.at(f3);
				if (!uvList.empty() && archive.isExportUvs)
				{
					uvListByMaterial[n * 3 + 0] = uvList.at(f1);
					uvListByMaterial[n * 3 + 1] = uvList.at(f2);
					uvListByMaterial[n * 3 + 2] = uvList.at(f3);
				}
				if (!normalList.empty() && archive.isExportNormals)
				{
					normalListByMaterial[vi1] = normalList.at(f1);
					normalListByMaterial[vi2] = normalList.at(f2);
					normalListByMaterial[vi3] = normalList.at(f3);
				}
				faceList[n * 3 + 0] = vi1;
				faceList[n * 3 + 1] = vi2;
				faceList[n * 3 + 2] = vi3;
				faceCountList[n] = 3;
			}

			preSurfaceSize = material->surface.faces.size();
			vertexListByMaterial.resize(fiToVi.size());
			normalListByMaterial.resize(fiToVi.size());
				
			for (int n = 0, nsize = vertexListByMaterial.size(); n < nsize; ++n)
			{
				vertexListByMaterial[n].z = -vertexListByMaterial[n].z;
			}

			Alembic::AbcGeom::OPolyMeshSchema::Sample sample;
				
			// vertex
			Alembic::AbcGeom::P3fArraySample positions( (const Imath::V3f *) &vertexListByMaterial.front(), vertexListByMaterial.size());
			sample.setPositions(positions);
				
			// face index
			if (isFirstMesh)
			{
				Alembic::Abc::Int32ArraySample faceIndices(faceList);
				Alembic::Abc::Int32ArraySample faceCounts(faceCountList);
				sample.setFaceIndices(faceIndices);
				sample.setFaceCounts(faceCounts);
			}

			// UVs
			if (!uvListByMaterial.empty() && archive.isExportUvs)
			{
				for (int n = 0, nsize = uvListByMaterial.size(); n < nsize; ++n)
				{
					uvListByMaterial[n].y = 1.0f - uvListByMaterial[n].y;
				}
				Alembic::AbcGeom::OV2fGeomParam::Sample uvSample;
				uvSample.setScope(Alembic::AbcGeom::kVertexScope );
				uvSample.setVals(Alembic::AbcGeom::V2fArraySample( ( const Imath::V2f *) &uvListByMaterial.front(), uvListByMaterial.size()));
				sample.setUVs(uvSample);
			}

			// Normals
			if (!normalListByMaterial.empty() && archive.isExportNormals)
			{
				for (int n = 0, nsize = normalListByMaterial.size(); n < nsize; ++n)
				{
					normalListByMaterial[n].z = -normalListByMaterial[n].z;
				}
				Alembic::AbcGeom::ON3fGeomParam::Sample normalSample;
				normalSample.setScope(Alembic::AbcGeom::kVertexScope );
				normalSample.setVals(Alembic::AbcGeom::N3fArraySample( (const Alembic::AbcGeom::N3f *) &normalListByMaterial.front(), normalListByMaterial.size()));
				sample.setNormals(normalSample);
			}
			
			meshSchema.set(sample);
		}
	}
	
	static void export_alembic_xform_by_buffer(AlembicArchive &archive, RenderedBuffer & renderedBuffer, int renderedBufferIndex)
	{
		Alembic::AbcGeom::OObject topObj(*archive.archive, Alembic::AbcGeom::kTop);

		Alembic::AbcGeom::OXform xform;
		if (archive.xformMap.find(renderedBufferIndex) != archive.xformMap.end())
		{
			xform = archive.xformMap[renderedBufferIndex];
		}
		else
		{
			xform = Alembic::AbcGeom::OXform(topObj, "xform_" + to_string(renderedBufferIndex), archive.timesampling);
			archive.xformMap[renderedBufferIndex] = xform;
		}
		
		bool isFirstMesh = false;
		Alembic::AbcGeom::OPolyMesh polyMesh;
		if (archive.meshMap.find(renderedBufferIndex) != archive.meshMap.end())
		{
			polyMesh = archive.meshMap[renderedBufferIndex];
		}
		else
		{
			polyMesh = Alembic::AbcGeom::OPolyMesh(xform, "mesh_" + to_string(renderedBufferIndex), archive.timesampling);
			archive.meshMap[renderedBufferIndex] = polyMesh;
			isFirstMesh = true;

			Alembic::AbcGeom::OPolyMeshSchema &meshSchema = polyMesh.getSchema();
			archive.schemaMap[renderedBufferIndex] = meshSchema;
		}

		Alembic::AbcGeom::OPolyMeshSchema &meshSchema = archive.schemaMap[renderedBufferIndex];
		meshSchema.setTimeSampling(archive.timesampling);
		
		std::vector<Alembic::Util::int32_t> faceList;
		std::vector<Alembic::Util::int32_t> faceCountList;
		
		RenderedBuffer::UVList &uvList = renderedBuffer.uvs;
		RenderedBuffer::VertexList &vertexList = renderedBuffer.vertecies;
		RenderedBuffer::NormalList &normalList =  renderedBuffer.normals;

		const int materialSize = static_cast<int>(renderedBuffer.materials.size());

		int totalFaceCount = 0;
		for (int k = 0; k < materialSize; ++k)
		{
			RenderedMaterial* material = renderedBuffer.materials.at(k);
			totalFaceCount += material->surface.faces.size();
		}
		faceCountList.resize(totalFaceCount);
		faceList.resize(totalFaceCount * 3);

		int faceCounter = 0;
		for (int k = 0; k < materialSize; ++k)
		{
			RenderedMaterial* material = renderedBuffer.materials.at(k);
			const int faceSize = material->surface.faces.size();
			for (int n = 0; n < faceSize; ++n)
			{
				um_vector3 face = material->surface.faces[n];
				faceList[faceCounter * 3 + 0] = (face.ix - 1);
				faceList[faceCounter * 3 + 1] = (face.iy - 1);
				faceList[faceCounter * 3 + 2] = (face.iz - 1);
				faceCountList[faceCounter] = 3;
				++faceCounter;
			}
		}

		Alembic::AbcGeom::OPolyMeshSchema::Sample sample;
				
		// vertex
		for (int n = 0, nsize = vertexList.size(); n < nsize; ++n)
		{
			vertexList[n].z = -vertexList[n].z;
		}
		Alembic::AbcGeom::P3fArraySample positions( (const Imath::V3f *) &vertexList.front(), vertexList.size());
		sample.setPositions(positions);
				
		// face index
		if (isFirstMesh)
		{
			Alembic::Abc::Int32ArraySample faceIndices(faceList);
			Alembic::Abc::Int32ArraySample faceCounts(faceCountList);
			sample.setFaceIndices(faceIndices);
			sample.setFaceCounts(faceCounts);
		}

		// UVs
		if (!uvList.empty() && archive.isExportUvs)
		{
			for (int n = 0, nsize = uvList.size(); n < nsize; ++n)
			{
				uvList[n].y = 1.0f - uvList[n].y;
			}
			Alembic::AbcGeom::OV2fGeomParam::Sample uvSample;
			uvSample.setScope(Alembic::AbcGeom::kVertexScope );
			uvSample.setVals(Alembic::AbcGeom::V2fArraySample( ( const Imath::V2f *) &uvList.front(), uvList.size()));
			sample.setUVs(uvSample);
		}

		// Normals
		if (!normalList.empty() && archive.isExportNormals)
		{
			for (int n = 0, nsize = normalList.size(); n < nsize; ++n)
			{
				normalList[n].z = -normalList[n].z;
			}
			Alembic::AbcGeom::ON3fGeomParam::Sample normalSample;
			normalSample.setScope(Alembic::AbcGeom::kVertexScope );
			normalSample.setVals(Alembic::AbcGeom::N3fArraySample( (const Alembic::AbcGeom::N3f *) &normalList.front(), normalList.size()));
			sample.setNormals(normalSample);
		}

		meshSchema.set(sample);

	}

	static double to_degree(double radian)
	{
		return (radian * 180) / M_PI;
	}

	static Imath::V3d quatToEuler(Imath::Quatd quat) {
		double xy = quat.v.x * quat.v.y;
		double zw = quat.v.z * quat.r;

		double test = xy + zw;
		if (test > 0.499) { // singularity at north pole
			double yaw = 2 * atan2(quat.v.x, quat.r);
			double pitch = M_PI/2;
			double roll = 0;
			return Imath::V3d(yaw, pitch, roll);
		}
		if (test < -0.499) { // singularity at south pole
			double yaw = -2 * atan2(quat.v.x, quat.r);
			double pitch = - M_PI/2;
			double roll = 0;
			return Imath::V3d(yaw, pitch, roll);
		}
		double xx = quat.v.x * quat.v.x;
		double yy = quat.v.y * quat.v.y;
		double zz = quat.v.z * quat.v.z;
		
		double yz = quat.v.y * quat.v.z;
		double xz = quat.v.x * quat.v.z;
		double wx = quat.r * quat.v.x;
		double wy = quat.r * quat.v.y;
		double wz = quat.r * quat.v.z;
		
		double yaw = atan2( 2*(wy - xz), 1 - 2*(yy + zz));
		double pitch = atan2( 2*(wx - yz), 1 - 2*(xx +zz));
		double roll = asin( 2*(test));
		return Imath::V3d(yaw, pitch, roll);
	}
	
	static void export_alembic_camera(AlembicArchive &archive, RenderedBuffer & renderedBuffer)
	{
		static const int cameraKey = 0xFFFFFF;
		Alembic::AbcGeom::OObject topObj(*archive.archive, Alembic::AbcGeom::kTop);

		Alembic::AbcGeom::OXform xform;
		if (archive.xformMap.find(cameraKey) != archive.xformMap.end())
		{
			xform = archive.xformMap[cameraKey];
		}
		else
		{
			xform = Alembic::AbcGeom::OXform(topObj, "camera_xform", archive.timesampling);
			archive.xformMap[cameraKey] = xform;

			Alembic::AbcGeom::OXformSchema &xformSchema = xform.getSchema();
			archive.xformSchemaMap[cameraKey] = xformSchema;
		}
		
		// set camera transform
		{
			Alembic::AbcGeom::OXformSchema &xformSchema = archive.xformSchemaMap[cameraKey];
			xformSchema.setTimeSampling(archive.timesampling);

			Alembic::AbcGeom::XformSample xformSample;

			D3DXMATRIX convertMat(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, -1, 0,
				0, 0, 0, 1);

			D3DXMATRIX convertedWordInv;
			::D3DXMatrixMultiply(&convertedWordInv, &renderedBuffer.world_inv, &convertMat);
			
			D3DXVECTOR3 eye;
			{
				D3DXVECTOR3 v;
				UMGetCameraEye(&v);
				d3d_vector3_transform(eye, v,convertedWordInv);
			}
			
			D3DXVECTOR3 at;
			{
				D3DXVECTOR3 v;
				UMGetCameraAt(&v);
				d3d_vector3_transform(at, v, convertedWordInv);
			}

			D3DXVECTOR3 up;
			{
				D3DXVECTOR3 v;
				UMGetCameraUp(&v);
				d3d_vector3_dir_transform(up, v, convertedWordInv);
				::D3DXVec3Normalize(&up, &up);
			}

			Imath::V3d trans((double)eye.x, (double)eye.y, (double)(eye.z));
			xformSample.setTranslation(trans);

			D3DXMATRIX view;
			::D3DXMatrixLookAtLH(&view, &eye, &at, &up);
			
			
			Imath::M44d rot(
				-view.m[0][0], view.m[0][1], view.m[0][2], 0,
				-view.m[1][0], view.m[1][1], view.m[1][2], 0,
				view.m[2][0], -view.m[2][1], -view.m[2][2], 0,
				0, 0, 0, 1);

			Imath::Quatd quat = Imath::extractQuat(rot);
			quat.normalize();

			Imath::V3d euler = quatToEuler(quat);
			xformSample.setXRotation(to_degree(euler.y));
			xformSample.setYRotation(to_degree(euler.x));
			xformSample.setZRotation(-to_degree(euler.z));
			//xformSample.setRotation(quat.axis(), to_degree(quat.angle()));

			xformSchema.set(xformSample);
		}
		
		Alembic::AbcGeom::OCamera camera;
		if (archive.cameraMap.find(cameraKey) != archive.cameraMap.end())
		{
			camera = archive.cameraMap[cameraKey];
		}
		else
		{
			camera = Alembic::AbcGeom::OCamera(xform, "camera", archive.timesampling);
			archive.cameraMap[cameraKey] = camera;
			
			Alembic::AbcGeom::OCameraSchema &cameraSchema = camera.getSchema();
			archive.cameraSchemaMap[cameraKey] = cameraSchema;
		}

		Alembic::AbcGeom::OCameraSchema &cameraSchema = archive.cameraSchemaMap[cameraKey];
		cameraSchema.setTimeSampling(archive.timesampling);
		Alembic::AbcGeom::CameraSample sample;

		
		D3DXVECTOR4 v;
		UMGetCameraFovLH(&v);

		sample.setNearClippingPlane(v.y);
		sample.setFarClippingPlane(v.w);

		cameraSchema.set(sample);
	}

	static PyObject * execute_alembic_export(PyObject *self, PyObject* args)
	{
		int currentframe;
		if (!PyArg_ParseTuple(args, "i", &currentframe)) { return Py_BuildValue(""); } 

		AlembicArchive &archive = AlembicArchive::instance();
		if (!archive.archive) { return Py_BuildValue(""); }

		for (int i = 0, isize = static_cast<int>(finishBuffers.size()); i < isize; ++i)
		{
			RenderedBuffer &renderedBuffer = renderedBuffers[finishBuffers.at(i)];

			if (i == 0)
			{
				export_alembic_camera(archive, renderedBuffer);
			}

			if (archive.exportMode == 0)
			{
				export_alembic_xform_by_material(archive, renderedBuffer, i);
			}
			else
			{
				export_alembic_xform_by_buffer(archive, renderedBuffer, i);
			}
		}

		return Py_BuildValue("i", 1); //success
	}

#endif //WITH_ALEMBIC

	static PyMethodDef PythonMethods[] = {
		{ "get_vertex_buffer_size", (PyCFunction)get_vertex_buffer_size, METH_VARARGS },
		{ "get_vertex_size", (PyCFunction)get_vertex_size, METH_VARARGS },
		{ "get_vertex", (PyCFunction)get_vertex, METH_VARARGS },
		{ "get_normal_size", (PyCFunction)get_normal_size, METH_VARARGS },
		{ "get_normal", (PyCFunction)get_normal, METH_VARARGS },
		{ "get_uv_size", (PyCFunction)get_uv_size, METH_VARARGS },
		{ "get_uv", (PyCFunction)get_uv, METH_VARARGS },
		{ "get_material_size", (PyCFunction)get_material_size, METH_VARARGS },
		{ "is_accessory", (PyCFunction)is_accessory, METH_VARARGS },
		{ "get_ambient", (PyCFunction)get_ambient, METH_VARARGS },
		{ "get_diffuse", (PyCFunction)get_diffuse, METH_VARARGS },
		{ "get_specular", (PyCFunction)get_specular, METH_VARARGS },
		{ "get_emissive", (PyCFunction)get_emissive, METH_VARARGS },
		{ "get_power", (PyCFunction)get_power, METH_VARARGS },
		{ "get_texture", (PyCFunction)get_texture, METH_VARARGS },
		{ "get_exported_texture", (PyCFunction)get_exported_texture, METH_VARARGS },
		{ "get_face_size", (PyCFunction)get_face_size, METH_VARARGS },
		{ "get_face", (PyCFunction)get_face, METH_VARARGS },
		{ "get_texture_buffer_size", (PyCFunction)get_texture_buffer_size, METH_VARARGS },
		{ "get_texture_size", (PyCFunction)get_texture_size, METH_VARARGS },
		{ "get_texture_name", (PyCFunction)get_texture_name, METH_VARARGS },
		{ "get_texture_pixel", (PyCFunction)get_texture_pixel, METH_VARARGS },
		{ "get_camera_up", (PyCFunction)get_camera_up, METH_VARARGS },
		{ "get_camera_up_org", (PyCFunction)get_camera_up_org, METH_VARARGS },
		{ "get_camera_at",  (PyCFunction)get_camera_at, METH_VARARGS },
		{ "get_camera_eye",  (PyCFunction)get_camera_eye, METH_VARARGS },
		{ "get_camera_eye_org",  (PyCFunction)get_camera_eye_org, METH_VARARGS },
		{ "get_camera_fovy", (PyCFunction)get_camera_fovy, METH_VARARGS },
		{ "get_camera_aspect", (PyCFunction)get_camera_aspect, METH_VARARGS },
		{ "get_camera_near", (PyCFunction)get_camera_near, METH_VARARGS },
		{ "get_camera_far", (PyCFunction)get_camera_far, METH_VARARGS },
		{ "messagebox", (PyCFunction)messagebox, METH_VARARGS },
		{ "export_texture", (PyCFunction)export_texture, METH_VARARGS },
		{ "export_textures", (PyCFunction)export_textures, METH_VARARGS },
		{ "export_uncopied_textures", (PyCFunction)export_textures, METH_VARARGS },
		{ "copy_textures", (PyCFunction)copy_textures, METH_VARARGS },
		{ "get_frame_number", (PyCFunction)get_frame_number, METH_VARARGS },
		{ "get_start_frame", (PyCFunction)get_start_frame, METH_VARARGS },
		{ "get_end_frame", (PyCFunction)get_end_frame, METH_VARARGS },
		{ "get_frame_width", (PyCFunction)get_frame_width, METH_VARARGS },
		{ "get_frame_height", (PyCFunction)get_frame_height, METH_VARARGS },
		{ "get_base_path", (PyCFunction)get_base_path, METH_VARARGS },
		{ "get_light", (PyCFunction)get_light, METH_VARARGS },
		{ "get_world", (PyCFunction)get_world, METH_VARARGS },
		{ "get_world_inv", (PyCFunction)get_world_inv, METH_VARARGS },
		{ "get_view", (PyCFunction)get_view, METH_VARARGS },
		{ "get_projection", (PyCFunction)get_projection, METH_VARARGS },
		{ "set_texture_buffer_enabled", (PyCFunction)set_texture_buffer_enabled, METH_VARARGS },
		{ "set_current_material", (PyCFunction)set_current_material, METH_VARARGS },
		{ "set_int_value", (PyCFunction)set_int_value, METH_VARARGS },
		{ "set_float_value", (PyCFunction)set_float_value, METH_VARARGS },
		{ "get_int_value", (PyCFunction)get_int_value, METH_VARARGS },
		{ "get_float_value", (PyCFunction)get_float_value, METH_VARARGS },
		{ "d3dx_invert_matrix", (PyCFunction)d3dx_invert_matrix, METH_VARARGS },
		{ "d3dx_vec3_normalize", (PyCFunction)d3dx_vec3_normalize, METH_VARARGS },
#ifdef WITH_ALEMBIC
		{ "start_alembic_export", (PyCFunction)start_alembic_export, METH_VARARGS },
		{ "end_alembic_export", (PyCFunction)end_alembic_export, METH_VARARGS },
		{ "execute_alembic_export", (PyCFunction)execute_alembic_export, METH_VARARGS },
#endif //WITH_ALEMBIC
		{NULL, NULL, 0, NULL}
	};

	static PyObject* output_error(PyObject *self, PyObject *args)
	{
		const char *str;
		if(!PyArg_ParseTuple(args, "s", &str)) return NULL;
		python_error_string += std::string(str);
		return Py_BuildValue("");
	}

	static PyMethodDef OutputErrorMethods[] = {
		{"write", output_error, METH_VARARGS, ""},
		{NULL, NULL, 0, NULL}
	};

	static struct PyModuleDef PythonMethodsModule = {
		PyModuleDef_HEAD_INIT,
		"mmdbridge",
		NULL,
        -1,
		PythonMethods
	};

	static struct PyModuleDef PythonOutputErrorModule = {
		PyModuleDef_HEAD_INIT,
		"output",
		NULL,
        -1,
		OutputErrorMethods
	};

	static PyObject *BridgeError;

	PyMODINIT_FUNC initfuncs()
	{
		PyObject *module = PyModule_Create(&PythonMethodsModule);
		PyImport_AddModule("mmdbridge");
		if (!module) {
			::MessageBox(NULL, _T("mmdbridge create failed"), _T("HOGE"), MB_OK);
		}

		PyObject *error_obj = PyModule_Create(&PythonOutputErrorModule);
		PyImport_AddModule("output");
		PySys_SetObject("stderr", error_obj);
		
		//// モジュール
		//PyObject *pModule = PyImport_ImportModule("mmdbridge");

		Py_InspectFlag = 1;


		return module;
	}
}

//-----------------------------------------------------------Hook関数ポインタ-----------------------------------------------------------

// Direct3DCreate9
IDirect3D9 *(WINAPI *original_direct3d_create)(UINT)(NULL);
// MME
IDirect3D9 *(WINAPI *mme_direct3d_create)(UINT)(NULL);
// IDirect3D9::CreateDevice
HRESULT (WINAPI *original_create_device)(IDirect3D9*,UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, IDirect3DDevice9**)(NULL);
// IDirect3DDevice9::BeginScene
HRESULT (WINAPI *original_begin_scene)(IDirect3DDevice9*)(NULL);
// IDirect3DDevice9::SetFVF
HRESULT (WINAPI *original_set_fvf)(IDirect3DDevice9*, DWORD);
// IDirect3DDevice9::Clear
HRESULT (WINAPI *original_clear)(IDirect3DDevice9*, DWORD, const D3DRECT*, DWORD, D3DCOLOR, float, DWORD)(NULL);
// IDirect3DDevice9::Present
HRESULT (WINAPI *original_present)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA *)(NULL);
// IDirect3DDevice9::Reset
HRESULT (WINAPI *original_reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)(NULL);

// IDirect3DDevice9::BeginStateBlock
// この関数で、lpVtblが修正されるので、lpVtbl書き換えなおす
HRESULT (WINAPI *original_begin_state_block)(IDirect3DDevice9 *)(NULL);

// IDirect3DDevice9::EndStateBlock
// この関数で、lpVtblが修正されるので、lpVtbl書き換えなおす
HRESULT (WINAPI *original_end_state_block)(IDirect3DDevice9*, IDirect3DStateBlock9**)(NULL);

// IDirect3DDevice9::DrawIndexedPrimitive
HRESULT (WINAPI *original_draw_indexed_primitive)(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT)(NULL);

// IDirect3DDevice9::SetStreamSource
HRESULT (WINAPI *original_set_stream_source)(IDirect3DDevice9*, UINT, IDirect3DVertexBuffer9*, UINT, UINT)(NULL);

// IDirect3DDevice9::SetIndices
HRESULT (WINAPI *original_set_indices)(IDirect3DDevice9*, IDirect3DIndexBuffer9*)(NULL);

// IDirect3DDevice9::CreateVertexBuffer
HRESULT (WINAPI *original_create_vertex_buffer)(IDirect3DDevice9*, UINT, DWORD, DWORD, D3DPOOL, IDirect3DVertexBuffer9**, HANDLE*)(NULL);

// IDirect3DDevice9::SetTexture
HRESULT (WINAPI *original_set_texture)(IDirect3DDevice9*, DWORD, IDirect3DBaseTexture9 *)(NULL);

// IDirect3DDevice9::CreateTexture
HRESULT (WINAPI *original_create_texture)(IDirect3DDevice9*, UINT, UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DTexture9**, HANDLE*)(NULL);
//-----------------------------------------------------------------------------------------------------------------------------

static bool writeTextureToFile(std::string &texturePath, IDirect3DTexture9 * texture, D3DXIMAGE_FILEFORMAT fileFormat)
{
	TextureBuffers::iterator tit = renderData.textureBuffers.find(texture);
	if(tit != renderData.textureBuffers.end())
	{
		if (texture->lpVtbl) {
			D3DXSaveTextureToFileA(texturePath.c_str(), fileFormat,(LPDIRECT3DBASETEXTURE9) texture, NULL);
			return true;
		}
	}
	return false;
}

static bool writeTextureToFiles(std::string &texturePath, std::string &textureType, bool uncopied)
{
	bool res = true;

	D3DXIMAGE_FILEFORMAT fileFormat;
	if (textureType == "bmp" || textureType == "BMP") { fileFormat = D3DXIFF_BMP; }
	else if (textureType == "png" || textureType == "PNG") { fileFormat = D3DXIFF_PNG; }
	else if (textureType == "jpg" || textureType == "JPG") { fileFormat = D3DXIFF_JPG; }
	else if (textureType == "tga" || textureType == "TGA") { fileFormat = D3DXIFF_TGA; }
	else if (textureType == "dds" || textureType == "DDS") { fileFormat = D3DXIFF_DDS; }
	else if (textureType == "ppm" || textureType == "PPM") { fileFormat = D3DXIFF_PPM; }
	else if (textureType == "dib" || textureType == "DIB") { fileFormat = D3DXIFF_DIB; }
	else if (textureType == "hdr" || textureType == "HDR") { fileFormat = D3DXIFF_HDR; }
	else if (textureType == "pfm" || textureType == "PFM") { fileFormat = D3DXIFF_PFM; }
	else { return false; }

	char dir[MAX_PATH];
	std::strcpy(dir, texturePath.c_str());
	PathRemoveFileSpecA(dir);
	
	for (size_t i = 0; i <  finishTextureBuffers.size(); ++i)
	{
		IDirect3DTexture9* texture = finishTextureBuffers[i].first;
		bool copied = finishTextureBuffers[i].second;
		if (texture) {
			if (uncopied)
			{
				if (!copied)
				{
					char path[MAX_PATH];
					PathCombineA(path, dir, to_string(texture).c_str());
					if (!writeTextureToFile(std::string(path) + "." + textureType, texture, fileFormat)) { res = false; }
				}
			} else {
				char path[MAX_PATH];
				PathCombineA(path, dir, to_string(texture).c_str());
				if (!writeTextureToFile(std::string(path) + "." + textureType, texture, fileFormat)) { res = false; }
			}
		}
	}

	return res;
}

static bool copyTextureToFiles(std::string &texturePath)
{
	bool res = true;

	char dir[MAX_PATH];
	std::strcpy(dir, texturePath.c_str());
	PathRemoveFileSpecA(dir);
	PathAddBackslashA(dir);

	std::wstring wstr;
	to_wstring(wstr, std::string(dir));

	if (!PathIsDirectory(wstr.c_str())) { return false; }

	

	for (size_t i = 0; i <  finishTextureBuffers.size(); ++i)
	{
		IDirect3DTexture9* texture = finishTextureBuffers[i].first;
		if (texture) {
			
			if (!UMCopyTexture(wstr.c_str(), texture)) { res = false; }
		}
	}

	return res;
}

static bool writeTextureToMemory(std::string &textureName, IDirect3DTexture9 * texture, bool copied)
{
	//　すでにfinishTexutureBufferにあるかどうか
	bool found = false;
	for (size_t i = 0; i < finishTextureBuffers.size(); ++i)
	{
		if (finishTextureBuffers[i].first == texture) { found = true; break; }
	}

	if (!found)
	{
		// 書き出していなかったので書き出しファイルリストに入れる
		std::pair<IDirect3DTexture9*, bool> texturebuffer(texture, copied);
		finishTextureBuffers.push_back(texturebuffer);
	}

	if (is_texture_buffer_enabled)
	{
		TextureBuffers::iterator tit = renderData.textureBuffers.find(texture);
		if(tit != renderData.textureBuffers.end())
		{
			// テクスチャをメモリに書き出し
			D3DLOCKED_RECT lockRect;
			HRESULT isLocked = texture->lpVtbl->LockRect(texture, 0, &lockRect, NULL, D3DLOCK_READONLY);
			if (isLocked != D3D_OK) { return false; }
			
			int width = tit->second.wh.ix;
			int height = tit->second.wh.iy;

			RenderedTexture tex;
			tex.size.ix = width;
			tex.size.iy = height;
			tex.name = textureName;

			D3DFORMAT format = tit->second.format;
			for(int y = 0; y < height; y++) {
				unsigned char *lineHead = (unsigned char*)lockRect.pBits + lockRect.Pitch * y;

				for (int x = 0; x < width; x++)
				{
					if (format == D3DFMT_A8R8G8B8) {
						um_vector4 rgba;
						rgba.x = lineHead[4 * x + 0];
						rgba.y = lineHead[4 * x + 1];
						rgba.z = lineHead[4 * x + 2];
						rgba.w = lineHead[4 * x + 3];
						tex.texture.push_back(rgba);
					} else {
						::MessageBox(NULL, (_T("not supported texture format:") + to_wstring(format)).c_str(), _T("info"), MB_OK);
					}
				}
			}
			renderedTextures[texture] = tex;

			texture->lpVtbl->UnlockRect(texture, 0);
			return true;
		}
	}
	return false;
}

static HRESULT WINAPI beginScene(IDirect3DDevice9 *device) 
{
	HRESULT res = (*original_begin_scene)(device);
	return res;
}

HWND g_hWnd=NULL;	//ウィンドウハンドル
HMENU g_hMenu=NULL;	//メニュー
HWND g_hFrame = NULL; //フレーム数
HWND g_hFrameArrowLeft = NULL; //フレーム矢印右
HWND g_hFrameArrowRight = NULL; //フレーム矢印右


static void GetFrame(HWND hWnd)
{
	char text[256];
	::GetWindowTextA(hWnd, text, sizeof(text)/sizeof(text[0]));
		
	currentFrame= atoi(text);
}

static BOOL CALLBACK enumChildWindowsProc(HWND hWnd, LPARAM lParam)
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	WCHAR buf[10];
	GetWindowText(hWnd, buf, 10);

	if (!g_hFrameArrowLeft && wcscmp(buf, _T("＜")) == 0)
	{
		g_hFrameArrowLeft = hWnd;
	}
	if (!g_hFrameArrowRight && wcscmp(buf, _T("＞")) == 0)
	{	
		g_hFrameArrowRight = hWnd;
	}
	if (!g_hFrame && rect.right == 48 && rect.bottom == 22)
	{
		g_hFrame = hWnd;
		GetFrame(hWnd);
	}
	if (g_hFrame && g_hFrameArrowLeft && g_hFrameArrowRight)
	{
		return FALSE;
	}
	return TRUE;	//continue
}

//乗っ取り対象ウィンドウの検索
static BOOL CALLBACK enumWindowsProc(HWND hWnd,LPARAM lParam)
{
	if (g_hWnd && g_hFrame) {
		GetFrame(g_hFrame);
		return FALSE;
	}
	HANDLE hModule=(HANDLE)GetWindowLongA(hWnd,GWL_HINSTANCE);
	if(GetModuleHandle(NULL)==hModule){
		//自分のプロセスが作ったウィンドウを見つけた
		char szClassName[256];
		GetClassNameA(hWnd,szClassName,sizeof(szClassName)/sizeof(szClassName[0]));

		std::string name(szClassName);

		if (name == "Polygon Movie Maker"){
			//::MessageBoxA(NULL, name.c_str(), "window", MB_OK);
			g_hWnd = hWnd;
			EnumChildWindows(hWnd, enumChildWindowsProc, 0);
			return FALSE;	//break
		}
	}
	return TRUE;	//continue
}

static void setMyMenu()
{
	if (g_hMenu) return;
	if (g_hWnd)
	{
		HMENU hmenu = GetMenu(g_hWnd);//CreateMenu();
		HMENU hsubs = CreatePopupMenu();
		int count = GetMenuItemCount(hmenu);
		
		MENUITEMINFO minfo;
		minfo.cbSize = sizeof(MENUITEMINFO);
		minfo.fMask = MIIM_ID | MIIM_TYPE | MIIM_SUBMENU;
		minfo.fType = MFT_STRING;
		minfo.dwTypeData = TEXT("MMDBridge");
		minfo.hSubMenu = hsubs;

		InsertMenuItem(hmenu, count + 1, TRUE, &minfo);
		minfo.fMask = MIIM_ID | MIIM_TYPE;
		minfo.dwTypeData = TEXT("プラグイン設定");
		minfo.wID = 1020;
		InsertMenuItem(hsubs, 1, TRUE, &minfo);


		SetMenu(g_hWnd, hmenu);
		g_hMenu = hmenu;
	}
}

WNDPROC originalWndProc  =NULL;
// このコード モジュールに含まれる関数の宣言を転送します:
BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInstance= NULL;
HWND pluginDialog = NULL;

static LRESULT CALLBACK overrideWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch( msg )
	{
		case WM_COMMAND:
		{
			switch(LOWORD(wp))
			{
			case 1020: // プラグイン設定
				if(hInstance)
				{
					pluginDialog = hWnd;
					::DialogBoxA(hInstance, "IDD_DIALOG1", NULL, DialogProc);
				}
				break;
			}
		}
		break;
		case WM_DESTROY:
			::DestroyWindow(pluginDialog);

		break;
	}

	// サブクラスで処理しなかったメッセージは、本来のウィンドウプロシージャに処理してもらう
	return CallWindowProc( originalWndProc, hWnd, msg, wp, lp );
}

static BOOL CALLBACK DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND hCombo1 = GetDlgItem(hWnd, IDC_COMBO1);
	HWND hCombo2 = GetDlgItem(hWnd, IDC_COMBO2);
	HWND hEdit1 = GetDlgItem(hWnd, IDC_EDIT1);
	HWND hEdit2 = GetDlgItem(hWnd, IDC_EDIT2);
	//HWND hEdit3 = GetDlgItem(hWnd, IDC_EDIT3);
	//HWND hEdit4 = GetDlgItem(hWnd, IDC_EDIT4);
	HWND hEdit5 = GetDlgItem(hWnd, IDC_EDIT5);
	//if (!hCombo1) { return FALSE; }
	//if (!hCombo2) { return FALSE; }
	//if (!hEdit1) { return FALSE; }
	//if (!hEdit2) { return FALSE; }
	//if (!hEdit5) { return FALSE; }
    switch (msg) {
        case WM_INITDIALOG:
			{
				//::MessageBoxA(NULL, "hoge出力設定", "menu", MB_OK);
				// コンボボックスにデータを詰めていく
				for (size_t i = 0 ; i < pythonNames.size() ; i++)
				{
					SendMessage(hCombo1 , CB_ADDSTRING , 0 , (LPARAM)pythonNames[i].c_str());
				}
				//SendMessage(hCombo2 , CB_ADDSTRING , 0 , (LPARAM)_T("画面操作時に実行"));
				SendMessage(hCombo2 , CB_ADDSTRING , 0 , (LPARAM)_T("実行する"));
				SendMessage(hCombo2 , CB_ADDSTRING , 0 , (LPARAM)_T("実行しない"));
				// ウインドウ生成時にはじめに表示するデータを指定
				UINT index1 = SendMessage(hCombo1, CB_FINDSTRINGEXACT, -1, (LPARAM)pythonName.c_str());
				SendMessage(hCombo1, CB_SETCURSEL, index1, 0);
				SendMessage(hCombo2, CB_SETCURSEL, scriptCallSetting - 1, 0);

				::SetWindowTextA(hEdit1, to_string(startFrame).c_str());
				::SetWindowTextA(hEdit2, to_string(endFrame).c_str());
				//::SetWindowTextA(hEdit3, to_string().c_str());
				//::SetWindowTextA(hEdit4, to_string().c_str());
				::SetWindowTextA(hEdit5, to_string(exportFPS).c_str());
			}
			return TRUE;
        case WM_CLOSE:
            EndDialog(hWnd, IDCANCEL);
            break;
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK: // ボタンが押されたとき
					{
						UINT num1 = (UINT)SendMessage(hCombo1, CB_GETCURSEL, 0, 0);
						if (num1 < pythonNames.size())
						{
							if (pythonName != pythonNames[num1]) {
								pythonName = pythonNames[num1];
								pythonScript = pythonScripts[num1];
								relaod_python_script();
							}
						}
						UINT num2 = (UINT)SendMessage(hCombo2, CB_GETCURSEL, 0, 0);
						if (num2 <= 2)
						{
							scriptCallSetting = num2 + 1;
						}

						char text1[32];
						char text2[32];
						//char text3[32];
						//char text4[32];
						char text5[32];
						::GetWindowTextA(hEdit1, text1, sizeof(text1)/sizeof(text1[0]));
						::GetWindowTextA(hEdit2, text2, sizeof(text2)/sizeof(text2[0]));
						//::GetWindowTextA(hEdit3, text3, sizeof(text3)/sizeof(text3[0]));
						//::GetWindowTextA(hEdit4, text4, sizeof(text4)/sizeof(text4[0]));
						::GetWindowTextA(hEdit5, text5, sizeof(text5)/sizeof(text5[0]));
						startFrame = atoi(text1);
						endFrame = atoi(text2);
						//startFrame = atoi(text3);
						//endFrame = atoi(text4);
						exportFPS = atoi(text5);
						
						if (startFrame < 2)
						{
							messagebox("info", "スタートフレームは2以上にしてください");
							startFrame = 2;
							::SetWindowTextA(hEdit1, to_string(2).c_str());
						}
						if (startFrame >= endFrame)
						{
							endFrame = startFrame + 1;
							::SetWindowTextA(hEdit2, to_string(endFrame).c_str());
						}

						EndDialog(hWnd, IDOK);

					}
					currentRenderedMaterial = NULL;
                    break;
				case IDCANCEL:
					EndDialog(hWnd, IDCANCEL);
					currentRenderedMaterial = NULL;
					break;
				case IDC_BUTTON1: // 再検索
					reload_python_file_paths();
					SendMessage(hCombo1, CB_SETCURSEL, SendMessage(hCombo1, CB_FINDSTRINGEXACT, -1, (LPARAM)pythonName.c_str()), 0);
					break;
				case IDC_BUTTON2: // 詳細設定

					// ダイアログ表示中は問答無用でスクリプト実行を専用のモードにする
					int preSetting = scriptCallSetting;
					scriptCallSetting = 3;

					// script再読み込み
					UINT num1 = (UINT)SendMessage(hCombo1, CB_GETCURSEL, 0, 0);
					if (num1 < pythonNames.size())
					{
						if (pythonName != pythonNames[num1]) {
							pythonName = pythonNames[num1];
							pythonScript = pythonScripts[num1];
							relaod_python_script();
						}
					}
					if (!mmdbridge_python_script.empty())
					{
						if (mmdbridge_python_script.find("#thread") != std::string::npos)
						{
							EndDialog(hWnd, IDOK);
							SendMessage(g_hFrameArrowRight, BM_CLICK, 0, 0);
							break;
						}
					}

					scriptCallSetting = preSetting;
					::MessageBoxA(NULL, "このスクリプトには詳細設定が無いようです", "info", MB_OK);
					EndDialog(hWnd, IDOK);
					break;

            }
            break;
        return TRUE;
    }
    return FALSE;
}

//ウィンドウの乗っ取り
static bool overrideGLWindow()
{
	EnumWindows(enumWindowsProc,0);

	setMyMenu();

	// サブクラス化
	if(g_hWnd && !originalWndProc){
		originalWndProc=(WNDPROC)GetWindowLong(g_hWnd,GWL_WNDPROC);
		SetWindowLong(g_hWnd,GWL_WNDPROC,(LONG)overrideWndProc);
		return true;
	}
	return false;
}

//////////////////// Python Thread
static unsigned int WINAPI runScript(void* data)
{
	const char* scriptStr = (const char*)data;

	relaod_python_script();

	if (mmdbridge_python_script.empty()) { return 0; }

	// Python初期化
	PyImport_AppendInittab("mmdbridge", initfuncs);

	if (!Py_IsInitialized())
	{
		Py_Initialize();

		// 入力引数の設定
		{
			to_wstring(wBasePath, basePath);
			int argc = 1;
			const wchar_t *cpath = wBasePath.c_str();
			wchar_t *path[] = {
				const_cast<wchar_t*>(cpath)
			};
			PySys_SetArgv(argc, path);
		}
	}

	//// Python Thread初期化
	if (!PyEval_ThreadsInitialized())
	{
		PyEval_InitThreads();
	}

	PyGILState_STATE gstate = PyGILState_Ensure();

	// 実行
	int res = PyRun_SimpleStringFlags(mmdbridge_python_script.c_str(), NULL);

	if (res < 0) {
		if (! python_error_string.find("SystemExit"))
		{
			::MessageBoxA(NULL, python_error_string.c_str(), "python error", MB_OK);
		}
		python_error_string = "";
	}

	Py_Finalize();
	PyGILState_Release(gstate);
	
	Py_InspectFlag = 0;

	_endthreadex(0);

	if (scriptCallSetting > 1)
	{
		scriptCallSetting = 0;
	}

	return 0;
}

static void runScriptMain()
{
	relaod_python_script();

	if (mmdbridge_python_script.empty()) { return; }

	if (Py_IsInitialized())
	{
		//
		PyEval_InitThreads();
		Py_InspectFlag = 0;
		
		if (scriptCallSetting > 1)
		{
			scriptCallSetting = 0;
		}
	}
	else
	{
		Py_Initialize();
		PyImport_AppendInittab("mmdbridge", initfuncs);		
		PyImport_ImportModule("mmdbridge");
		
		// 入力引数の設定
		{
			to_wstring(wBasePath, basePath);
			int argc = 1;
			const wchar_t *cpath = wBasePath.c_str();
			wchar_t *path[] = {
				const_cast<wchar_t*>(cpath)
			};
			PySys_SetArgv(argc, path);
		}
	}

	relaod_python_script();
	
	PyGILState_STATE gstate = PyGILState_Ensure();

	if (!mmdbridge_python_script.empty()) 
	{
		int res = PyRun_SimpleString(mmdbridge_python_script.c_str());
		if (res < 0) {
			::MessageBoxA(NULL, python_error_string.c_str(), "python error", MB_OK);
			python_error_string = "";
		}
	}

	PyGILState_Release(gstate);
}


static bool IsValidCallSetting() { 
	return (scriptCallSetting == 0) || (scriptCallSetting == 1);
}

static bool IsValidFrame() {
	float time = ExpGetFrameTime();
	return ((scriptCallSetting == 0) && (preFrame != currentFrame)) ||
			((scriptCallSetting == 1) && (time > 0));
}

static bool IsValidTechniq() {
	const int technic = ExpGetCurrentTechnic();
	return (technic == 0 || technic == 1 || technic == 2);
}

static HRESULT WINAPI present(
	IDirect3DDevice9 *device, 
	const RECT * pSourceRect, 
	const RECT * pDestRect, 
	HWND hDestWindowOverride, 
	const RGNDATA * pDirtyRegion)
{

	float time = ExpGetFrameTime();

	if (pDestRect)
	{
		frameWidth = pDestRect->right - pDestRect->left;
		frameHeight = pDestRect->bottom - pDestRect->top;
	}
	overrideGLWindow();

	if (time > 0 && primitiveCounter > 0 && scriptCallSetting != 2) {

		preFrameTime = time;
		
		if (scriptCallSetting == 0)
		{
			if (preFrame != currentFrame)
			{
				runScriptMain();
				preFrame = currentFrame;
				//::MessageBox(NULL, (to_wstring(preFrame) + _T("preFrame")).c_str(), _T("HOGE"), MB_OK);
				//::MessageBox(NULL, (to_wstring(currentFrame) + _T("currentFrame")).c_str(), _T("HOGE"), MB_OK);
			}
		}
		else if (scriptCallSetting == 1)
		{
			float time = ExpGetFrameTime();
			int frame = static_cast<int>(time * exportFPS);
			if (currentFrame == startFrame || frame == startFrame)
			{
				isExportedFrame = true;
			}
			if (isExportedFrame)
			{
				if (currentFrame >= startFrame && currentFrame <= endFrame)
				{
					if (exportedFrames.find(currentFrame) == exportedFrames.end())
					{
						runScriptMain();
						exportedFrames[currentFrame] = 1;
						preFrame = currentFrame;
					}
					if (currentFrame == endFrame)
					{
						exportedFrames.clear();
						isExportedFrame = false;
					}
				}
				else if (frame >= startFrame && frame <= endFrame)
				{
					if (exportedFrames.find(frame) == exportedFrames.end())
					{
						runScriptMain();
						exportedFrames[frame] = 1;
						preFrame = frame;
					}
					if (frame == endFrame)
					{
						exportedFrames.clear();
						isExportedFrame = false;
					}
				}
			}
		}
		else if (scriptCallSetting == 3)
		{
			if (preFrame != currentFrame)
			{
				//::MessageBox(NULL, (to_wstring(time) + _T("秒です")).c_str(), _T("HOGE"), MB_OK);
				HWND hCombo2 = GetDlgItem(pluginDialog, IDC_COMBO2);
				{
					if (pythonThreadHandle)
					{
						CloseHandle(pythonThreadHandle);
						pythonThreadHandle = NULL;
					}
				}
				if (!pythonThreadHandle)
				{
					//renderedBuffersCopy.clear();
					//std::copy(renderedBuffers.begin(), renderedBuffers.end(), renderedBuffersCopy.begin());
					finishBuffersCopy.clear();
					finishBuffersCopy.resize(finishBuffers.size());
					std::copy(finishBuffers.begin(), finishBuffers.end(), finishBuffersCopy.begin());
					pythonThreadHandle = (HANDLE)_beginthreadex(NULL, 0, runScript, (void*)mmdbridge_python_script.c_str(), 0, NULL);
				}
				 if (pythonThreadHandle)
				 {
					 // メッセージボックスを出して強制的にスレッド変更
					 ::MessageBox(NULL, _T("設定画面起動"), _T("HOGE"), MB_OK);
				 }
				//// スクリプト呼び出し設定
				//UINT num2 = (UINT)SendMessage(hCombo2, CB_GETCURSEL, 0, 0);
				//if (num2 < 3)
				//{
				//	scriptCallSetting = 2;
				//};
				preFrame = currentFrame;
			}
		}

		finishBuffers.clear();

		presentCount++;
	}
	HRESULT res = (*original_present)(device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	
	const bool validCallSetting = IsValidCallSetting();
	const bool validFrame = IsValidFrame();
	const bool validTechniq =  IsValidTechniq();

	if (validCallSetting && validFrame && validTechniq)
	{
		primitiveCounter = 0;
	}
	
	return res;
}

HRESULT WINAPI reset(IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	HRESULT res = (*original_reset)(device, pPresentationParameters);

	::MessageBox(NULL, _T("MMDBridgeは、現状3D vision 未対応です"), _T("HOGE"), MB_OK);

	return res;
}

HRESULT WINAPI setFVF(IDirect3DDevice9 *device, DWORD fvf)
{
	HRESULT res = (*original_set_fvf)(device, fvf);

	float time = ExpGetFrameTime();
	if (scriptCallSetting != 2) {
		renderData.fvf = fvf;

		DWORD pos = (fvf & D3DFVF_POSITION_MASK);
		renderData.pos = (pos > 0);
		renderData.pos_xyz	= ((pos & D3DFVF_XYZ) > 0);
		renderData.pos_rhw	= ((pos & D3DFVF_XYZRHW) > 0);
		renderData.pos_xyzb[0] = ((fvf & D3DFVF_XYZB1) == D3DFVF_XYZB1);
		renderData.pos_xyzb[1] = ((fvf & D3DFVF_XYZB2) == D3DFVF_XYZB2);
		renderData.pos_xyzb[2] = ((fvf & D3DFVF_XYZB3) == D3DFVF_XYZB3);
		renderData.pos_xyzb[3] = ((fvf & D3DFVF_XYZB4) == D3DFVF_XYZB4);
		renderData.pos_xyzb[4] = ((fvf & D3DFVF_XYZB5) == D3DFVF_XYZB5);
		renderData.pos_last_beta_ubyte4 = ((fvf & D3DFVF_LASTBETA_UBYTE4) > 0);
		renderData.normal	= ((fvf & D3DFVF_NORMAL) > 0);
		renderData.psize	= ((fvf & D3DFVF_PSIZE) > 0);
		renderData.diffuse	= ((fvf & D3DFVF_DIFFUSE) > 0);
		renderData.specular	= ((fvf & D3DFVF_SPECULAR) > 0);
		renderData.texcount	= (fvf & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT;
	}

	return res;
}

static void setFVF(DWORD fvf)
{
	renderData.fvf = fvf;

	DWORD pos = (fvf & D3DFVF_POSITION_MASK);
	renderData.pos = (pos > 0);
	renderData.pos_xyz	= ((pos & D3DFVF_XYZ) > 0);
	renderData.pos_rhw	= ((pos & D3DFVF_XYZRHW) > 0);
	renderData.pos_xyzb[0] = ((fvf & D3DFVF_XYZB1) == D3DFVF_XYZB1);
	renderData.pos_xyzb[1] = ((fvf & D3DFVF_XYZB2) == D3DFVF_XYZB2);
	renderData.pos_xyzb[2] = ((fvf & D3DFVF_XYZB3) == D3DFVF_XYZB3);
	renderData.pos_xyzb[3] = ((fvf & D3DFVF_XYZB4) == D3DFVF_XYZB4);
	renderData.pos_xyzb[4] = ((fvf & D3DFVF_XYZB5) == D3DFVF_XYZB5);
	renderData.pos_last_beta_ubyte4 = ((fvf & D3DFVF_LASTBETA_UBYTE4) > 0);
	renderData.normal	= ((fvf & D3DFVF_NORMAL) > 0);
	renderData.psize	= ((fvf & D3DFVF_PSIZE) > 0);
	renderData.diffuse	= ((fvf & D3DFVF_DIFFUSE) > 0);
	renderData.specular	= ((fvf & D3DFVF_SPECULAR) > 0);
	renderData.texcount	= (fvf & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT;

}

HRESULT WINAPI clear(
	IDirect3DDevice9 *device, 
	DWORD count, 
	const D3DRECT *pRects, 
	DWORD flags, 
	D3DCOLOR color, 
	float z, 
	DWORD stencil)
{
	HRESULT res = (*original_clear)(device, count, pRects, flags, color, z, stencil);
	return res;
}

static void getTextureParameter(TextureParameter &param)
{
	TextureSamplers::iterator tit0 = renderData.textureSamplers.find(0);
	TextureSamplers::iterator tit1 = renderData.textureSamplers.find(1);
	TextureSamplers::iterator tit2 = renderData.textureSamplers.find(2);

	param.hasTextureSampler0 = (tit0 != renderData.textureSamplers.end());
	param.hasTextureSampler1 = (tit1 != renderData.textureSamplers.end());
	param.hasTextureSampler2 = (tit2 != renderData.textureSamplers.end());

	if (param.hasTextureSampler1) {
		LPWSTR name = UMGetTextureName(tit1->second);
		param.texture = tit1->second;
		param.textureMemoryName = to_string(param.texture);
		if (name)
		{
			std::wstring wname(name);
			to_string(param.textureName, wname);
		}
		if (UMIsAlphaTexture(param.texture))
		{
			param.hasAlphaTexture = true;
		}
	}
}

// 頂点・法線バッファ・テクスチャをメモリに書き込み
static bool writeBuffersToMemory(IDirect3DDevice9 *device)
{
	const int currentTechnic = ExpGetCurrentTechnic();
	const int currentMaterial = ExpGetCurrentMaterial();
	const int currentObject = ExpGetCurrentObject();

	BYTE *pVertexBuf;
	IDirect3DVertexBuffer9 *pStreamData = renderData.pStreamData;

	if (std::find(finishBuffers.begin(), finishBuffers.end(), pStreamData) == finishBuffers.end())
	{
		VertexBuffers::iterator vit = renderData.vertexBuffers.find(pStreamData);
		if(vit != renderData.vertexBuffers.end())
		{
			pStreamData->lpVtbl->Lock(pStreamData, 0, 0, (void**)&pVertexBuf, D3DLOCK_READONLY);

			// FVF取得
			DWORD fvf;
			device->lpVtbl->GetFVF(device, &fvf);
			if (renderData.fvf != fvf)
			{
				setFVF(fvf);
			}

			RenderedBuffer renderedBuffer;

			::D3DXMatrixIdentity(&renderedBuffer.world);
			::D3DXMatrixIdentity(&renderedBuffer.view);
			::D3DXMatrixIdentity(&renderedBuffer.projection);
			::D3DXMatrixIdentity(&renderedBuffer.world_inv);
			device->lpVtbl->GetTransform(device ,D3DTS_WORLD, &renderedBuffer.world);
			device->lpVtbl->GetTransform(device ,D3DTS_VIEW, &renderedBuffer.view);
			device->lpVtbl->GetTransform(device ,D3DTS_PROJECTION, &renderedBuffer.projection);
			
			::D3DXMatrixInverse(&renderedBuffer.world_inv, NULL, &renderedBuffer.world);

			int bytePos = 0;

			if (renderedBuffer.world.m[0][0] == 0 && renderedBuffer.world.m[1][1]== 0 && renderedBuffer.world.m[2][2]== 0)
			{
				return false;
			}

			renderedBuffer.isAccessory = false;
			D3DXMATRIX accesosoryMat;
			for (int i = 0; i < ExpGetAcsNum(); ++i)
			{
				int order = ExpGetAcsOrder(i);
				if (order == currentObject)
				{
					renderedBuffer.isAccessory = true;
					renderedBuffer.accessory = i;
					accesosoryMat = ExpGetAcsWorldMat(i);
				}
			}

			// 頂点
			if (renderData.pos_xyz)
			{
				int initialVertexSize = renderedBuffer.vertecies.size();
				const int size = (vit->second - bytePos) / renderData.stride;
				renderedBuffer.vertecies.resize(size);
				for (size_t i =  bytePos, n = 0; i < vit->second; i += renderData.stride, ++n)
				{
					D3DXVECTOR3 v;
					memcpy(&v, &pVertexBuf[i], sizeof( D3DXVECTOR3 ));
					if (renderedBuffer.isAccessory)
					{
						D3DXVECTOR4 dst;
						::D3DXVec3Transform(&dst, &v, &accesosoryMat);
						v.x = dst.x;
						v.y = dst.y;
						v.z = dst.z;
					}

					renderedBuffer.vertecies[n] = v;
				}
				bytePos += (sizeof(DWORD) * 3);
			}

			// ウェイト（略）

			// 法線
			if (renderData.normal)
			{
				for (size_t i = bytePos; i < vit->second; i += renderData.stride)
				{
					D3DXVECTOR3 n;
					memcpy(&n, &pVertexBuf[i], sizeof( D3DXVECTOR3 ));
					renderedBuffer.normals.push_back(n);
				}
				bytePos += (sizeof(DWORD) * 3);
			}

			// 頂点カラー
			if (renderData.diffuse)
			{
				for (size_t i = 0; i < vit->second; i += renderData.stride)
				{
					DWORD diffuse;
					memcpy(&diffuse, &pVertexBuf[i], sizeof( DWORD ));
					renderedBuffer.diffuses.push_back(diffuse);

				}
				bytePos += (sizeof(DWORD));
			}

			// ＵＶ
			if (renderData.texcount > 0) 
			{
				for (int n = 0; n < renderData.texcount; ++n) 
				{
					for (size_t i = bytePos; i < vit->second; i += renderData.stride) 
					{
						um_vector2 uv;
						memcpy(&uv, &pVertexBuf[i], sizeof( um_vector2 ));
						renderedBuffer.uvs.push_back(uv);
					}
					bytePos += (sizeof(DWORD) * 2);
				}
			}

			
			pStreamData->lpVtbl->Unlock(pStreamData);

			// メモリに保存
			finishBuffers.push_back(pStreamData);
			renderedBuffers[pStreamData] = renderedBuffer;
		}
	}
	return true;
}

static bool writeMaterialsToMemory(TextureParameter & textureParameter)
{
	const int currentTechnic = ExpGetCurrentTechnic();
	const int currentMaterial = ExpGetCurrentMaterial();
	const int currentObject = ExpGetCurrentObject();

	IDirect3DVertexBuffer9 *pStreamData = renderData.pStreamData;
	if (renderedBuffers.find(pStreamData) == renderedBuffers.end())
	{
		return false;
	}

	bool notFoundObjectMaterial = (renderedMaterials.find(currentObject) == renderedMaterials.end());
	if (notFoundObjectMaterial || renderedMaterials[currentObject].find(currentMaterial) == renderedMaterials[currentObject].end())
	{
		// D3DMATERIAL9 取得
		D3DMATERIAL9 material = ExpGetPmdMaterial(currentObject, currentMaterial);
		//p_device->lpVtbl->GetMaterial(p_device, &material);
		
		RenderedMaterial *mat = new RenderedMaterial();
		mat->diffuse.x = material.Diffuse.r;
		mat->diffuse.y = material.Diffuse.g;
		mat->diffuse.z = material.Diffuse.b;
		mat->diffuse.w = material.Diffuse.a;
		mat->specular.x = material.Specular.r;
		mat->specular.y = material.Specular.g;
		mat->specular.z = material.Specular.b;
		mat->ambient.x = material.Ambient.r;
		mat->ambient.y = material.Ambient.g;
		mat->ambient.z = material.Ambient.b;
		mat->emissive.x = material.Emissive.r;
		mat->emissive.y = material.Emissive.g;
		mat->emissive.z = material.Emissive.b;
		mat->power = material.Power;
		
		// シェーダー時
		if (currentTechnic == 2) {
			LPD3DXEFFECT* effect =  UMGetEffect();

			if (effect) {
				D3DXHANDLE current = (*effect)->lpVtbl->GetCurrentTechnique(*effect);
				D3DXHANDLE texHandle1 = (*effect)->lpVtbl->GetTechniqueByName(*effect, "DiffuseBSSphiaTexTec");
				D3DXHANDLE texHandle2 = (*effect)->lpVtbl->GetTechniqueByName(*effect, "DiffuseBSTextureTec");
				D3DXHANDLE texHandle3 = (*effect)->lpVtbl->GetTechniqueByName(*effect, "BShadowSphiaTextureTec");
				D3DXHANDLE texHandle4 = (*effect)->lpVtbl->GetTechniqueByName(*effect, "BShadowTextureTec");
				if (current == texHandle1) {
					//::MessageBoxA(NULL, "1", "transp", MB_OK);
					textureParameter.hasTextureSampler2 = true;
				}
				if (current == texHandle2) {
					//::MessageBoxA(NULL, "2", "transp", MB_OK);
					textureParameter.hasTextureSampler2 = true;
				}
				if (current == texHandle3) {
					//::MessageBoxA(NULL, "3", "transp", MB_OK);
					textureParameter.hasTextureSampler2 = true;
				}
				if (current == texHandle4) {
					//::MessageBoxA(NULL, "4", "transp", MB_OK);
					textureParameter.hasTextureSampler2 = true;
				}

				D3DXHANDLE hEdge = (*effect)->lpVtbl->GetParameterByName(*effect, NULL, "EgColor");
				D3DXHANDLE hDiffuse = (*effect)->lpVtbl->GetParameterByName(*effect, NULL, "DifColor");
				D3DXHANDLE hToon = (*effect)->lpVtbl->GetParameterByName(*effect, NULL, "ToonColor");
				D3DXHANDLE hSpecular = (*effect)->lpVtbl->GetParameterByName(*effect, NULL, "SpcColor");
				D3DXHANDLE hTransp = (*effect)->lpVtbl->GetParameterByName(*effect, NULL, "transp");
				
				float edge[4];
				float diffuse[4];
				float specular[4];
				float toon[4];
				BOOL transp;
				(*effect)->lpVtbl->GetFloatArray(*effect, hEdge, edge, 4);
				(*effect)->lpVtbl->GetFloatArray(*effect, hToon, toon, 4);
				(*effect)->lpVtbl->GetFloatArray(*effect, hDiffuse, diffuse, 4);
				(*effect)->lpVtbl->GetFloatArray(*effect, hSpecular, specular, 4);
				(*effect)->lpVtbl->GetBool(*effect, hTransp, &transp);
				mat->diffuse.x = edge[0] * toon[0] + specular[0];
				if (mat->diffuse.x > 1) { mat->diffuse.x = 1.0f; }
				mat->diffuse.y = edge[1] * toon[1] + specular[1];
				if (mat->diffuse.y > 1) { mat->diffuse.y = 1.0f; }
				mat->diffuse.z = edge[2] * toon[2] + specular[2];
				if (mat->diffuse.z > 1) { mat->diffuse.z = 1.0f; }
				mat->diffuse.w = edge[3];

				if (specular[0] != 0 || specular[1] != 0 || specular[2] != 0)
				{
					mat->specular.x = specular[0];
					mat->specular.y = specular[1];
					mat->specular.z = specular[2];
				}
			}
		}

		if (renderData.texcount > 0)
		{
			DWORD colorRop0;
			DWORD colorRop1;

			p_device->lpVtbl->GetTextureStageState(p_device, 0, D3DTSS_COLOROP, &colorRop0);
			p_device->lpVtbl->GetTextureStageState(p_device, 1, D3DTSS_COLOROP, &colorRop1);

			if (textureParameter.hasTextureSampler2) {

				mat->tex = textureParameter.texture;
				mat->texture = textureParameter.textureName;
				mat->memoryTexture = textureParameter.textureMemoryName;

				if (!textureParameter.hasAlphaTexture)
				{
					mat->diffuse.w = 1.0f;
				}
			} else	if (textureParameter.hasTextureSampler0 || textureParameter.hasTextureSampler1) {
				if (colorRop0 != D3DTOP_DISABLE && colorRop1 != D3DTOP_DISABLE)
				{
					mat->tex = textureParameter.texture;
					mat->texture = textureParameter.textureName;
					mat->memoryTexture = textureParameter.textureMemoryName;
					if (!textureParameter.hasAlphaTexture)
					{
						mat->diffuse.w = 1.0f;
					}
				}
			}
		}

		RenderedBuffer &renderedBuffer = renderedBuffers[pStreamData];
		if (renderedBuffer.isAccessory)
		{
			D3DMATERIAL9 accessoryMat = ExpGetAcsMaterial(renderedBuffer.accessory, currentMaterial);
			mat->diffuse.x = accessoryMat.Diffuse.r;
			mat->diffuse.y = accessoryMat.Diffuse.g;
			mat->diffuse.z = accessoryMat.Diffuse.b;
			mat->specular.x = accessoryMat.Specular.r;
			mat->specular.y = accessoryMat.Specular.g;
			mat->specular.z = accessoryMat.Specular.b;
			mat->ambient.x = accessoryMat.Ambient.r;
			mat->ambient.y = accessoryMat.Ambient.g;
			mat->ambient.z = accessoryMat.Ambient.b;
			mat->diffuse.w = ::ExpGetAcsTr(renderedBuffer.accessory);
		}

		renderedBuffer.materials.push_back(mat);
		renderedBuffer.material_map[currentMaterial] = mat;
		renderedMaterials[currentObject][currentMaterial] = mat;
	}
	else
	{
		std::map<int, RenderedMaterial*>& materialMap = renderedMaterials[currentObject];
		renderedBuffers[pStreamData].materials.push_back(materialMap[currentMaterial]);
		renderedBuffers[pStreamData].material_map[currentMaterial] = materialMap[currentMaterial];
		renderedMaterials[currentObject][currentMaterial] = materialMap[currentMaterial];
	}

	if (renderedBuffers[pStreamData].materials.size() > 0) 
	{
		return true;
	}
	else
	{
		return false;
	}
}

static void writeMatrixToMemory(IDirect3DDevice9 *device, RenderedBuffer &dst)
{
	::D3DXMatrixIdentity(&dst.world);
	::D3DXMatrixIdentity(&dst.view);
	::D3DXMatrixIdentity(&dst.projection);
	device->lpVtbl->GetTransform(device ,D3DTS_WORLD, &dst.world);
	device->lpVtbl->GetTransform(device ,D3DTS_VIEW, &dst.view);
	device->lpVtbl->GetTransform(device ,D3DTS_PROJECTION, &dst.projection);
}

static void writeLightToMemory(IDirect3DDevice9 *device, RenderedBuffer &renderedBuffer)
{
	BOOL isLight;
	int lightNumber = 0;			
	 p_device->lpVtbl->GetLightEnable(p_device, lightNumber, &isLight);
	 if (isLight)
	 {
		D3DLIGHT9  light;
		p_device->lpVtbl->GetLight(p_device, lightNumber, &light);
		um_vector3& umlight = renderedBuffer.light;
		D3DXVECTOR3 v(light.Direction.x, light.Direction.y, light.Direction.z);
		D3DXVECTOR4 dst;
		//D3DXVec3Transform(&dst, &v, &renderedBuffer.world);
		umlight.x = v.x;
		umlight.y = v.y;
		umlight.z = v.z;
	}
}

static HRESULT WINAPI drawIndexedPrimitive(
	IDirect3DDevice9 *device, 
	D3DPRIMITIVETYPE type, 
	INT baseVertexIndex, 
	UINT minIndex,
	UINT numVertices, 
	UINT startIndex, 
	UINT primitiveCount)
{
	float time = ExpGetFrameTime();

	const int currentMaterial = ExpGetCurrentMaterial();
	const int currentObject = ExpGetCurrentObject();

	const bool validCallSetting = IsValidCallSetting();
	const bool validFrame = IsValidFrame();
	const bool validTechniq = IsValidTechniq();

	if (validCallSetting && validFrame && validTechniq) 
	{
		// レンダリング開始
		if (renderData.pIndexData && renderData.pStreamData && renderData.pos_xyz && primitiveCounter > 0)
		{
			// テクスチャ情報取得
			TextureParameter textureParameter;
			getTextureParameter(textureParameter);

			// テクスチャをメモリに保存
			if (textureParameter.texture)
			{
				if (!textureParameter.textureName.empty())
				{
					writeTextureToMemory(textureParameter.textureName, textureParameter.texture, true);
				}
				else
				{
					writeTextureToMemory(textureParameter.textureName, textureParameter.texture, false);
				}
			}

			// 頂点バッファ・法線バッファ・テクスチャバッファをメモリに書き込み
			if (!writeBuffersToMemory(device))
			{
				return (*original_draw_indexed_primitive)(device, type, baseVertexIndex, minIndex, numVertices, startIndex, primitiveCount);
			}
			
			// マテリアルをメモリに書き込み
			if (!writeMaterialsToMemory(textureParameter))
			{
				return  (*original_draw_indexed_primitive)(device, type, baseVertexIndex, minIndex, numVertices, startIndex, primitiveCount);
			}

			// インデックスバッファをメモリに書き込み
			// 法線がない場合法線を計算
			IDirect3DVertexBuffer9 *pStreamData = renderData.pStreamData;
			WORD *pIndexBuf;
			IDirect3DIndexBuffer9 *pIndexData = renderData.pIndexData;
			pIndexData->lpVtbl->Lock(pIndexData, 0, 0, (void**)&pIndexBuf, D3DLOCK_READONLY);
			{
				RenderedBuffer &renderedBuffer = renderedBuffers[pStreamData];
				RenderedSurface &renderedSurface = renderedBuffer.material_map[currentMaterial]->surface;
				renderedSurface.faces.clear();

				// 変換行列をメモリに書き込み
				writeMatrixToMemory(device, renderedBuffer);

				// ライトをメモリに書き込み
				writeLightToMemory(device, renderedBuffer);

				// インデックスバッファをメモリに書き込み
				// 法線を修正
				for (size_t i = 0, size = primitiveCount * 3; i < size; i += 3)
				{
					um_vector3 face;
					face.ix = (int)(pIndexBuf[startIndex + i + 0]) + 1;
					face.iy = (int)(pIndexBuf[startIndex + i + 1]) + 1;
					face.iz = (int)(pIndexBuf[startIndex + i + 2]) + 1;
					renderedSurface.faces.push_back(face);
					if (!renderData.normal)
					{
						int vsize = renderedBuffer.vertecies.size();
						if (renderedBuffer.normals.size() != vsize)
						{
							renderedBuffer.normals.resize(vsize);
						}
						if (face.ix >= vsize || face.iy >= vsize || face.iz >= vsize) continue;
						if (face.ix < 0 || face.iy < 0 || face.iz < 0) continue;

						D3DXVECTOR3 n;
						D3DXVECTOR3 v0 = renderedBuffer.vertecies[face.ix];
						D3DXVECTOR3 v1 = renderedBuffer.vertecies[face.iy];
						D3DXVECTOR3 v2 = renderedBuffer.vertecies[face.iz];
						D3DXVECTOR3 v10 = v1-v0;
						D3DXVECTOR3 v20 = v2-v0;
						::D3DXVec3Cross(&n, &v10, &v20);
						renderedBuffer.normals[face.ix] += n;
						renderedBuffer.normals[face.iy] += n;
						renderedBuffer.normals[face.iz] += n;
					}
					if (!renderData.normal)
					{
						for (size_t i = 0, size = renderedBuffer.normals.size(); i < size; ++i)
						{
							D3DXVec3Normalize(
								&renderedBuffer.normals[i],
								&renderedBuffer.normals[i]);
						}
					}
				}
			}
			pIndexData->lpVtbl->Unlock(pIndexData);
		}
		primitiveCounter++;
	}

	
	HRESULT res;

	if (currentRenderedMaterial)
	{
		std::map<int, RenderedMaterial*>& materialMap = renderedMaterials[currentObject];
		if (materialMap[currentMaterial] == currentRenderedMaterial)
		{
			res = (*original_draw_indexed_primitive)(device, type, baseVertexIndex, minIndex, numVertices, startIndex, primitiveCount);
		}
		else
		{
			res = FALSE;
		}
	}
	else
	{
		res = (*original_draw_indexed_primitive)(device, type, baseVertexIndex, minIndex, numVertices, startIndex, primitiveCount);
	}

	UMSync();
	return res;
}

static HRESULT WINAPI createTexture(
	IDirect3DDevice9* device,
	UINT width,
	UINT height,
	UINT levels,
	DWORD usage,
	D3DFORMAT format,
	D3DPOOL pool,
	IDirect3DTexture9** ppTexture,
	HANDLE* pSharedHandle)
{
	HRESULT res = (*original_create_texture)(device, width, height, levels, usage, format, pool, ppTexture, pSharedHandle);

	TextureInfo info;
	info.wh.ix = width;
	info.wh.iy = height;
	info.format = format;

	renderData.textureBuffers[*ppTexture] = info;

	
	return res;

}

static HRESULT WINAPI createVertexBuffer(
	IDirect3DDevice9* device,
	UINT length,
	DWORD usage,
	DWORD fvf,
	D3DPOOL pool,
	IDirect3DVertexBuffer9** ppVertexBuffer,
	HANDLE* pHandle)
{
	HRESULT res = (*original_create_vertex_buffer)(device, length, usage, fvf, pool, ppVertexBuffer, pHandle);
	
	renderData.vertexBuffers[*ppVertexBuffer] = length;

	return res;
}

static HRESULT WINAPI setTexture(
	IDirect3DDevice9* device,
	DWORD sampler,	
	IDirect3DBaseTexture9 * pTexture)
{
	if (presentCount == 0) {
		IDirect3DTexture9* texture = reinterpret_cast<IDirect3DTexture9*>(pTexture);
		renderData.textureSamplers[sampler] = texture;
	}

	HRESULT res = (*original_set_texture)(device, sampler, pTexture);

	return res;
}

static HRESULT WINAPI setStreamSource(
	IDirect3DDevice9 *device, 
	UINT streamNumber,
	IDirect3DVertexBuffer9 *pStreamData,
	UINT offsetInBytes,
	UINT stride)
{
	HRESULT res = (*original_set_stream_source)(device, streamNumber, pStreamData, offsetInBytes, stride);

	float time = ExpGetFrameTime();

	int currentTechnic = ExpGetCurrentTechnic();

	const bool validCallSetting = IsValidCallSetting();
	const bool validFrame = IsValidFrame();
	const bool validTechniq = IsValidTechniq() || currentTechnic == 5;

	if (validCallSetting && validFrame && validTechniq) 
	{
		if (pStreamData) {
			renderData.streamNumber = streamNumber;
			renderData.pStreamData = pStreamData;
			renderData.offsetInBytes = offsetInBytes;
			renderData.stride = stride;
		}
	}

	return res;
}

// IDirect3DDevice9::SetIndices
static HRESULT WINAPI setIndices(IDirect3DDevice9 *device, IDirect3DIndexBuffer9 * pIndexData)
{
	HRESULT res = (*original_set_indices)(device, pIndexData);

	float time = ExpGetFrameTime();
		
	int currentTechnic = ExpGetCurrentTechnic();

	const bool validCallSetting = IsValidCallSetting();
	const bool validFrame = IsValidFrame();
	const bool validTechniq =  IsValidTechniq() || currentTechnic == 5;
	if (validCallSetting && validFrame && validTechniq) 
	{
		renderData.pIndexData = pIndexData;
	}
	
	return res;
}

// IDirect3DDevice9::BeginStateBlock
// この関数で、lpVtblが修正されるので、lpVtbl書き換えなおす
static HRESULT WINAPI beginStateBlock(IDirect3DDevice9 *device)
{
	originalDevice();
	HRESULT res = (*original_begin_state_block)(device);
	
	p_device = device;
	hookDevice();

	return res;
}

// IDirect3DDevice9::EndStateBlock
// この関数で、lpVtblが修正されるので、lpVtbl書き換えなおす
static HRESULT WINAPI endStateBlock(IDirect3DDevice9 *device, IDirect3DStateBlock9 **ppSB)
{
	originalDevice();
	HRESULT res = (*original_end_state_block)(device, ppSB);

	p_device = device;
	hookDevice();

	return res;
}

static void hookDevice()
{
	if (p_device) 
	{
		// 書き込み属性付与
		DWORD old_protect;
		VirtualProtect(reinterpret_cast<void *>(p_device->lpVtbl), sizeof(p_device->lpVtbl), PAGE_EXECUTE_READWRITE, &old_protect);
		
		p_device->lpVtbl->BeginScene = beginScene;
		//p_device->lpVtbl->Clear = clear;
		p_device->lpVtbl->Present = present;
		//p_device->lpVtbl->Reset = reset;
		p_device->lpVtbl->BeginStateBlock = beginStateBlock;
		p_device->lpVtbl->EndStateBlock = endStateBlock;		
		p_device->lpVtbl->SetFVF = setFVF;
		p_device->lpVtbl->DrawIndexedPrimitive = drawIndexedPrimitive;
		p_device->lpVtbl->SetStreamSource = setStreamSource;
		p_device->lpVtbl->SetIndices = setIndices;
		p_device->lpVtbl->CreateVertexBuffer = createVertexBuffer;
		p_device->lpVtbl->SetTexture = setTexture;
		p_device->lpVtbl->CreateTexture = createTexture;
		//p_device->lpVtbl->SetTextureStageState = setTextureStageState;

		// 書き込み属性元に戻す
		VirtualProtect(reinterpret_cast<void *>(p_device->lpVtbl), sizeof(p_device->lpVtbl), old_protect, &old_protect);
	}
}

static void originalDevice()
{
	if (p_device) 
	{
		// 書き込み属性付与
		DWORD old_protect;
		VirtualProtect(reinterpret_cast<void *>(p_device->lpVtbl), sizeof(p_device->lpVtbl), PAGE_EXECUTE_READWRITE, &old_protect);
		
		p_device->lpVtbl->BeginScene = original_begin_scene;
		//p_device->lpVtbl->Clear = clear;
		p_device->lpVtbl->Present = original_present;
		//p_device->lpVtbl->Reset = reset;
		p_device->lpVtbl->BeginStateBlock = original_begin_state_block;
		p_device->lpVtbl->EndStateBlock = original_end_state_block;		
		p_device->lpVtbl->SetFVF = original_set_fvf;
		p_device->lpVtbl->DrawIndexedPrimitive = original_draw_indexed_primitive;
		p_device->lpVtbl->SetStreamSource = original_set_stream_source;
		p_device->lpVtbl->SetIndices = original_set_indices;
		p_device->lpVtbl->CreateVertexBuffer = original_create_vertex_buffer;
		p_device->lpVtbl->SetTexture = original_set_texture;
		p_device->lpVtbl->CreateTexture = original_create_texture;
		//p_device->lpVtbl->SetTextureStageState = setTextureStageState;

		// 書き込み属性元に戻す
		VirtualProtect(reinterpret_cast<void *>(p_device->lpVtbl), sizeof(p_device->lpVtbl), old_protect, &old_protect);
	}
}


static HRESULT WINAPI createDevice(
	IDirect3D9 *direct3d,
	UINT adapter,
	D3DDEVTYPE type,
	HWND window,
	DWORD flag,
	D3DPRESENT_PARAMETERS *param,
	IDirect3DDevice9 **device) 
{
	HRESULT res = (*original_create_device)(direct3d, adapter, type, window, flag, param, device);
	p_device = (*device);
	
	if (p_device) {
		original_begin_scene = p_device->lpVtbl->BeginScene;
		//original_clear = p_device->lpVtbl->Clear;
		original_present = p_device->lpVtbl->Present;
		//original_reset = p_device->lpVtbl->Reset;
		original_begin_state_block = p_device->lpVtbl->BeginStateBlock;
		original_end_state_block = p_device->lpVtbl->EndStateBlock;
		original_set_fvf = p_device->lpVtbl->SetFVF;
		original_draw_indexed_primitive = p_device->lpVtbl->DrawIndexedPrimitive;
		original_set_stream_source = p_device->lpVtbl->SetStreamSource;
		original_set_indices = p_device->lpVtbl->SetIndices;
		original_create_vertex_buffer = p_device->lpVtbl->CreateVertexBuffer;
		original_set_texture = p_device->lpVtbl->SetTexture;
		original_create_texture = p_device->lpVtbl->CreateTexture;
		//original_set_texture_stage_state = p_device->lpVtbl->SetTextureStageState;

		hookDevice();
	}
	return res;
}

extern "C" {
	// 偽Direct3DCreate9
	IDirect3D9 * WINAPI Direct3DCreate9(UINT SDKVersion) {
		IDirect3D9 *direct3d((*original_direct3d_create)(SDKVersion));
		original_create_device = direct3d->lpVtbl->CreateDevice;

		// 書き込み属性付与
		DWORD old_protect;
		VirtualProtect(reinterpret_cast<void *>(direct3d->lpVtbl), sizeof(direct3d->lpVtbl), PAGE_EXECUTE_READWRITE, &old_protect);
		
		direct3d->lpVtbl->CreateDevice = createDevice;

		// 書き込み属性元に戻す
		VirtualProtect(reinterpret_cast<void *>(direct3d->lpVtbl), sizeof(direct3d->lpVtbl), old_protect, &old_protect);

		//InitializeCriticalSection(&criticalSection);
		hMutex = CreateMutex(NULL,FALSE,NULL);	//ミューテックス生成

		if (mme_direct3d_create)
		{
			return ((*mme_direct3d_create)(SDKVersion));
		}
		else
		{
			return direct3d;
		}
	}
} // extern "C"

static void setBasePath()
{
	char app_full_path[1024];	// アプリフルパス
	
	GetModuleFileNameA(NULL, app_full_path, sizeof(app_full_path) / sizeof(char));

	std::string path(app_full_path);
	basePath = path.substr(0, path.rfind("MikuMikuDance.exe"));
}

static BOOL init() {
	setlocale(LC_CTYPE, "");
	setBasePath();
	reload_python_file_paths();
	relaod_python_script();

	TCHAR system_path_buffer[1024]; // システムパス保存用
	GetSystemDirectory(system_path_buffer, MAX_PATH );
	std::wstring d3d9_path(system_path_buffer);
	d3d9_path.append(_T("\\D3D9.DLL"));
	HMODULE d3d9_module(LoadLibrary(d3d9_path.c_str())); // オリジナルのD3D9.DLLのモジュール

	if (!d3d9_module) {
		return FALSE;
	}
	
	// MME
	{
		TCHAR app_full_path[1024];	// アプリフルパス
		GetModuleFileNameW(NULL, app_full_path, sizeof(app_full_path) / sizeof(char));
		std::wstring full_path(app_full_path);
		std::wstring mme_path = full_path.substr(0, full_path.rfind(_T("MikuMikuDance.exe")));
		mme_path.append(_T("\\d3d9_mme.dll"));
		HMODULE mme_module = NULL;
		if (PathFileExists(mme_path.c_str()))
		{
			mme_module = (LoadLibrary(mme_path.c_str()));
		}
		if (mme_module)
		{
			// MMEのDirect3DCreate9の関数ポインタを取得
			mme_direct3d_create = reinterpret_cast<IDirect3D9 *(WINAPI*)(UINT)>(GetProcAddress(mme_module, "Direct3DCreate9"));
		}
		else
		{
			mme_direct3d_create = NULL;
		}
	}

	// オリジナルDirect3DCreate9の関数ポインタを取得
	original_direct3d_create = reinterpret_cast<IDirect3D9 *(WINAPI*)(UINT)>(GetProcAddress(d3d9_module, "Direct3DCreate9"));
	if (!original_direct3d_create ) {
		return FALSE;
	}

	return TRUE;
}

static void dispose() 
{
	//DeleteCriticalSection(&criticalSection);
	renderData.dispose();
	//Py_Finalize();
	CloseHandle(hMutex);
#ifdef WITH_ALEMBIC
	AlembicArchive::instance().end();
#endif
}

// DLLエントリポイント
BOOL APIENTRY DllMain(HINSTANCE hinst, DWORD reason, LPVOID) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			hInstance=hinst;
			init();
			break;
		case DLL_PROCESS_DETACH:
			dispose();
			break;
	}
	return TRUE;
}
