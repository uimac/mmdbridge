import unreal
import os
import sys

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
		self.isAccessory = False

def import_mtl(path, result):
	
	current = None
	
	mtl = open(path, 'r', encoding = "utf-8")
	for line in mtl.readlines():
		words = line.split()
		if len(words) < 2:
			continue
		if "newmtl" in words[0]:
			# save previous mtl
			if current != None and current.name != "":
				result[current.name] = current
			# new mtl
			current = Mtl()
			current.name = str(words[1])
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
			if words[1] == "is_accessory":
				current.isAccessory = True

	if current != None and current.name != "":
		result[current.name] = current

	mtl.close()


# get selected abc asset
actors = unreal.EditorLevelLibrary.get_selected_level_actors()
abcActors = unreal.EditorFilterLibrary.by_class(actors, unreal.GeometryCacheAbcFileActor)

if len(abcActors) > 0:
	actor = abcActors[0]
	comp = actor.get_component_by_class(unreal.GeometryCacheAbcFileComponent)
	abcPath = comp.get_editor_property('alembic_file_path').file_path
	abc = os.path.dirname(abcPath)
	docName = os.path.splitext(os.path.basename(abcPath))[0]

	files = os.listdir(abc)

	# find docName.mtl
	mtl = ""
	for file in files:
		if file == docName:
			root, ext = os.path.splitext(file)
			maybeMtl = root + ".mtl"
			if os.path.exists(maybeMtl):
				mtl = os.path.join(abc, maybeMtl)
				print("a", mtl)
				break

	# find first mtl
	if mtl == "":
		for file in files:
			root, ext = os.path.splitext(file)
			if ext == ".mtl":
				mtl = os.path.join(abc, file)
				break

	mtlDict = {}
	import_mtl(mtl, mtlDict)
	print(mtlDict.keys())

	# create Textures folder
	texPath = '/Game/GeometryCacheAbcFile/Textures/' + docName
	if not unreal.EditorAssetLibrary.does_directory_exist(texPath):
		unreal.EditorAssetLibrary.make_directory(texPath)

	registory = unreal.AssetRegistryHelpers.get_asset_registry()
	assetPath = '/Game/GeometryCacheAbcFile/Materials/' + docName

	ME = unreal.MaterialEditingLibrary

	assetTool = unreal.AssetToolsHelpers.get_asset_tools()
	assets = registory.get_assets_by_path(assetPath)

	texturePathList = []
	for asset in assets:
		materialName = str(asset.asset_name)
		materialKey = materialName.replace('material_', '')
		materialKey = materialKey.replace('mesh_', 'material_')

		if materialKey in mtlDict.keys():
			mtlData = mtlDict[materialKey]
			# load texture files as asset
			if len(mtlData.textureMap) > 0:
				texturePath = os.path.join(abc, mtlData.textureMap)
				texturePathList.append(texturePath)

	importedTexList = []
	if len(texturePathList) > 0:
		importData = unreal.AutomatedAssetImportData()
		importData.set_editor_property('destination_path', texPath)
		importData.set_editor_property('filenames', texturePathList)
		importData.set_editor_property('replace_existing', True)
		importedTexList = assetTool.import_assets_automated(importData)


	for asset in assets:
		materialName = str(asset.asset_name)
		materialKey = materialName.replace('material_', '')
		materialKey = materialKey.replace('mesh_', 'material_')
		matInstance = asset.get_asset()
		if materialKey in mtlDict.keys():
			mtlData = mtlDict[materialKey]
			if len(mtlData.textureMap) > 0:

				texturePath = os.path.join(abc, mtlData.textureMap)
				if texturePath in texturePathList:
					texIndex = texturePathList.index(texturePath)
					importedTex = importedTexList[texIndex]
					colorTexNode = ME.get_material_property_input_node(matInstance, unreal.MaterialProperty.MP_BASE_COLOR)
					print(materialName, colorTexNode)
					if importedTex and colorTexNode == None:
						colorTexNode = ME.create_material_expression(matInstance, unreal.MaterialExpressionTextureSample, -350, -200)
						ME.connect_material_property(colorTexNode, "RGBA", unreal.MaterialProperty.MP_BASE_COLOR)

					colorTexNode.texture = importedTex

					if len(mtlData.alphaMap) > 0:
						ME.connect_material_property(colorTexNode, "A", unreal.MaterialProperty.MP_OPACITY_MASK)
						matInstance.set_editor_property('blend_mode', unreal.BlendMode.BLEND_MASKED)
			else:
				colorNode = ME.create_material_expression(matInstance, unreal.MaterialExpressionConstant4Vector, -350, -200)
				col = unreal.LinearColor()
				if mtlData.isAccessory:
					col.set_editor_property("r", pow(mtlData.diffuse[0] + 0.5 * mtlData.ambient[0], 2.2))
					col.set_editor_property("g", pow(mtlData.diffuse[1] + 0.5 * mtlData.ambient[1], 2.2))
					col.set_editor_property("b", pow(mtlData.diffuse[2] + 0.5 * mtlData.ambient[2], 2.2))
					col.set_editor_property("a", mtlData.trans)
				else:
					col.set_editor_property("r", pow(mtlData.diffuse[0], 2.2))
					col.set_editor_property("g", pow(mtlData.diffuse[1], 2.2))
					col.set_editor_property("b", pow(mtlData.diffuse[2], 2.2))
					col.set_editor_property("a", mtlData.trans)
				colorNode.constant = col
				ME.connect_material_property(colorNode, "", unreal.MaterialProperty.MP_BASE_COLOR)

			ME.layout_material_expressions(matInstance)
			ME.recompile_material(matInstance)