#ifdef WITH_PMX

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

#include <EncodingHelper.h>
#include <Pmd.h>
#include <Pmx.h>
#include <Vmd.h>

#include "MMDExport.h"

typedef std::shared_ptr<pmd::PmdModel> PMDPtr;
typedef std::shared_ptr<pmx::PmxModel> PMXPtr;
typedef std::shared_ptr<vmd::VmdMotion> VMDPtr;
typedef std::shared_ptr<pmx::PmxMorph> PmxMorphPtr;

class FileData {
public:
	PMXPtr pmx;
	PMDPtr pmd;
	VMDPtr vmd;
	std::map<int, int> parent_index_map;
	std::map<int, std::string> bone_name_map;
	std::map<int, int> physics_bone_map;
	std::map<int, int> ik_bone_map;
};

class PMXArchive {
public:
	
	static PMXArchive& instance() {
		static PMXArchive instance;
		return instance; 
	}

	FileData file_data;
	std::string output_path;
	std::string model_name;
	RenderedBuffer::VertexList base_vertex_list;

	std::vector<PmxMorphPtr> morph_list;

	void end()
	{
		output_path.clear();
	}
	~PMXArchive() {}
private:
	PMXArchive() {}
};

static bool start_pmx_export(const std::string& directory_path, const std::string& model_name)
{
	PMXArchive &archive = PMXArchive::instance();
	
	const BridgeParameter& parameter = BridgeParameter::instance();
	if (parameter.export_fps <= 0)
	{
		return false;
	}

	archive.model_name = model_name;

	if (directory_path.empty())
	{
		archive.output_path = umbase::UMStringUtil::wstring_to_utf8(parameter.base_path) + ("out\\");
	}
	else
	{
		archive.output_path = directory_path;
	}
	return true;
}

static bool end_pmx_export()
{
	PMXArchive &archive = PMXArchive::instance();
	const BridgeParameter& parameter = BridgeParameter::instance();
	oguna::EncodingConverter converter;

	umstring filename = umbase::UMStringUtil::utf8_to_utf16(archive.model_name + ".pmx");
	umstring output_filepath = umbase::UMStringUtil::utf8_to_utf16(archive.output_path) + filename;

	PMXPtr pmx = archive.file_data.pmx;
	converter.Utf8ToUtf16(archive.model_name.c_str(), archive.model_name.size(), &pmx->model_name);
	pmx->morph_count = static_cast<int>(archive.morph_list.size());
	pmx->morphs = std::make_unique<pmx::PmxMorph[]>(pmx->morph_count);

	archive.file_data.vmd = std::make_unique<vmd::VmdMotion>();
	VMDPtr vmd = archive.file_data.vmd;
	vmd->model_name = archive.model_name;

	int frame_number = parameter.start_frame;
	for (int i = 0; i < pmx->morph_count; ++i)
	{
		// pmx morph
		PmxMorphPtr mo = archive.morph_list[i];
		pmx->morphs[i].morph_name = mo->morph_name;
		pmx->morphs[i].morph_type = mo->morph_type;
		pmx->morphs[i].category = mo->category;
		pmx->morphs[i].offset_count = mo->offset_count;
		pmx->morphs[i].vertex_offsets = std::make_unique<pmx::PmxMorphVertexOffset[]>(mo->offset_count);
		for (int k = 0; k < mo->offset_count; ++k)
		{
			pmx->morphs[i].vertex_offsets[k] = mo->vertex_offsets[k];
		}

		// vmd
		{
			vmd::VmdFaceFrame frame;
			converter.Utf16ToCp932(mo->morph_name.c_str(), mo->morph_name.size(), &frame.face_name);

			if (frame_number > 0)
			{
				frame.frame = frame_number - 1;
				frame.weight = 0.0f;
				vmd->face_frames.push_back(frame);
			}

			frame.frame = frame_number;
			frame.weight = 1.0f;
			vmd->face_frames.push_back(frame);

			frame.frame = frame_number + 1;
			frame.weight = 0.0f;
			vmd->face_frames.push_back(frame);
		}
		++frame_number;
	}
	std::ofstream stream(output_filepath.c_str(), std::ios::binary);
	archive.file_data.pmx->Write(&stream);
	stream.close();
	
	umstring vmd_filename = umbase::UMStringUtil::utf8_to_utf16(archive.model_name + ".vmd");
	umstring vmd_output_filepath = umbase::UMStringUtil::utf8_to_utf16(archive.output_path) + vmd_filename;
	vmd->SaveToFile(vmd_output_filepath.c_str());

	PMXArchive::instance().end();
	return true;
}

