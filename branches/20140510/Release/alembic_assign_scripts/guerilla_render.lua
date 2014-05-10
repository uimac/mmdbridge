Mtl = {
	name = '',
	textureMap = '',
	alphaMap = '',
	diffuse_r = 0.7,
	diffuse_g = 0.7,
	diffuse_b = 0.7,
	specular_r = 0.7,
	specular_g = 0.7, 
	specular_b = 0.7,
	ambient_r = 0.7,
	ambient_g = 0.7,
	ambient_b = 0.7,
	trans = 1.0,
	power = 0.0,
	lum = 1,
	isAccessory = false
};

function table.copy(t)
  local u = { }
  for k, v in pairs(t) do u[k] = v end
  return setmetatable(u, getmetatable(t))
end

function import_mtl(path)
	local result = {}
	local current = nil
	local file = io.open(path, "r")
	for line in file:lines() do
		local words = {}
		for w in line:gmatch("%S+") do
		   table.insert(words, w);
		end
		if words[1]:find("newmtl") then
			--save previous mtl
			if current and current.name ~= "" then
				result[current.name] = table.copy(current)
				print(current.name)
			end
			current = table.copy(Mtl)
			current.name = words[2]
		end
		if words[1] == "Ka" then
			current.ambient_r = tonumber(words[2])
			current.ambient_g = tonumber(words[3])
			current.ambient_b = tonumber(words[4])
		elseif words[1] == "Kd" then
			current.diffuse_r = tonumber(words[2])
			current.diffuse_g = tonumber(words[3])
			current.diffuse_b = tonumber(words[4])
		elseif words[1] == "Ks" then
			current.specular_r = tonumber(words[2])
			current.specular_g = tonumber(words[3])
			current.specular_b = tonumber(words[4])
		elseif words[1] == "Ns" then
			current.power = tonumber(words[2])
		elseif words[1] == "d" then
			current.trans = tonumber(words[2])
		elseif words[1] == "map_Kd" then
			current.textureMap = words[2]
		elseif words[1] == "map_d" then
			current.alphaMap = words[2]
		elseif words[1] == "#" then
			if words[2] == "is_accessory" then
				current.isAccessory = true
			end
		end
	end

	if current then
		result[current.name] = current
	end

	file:close()
	return result
end

function get_reference_abc_path()
	local path = ''
	for arch in children (Document, "ArchReference", nil, true) do
		local name = arch.ReferenceFileName
		for k, v in pairs(name) do
			if type(v) == 'string' then
				path = v
			end
		end
	end
	return path
end

function to_full_path(filename)
	local abc_path = get_reference_abc_path()
	local directory = string.match(abc_path, "(.-)([^\\/]-%.?([^%.\\/]*))$")
	return directory .. filename
end

function to_material_name(primitive_name)
	local mesh_key = "mesh_"
	local mat_key = "material_"
	local mesh_index = primitive_name:find(mesh_key)
	local mat_index = primitive_name:find(mat_key)
	local mesh_number = primitive_name:sub(mesh_index+mesh_key:len(), mat_index-2)
	local mat_number = primitive_name:sub(mat_index+mat_key:len(), primitive_name:len())
	return "material_" .. mesh_number .. "_" .. mat_number
end

function to_tex_name(mat_name)
	return "tex_" .. mat_name
end

function get_mtl_path()
	local abc_path = get_reference_abc_path()
	local mtl_path = abc_path:sub(1, abc_path:len() - 3)
	return mtl_path .. "mtl"
end

function get_texture_gnode_path()
	return to_full_path("surface_with_tex.gnode")
end

function has_texture(mtl_data)
	return mtl_data.textureMap ~= ''
end

function assign_plug(modifier, plug_name, plug_parent, plug_node, mtl)
	if plug_name == "DiffuseColor" then
		if mtl.isAccessory then
			plug_node.Value[4][1] = mtl.diffuse_r + 0.5 * mtl.ambient_r
			plug_node.Value[4][2] = mtl.diffuse_g + 0.5 * mtl.ambient_g
			plug_node.Value[4][3] = mtl.diffuse_b + 0.5 * mtl.ambient_b
		else
			plug_node.Value[4][1] = mtl.diffuse_r
			plug_node.Value[4][2] = mtl.diffuse_g
			plug_node.Value[4][3] = mtl.diffuse_b
		end
	elseif plug_name == "Spec1Color" then
		plug_node.Value[4][1] = mtl.specular_r
		plug_node.Value[4][2] = mtl.specular_g
		plug_node.Value[4][3] = mtl.specular_b
	elseif plug_name == "DiffuseTranslucenceColor" then
		 if mtl.trans < 0.9999 then
			plug_node.Value[4][1] = mtl.trans
			plug_node.Value[4][2] = mtl.trans
			plug_node.Value[4][3] = mtl.trans
		end
	end
end

