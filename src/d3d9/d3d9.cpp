
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

#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/copy_non_const_reference.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/args.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/class.hpp>
#include <boost/python/overloads.hpp>
#include <boost/format.hpp>
#include <commctrl.h>
#include <richedit.h>

#include <process.h>

#include "bridge_parameter.h"
#include "alembic.h"
#include "resource.h"
#include "MMDExport.h"
#include "UMStringUtil.h"
#include "UMPath.h"

#ifdef _WIN64
#define _LONG_PTR LONG_PTR
#else
#define _LONG_PTR LONG
#endif

template <class T> std::string to_string(T value)
{
	return umbase::UMStringUtil::number_to_string(value);
}

//ワイド文字列からutf8文字列に変換
static void to_string(std::string &dest, const std::wstring &src) 
{
	dest = umbase::UMStringUtil::wstring_to_utf8(src);
}

static void messagebox(std::string title, std::string message)
{
	::MessageBoxA(NULL, message.c_str(), title.c_str(), MB_OK);
}

static void message(std::string message)
{
	::MessageBoxA(NULL, message.c_str(), "message", MB_OK);
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
IDirect3DDevice9 *p_device = NULL;

RenderData renderData;

std::vector<std::pair<IDirect3DTexture9*, bool> > finishTextureBuffers;

std::map<IDirect3DTexture9*, RenderedTexture> renderedTextures;
std::map<int, std::map<int , RenderedMaterial*> > renderedMaterials;
//-----------------------------------------------------------------------------------------------------------------

static bool writeTextureToFile(const std::string &texturePath, IDirect3DTexture9 * texture, D3DXIMAGE_FILEFORMAT fileFormat);

static bool writeTextureToFiles(const std::string &texturePath, const std::string &textureType, bool uncopied = false);

static bool copyTextureToFiles(const std::u16string &texturePath);

static bool writeTextureToMemory(const std::string &textureName, IDirect3DTexture9 * texture, bool copied);

//------------------------------------------Python呼び出し--------------------------------------------------------
static int pre_frame = 0;
static int presentCount = 0;
static int process_frame = -1;
static int ui_frame = 0;

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
namespace
{
	using namespace boost::python;
	std::wstring pythonName; // スクリプト名
	int script_call_setting = 2; // スクリプト呼び出し設定
	std::map<int, int> exportedFrames;

	/// スクリプトのリロード.
	bool relaod_python_script()
	{
		BridgeParameter::mutable_instance().mmdbridge_python_script.clear();
		std::ifstream ifs(BridgeParameter::instance().python_script_path.c_str());
		if (!ifs) return false;
		char buf[2048];
		while (ifs.getline( buf, sizeof(buf))) {
			BridgeParameter::mutable_instance().mmdbridge_python_script.append(buf);
			BridgeParameter::mutable_instance().mmdbridge_python_script.append("\r\n");
		}
		ifs.close();
		return true;
	}

	/// スクリプトパスのリロード.
	void reload_python_file_paths()
	{
		TCHAR app_full_path[1024];	// アプリフルパス
		
		GetModuleFileName(NULL, app_full_path, sizeof(app_full_path) / sizeof(TCHAR));

		BridgeParameter& mutable_parameter = BridgeParameter::mutable_instance();
		std::wstring searchPath = mutable_parameter.base_path;
		std::wstring searchStr(searchPath + _T("*.py"));

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
					if (mutable_parameter.python_script_name.empty()) { 
						mutable_parameter.python_script_name = name;
						mutable_parameter.python_script_path = path;
					}
					mutable_parameter.python_script_name_list.push_back(name);
					mutable_parameter.python_script_path_list.push_back(path);
				}
			} while(FindNextFile(hFind, &find));
			FindClose(hFind);
		}
	}

	// Get a reference to the main module.
	PyObject* main_module = NULL; 

	// Get the main module's dictionary
	// and make a copy of it.
	PyObject* main_dict = NULL;

	int get_vertex_buffer_size()
	{
		return BridgeParameter::instance().finish_buffer_list.size();
	}

	int get_vertex_size(int at)
	{
		return BridgeParameter::instance().render_buffer(at).vertecies.size();
	}

	boost::python::list get_vertex(int at, int vpos)
	{
		const RenderedBuffer& buffer = BridgeParameter::instance().render_buffer(at);
		float x = buffer.vertecies[vpos].x;
		float y = buffer.vertecies[vpos].y;
		float z = buffer.vertecies[vpos].z;
		boost::python::list result;
		result.append(x);
		result.append(y);
		result.append(z);
		return result;
	}

	int get_normal_size(int at)
	{
		return BridgeParameter::instance().render_buffer(at).normals.size();
	}

	boost::python::list get_normal(int at, int vpos)
	{
		const RenderedBuffer& buffer = BridgeParameter::instance().render_buffer(at);
		float x = buffer.normals[vpos].x;
		float y = buffer.normals[vpos].y;
		float z = buffer.normals[vpos].z;
		boost::python::list result;
		result.append(x);
		result.append(y);
		result.append(z);
		return result;
	}

	int get_uv_size(int at)
	{
		return BridgeParameter::instance().render_buffer(at).uvs.size();
	}

	boost::python::list get_uv(int at, int vpos)
	{
		const RenderedBuffer& buffer = BridgeParameter::instance().render_buffer(at);
		float u = buffer.uvs[vpos].x;
		float v = buffer.uvs[vpos].y;
		boost::python::list result;
		result.append(u);
		result.append(v);
		return result;
	}

	int get_material_size(int at)
	{
		return BridgeParameter::instance().render_buffer(at).materials.size();
	}

	bool is_accessory(int at)
	{
		int result = 0;
		if (BridgeParameter::instance().render_buffer(at).isAccessory)
		{
			return true;
		}
		return false;
	}

	int get_pre_accessory_count()
	{
		return ExpGetPreAcsNum();
	}

	boost::python::list get_diffuse(int at, int mpos)
	{
		RenderedMaterial* mat = BridgeParameter::instance().render_buffer(at).materials[mpos];
		boost::python::list result;
		result.append(mat->diffuse.x);
		result.append(mat->diffuse.y);
		result.append(mat->diffuse.z);
		result.append(mat->diffuse.w);
		return result;
	}

	boost::python::list get_ambient(int at, int mpos)
	{
		RenderedMaterial* mat = BridgeParameter::instance().render_buffer(at).materials[mpos];
		boost::python::list result;
		result.append(mat->ambient.x);
		result.append(mat->ambient.y);
		result.append(mat->ambient.z);
		return result;
	}

	boost::python::list get_specular(int at, int mpos)
	{
		RenderedMaterial* mat = BridgeParameter::instance().render_buffer(at).materials[mpos];
		boost::python::list result;
		result.append(mat->specular.x);
		result.append(mat->specular.y);
		result.append(mat->specular.z);
		return result;
	}

	boost::python::list get_emissive(int at, int mpos)
	{
		RenderedMaterial* mat = BridgeParameter::instance().render_buffer(at).materials[mpos];
		boost::python::list result;
		result.append(mat->emissive.x);
		result.append(mat->emissive.y);
		result.append(mat->emissive.z);
		return result;
	}

	float get_power(int at, int mpos)
	{
		RenderedMaterial* mat = BridgeParameter::instance().render_buffer(at).materials[mpos];
		float power = mat->power;
		return power;
	}

	std::string get_texture(int at, int mpos)
	{
		RenderedMaterial* mat = BridgeParameter::instance().render_buffer(at).materials[mpos];
		return mat->texture;
	}

	std::string get_exported_texture(int at, int mpos)
	{
		RenderedMaterial* mat = BridgeParameter::instance().render_buffer(at).materials[mpos];
		return mat->memoryTexture;
	}

	int get_face_size(int at, int mpos)
	{
		return BridgeParameter::instance().render_buffer(at).materials[mpos]->surface.faces.size();
	}

	boost::python::list get_face(int at, int mpos, int fpos)
	{
		RenderedSurface &surface = BridgeParameter::instance().render_buffer(at).materials[mpos]->surface;
		int v1 = surface.faces[fpos].x;
		int v2 = surface.faces[fpos].y;
		int v3 = surface.faces[fpos].z;
		boost::python::list result;
		result.append(v1);
		result.append(v2);
		result.append(v3);
		return result;
	}

	int get_texture_buffer_size()
	{
		return finishTextureBuffers.size();
	}

	boost::python::list get_texture_size(int at)
	{
		boost::python::list result;
		result.append(renderedTextures[finishTextureBuffers[at].first].size.x);
		result.append(renderedTextures[finishTextureBuffers[at].first].size.y);
		return result;
	}

	std::string get_texture_name(int at)
	{
		return renderedTextures[finishTextureBuffers[at].first].name;
	}

	boost::python::list get_texture_pixel(int at, int tpos)
	{
		UMVec4f &rgba = renderedTextures[finishTextureBuffers[at].first].texture[tpos];
		boost::python::list result;
		result.append(rgba.x);
		result.append(rgba.y);
		result.append(rgba.z);
		result.append(rgba.w);
		return result;
	}

	bool export_texture(int at, int mpos, const std::string& dst)
	{
		RenderedMaterial* mat = BridgeParameter::instance().render_buffer(at).materials[mpos];
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
		else { return false; }

		if (mat->tex)
		{
			return writeTextureToFile(path, mat->tex, fileFormat);
		}
		return false;
	}

	bool export_textures(const std::string& p, const std::string& t)
	{
		std::u16string path = umbase::UMStringUtil::utf8_to_utf16(p);
		std::string type(t);
		if (umbase::UMPath::exists(path))
		{
			return writeTextureToFiles(p, t);
		}
		return false;
	}

	bool export_uncopied_textures(const std::string& p, const std::string& t)
	{
		std::u16string path = umbase::UMStringUtil::utf8_to_utf16(p);
		if (umbase::UMPath::exists(path))
		{
			return writeTextureToFiles(p, t, true);
		}
		return false;
	}

	bool copy_textures(const std::string& s)
	{
		std::u16string path = umbase::UMStringUtil::utf8_to_utf16(s);
		if (umbase::UMPath::exists(path))
		{
			std::wstring wpath = umbase::UMStringUtil::utf16_to_wstring(path);
			return copyTextureToFiles(path);
		}
		return false;
	}

	std::string get_base_path()
	{
		std::string path = umbase::UMStringUtil::wstring_to_utf8(BridgeParameter::instance().base_path);
		return path;
	}

	boost::python::list get_camera_up()
	{
		D3DXVECTOR3 v;
		D3DXVECTOR3 dst;
		UMGetCameraUp(&v);
		d3d_vector3_dir_transform(dst, v, BridgeParameter::instance().first_noaccessory_buffer().world_inv);
		boost::python::list result;
		result.append(dst.x);
		result.append(dst.y);
		result.append(dst.z);
		return result;
	}

	boost::python::list get_camera_up_org()
	{
		D3DXVECTOR3 v;
		UMGetCameraUp(&v);
		boost::python::list result;
		result.append(v.x);
		result.append(v.y);
		result.append(v.z);
		return result;
	}
	
	boost::python::list get_camera_at()
	{
		D3DXVECTOR3 v;
		D3DXVECTOR3 dst;
		UMGetCameraAt(&v);
		d3d_vector3_transform(dst, v, BridgeParameter::instance().first_noaccessory_buffer().world_inv);
		boost::python::list result;
		result.append(dst.x);
		result.append(dst.y);
		result.append(dst.z);
		return result;
	}

	boost::python::list get_camera_eye()
	{
		D3DXVECTOR3 v;
		D3DXVECTOR3 dst;
		UMGetCameraEye(&v);
		d3d_vector3_transform(dst, v, BridgeParameter::instance().first_noaccessory_buffer().world_inv);
		boost::python::list result;
		result.append(dst.x);
		result.append(dst.y);
		result.append(dst.z);
		return result;
	}

	boost::python::list get_camera_eye_org()
	{
		D3DXVECTOR3 v;
		UMGetCameraEye(&v);
		boost::python::list result;
		result.append(v.x);
		result.append(v.y);
		result.append(v.z);
		return result;
	}

	float get_camera_fovy()
	{
		D3DXVECTOR4 v;
		UMGetCameraFovLH(&v);
		return  v.x;
	}

	float get_camera_aspect()
	{
		D3DXVECTOR4 v;
		UMGetCameraFovLH(&v);
		return v.y;
	}

	float get_camera_near()
	{
		D3DXVECTOR4 v;
		UMGetCameraFovLH(&v);
		return v.z;
	}

	float get_camera_far()
	{
		D3DXVECTOR4 v;
		UMGetCameraFovLH(&v);
		return v.w;
	}
	
	int get_frame_number()
	{
		if (process_frame >= 0) 
		{
			return process_frame;
		}
		else
		{
			return ui_frame;
		}
	}

	int get_start_frame()
	{
		return BridgeParameter::instance().start_frame;
	}
	
	int get_end_frame()
	{
		return BridgeParameter::instance().end_frame;
	}

	int get_frame_width()
	{
		return BridgeParameter::instance().frame_width;
	}

	int get_frame_height()
	{
		return BridgeParameter::instance().frame_height;
	}

	boost::python::list get_light(int at)
	{
		const UMVec3f &light = BridgeParameter::instance().render_buffer(at).light;
		boost::python::list result;
		result.append(light.x);
		result.append(light.y);
		result.append(light.z);
		return result;
	}

	boost::python::list get_light_color(int at)
	{
		const UMVec3f &light = BridgeParameter::instance().render_buffer(at).light_color;
		boost::python::list result;
		result.append(light.x);
		result.append(light.y);
		result.append(light.z);
		return result;
	}

	boost::python::list get_world(int at)
	{
		const D3DXMATRIX& world = BridgeParameter::instance().render_buffer(at).world;
		boost::python::list result;
		for (int i = 0; i < 4; ++i)
		{
			for (int k = 0; k < 4; ++k)
			{
				result.append(world.m[i][k]);
			}
		}
		return result;
	}

	boost::python::list get_world_inv(int at)
	{
		const D3DXMATRIX& world_inv = BridgeParameter::instance().render_buffer(at).world_inv;
		boost::python::list result;
		for (int i = 0; i < 4; ++i)
		{
			for (int k = 0; k < 4; ++k)
			{
				result.append(world_inv.m[i][k]);
			}
		}
		return result;
	}

	boost::python::list get_view(int at)
	{
		const D3DXMATRIX& view = BridgeParameter::instance().render_buffer(at).view;
		boost::python::list result;
		for (int i = 0; i < 4; ++i)
		{
			for (int k = 0; k < 4; ++k)
			{
				result.append(view.m[i][k]);
			}
		}
		return result;
	}

	boost::python::list get_projection(int at)
	{
		const D3DXMATRIX& projection = BridgeParameter::instance().render_buffer(at).projection;
		boost::python::list result;
		for (int i = 0; i < 4; ++i)
		{
			for (int k = 0; k < 4; ++k)
			{
				result.append(projection.m[i][k]);
			}
		}
		return result;
	}

	bool set_texture_buffer_enabled(bool enabled)
	{
		BridgeParameter::mutable_instance().is_texture_buffer_enabled = enabled;
		return true;
	}

	bool set_int_value(int pos, int value)
	{
		BridgeParameter::mutable_instance().py_int_map[pos] = value;
		return true;
	}

	bool set_float_value(int pos, float value)
	{
		BridgeParameter::mutable_instance().py_float_map[pos] = value;
		return true;
	}

	int get_int_value(int pos)
	{
		if (BridgeParameter::instance().py_int_map.find(pos) != BridgeParameter::instance().py_int_map.end())
		{
			return BridgeParameter::mutable_instance().py_int_map[pos];
		}
		return 0;
	}

	float get_float_value(int pos)
	{
		if (BridgeParameter::instance().py_float_map.find(pos) != BridgeParameter::instance().py_float_map.end())
		{
			return BridgeParameter::mutable_instance().py_float_map[pos];
		}
		return 0;
	}

	//boost::python::list d3dx_invert_matrix(PyObject *self, PyObject* args)
	//{
	//	D3DXMATRIX src;		
	//	if (!PyArg_ParseTuple(args, "(ffffffffffffffff)", 			
	//		&src.m[0][0], &src.m[0][1], &src.m[0][2], &src.m[0][3],
	//		&src.m[1][0], &src.m[1][1], &src.m[1][2], &src.m[1][3],
	//		&src.m[2][0], &src.m[2][1], &src.m[2][2], &src.m[2][3],
	//		&src.m[3][0], &src.m[3][1], &src.m[3][2], &src.m[3][3])) { return NULL; }

	//	D3DXMATRIX dst;
	//	::D3DXMatrixInverse(&dst, NULL, &src);

	//	return Py_BuildValue("[f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f]", 
	//		dst.m[0][0], dst.m[0][1], dst.m[0][2], dst.m[0][3],
	//		dst.m[1][0], dst.m[1][1], dst.m[1][2], dst.m[1][3],
	//		dst.m[2][0], dst.m[2][1], dst.m[2][2], dst.m[2][3],
	//		dst.m[3][0], dst.m[3][1], dst.m[3][2], dst.m[3][3]);
	//}

	boost::python::list d3dx_vec3_normalize(float x, float y, float z)
	{
		D3DXVECTOR3 vec(x, y, z);
		::D3DXVec3Normalize(&vec, &vec);
		boost::python::list result;
		result.append(vec.x);
		result.append(vec.y);
		result.append(vec.z);
		return result;
	}

	//class PythonRedirectBuffer
	//{
	//public:
	//	typedef std::string Buffer;
	//	Buffer& output() { return buffer_; }
	//	bool flush() {
	//		//std::cout << buffer_ << std::flush;
	//		for (int i = 0, size = buffer_.size(); i < size; ++i) {
	//			putchar(buffer_[i]);
	//		}
	//		buffer_.clear();
	//		return true;
	//	}
	//	void write(std::string const& str) {
	//		const int current = static_cast<int>(buffer_.size());
	//		const int size = static_cast<int>(str.size());
	//		const int next = current + size;
	//		buffer_.resize(next);
	//		memcpy(&buffer_[current], str.c_str(), size);
	//	}
	//private:
	//	static Buffer buffer_;
	//};
	//PythonRedirectBuffer::Buffer PythonRedirectBuffer::buffer_;
	//
	//class PythonRedirect {
	//public:
	//	PythonRedirectBuffer redirect_buffer_;
	//	void write(std::string const& str) {
	//		const int current = static_cast<int>(redirect_buffer_.output().size());
	//		const int size = static_cast<int>(str.size());
	//		const int next = current + size;
	//		redirect_buffer_.output().resize(next);
	//		memcpy(&redirect_buffer_.output()[current], str.c_str(), size);
	//	}
	//};

	object init_python()
	{
		object main_module = import("__main__");
		object main_namespace = main_module.attr("__dict__");
		//main_namespace["PythonRedirectBuffer"]
		//	= class_<PythonRedirectBuffer>("PythonRedirectBuffer", init<>())
		//	.def("flush", &PythonRedirectBuffer::flush)
		//	.def("write", &PythonRedirectBuffer::write);
		//main_namespace["PythonRedirect"] 
		//	= class_<PythonRedirect>("PythonRedirect", init<>())
		//	.add_property("buffer", &PythonRedirect::redirect_buffer_)
		//	.def("write", &PythonRedirect::write);
		//PythonRedirect redirect;
		//boost::python::import("sys").attr("stderr") = redirect;
		//boost::python::import("sys").attr("stdout") = redirect;
		return main_namespace;
	}
	
	std::string parse_python_exception()
	{
		PyObject *type_ptr = NULL;
		PyObject *value_ptr = NULL;
		PyObject *traceback_ptr = NULL;
		PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);

		// Fallback error
		std::string ret("Fallback error");
		if(type_ptr != NULL)
		{
			boost::python::handle<> h_type(type_ptr);
			boost::python::str type_pstr(h_type);
			boost::python::extract<std::string> e_type_pstr(type_pstr);
			//  otherwise use fallback
			if(e_type_pstr.check())
			{
				ret = e_type_pstr();
			}
			else
			{
				ret = "Unknown exception";
			}
		}
		// Do the same for the exception value (the stringification of the exception)
		if(value_ptr != NULL){
			boost::python::handle<> h_val(value_ptr);
			boost::python::str a(h_val);
			boost::python::extract<std::string> returned(a);
			if(returned.check()) 
			{
				ret +=  ": " + returned();
			}
			else
			{
				ret += std::string(": Unparseable Python error: ");
			}
		}
		// Parse lines from the traceback using the Python traceback module
		if(traceback_ptr != NULL)
		{
			boost::python::handle<> h_tb(traceback_ptr);
			// Load the traceback module and the format_tb function
			boost::python::object tb(boost::python::import("traceback"));
			boost::python::object fmt_tb(tb.attr("format_tb"));
			// Call format_tb to get a list of traceback strings
			boost::python::object tb_list(fmt_tb(h_tb));
			// Join the traceback strings into a single string
			boost::python::object tb_str(boost::python::str("\n").join(tb_list));
			// Extract the string, check the extraction, and fallback in necessary
			boost::python::extract<std::string> returned(tb_str);
			if(returned.check()) 
			{
				ret += ": " + returned();
			}
			else
			{
				ret += std::string(": Unparseable Python traceback");
			}
		}
		return ret;
	}
}


