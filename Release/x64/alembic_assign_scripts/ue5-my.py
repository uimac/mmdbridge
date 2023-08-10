import unreal
from pathlib import Path


class Mtl:
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

    mtl = open(path, "r", encoding="utf-8")
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
            current.textureMap = line[line.find(words[1]) : line.find(".png") + 4]
        elif "map_d" == words[0]:
            current.alphaMap = line[line.find(words[1]) : line.find(".png") + 4]
        elif "#" == words[0]:
            if words[1] == "is_accessory":
                current.isAccessory = True

    if current != None and current.name != "":
        result[current.name] = current

    mtl.close()


def assignTexAndMat(mtlDict, importedTexDict, materialAssets):
    ME = unreal.MaterialEditingLibrary
    for asset in materialAssets:
        materialName = str(asset.asset_name)
        materialKey = materialName.replace("material_", "")
        materialKey = materialKey.replace("mesh_", "material_")
        matInstance = asset.get_asset()
        if materialKey in mtlDict.keys():
            mtlData = mtlDict[materialKey]
            if len(mtlData.textureMap) > 0:
                texKey = mtlData.textureMap.replace(".png", "")
                importedTex = importedTexDict[texKey].get_asset()
                colorTexNode = ME.get_material_property_input_node(matInstance, unreal.MaterialProperty.MP_BASE_COLOR)
                if importedTex and colorTexNode == None:
                    colorTexNode = ME.create_material_expression(
                        matInstance, unreal.MaterialExpressionTextureSample, -350, -200
                    )
                    ME.connect_material_property(colorTexNode, "RGB", unreal.MaterialProperty.MP_BASE_COLOR)
                    ME.connect_material_property(colorTexNode, "A", unreal.MaterialProperty.MP_OPACITY)
                    matInstance.set_editor_property("blend_mode", unreal.BlendMode.BLEND_OPAQUE)
                    matInstance.set_editor_property("shading_model", unreal.MaterialShadingModel.MSM_SUBSURFACE_PROFILE)
                    matInstance.set_editor_property("two_sided", True)

                colorTexNode.texture = importedTex

                if len(mtlData.alphaMap) > 0:
                    ME.connect_material_property(colorTexNode, "A", unreal.MaterialProperty.MP_OPACITY_MASK)
                    matInstance.set_editor_property("blend_mode", unreal.BlendMode.BLEND_MASKED)
            else:
                colorNode = ME.create_material_expression(
                    matInstance, unreal.MaterialExpressionConstant4Vector, -350, -200
                )
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


def generateMatAssetsData(assetsPath, assetRegistry):
    materialAssets = assetRegistry.get_assets(unreal.ARFilter(package_paths=[assetsPath], class_names=["Material"]))
    return materialAssets


def generateTexDict(assetsPath, assetRegistry):
    importedTexArray = assetRegistry.get_assets(unreal.ARFilter(package_paths=[assetsPath], class_names=["Texture2D"]))
    importedTexDict = {}
    for texAssetData in importedTexArray:
        importedTexDict[str(texAssetData.asset_name)] = texAssetData
    return importedTexDict


def generateMtlDict(abcPath: Path):
    mtlPath = abcPath.with_suffix(".mtl")
    mtlDict = {}
    import_mtl(mtlPath, mtlDict)
    print(mtlDict.keys())
    return mtlDict


def getPathToOriginalAbc():
    selectedAssets = unreal.EditorUtilityLibrary.get_selected_assets()
    geometryCache = unreal.EditorFilterLibrary.by_class(selectedAssets, unreal.GeometryCache)[0]
    assetImportData = geometryCache.get_editor_property("asset_import_data")
    sourceAbcPathStr = assetImportData.get_first_filename()

    abcPath = Path(sourceAbcPathStr)
    return abcPath


def main():
    abcPath = getPathToOriginalAbc()
    mtlDict = generateMtlDict(abcPath)

    assetsPath = unreal.EditorUtilityLibrary.get_current_content_browser_path()
    assetRegistry = unreal.AssetRegistryHelpers.get_asset_registry()
    importedTexDict = generateTexDict(assetsPath, assetRegistry)
    materialAssets = generateMatAssetsData(assetsPath, assetRegistry)

    assignTexAndMat(mtlDict, importedTexDict, materialAssets)


main()
