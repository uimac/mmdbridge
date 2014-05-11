#pragma once

#include <string>
#include <vector>
#include <map>
#include "d3d9.h"

class BridgeParameter
{
public:

	static const BridgeParameter& instance()
	{
		return parameter;
	}
	static BridgeParameter& mutable_instance()
	{
		return parameter;
	}

	/// �x�[�X�f�B���N�g��
	std::wstring base_path;
	/// python�X�N���v�g
	std::string mmdbridge_python_script;
	/// �X�N���v�g�p�X
	std::wstring python_script_path;
	/// �X�N���v�g��
	std ::wstring python_script_name;
	/// �X�N���v�g��
	std::vector<std::wstring> python_script_name_list;
	/// �X�N���v�g�p�X
	std::vector<std::wstring> python_script_path_list;

	/// �X�N���v�g�Ăяo���ݒ�
	//int script_call_setting;
	/// �J�n�t���[��
	int start_frame;
	/// �I���t���[��
	int end_frame;
	/// �o�͕�
	int frame_width;
	/// �o�͍���
	int frame_height;
	/// �o��fps
	double export_fps;
	/// �e�N�X�`���o�b�t�@���L�����ǂ���.
	bool is_texture_buffer_enabled;

	/// �X�N���v�g����̈ꎞ�ۑ��l(int)
	std::map<int, int> py_int_map;
	/// �X�N���v�g����̈ꎞ�ۑ��l(float)
	std::map<int, float> py_float_map;
	
	VertexBufferList finish_buffer_list;
	RenderBufferMap render_buffer_map;

	const RenderedBuffer& render_buffer(int finish_buffer_index) const;

private:
	BridgeParameter() :
		//script_call_setting(2)
		 start_frame(5)
		, end_frame(100)
		, frame_width(800)
		, frame_height(450)
		, export_fps(30.0)
		, is_texture_buffer_enabled(false) {}
	
	static BridgeParameter parameter;
};

