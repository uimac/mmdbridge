#include "bridge_parameter.h"

BridgeParameter BridgeParameter::parameter;

const RenderedBuffer& BridgeParameter::render_buffer(int finish_buffer_index) const
{
	static RenderedBuffer none;
	if (0 <= finish_buffer_index && finish_buffer_index < finish_buffer_list.size())
	{
		if (IDirect3DVertexBuffer9* buffer = finish_buffer_list.at(finish_buffer_index))
		{
			RenderBufferMap::const_iterator it = render_buffer_map.find(buffer);
			if (it != render_buffer_map.end())
			{
				return it->second;
			}
		}
	}
	return none;
}
