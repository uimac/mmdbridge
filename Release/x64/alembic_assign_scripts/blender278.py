# author: tori31001 at gmail.com
# website: http://mmdbridge.render.jp

bl_info = {
	"name": "MMDBridge Material Import",
	"author": "Kazuma Hatta",
	"version": (0, 0, 1),
	"blender": (2, 7, 7),
	"location": "File > Import > mmdbmaterial",
	"description": "Import MMDBridge Materials(.mtl)",
	"warning": "",
	"wiki_url": "",
	"tracker_url": "",
	"category": "Import-Export"}

import os
import sys
import bpy

from bpy.props import (StringProperty,\
						BoolProperty,\
						FloatProperty,\
						EnumProperty,\
						)

from bpy_extras.io_utils import (ImportHelper,\
								ExportHelper,\
								path_reference_mode\
								)

class Mtl():
	def __init__(self):
		self.name = ""	
		self.textureMap = ""
		self.alphaMap = ""
		self.diffuse = [0.7, 0.7, 0.7]
		self.specular = [0.0, 0.0, 0.0]
		self.ambient = [0.0, 0.0, 0.0]
		self.trans = 1.0
		self.power = 0.0
		self.lum = 1
		self.faceSize = 0
		self.isAccessory = False

def import_mtl(path, result, relation):
	
	current = None
	
	export_mode = 0
	
	mtl = open(path, 'r', encoding = "utf-8")
	for line in mtl.readlines():
		words = line.split()
		if len(words) < 2:
			continue
		if "newmtl" in words[0]:
			# save previous mtl
			if current != None and current.name != "":
				# save previous mtl
				result[current.name] = current
			# new mtl
			current = Mtl()
			current.name = str(words[1])		   
			
			# object relations
			nameSplits = current.name.split("_")
			objectNumber = int(nameSplits[1])
			materialNumber = int(nameSplits[2])
			if not objectNumber in relation.keys():
				relation[objectNumber] = []
			
			relation[objectNumber].append(materialNumber)
			
		if "Ka" == words[0]:
			current.ambient[0] = float(words[1])
			current.ambient[1] = float(words[2])
			current.ambient[2] = float(words[3])
		elif "Kd" == words[0]:
			current.diffuse[0] = float(words[1])
			current.diffuse[1] = float(words[2])
			current.diffuse[2] = float(words[3])
		elif "Ks" == words[0]:
			current.specular[0] = float(words[1])
			current.specular[1] = float(words[2])
			current.specular[2] = float(words[3])
		elif "Ns" == words[0]:
			current.power = float(words[1])
		elif "d" == words[0]:
			current.trans = float(words[1])
		elif "map_Kd" == words[0]:
			current.textureMap = line[line.find(words[1]):line.find(".png")+4]
		elif "map_d" == words[0]:
			current.alphaMap = line[line.find(words[1]):line.find(".png")+4]
		elif "#" == words[0]:
			if words[1] == "face_size":
				current.faceSize = int(words[2])
			elif words[1] == "is_accessory":
				current.isAccessory = True
			elif words[1] == "mode":
				export_mode = int(words[2])
	mtl.close()
		
	if current != None and current.name != "":
		result[current.name] = current

	for rel in relation.values():
		rel.sort()
		
	return export_mode

def assign_material(base_path, obj, mesh, mtlmat, image_dict):
	mat = bpy.data.materials.new(mtlmat.name)
	mesh.materials.append(mat)
	mat.diffuse_color = mtlmat.diffuse
	mat.specular_color = mtlmat.specular
	mat.specular_intensity = mtlmat.power
	mat.transparency_method = 'RAYTRACE'
	mat.use_transparency = True
	if mtlmat.textureMap != "":
		mat.alpha = 0.0
		tex = mat.texture_slots.create(0)
		tex.texture = bpy.data.textures.new("diffuse", type='IMAGE')
		tex.texture_coords = 'UV'
		tex.use = True
		tex.use_map_color_diffuse = True
		tex.use_map_alpha = True
		texture_file_path = ""
		if os.path.exists(mtlmat.textureMap):
			texture_file_path = mtlmat.textureMap
		else:
			texture_file_path = os.path.normpath(bpy.path.abspath(os.path.join(base_path, mtlmat.textureMap)))

		if texture_file_path in image_dict:
			tex.texture.image = image_dict[texture_file_path]
		else:
			if os.path.exists(texture_file_path):
				tex.texture.image = bpy.data.images.load(texture_file_path)
				image_dict[texture_file_path] = tex.texture.image

	for poly in mesh.polygons:
		poly.use_smooth = True

def import_mmdbridge_material(filepath, context):
	image_dict = {}
	mtlDict = {}
	relationDict = {}
	import_mtl(filepath, mtlDict, relationDict)
	print(mtlDict, relationDict)
	base_path, file_name = os.path.split(filepath)
	for key in mtlDict:
		for obj in bpy.data.objects:
			if obj.type == "MESH" and obj.data != None:
				name = obj.data.name.replace('material_', '')
				name = name.replace('mesh_', 'material_')
				if key == name:
					assign_material(base_path, obj, obj.data, mtlDict[key], image_dict)

class MMDBridgeImportOperator(bpy.types.Operator, ImportHelper):
	bl_idname = "import_scene.mmdbridge_material"
	bl_label = "MMDBridge Material Importer(.mtl)"
	
	filename_ext = ".mtl"
	fliter_glob = bpy.props.StringProperty(default="*.mtl")

	filepath = bpy.props.StringProperty(subtype="FILE_PATH")
		
	path_mode = path_reference_mode

	def execute(self, context):
		import_mmdbridge_material(self.filepath, bpy.context)
		return {'FINISHED'}

	def invoke(self, context, event):
		context.window_manager.fileselect_add(self)
		return {'RUNNING_MODAL'}

#
# Registration
#
def menu_func_import(self, context):
	self.layout.operator(MMDBridgeImportOperator.bl_idname, text="MMDBridge Material (.mtl)")

def register():
	bpy.utils.register_module(__name__)
	bpy.types.INFO_MT_file_import.append(menu_func_import)

def unregister():
	bpy.utils.unregister_module(__name__)
	bpy.types.INFO_MT_file_import.remove(menu_func_import)

if __name__ == "__main__":
	register()