#ifdef WITH_VMD

#include "d3d9.h"
#include "d3dx9.h"

#include "bridge_parameter.h"
#include "UMStringUtil.h"
#include "UMPath.h"
#include "UMMath.h"
#include "UMVector.h"
#include "UMMatrix.h"

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

#include <Imath/ImathMatrix.h>
#include <Imath/ImathQuat.h>

#include <EncodingHelper.h>
#include <Pmd.h>
#include <Pmx.h>
#include <Vmd.h>

#include "MMDExport.h"

template <class T> std::string to_string(T value)
{
	return umbase::UMStringUtil::number_to_string(value);
}

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

typedef std::shared_ptr<pmd::PmdModel> PMDPtr;
typedef std::shared_ptr<pmx::PmxModel> PMXPtr;
typedef std::shared_ptr<vmd::VmdMotion> VMDPtr;

class FileData {
public:
	VMDPtr vmd;
	PMDPtr pmd;
	PMXPtr pmx;
	std::map<int, int> parent_index_map;
	std::map<int, std::string> bone_name_map;
	std::map<int, int> physics_bone_map;
	std::map<int, int> ik_bone_map;
};

class VMDArchive {
public:
	
	static VMDArchive& instance() {
		static VMDArchive instance;
		return instance; 
	}

	std::map<std::string, int> file_path_map;

	std::vector<FileData> data_list;

	std::string output_path;

	int export_mode;

	void end()
	{
		data_list.clear();
		file_path_map.clear();
		output_path.clear();
	}

	~VMDArchive() {
	}
private:
	VMDArchive() {}
};

static umstring to_umpath(const char* path)
{
	const int size = ::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)path, -1, NULL, 0);
	wchar_t* utf16 = new wchar_t[size];
	::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)path, -1, (LPWSTR)utf16, size);
	std::wstring wstr(utf16);
	delete[] utf16;
	return umbase::UMStringUtil::wstring_to_utf16(wstr);
}

static bool start_vmd_export(
	const std::string& directory_path,
	int export_mode)
{
	VMDArchive &archive = VMDArchive::instance();
	BridgeParameter::mutable_instance().is_exporting_without_mesh = true;
	const BridgeParameter& parameter = BridgeParameter::instance();
	if (parameter.export_fps <= 0)
	{
		return false;
	}
	std::string output_path(directory_path);
	if (output_path.empty())
	{
		VMDArchive::instance().output_path = umbase::UMStringUtil::wstring_to_utf8(parameter.base_path) + ("out\\");
	}

	archive.export_mode = export_mode;
	const int pmd_num = ExpGetPmdNum();
	for (int i = 0; i < pmd_num; ++i) {
		const char* filename = ExpGetPmdFilename(i);
		if (archive.file_path_map.find(filename) != archive.file_path_map.end()) {
			continue;
		}
		PMDPtr pmd = pmd::PmdModel::LoadFromFile(filename);
		if (pmd)
		{
			FileData data;
			data.pmd = pmd;
			archive.data_list.push_back(data);
			archive.file_path_map[filename] = archive.data_list.size() - 1;
		}
		else
		{
			PMXPtr pmx = std::make_shared<pmx::PmxModel>();
			std::ifstream stream(filename, std::ios_base::binary);
			if (stream.good())
			{
				pmx->Init();
				pmx->Read(&stream);
				FileData data;
				data.pmx = pmx;
				archive.data_list.push_back(data);
				archive.file_path_map[filename] = archive.data_list.size() - 1;
			}
		}
	}

	return true;
}