BOOST_PYTHON_MODULE( mmdbridge )
{
	using namespace boost::python;
	def("get_vertex_buffer_size", get_vertex_buffer_size);
	def("get_vertex_size", get_vertex_size);
	def("get_vertex", get_vertex);
	def("get_normal_size", get_normal_size);
	def("get_normal", get_normal);
	def("get_uv_size", get_uv_size);
	def("get_uv", get_uv);
	def("get_material_size", get_material_size);
	def("is_accessory", is_accessory);
	def("get_pre_accessory_count", get_pre_accessory_count);
	def("get_ambient", get_ambient);
	def("get_diffuse", get_diffuse);
	def("get_specular", get_specular);
	def("get_emissive", get_emissive);
	def("get_power", get_power);
	def("get_texture", get_texture);
	def("get_exported_texture", get_exported_texture);
	def("get_face_size", get_face_size);
	def("get_face", get_face);
	def("get_texture_buffer_size", get_texture_buffer_size);
	def("get_texture_size", get_texture_size);
	def("get_texture_name", get_texture_name);
	def("get_texture_pixel", get_texture_pixel);
	def("get_camera_up", get_camera_up);
	def("get_camera_up_org", get_camera_up_org);
	def("get_camera_at",  get_camera_at);
	def("get_camera_eye",  get_camera_eye);
	def("get_camera_eye_org",  get_camera_eye_org);
	def("get_camera_fovy", get_camera_fovy);
	def("get_camera_aspect", get_camera_aspect);
	def("get_camera_near", get_camera_near);
	def("get_camera_far", get_camera_far);
	def("messagebox", message);
	def("export_texture", export_texture);
	def("export_textures", export_textures);
	def("export_uncopied_textures", export_textures);
	def("copy_textures", copy_textures);
	def("get_frame_number", get_frame_number);
	def("get_start_frame", get_start_frame);
	def("get_end_frame", get_end_frame);
	def("get_frame_width", get_frame_width);
	def("get_frame_height", get_frame_height);
	def("get_base_path", get_base_path);
	def("get_light", get_light);
	def("get_light_color", get_light_color);
	def("get_world", get_world);
	def("get_world_inv", get_world_inv);
	def("get_view", get_view);
	def("get_projection", get_projection);
	def("set_texture_buffer_enabled", set_texture_buffer_enabled);
	//def("set_current_material", set_current_material);
	def("set_int_value", set_int_value);
	def("set_float_value", set_float_value);
	def("get_int_value", get_int_value);
	def("get_float_value", get_float_value);
	//def("d3dx_invert_matrix", d3dx_invert_matrix);
	def("d3dx_vec3_normalize", d3dx_vec3_normalize);
}