function assign_shader(modifier, mtl, mat_name, shader, tex)
	if tex then
		tex.Filename:set(to_full_path(mtl.textureMap))
		local tex_name = to_tex_name(mat_name)
		modifier.renamenode(tex, tex_name)
		for i, v in pairs(shader) do
			if type(v) == "table" then
				for k, w in pairs(v) do
					if k == "DiffuseColor" then	
						w.Shader:set(tex_name)
					end
				end
			end
		end
	end
	shader.Shader:set(mat_name)
	modifier.renamenode(shader, mat_name)
end

function create_material(modifier, mtl, mat_name)
	local material = Document:loadfile("$(LIBRARY)/materials/surface.gmaterial")[1]
	for k, w in pairs(material) do
		if type(w) == "table" then
			for n, z in pairs(w) do
				if type(z) == "table" then
					for m, y in pairs(z) do
						if m == "PlugName" then
							local name = z.PlugName:get();
							assign_plug(modifier, name, w, z, mtl)
						end
					end
				end
			end
		end
	end
	modifier.renamenode(material, mat_name)
	return material
end

function create_trace_to_output_nodes()
	local trace = nil
	for graph in children (Document, "RenderGraph", nil, true) do
		for outnode in children (graph, "RenderGraphNodeOutput", nil, true) do
			-- create new layer in RenderGraph
			local layer = graph:loadfile("$(LIBRARY)/rendergraph/renderlayer.gnode")[1]
			layer.Membership:set("layer:Layer")
			-- create "Set", change to "Lighting"
			local lighting = graph:loadfile("$(LIBRARY)/rendergraph/set.gnode")[1]
			lighting.Membership:set("Lights,Shadows")
			lighting:rename("Lighting")
			-- create "Set", change to "Trace"
			trace = graph:loadfile("$(LIBRARY)/rendergraph/set.gnode")[1]
			trace.Membership:set("All,Diffuse,Reflection,Refraction")
			trace:rename("Trace")
			-- connects
			outnode:getinput("Output"):connect(layer:getoutput("Output"))
			layer:getinput("visible"):connect(lighting:getoutput("Output"))
			lighting:getinput("set"):connect(trace:getoutput("Output"))
			break
		end
	end
	return trace
end

function create_prim_mat_node(modifier, trace, primitive, material, mtl, mat_name, pre_binop)
	local binop = nil
	for graph in children (Document, "RenderGraph", nil, true) do
		for outnode in children (graph, "RenderGraphNodeOutput", nil, true) do
			-- create "Path", change to primitive path
			local prim = graph:loadfile("$(LIBRARY)/rendergraph/path.gnode")[1]
			prim.Path:set('^' .. primitive:getpath() .. "$")
			prim:rename(primitive:getpath())
			-- create "Shader", assing material
			local shader = nil
			if has_texture(mtl) then
				-- create texture node
				local tex = Document:loadfile("$(LIBRARY)/nodes/texture/texture.gnode")[1]
				shader = graph:loadfile(get_texture_gnode_path())[1]
				assign_shader(modifier, mtl, mat_name, shader, tex)
			else
				shader = graph:loadfile("$(LIBRARY)/rendergraph/shader.gnode")[1]
				assign_shader(modifier, mtl, mat_name, shader, nil)
			end
			-- create union Bindop
			binop = graph:loadfile("$(LIBRARY)/rendergraph/binop.gnode")[1]

			-- connects
			if pre_binop then
				pre_binop:getinput("X"):connect(binop:getoutput("Output"))
				binop:getinput("Y"):connect(shader:getoutput("Output"))
				shader:getinput("Input"):connect(prim:getoutput("Output"))
			else
				trace:getinput("set"):connect(binop:getoutput("Output"))
				binop:getinput("Y"):connect(shader:getoutput("Output"))
				shader:getinput("Input"):connect(prim:getoutput("Output"))
			end
			break
		end
	end
	return binop
end

function assign_mtl(modifier, mtl_data)
	primitives = {}
	for primitive in children (Document, "Primitive", nil, true) do
		table.insert(primitives, primitive)
	end
	local trace = create_trace_to_output_nodes();
	local pre_binop = nil
	for i, primitive in pairs(primitives) do
		local mat_name = to_material_name(primitive:getname())
		if mtl_data[mat_name] then
			print(primitive:getname(), mat_name)
			local mtl = mtl_data[mat_name]
			local material = create_material(modifier, mtl, mat_name)
			local binop = create_prim_mat_node(modifier, trace, primitive, material, mtl, mat_name, pre_binop)
			pre_binop = binop
		end
	end
end

function import_mmdbridge_mtl()
	local abc_path = get_reference_abc_path()
	print("abc_path:" .. abc_path)
	local tex_mat_path = get_texture_gnode_path()
	print("tex_mat_path:" .. tex_mat_path)
	local mtl_path = get_mtl_path()
	print("mtl_path:" .. mtl_path)
	local mtl_data = import_mtl(mtl_path)

	-- Start a document modification
	local modifier = Document:modify()
	if has_texture(mtl_data) then
		assign_mtl(modifier, mtl_data)
	else
		assign_mtl(modifier, mtl_data)
	end
	modifier.finish()
end

import_mmdbridge_mtl()