static bool end_vmd_export()
{
	VMDArchive &archive = VMDArchive::instance();
	BridgeParameter::mutable_instance().is_exporting_without_mesh = true;
	const BridgeParameter& parameter = BridgeParameter::instance();
	const int pmd_num = ExpGetPmdNum();
	oguna::EncodingConverter converter;

	for (int i = 0; i < pmd_num; ++i)
	{
		const char* filename = ExpGetPmdFilename(i);
		FileData& file_data = archive.data_list.at(archive.file_path_map[filename]);
		if (file_data.vmd)
		{
			std::string dst;
			converter.Cp932ToUtf8(filename, strnlen(filename, 4096), &dst);
			const umstring umstr = umbase::UMStringUtil::utf8_to_utf16(dst);
			 umstring filename = umbase::UMPath::get_file_name(umstr);
			const umstring extension = umbase::UMStringUtil::utf8_to_utf16(".vmd");
			filename.replace(filename.size() - 4, 4, extension);
			umstring output_filepath = umbase::UMStringUtil::utf8_to_utf16(archive.output_path) + filename;
			file_data.vmd->SaveToFile(output_filepath);
		}
	}

	VMDArchive::instance().end();
	return true;
}

static void init_file_data(FileData& data)
{
	if (data.pmd) {
		const std::vector<pmd::PmdBone>& bones = data.pmd->bones;
		const std::vector<pmd::PmdRigidBody>& rigids = data.pmd->rigid_bodies;
		for (int i = 0, isize = static_cast<int>(bones.size()); i < isize; ++i)
		{
			const pmd::PmdBone& bone = bones[i];
			data.parent_index_map[i] = bone.parent_bone_index;
			data.bone_name_map[i] = bone.name;
			if (bone.bone_type == pmd::BoneType::IkEffectable)
			{
				data.ik_bone_map[i] = 1;
			}
		}

		for (int i = 0, isize = static_cast<int>(rigids.size()); i < isize; ++i)
		{
			const pmd::PmdRigidBody& rigid = rigids[i];
			if (rigid.rigid_type != pmd::RigidBodyType::BoneConnected)
			{
				uint16_t bone_index = rigid.related_bone_index;
				if (data.bone_name_map.find(bone_index) != data.bone_name_map.end())
				{
					if (rigid.rigid_type == pmd::RigidBodyType::ConnectedPhysics)
					{
						data.physics_bone_map[bone_index] = 2;
					}
					else
					{
						data.physics_bone_map[bone_index] = 1;
					}
				}
			}
		}
	}
	else if (data.pmx)
	{
		oguna::EncodingConverter converter;
		const int bone_count = data.pmx->bone_count;
		for (int i = 0; i < bone_count; ++i)
		{
			const pmx::PmxBone& bone = data.pmx->bones[i];
			data.parent_index_map[i] = bone.parent_index;
			converter.Utf16ToCp932(bone.bone_name.c_str(), bone.bone_name.length(), &data.bone_name_map[i]);
			for (int k = 0; k < bone.ik_link_count; ++k)
			{
				pmx::PmxIkLink& link = bone.ik_links[k];
				data.ik_bone_map[link.link_target] = 1;
			}
		}
		const int rigid_count = data.pmx->rigid_body_count;
		for (int i = 0; i < rigid_count; ++i)
		{
			const pmx::PmxRigidBody& rigid = data.pmx->rigid_bodies[i];
			if (rigid.physics_calc_type != 0)
			{
				uint16_t bone_index = rigid.target_bone;
				if (data.bone_name_map.find(bone_index) != data.bone_name_map.end())
				{
					if (rigid.physics_calc_type == 2) {
						data.physics_bone_map[bone_index] = 2;
					}
					else
					{
						data.physics_bone_map[bone_index] = 1;
					}
				}
			}
		}
	}
}

static UMMat44d to_ummat(const D3DMATRIX& mat)
{
	UMMat44d ummat;
	for (int n = 0; n < 4; ++n)
	{
		for (int m = 0; m < 4; ++m)
		{
			ummat[n][m] = mat.m[n][m];
		}
	}
	return ummat;
}