static void export_pmx(int currentframe, bool isfirst)
{
	PMXArchive &archive = PMXArchive::instance();
	const BridgeParameter& parameter = BridgeParameter::instance();
	const VertexBufferList& finishBuffers = BridgeParameter::instance().finish_buffer_list;
	if (isfirst)
	{
		archive.file_data.pmx = std::make_shared<pmx::PmxModel>();
		PMXPtr pmx = archive.file_data.pmx;
		pmx->Init();

		int vertex_count = 0;
		int index_count = 0;
		int material_count = 0;
		typedef std::map<std::string, int> TextureMap;
		TextureMap texture_map;
		for (int i = 0, isize = static_cast<int>(finishBuffers.size()); i < isize; ++i)
		{
			const RenderedBuffer &renderedBuffer = parameter.render_buffer(i);
			vertex_count += renderedBuffer.vertecies.size();
			const int material_size = static_cast<int>(renderedBuffer.materials.size());
			material_count += material_size;
			for (int k = 0; k < material_size; ++k)
			{
				RenderedMaterial* material = renderedBuffer.materials.at(k);
				index_count += static_cast<int>(material->surface.faces.size() * 3);
				if (texture_map.find(material->memoryTexture) == texture_map.end())
				{
					int index = texture_map.size();
					texture_map[material->memoryTexture] = index;
				}
			}
		}

		pmx->vertex_count = vertex_count;
		pmx->vertices = std::make_unique<pmx::PmxVertex[]>(vertex_count);
		pmx->index_count = index_count;
		pmx->indices = std::make_unique<int[]>(index_count);
		pmx->material_count = material_count;
		pmx->materials = std::make_unique<pmx::PmxMaterial[]>(material_count);
		pmx->bone_count = 1;
		pmx->bones = std::make_unique<pmx::PmxBone[]>(1);
		pmx->texture_count = texture_map.size();
		pmx->textures = std::make_unique<std::wstring[]>(pmx->texture_count);
		int i = 0;
		for (TextureMap::iterator it = texture_map.begin(); it != texture_map.end(); ++it, ++i)
		{
			std::wstring&  tex = pmx->textures[it->second];
			tex.swap(
				umbase::UMStringUtil::utf16_to_wstring(
				umbase::UMStringUtil::utf8_to_utf16(it->first + ".png")));
		}

		pmx->setting.bone_index_size = 1;
		pmx->setting.material_index_size = 4;
		pmx->setting.morph_index_size = 4;
		pmx->setting.rigidbody_index_size = 4;
		pmx->setting.texture_index_size = 4;
		pmx->setting.vertex_index_size = 4;
		pmx->setting.uv = 0;
		pmx->setting.encoding = 0;

		pmx::PmxBone& center = pmx->bones[0];
		center.bone_name = std::wstring(L"センター");
		center.parent_index = -1;
		center.bone_flag = (0x0001 | 0x0002 | 0x0004 | 0x0008 | 0x0010);
		center.offset[1] = 1;

		int vertex_offset = 0;
		int index_offset = 0;
		int material_offset = 0;
		for (int i = 0, isize = static_cast<int>(finishBuffers.size()); i < isize; ++i)
		{
			const RenderedBuffer &renderedBuffer = parameter.render_buffer(i);
			const RenderedBuffer::VertexList& vertexList = renderedBuffer.vertecies;
			const RenderedBuffer::NormalList& normalList = renderedBuffer.normals;
			const RenderedBuffer::UVList &uvList = renderedBuffer.uvs;
			const int material_size = static_cast<int>(renderedBuffer.materials.size());
			for (int k = 0; k < material_size; ++k)
			{
				RenderedMaterial* material = renderedBuffer.materials.at(k);
				const int materialSurfaceSize = static_cast<int>(material->surface.faces.size());
				if (materialSurfaceSize <= 0) continue;
				pmx::PmxMaterial& mat = pmx->materials[material_offset + k];

				mat.diffuse_texture_index = texture_map[material->memoryTexture];
				mat.common_toon_flag = 1;
				mat.sphere_texture_index = -1;
				mat.toon_texture_index = 4;

				mat.index_count = materialSurfaceSize * 3;
				mat.material_name = 
					std::wstring(L"buffer_") + umbase::UMStringUtil::number_to_wstring(i)
					 + std::wstring(L"_mat_") + umbase::UMStringUtil::number_to_wstring(k);
				for (int n = 0; n < 4; ++n) {
					mat.diffuse[n] = 1.0f;
				}
				for (int n = 0; n < 3; ++n) {
					mat.specular[n] = material->specular[n];
					mat.ambient[n] = 1.0f;
				}

				for (int n = 0; n < materialSurfaceSize; ++n)
				{
					UMVec3i face = material->surface.faces[n];
					const int vi[] = {
						face.x - 1,
						face.y - 1,
						face.z - 1
					};

					for (int m = 0; m < 3; ++m) {
						pmx::PmxVertex& v = pmx->vertices[vertex_offset + vi[m]];
						pmx->indices[index_offset + m] = vertex_offset + vi[m];

						v.positon[0] = vertexList[vi[m]][0];
						v.positon[1] = vertexList[vi[m]][1];
						v.positon[2] = vertexList[vi[m]][2];
						if (!normalList.empty())
						{
							v.normal[0] = normalList[vi[m]][0];
							v.normal[1] = normalList[vi[m]][1];
							v.normal[2] = normalList[vi[m]][2];
						}
						if (!uvList.empty())
						{
							v.uv[0] = uvList[vi[m]].x;
							v.uv[1] = uvList[vi[m]].y;
						}
						v.skinning = std::make_unique<pmx::PmxVertexSkinningBDEF1>();
						v.skinning_type = pmx::PmxVertexSkinningType::BDEF1;
					}

					index_offset += 3;
				}
			}
			material_offset += material_size;
			vertex_offset += static_cast<int>(vertexList.size());
		}
	}

	{
		PMXPtr pmx = archive.file_data.pmx;
		PmxMorphPtr morph = PmxMorphPtr(new pmx::PmxMorph);
		const int vertex_count = static_cast<int>(archive.base_vertex_list.size());
		morph->morph_type = pmx::MorphType::Vertex;
		morph->category = pmx::MorphCategory::Other;
		morph->offset_count = vertex_count;
		morph->vertex_offsets = std::make_unique<pmx::PmxMorphVertexOffset []>(vertex_count);

		int vertex_offset = 0;
		for (int i = 0, isize = static_cast<int>(finishBuffers.size()); i < isize; ++i)
		{
			const RenderedBuffer &renderedBuffer = parameter.render_buffer(i);
			const RenderedBuffer::VertexList& vertexList = renderedBuffer.vertecies;
			for (int k = 0; k < vertexList.size(); ++k)
			{
				const int index = vertex_offset + k;
				UMVec3f v(
					vertexList[k][0],
					vertexList[k][1],
					vertexList[k][2]);

				UMVec3f base(
					archive.base_vertex_list[index][0],
					archive.base_vertex_list[index][1],
					archive.base_vertex_list[index][2]);

				UMVec3f sub = v - base;
				morph->vertex_offsets[index].position_offset[0] = sub[0];
				morph->vertex_offsets[index].position_offset[1] = sub[1];
				morph->vertex_offsets[index].position_offset[2] = sub[2];
				morph->vertex_offsets[index].vertex_index = index;
			}
			vertex_offset += static_cast<int>(vertexList.size());
		}
		morph->morph_name =
			std::wstring(L"frame_") + umbase::UMStringUtil::number_to_wstring(currentframe);
		archive.morph_list.push_back(morph);
	}
}

