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
	result = {}
	current = nil
	file = io.open(path, "r")
	for line in file:lines() do
		local words = {}
		for w in line:gmatch("%S+") do
		   table.insert(words, w);
		end
		if words[1]:find("newmtl") then
			--save previous mtl
			if current and current.name ~= "" then
				result[current.name] = table.copy(current)
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
		name = arch.ReferenceFileName
		for k, v in pairs(name) do
			if type(v) == 'string' then
				path = v
			end
		end
	end
	return path
end

function to_mtl_path(path)
	mtl_path = path:sub(1, path:len() - 3)
	return mtl_path .. "mtl"
end

function to_material_name(primitive_name)
	mesh_index = primitive_name:find("mesh_") + 5
	mesh_number = primitive_name:sub(mesh_index, mesh_index)
	material_index = primitive_name:find("material_") + 9
	material_number = primitive_name:sub(material_index,material_index)
	return "material_" .. mesh_number .. "_" .. material_number
end

function assign_plug(modifier, plug_name, plug_parent, plug_node, mtl)
	--print(plug_name)
	if plug_name == "DiffuseColor" then
		if mtl.isAccessory then
			plug_node.Value[4][1] = 1.0 --mtl.diffuse_r + 0.5 * mtl.ambient_r
			plug_node.Value[4][2] = mtl.diffuse_g + 0.5 * mtl.ambient_g
			plug_node.Value[4][3] = mtl.diffuse_b + 0.5 * mtl.ambient_b
		else
			plug_node.Value[4][1] = 1.0 --mtl.diffuse_r
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

function create_material(modifier, mat_name, mtl)
	if mtl == nil then
		return nil
	end
	local material = Document:loadfile("$(LIBRARY)/materials/surface.gmaterial")
	local materialdata = nil
	for i, v in pairs(material) do
		materialdata = v
		for k, w in pairs(v) do
			if type(w) == "table" then
				for n, z in pairs(w) do
					if type(z) == "table" then
						for m, y in pairs(z) do
							if m == "PlugName" then
								name = z.PlugName:get();
								assign_plug(modifier, name, w, z, mtl)
							end
						end
					end
				end
			end
		end
	end
	if materialdata then
		modifier.renamenode(materialdata, mat_name)
	end
	return materialdata
end

function assign_mtl(modifier, mtl_data)
	for node in children (Document, "Primitive", nil, true) do
		mat_name = to_material_name(node:getname())
		print (mat_name)
		for k, v in pairs(mtl_data) do
			if k == mat_name then
				material = create_material(modifier, k, v)
				if material then
					primname = node:getname()
					node:setmaterial(Document:getchild(mat_name))
				end
			end
		end
		--break --for test
	end
end

function import_mmdbridge_mtl()
	mtl_path = to_mtl_path(get_reference_abc_path())
	print(mtl_path)
	local mtl_data = import_mtl(mtl_path)

	-- Start a document modification
	local modifier = Document:modify ()
	assign_mtl(modifier, mtl_data)
	modifier.finish ()
end

import_mmdbridge_mtl()
