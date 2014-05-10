#pragma once

#include <string>
#include <vector>
#include <map>

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

	/// ベースディレクトリ
	std::wstring base_path;
	/// pythonスクリプト
	std::string mmdbridge_python_script;
	/// スクリプトパス
	std::wstring python_script_path;
	/// スクリプト名
	std ::wstring python_script_name;
	/// スクリプト名
	std::vector<std::wstring> python_script_name_list;
	/// スクリプトパス
	std::vector<std::wstring> python_script_path_list;

	/// スクリプト呼び出し設定
	//int script_call_setting;
	/// 開始フレーム
	int start_frame;
	/// 終了フレーム
	int end_frame;
	/// 出力幅
	int frame_width;
	/// 出力高さ
	int frame_height;
	/// 出力fps
	double export_fps;
	/// テクスチャバッファが有効かどうか.
	bool is_texture_buffer_enabled;

	/// スクリプトからの一時保存値(int)
	std::map<int, int> py_int_map;
	/// スクリプトからの一時保存値(float)
	std::map<int, float> py_float_map;
	
	// TODO
	//VertexBufferList& finishBuffers;
	//std::map<IDirect3DVertexBuffer9*, RenderedBuffer> & renderedBuffers;

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