static bool execute_pmx_export(int currentframe)
{
	PMXArchive &archive = PMXArchive::instance();

	const BridgeParameter& parameter = BridgeParameter::instance();
	const VertexBufferList& finishBuffers = BridgeParameter::instance().finish_buffer_list;

	if (currentframe == parameter.start_frame)
	{
		int vertex_size = 0;
		for (int i = 0, isize = static_cast<int>(finishBuffers.size()); i < isize; ++i)
		{
			const RenderedBuffer &renderedBuffer = parameter.render_buffer(i);
			vertex_size += renderedBuffer.vertecies.size();
		}
		for (int i = 0, isize = static_cast<int>(finishBuffers.size()); i < isize; ++i)
		{
			const RenderedBuffer &renderedBuffer = parameter.render_buffer(i);
			archive.base_vertex_list.insert(
				archive.base_vertex_list.end(),
				renderedBuffer.vertecies.begin(),
				renderedBuffer.vertecies.end());
		}
		export_pmx(currentframe, true);
	}
	else
	{
		export_pmx(currentframe, false);
	}

	return true;
}

// ---------------------------------------------------------------------------
BOOST_PYTHON_MODULE( mmdbridge_pmx )
{
	using namespace boost::python;
	def("start_pmx_export", start_pmx_export);
	def("end_pmx_export", end_pmx_export);
	def("execute_pmx_export", execute_pmx_export);
}

#endif //WITH_PMX

// ---------------------------------------------------------------------------
#ifdef WITH_PMX
void InitPMX()
	{
		PyImport_AppendInittab("mmdbridge_pmx", PyInit_mmdbridge_pmx);
	}
	void DisposePMX() 
	{
		PMXArchive::instance().end();
	}
#else
	void InitPMX(){}
	void DisposePMX() {}
#endif //WITH_PMXD