// from imath
static UMVec4d extractQuat(const UMMat44d &mat)
{
	UMMat44d rot;

	double tr, s;
	double q[4];
	int    i, j, k;
	UMVec4d   quat;

	int nxt[3] = { 1, 2, 0 };
	tr = mat[0][0] + mat[1][1] + mat[2][2];

	// check the diagonal
	if (tr > 0.0) {
		s = sqrt(tr + double(1.0));
		quat.w = s / double(2.0);
		s = double(0.5) / s;

		quat.x = (mat[1][2] - mat[2][1]) * s;
		quat.y = (mat[2][0] - mat[0][2]) * s;
		quat.z = (mat[0][1] - mat[1][0]) * s;
	}
	else {
		// diagonal is negative
		i = 0;
		if (mat[1][1] > mat[0][0])
			i = 1;
		if (mat[2][2] > mat[i][i])
			i = 2;

		j = nxt[i];
		k = nxt[j];
		s = sqrt((mat[i][i] - (mat[j][j] + mat[k][k])) + double(1.0));

		q[i] = s * double(0.5);
		if (s != double(0.0))
			s = double(0.5) / s;

		q[3] = (mat[j][k] - mat[k][j]) * s;
		q[j] = (mat[i][j] + mat[j][i]) * s;
		q[k] = (mat[i][k] + mat[k][i]) * s;

		quat.x = q[0];
		quat.y = q[1];
		quat.z = q[2];
		quat.w = q[3];
	}

	return quat;
}