void run_python_script()
{
	relaod_python_script();
	if (BridgeParameter::instance().mmdbridge_python_script.empty()) { return; }

	if (Py_IsInitialized())
	{
		//
		PyEval_InitThreads();
		Py_InspectFlag = 0;
		
		if (script_call_setting > 1)
		{
			script_call_setting = 0;
		}
	}
	else
	{
		InitAlembic();
		PyImport_AppendInittab("mmdbridge", PyInit_mmdbridge);
		Py_Initialize();
			
		// 入力引数の設定
		{
			int argc = 1;
			const std::wstring wpath = BridgeParameter::instance().base_path;
			wchar_t *path[] = {
				const_cast<wchar_t*>(wpath.c_str())
			};
			PySys_SetArgv(argc, path);
		}
	}

	try
	{
		// モジュール初期化.
		boost::python::object main_namespace = init_python();
		// スクリプトの実行.
		boost::python::object res = boost::python::exec(
			BridgeParameter::instance().mmdbridge_python_script.c_str(),
			main_namespace);
	}
	catch(error_already_set const& )
	{
		std::string python_error_string = parse_python_exception();
		::MessageBoxA(NULL, python_error_string.c_str(), "python error", MB_OK);
	}
}
//-----------------------------------------------------------Hook関数ポインタ-----------------------------------------------------------

