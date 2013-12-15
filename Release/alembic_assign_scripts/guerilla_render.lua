Mtl = {
	name = '',
	textureMap = '',
	alphaMap = '',
	diffuse = {0.7, 0.7, 0.7},
	specular = {0.7, 0.7, 0.7},
	ambient = {0.7, 0.7, 0.7},
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

function import_mtl(path, result)
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
				result[current.name] = current
			end
			current = table.copy(Mtl)
			current.name = words[2]
		end
		if words[1]:find("Ka") then
			current.ambient[1] = tonumber(words[2])
			current.ambient[2] = tonumber(words[3])
			current.ambient[3] = tonumber(words[4])
		elseif words[1]:find("Kd") then
			current.diffuse[1] = tonumber(words[2])
			current.diffuse[2] = tonumber(words[3])
			current.diffuse[3] = tonumber(words[4])
		elseif words[1]:find("Ks") then
			current.specular[1] = tonumber(words[2])
			current.specular[2] = tonumber(words[3])
			current.specular[3] = tonumber(words[4])
		elseif words[1]:find("Ns") then
			current.power = tonumber(words[2])
		elseif words[1]:find("d") then
			current.trans = tonumber(words[2])
		elseif words[1]:find("map_Kd") then
			current.textureMap = words[2]
		elseif words[1]:find("map_d") then
			current.alphaMap = words[2]
		elseif words[1]:find("#") then
			if words[2]:find("is_accessory") then
				current.isAccessory = true
			end
		end
	end

	if current then
		result[current.name] = current
	end

	file:close()
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

function assing_material(node, name, mtl)
	if node == nil or mtl == nil then
		return
	end
	-- TODO: create material, assign to primitive, set material shader parameters
end

function assing_mtl(mtl_data)
	for node in children (Document, "Primitive", nil, true) do
		mat_name = to_material_name(node:getname())
		print (mat_name)
		for k, v in pairs(mtl_data) do
			if k == mat_name then
				assing_material(node, k, v)
			end
		end
	end
end

function import_mmdbridge_mtl()
	mtl_path = to_mtl_path(get_reference_abc_path())
	print(mtl_path)
	mtl_data = {}
	import_mtl(mtl_path, mtl_data)

	-- Start a document modification
	local modifier = Document:modify ()
	assing_mtl(mtl_data)
	modifier.finish ()
end

import_mmdbridge_mtl()