static bool execute_vmd_export(int currentframe)
{
	VMDArchive &archive = VMDArchive::instance();
	BridgeParameter::mutable_instance().is_exporting_without_mesh = true;

	const BridgeParameter& parameter = BridgeParameter::instance();

	const int pmd_num = ExpGetPmdNum();
	oguna::EncodingConverter converter;

	if (currentframe == parameter.start_frame)
	{
		for (int i = 0; i < pmd_num; ++i)
		{
			const char* filename = ExpGetPmdFilename(i);
			FileData& file_data = archive.data_list.at(archive.file_path_map[filename]);
			init_file_data(file_data);

			file_data.vmd = std::make_unique<vmd::VmdMotion>();
			if (file_data.pmd) 
			{
				file_data.vmd->model_name = file_data.pmd->header.name;
			}
			else if (file_data.pmx) 
			{
				converter.Utf16ToCp932(file_data.pmx->model_name.c_str(), file_data.pmx->model_name.length(), &file_data.vmd->model_name);
			}
		}
	}

	for (int i = 0; i < pmd_num; ++i)
	{
		const char* filename = ExpGetPmdFilename(i);
		FileData& file_data = archive.data_list.at(archive.file_path_map[filename]);
		const int bone_num = ExpGetPmdBoneNum(i);
		for (int k = 0; k < bone_num; ++k)
		{
			if (k == 96) {
				k = k;
			}
			UMVec3f initial_trans;
			const char* bone_name = ExpGetPmdBoneName(i, k);
			if (file_data.bone_name_map.find(k) == file_data.bone_name_map.end()) {
				continue;
			}

			if (file_data.bone_name_map[k] != bone_name) {
				continue;
			}

			// export mode
			if (file_data.physics_bone_map.find(k) == file_data.physics_bone_map.end()) {
				if (archive.export_mode == 0)
				{
					// physics + ik
					if (file_data.ik_bone_map.find(k) == file_data.ik_bone_map.end()) {
						continue;
					}
				}
				else if (archive.export_mode == 1)
				{
					// physics only
					continue;
				}
				else
				{
					// all (buggy)
				}
			}

			// get initial world position
			if (file_data.pmd)
			{
				pmd::PmdBone& bone = file_data.pmd->bones[k];
				if (bone.bone_type == pmd::BoneType::Invisible)
				{
					continue;
				}
				initial_trans[0] = bone.bone_head_pos[0];
				initial_trans[1] = bone.bone_head_pos[1];
				initial_trans[2] = bone.bone_head_pos[2];
			}
			else if (file_data.pmx)
			{
				pmx::PmxBone& bone = file_data.pmx->bones[k];
				initial_trans[0] = bone.position[0];
				initial_trans[1] = bone.position[1];
				initial_trans[2] = bone.position[2];
			}

			UMMat44d world = to_ummat(ExpGetPmdBoneWorldMat(i, k));
			UMMat44d local = world;
			int parent_index = file_data.parent_index_map[k];
			if (parent_index != 0xFFFF && file_data.parent_index_map.find(parent_index) != file_data.parent_index_map.end()) {
				UMMat44d parent_world = to_ummat(ExpGetPmdBoneWorldMat(i, parent_index));
				local = world * parent_world.inverted();
			}
			local[3][0] = world[3][0] - initial_trans[0];
			local[3][1] = world[3][1] - initial_trans[1];
			local[3][2] = world[3][2] - initial_trans[2];
			
			vmd::VmdBoneFrame bone_frame;
			bone_frame.frame = currentframe;
			bone_frame.name = ExpGetPmdBoneName(i, k);
			bone_frame.position[0] = static_cast<float>(local[3][0]);
			bone_frame.position[1] = static_cast<float>(local[3][1]);
			bone_frame.position[2] = static_cast<float>(local[3][2]);
			local[3][0] = local[3][1] = local[3][2] = 0.0;
			UMVec4d quat = extractQuat(local);
			bone_frame.orientation[0] = static_cast<float>(quat[0]);
			bone_frame.orientation[1] = static_cast<float>(quat[1]);
			bone_frame.orientation[2] = static_cast<float>(quat[2]);
			bone_frame.orientation[3] = static_cast<float>(quat[3]);
			for (int n = 0; n < 4; ++n) {
				for (int m = 0; m < 4; ++m) {
					bone_frame.interpolation[n][0][m] = 20;
					bone_frame.interpolation[n][1][m] = 20;
				}
				for (int m = 0; m < 4; ++m) {
					bone_frame.interpolation[n][2][m] = 107;
					bone_frame.interpolation[n][3][m] = 107;
				}
			}

			// constraints
			if (file_data.pmd)
			{
				pmd::PmdBone& bone = file_data.pmd->bones[k];
				if (bone.bone_type == pmd::BoneType::Rotation)
				{
					bone_frame.position[0] = 0.0f;
					bone_frame.position[1] = 0.0f;
					bone_frame.position[2] = 0.0f;
				}
			}
			else if (file_data.pmx)
			{
				pmx::PmxBone& bone = file_data.pmx->bones[k];
				if (!(bone.bone_flag & 0x0004))
				{
					bone_frame.position[0] = 0.0f;
					bone_frame.position[1] = 0.0f;
					bone_frame.position[2] = 0.0f;
				}
				if (!(bone.bone_flag & 0x0002))
				{
					bone_frame.orientation[0] = 0.0f;
					bone_frame.orientation[1] = 0.0f;
					bone_frame.orientation[2] = 0.0f;
					bone_frame.orientation[3] = 1.0f;
				}
				if (file_data.physics_bone_map.find(k) != file_data.physics_bone_map.end()) {
					if (file_data.physics_bone_map[k] == 2)
					{
						bone_frame.position[0] = 0.0f;
						bone_frame.position[1] = 0.0f;
						bone_frame.position[2] = 0.0f;
					}
				}
			}
			file_data.vmd->bone_frames.push_back(bone_frame);
		}
	}

	return true;
}

// ---------------------------------------------------------------------------
BOOST_PYTHON_MODULE( mmdbridge_vmd )
{
	using namespace boost::python;
	def("start_vmd_export", start_vmd_export);
	def("end_vmd_export", end_vmd_export);
	def("execute_vmd_export", execute_vmd_export);
}

#endif //WITH_VMD

// ---------------------------------------------------------------------------
#ifdef WITH_VMD
void InitVMD()
	{
		PyImport_AppendInittab("mmdbridge_vmd", PyInit_mmdbridge_vmd);
	}
	void DisposeVMD() 
	{
		VMDArchive::instance().end();
	}
#else
	void InitVMD(){}
	void DisposeVMD() {}
#endif //WITH_VMD