// Direct3DCreate9
IDirect3D9 *(WINAPI *original_direct3d_create)(UINT)(NULL);

HRESULT (WINAPI *original_direct3d9ex_create)(UINT, IDirect3D9Ex**)(NULL);
// IDirect3D9::CreateDevice
HRESULT (WINAPI *original_create_device)(IDirect3D9*,UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, IDirect3DDevice9**)(NULL);

HRESULT(WINAPI *original_create_deviceex)(IDirect3D9Ex*, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, D3DDISPLAYMODEEX*, IDirect3DDevice9Ex**)(NULL);
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

static bool writeTextureToFile(const std::string &texturePath, IDirect3DTexture9 * texture, D3DXIMAGE_FILEFORMAT fileFormat)
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

static bool writeTextureToFiles(const std::string &texturePath, const std::string &textureType, bool uncopied)
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

static bool copyTextureToFiles(const std::u16string &texturePath)
{
	if (texturePath.empty()) return false;

	std::wstring path = umbase::UMStringUtil::utf16_to_wstring(texturePath);
	PathRemoveFileSpec(&path[0]);
	PathAddBackslash(&path[0]);
	if (!PathIsDirectory(path.c_str())) { return false; }
	
	bool res = true;
	for (size_t i = 0; i <  finishTextureBuffers.size(); ++i)
	{
		IDirect3DTexture9* texture = finishTextureBuffers[i].first;
		if (texture) {
			if (!UMCopyTexture(path.c_str(), texture)) { res = false; }
		}
	}
	return res;
}

static bool writeTextureToMemory(const std::string &textureName, IDirect3DTexture9 * texture, bool copied)
{
	// すでにfinishTexutureBufferにあるかどうか
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

	if (BridgeParameter::instance().is_texture_buffer_enabled)
	{
		TextureBuffers::iterator tit = renderData.textureBuffers.find(texture);
		if(tit != renderData.textureBuffers.end())
		{
			// テクスチャをメモリに書き出し
			D3DLOCKED_RECT lockRect;
			HRESULT isLocked = texture->lpVtbl->LockRect(texture, 0, &lockRect, NULL, D3DLOCK_READONLY);
			if (isLocked != D3D_OK) { return false; }
			
			int width = tit->second.wh.x;
			int height = tit->second.wh.y;

			RenderedTexture tex;
			tex.size.x = width;
			tex.size.y = height;
			tex.name = textureName;

			D3DFORMAT format = tit->second.format;
			for(int y = 0; y < height; y++)
			{
				unsigned char *lineHead = (unsigned char*)lockRect.pBits + lockRect.Pitch * y;

				for (int x = 0; x < width; x++)
				{
					if (format == D3DFMT_A8R8G8B8) {
						UMVec4f rgba;
						rgba.x = lineHead[4 * x + 0];
						rgba.y = lineHead[4 * x + 1];
						rgba.z = lineHead[4 * x + 2];
						rgba.w = lineHead[4 * x + 3];
						tex.texture.push_back(rgba);
					} else {
						::MessageBoxA(NULL, std::string("not supported texture format:" + format).c_str(), "info", MB_OK);
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
		
	ui_frame= atoi(text);
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
	HANDLE hModule=(HANDLE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
	if(GetModuleHandle(NULL)==hModule)
	{
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

LONG_PTR originalWndProc  =NULL;
// このコード モジュールに含まれる関数の宣言を転送します:
INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
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
					::DialogBoxA(hInstance, "IDD_DIALOG1", NULL,  DialogProc);
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
	return CallWindowProc( (WNDPROC)originalWndProc, hWnd, msg, wp, lp );
}

static INT_PTR CALLBACK DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const BridgeParameter& parameter = BridgeParameter::instance();
	BridgeParameter& mutable_parameter = BridgeParameter::mutable_instance();
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
				for (size_t i = 0 ; i < parameter.python_script_name_list.size() ; i++)
				{
					SendMessage(hCombo1 , CB_ADDSTRING , 0 , (LPARAM)parameter.python_script_name_list[i].c_str());
				}
				//SendMessage(hCombo2 , CB_ADDSTRING , 0 , (LPARAM)_T("画面操作時に実行"));
				SendMessage(hCombo2 , CB_ADDSTRING , 0 , (LPARAM)_T("実行する"));
				SendMessage(hCombo2 , CB_ADDSTRING , 0 , (LPARAM)_T("実行しない"));
				// ウインドウ生成時にはじめに表示するデータを指定
				UINT index1 = SendMessage(hCombo1, CB_FINDSTRINGEXACT, -1, (LPARAM)parameter.python_script_name.c_str());
				SendMessage(hCombo1, CB_SETCURSEL, index1, 0);
				SendMessage(hCombo2, CB_SETCURSEL, script_call_setting - 1, 0);

				::SetWindowTextA(hEdit1, to_string(parameter.start_frame).c_str());
				::SetWindowTextA(hEdit2, to_string(parameter.end_frame).c_str());
				//::SetWindowTextA(hEdit3, to_string().c_str());
				//::SetWindowTextA(hEdit4, to_string().c_str());
				::SetWindowTextA(hEdit5, to_string(parameter.export_fps).c_str());
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
						if (num1 < parameter.python_script_name_list.size())
						{
							if (pythonName != parameter.python_script_name_list[num1])
							{
								pythonName = parameter.python_script_name_list[num1];
								mutable_parameter.python_script_path = parameter.python_script_path_list[num1];
								relaod_python_script();
							}
						}
						UINT num2 = (UINT)SendMessage(hCombo2, CB_GETCURSEL, 0, 0);
						if (num2 <= 2)
						{
							script_call_setting = num2 + 1;
						}

						char text1[32];
						char text2[32];
						char text5[32];
						::GetWindowTextA(hEdit1, text1, sizeof(text1)/sizeof(text1[0]));
						::GetWindowTextA(hEdit2, text2, sizeof(text2)/sizeof(text2[0]));
						//::GetWindowTextA(hEdit3, text3, sizeof(text3)/sizeof(text3[0]));
						//::GetWindowTextA(hEdit4, text4, sizeof(text4)/sizeof(text4[0]));
						::GetWindowTextA(hEdit5, text5, sizeof(text5)/sizeof(text5[0]));
						mutable_parameter.start_frame = atoi(text1);
						mutable_parameter.end_frame = atoi(text2);
						mutable_parameter.export_fps = atof(text5);
						
						if (parameter.start_frame >= parameter.end_frame)
						{
							mutable_parameter.end_frame = parameter.start_frame + 1;
							::SetWindowTextA(hEdit2, to_string(parameter.end_frame).c_str());
						}
						EndDialog(hWnd, IDOK);
					}
					break;
				case IDCANCEL:
					EndDialog(hWnd, IDCANCEL);
					break;
				case IDC_BUTTON1: // 再検索
					reload_python_file_paths();
					SendMessage(hCombo1, CB_SETCURSEL, SendMessage(hCombo1, CB_FINDSTRINGEXACT, -1, (LPARAM)pythonName.c_str()), 0);
					break;
				case IDC_BUTTON2: // 詳細設定

					// ダイアログ表示中は問答無用でスクリプト実行を専用のモードにする
					int preSetting = script_call_setting;
					script_call_setting = 3;

					// script再読み込み
					UINT num1 = (UINT)SendMessage(hCombo1, CB_GETCURSEL, 0, 0);
					if (num1 < parameter.python_script_name_list.size())
					{
						if (pythonName != parameter.python_script_name_list[num1]) {
							pythonName = parameter.python_script_name_list[num1];
							mutable_parameter.python_script_path = parameter.python_script_path_list[num1];
							relaod_python_script();
						}
					}
					if (!parameter.mmdbridge_python_script.empty())
					{
						if (parameter.mmdbridge_python_script.find("#thread") != std::string::npos)
						{
							EndDialog(hWnd, IDOK);
							SendMessage(g_hFrameArrowRight, BM_CLICK, 0, 0);
							break;
						}
					}
					script_call_setting = preSetting;
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
static void overrideGLWindow()
{
	EnumWindows(enumWindowsProc,0);
	setMyMenu();
	// サブクラス化
	if(g_hWnd && !originalWndProc){
		originalWndProc = GetWindowLongPtr(g_hWnd,GWLP_WNDPROC);
		SetWindowLongPtr(g_hWnd,GWLP_WNDPROC,(_LONG_PTR)overrideWndProc);
	}
}


static bool IsValidCallSetting() { 
	return (script_call_setting == 0) || (script_call_setting == 1);
}

static bool IsValidFrame() {
	HWND recWindow = FindWindowA("RecWindow", NULL);
	return (recWindow != NULL);
}

static bool IsValidTechniq() {
	const int technic = ExpGetCurrentTechnic();
	return (technic == 0 || technic == 1 || technic == 2);
}

float pretime = 0.0f;

static HRESULT WINAPI present(
	IDirect3DDevice9 *device, 
	const RECT * pSourceRect, 
	const RECT * pDestRect, 
	HWND hDestWindowOverride, 
	const RGNDATA * pDirtyRegion)
{
	static bool isExportedFrame = false;

	float time = ExpGetFrameTime();

	if (pDestRect)
	{
		BridgeParameter::mutable_instance().frame_width = pDestRect->right - pDestRect->left;
		BridgeParameter::mutable_instance().frame_height = pDestRect->bottom - pDestRect->top;
	}
	overrideGLWindow();
	const bool validFrame = IsValidFrame();
	const bool validCallSetting = IsValidCallSetting();
	const bool validTechniq = IsValidTechniq();
	if (validFrame && validCallSetting && validTechniq)
	{
		if (script_call_setting == 1)
		{
			const BridgeParameter& parameter = BridgeParameter::instance();
			int frame = static_cast<int>(time * BridgeParameter::instance().export_fps + 0.5f);
			if (frame >= parameter.start_frame && frame <= parameter.end_frame)
			{
				if (exportedFrames.find(frame) == exportedFrames.end())
				{
					process_frame = frame;
					run_python_script();
					exportedFrames[process_frame] = 1;
					if (process_frame == parameter.end_frame)
					{
						exportedFrames.clear();
						isExportedFrame = false;
					}
					pre_frame = frame;
				}
			}
		}
		BridgeParameter::mutable_instance().finish_buffer_list.clear();
		presentCount++;
	}
	HRESULT res = (*original_present)(device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
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

	//float time = ExpGetFrameTime();
	if (script_call_setting != 2)
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

	VertexBufferList& finishBuffers = BridgeParameter::mutable_instance().finish_buffer_list;
	if (std::find(finishBuffers.begin(), finishBuffers.end(), pStreamData) == finishBuffers.end())
	{
		VertexBuffers::iterator vit = renderData.vertexBuffers.find(pStreamData);
		if(vit != renderData.vertexBuffers.end())
		{
			RenderBufferMap& renderedBuffers = BridgeParameter::mutable_instance().render_buffer_map;
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
						UMVec2f uv;
						memcpy(&uv, &pVertexBuf[i], sizeof( UMVec2f ));
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
	RenderBufferMap& renderedBuffers = BridgeParameter::mutable_instance().render_buffer_map;
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
				mat->diffuse.x = edge[0] + specular[0];
				if (mat->diffuse.x > 1) { mat->diffuse.x = 1.0f; }
				mat->diffuse.y = edge[1] + specular[1];
				if (mat->diffuse.y > 1) { mat->diffuse.y = 1.0f; }
				mat->diffuse.z = edge[2] + specular[2];
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
		UMVec3f& umlight = renderedBuffer.light;
		D3DXVECTOR3 v(light.Direction.x, light.Direction.y, light.Direction.z);
		D3DXVECTOR4 dst;
		//D3DXVec3Transform(&dst, &v, &renderedBuffer.world);
		umlight.x = v.x;
		umlight.y = v.y;
		umlight.z = v.z;
		
		renderedBuffer.light_color.x = light.Ambient.r;
		renderedBuffer.light_color.y = light.Ambient.g;
		renderedBuffer.light_color.z = light.Ambient.b;
		//renderedBuffer.light_diffuse.x = light.Diffuse.r;
		//renderedBuffer.light_diffuse.y = light.Diffuse.g;
		//renderedBuffer.light_diffuse.z = light.Diffuse.b;
		//renderedBuffer.light_diffuse.w = light.Diffuse.a;
		//renderedBuffer.light_specular.x = light.Specular.r;
		//renderedBuffer.light_specular.y = light.Specular.g;
		//renderedBuffer.light_specular.z = light.Specular.b;
		//renderedBuffer.light_specular.w = light.Specular.a;
		//renderedBuffer.light_position.x = light.Position.x;
		//renderedBuffer.light_position.y = light.Position.y;
		//renderedBuffer.light_position.z = light.Position.z;
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
	//float time = ExpGetFrameTime();

	const int currentMaterial = ExpGetCurrentMaterial();
	const int currentObject = ExpGetCurrentObject();

	const bool validCallSetting = IsValidCallSetting();
	const bool validFrame = IsValidFrame();
	const bool validTechniq = IsValidTechniq();

	if (validCallSetting && validFrame && validTechniq && type == D3DPT_TRIANGLELIST)
	{
		// レンダリング開始
		if (renderData.pIndexData && renderData.pStreamData && renderData.pos_xyz)
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
			IDirect3DIndexBuffer9 *pIndexData = renderData.pIndexData;

			D3DINDEXBUFFER_DESC indexDesc;
			if (pIndexData->lpVtbl->GetDesc(pIndexData, &indexDesc) == D3D_OK)
			{
				void *pIndexBuf;
				if (pIndexData->lpVtbl->Lock(pIndexData, 0, 0, (void**)&pIndexBuf, D3DLOCK_READONLY) == D3D_OK)
				{
					RenderBufferMap& renderedBuffers = BridgeParameter::mutable_instance().render_buffer_map;
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
						UMVec3i face;
						if (indexDesc.Format == D3DFMT_INDEX16)
						{
							WORD* p = (WORD*)pIndexBuf;
							face.x = static_cast<int>((p[startIndex + i + 0]) + 1);
							face.y = static_cast<int>((p[startIndex + i + 1]) + 1);
							face.z = static_cast<int>((p[startIndex + i + 2]) + 1);
						}
						else
						{
							DWORD* p = (DWORD*)pIndexBuf;
							face.x = static_cast<int>((p[startIndex + i + 0]) + 1);
							face.y = static_cast<int>((p[startIndex + i + 1]) + 1);
							face.z = static_cast<int>((p[startIndex + i + 2]) + 1);
						}
						const int vsize = renderedBuffer.vertecies.size();
						if (face.x > vsize || face.y > vsize || face.z > vsize) {
							continue;
						}
						if (face.x <= 0 || face.y <= 0 || face.z <= 0) {
							continue;
						}
						renderedSurface.faces.push_back(face);
						if (!renderData.normal)
						{
							if (renderedBuffer.normals.size() != vsize)
							{
								renderedBuffer.normals.resize(vsize);
							}
							D3DXVECTOR3 n;
							D3DXVECTOR3 v0 = renderedBuffer.vertecies[face.x - 1];
							D3DXVECTOR3 v1 = renderedBuffer.vertecies[face.y - 1];
							D3DXVECTOR3 v2 = renderedBuffer.vertecies[face.z - 1];
							D3DXVECTOR3 v10 = v1-v0;
							D3DXVECTOR3 v20 = v2-v0;
							::D3DXVec3Cross(&n, &v10, &v20);
							renderedBuffer.normals[face.x - 1] += n;
							renderedBuffer.normals[face.y - 1] += n;
							renderedBuffer.normals[face.z - 1] += n;
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
			}
			pIndexData->lpVtbl->Unlock(pIndexData);
		}
	}

	
	HRESULT res = (*original_draw_indexed_primitive)(device, type, baseVertexIndex, minIndex, numVertices, startIndex, primitiveCount);

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
	info.wh.x = width;
	info.wh.y = height;
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

	//float time = ExpGetFrameTime();

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

	//float time = ExpGetFrameTime();
		
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

static HRESULT WINAPI createDeviceEx(
	IDirect3D9Ex *direct3dex,
	UINT adapter,
	D3DDEVTYPE type,
	HWND window,
	DWORD flag,
	D3DPRESENT_PARAMETERS *param,
	D3DDISPLAYMODEEX* displayMode,
	IDirect3DDevice9Ex **device)
{
	HRESULT res = (*original_create_deviceex)(direct3dex, adapter, type, window, flag, param, displayMode, device);
	p_device = reinterpret_cast<IDirect3DDevice9*>(*device);

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

		return direct3d;
	}

	HRESULT WINAPI Direct3DCreate9Ex(UINT SDKVersion, IDirect3D9Ex **ppD3D) {
		IDirect3D9Ex *direct3d9ex = NULL;
		(*original_direct3d9ex_create)(SDKVersion, &direct3d9ex);

		if (direct3d9ex) 
		{
			original_create_deviceex = direct3d9ex->lpVtbl->CreateDeviceEx;
			if (original_create_deviceex)
			{
				// 書き込み属性付与
				DWORD old_protect;
				VirtualProtect(reinterpret_cast<void *>(direct3d9ex->lpVtbl), sizeof(direct3d9ex->lpVtbl), PAGE_EXECUTE_READWRITE, &old_protect);

				direct3d9ex->lpVtbl->CreateDeviceEx = createDeviceEx;

				// 書き込み属性元に戻す
				VirtualProtect(reinterpret_cast<void *>(direct3d9ex->lpVtbl), sizeof(direct3d9ex->lpVtbl), old_protect, &old_protect);

				*ppD3D = direct3d9ex;
				return S_OK;
			}
		}
		return E_ABORT;
	}

} // extern "C"

bool d3d9_initialize()
{
	// MMDフルパスの取得.
	{
		wchar_t app_full_path[1024];
		GetModuleFileName(NULL, app_full_path, sizeof(app_full_path) / sizeof(wchar_t));
		std::wstring path(app_full_path);
		BridgeParameter::mutable_instance().base_path = path.substr(0, path.rfind(_T("MikuMikuDance.exe")));
	}

	reload_python_file_paths();
	relaod_python_script();

	// システムパス保存用
	TCHAR system_path_buffer[1024];
	GetSystemDirectory(system_path_buffer, MAX_PATH );
	std::wstring d3d9_path(system_path_buffer);
	d3d9_path.append(_T("\\D3D9.DLL"));
	// オリジナルのD3D9.DLLのモジュール
	HMODULE d3d9_module(LoadLibrary(d3d9_path.c_str()));

	if (!d3d9_module) {
		return FALSE;
	}

	// オリジナルDirect3DCreate9の関数ポインタを取得
	original_direct3d_create = reinterpret_cast<IDirect3D9 *(WINAPI*)(UINT)>(GetProcAddress(d3d9_module, "Direct3DCreate9"));
	if (!original_direct3d_create) {
		return FALSE;
	}
	original_direct3d9ex_create = reinterpret_cast<HRESULT(WINAPI*)(UINT, IDirect3D9Ex**)>(GetProcAddress(d3d9_module, "Direct3DCreate9Ex"));
	if (!original_direct3d9ex_create) {
		return FALSE;
	}

	return TRUE;
}
	
void d3d9_dispose() 
{
	renderData.dispose();
	DisposeAlembic();
}

// DLLエントリポイント
BOOL APIENTRY DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
	switch (reason) 
	{
		case DLL_PROCESS_ATTACH:
			hInstance=hinst;
			d3d9_initialize();
			break;
		case DLL_PROCESS_DETACH:
			d3d9_dispose();
			break;
	}
	return TRUE;
}

