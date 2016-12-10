
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
        self.faceSize = 0
        self.isAccessory = False

def import_mtl(path, result, relation):
    
    current = None
    
    export_mode = 0
    
    mtl = open(path, 'r')
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
    

def descendants(node):
    for c in node.Children:
        yield c
        for d in descendants(c):
            yield d

def allNodes():
    return descendants(MaxPlus.Core.GetRootNode())

def assignMaterial(abc, mtlDict, relationDict):
    for n in allNodes():
        name = n.Name
        print(name)
        if 'mesh_' in name and 'material_' in name:
            target = n
            temp = name[name.find('mesh_')+5 : len(name)]
            objectNumber = int(temp[0 : temp.find('_material_')])
            materialNumber = temp[temp.find('_material_')+10 : len(temp)]
            materialName = 'material_' + str(objectNumber) + '_' + str(materialNumber)
            
            if materialName in mtlDict.keys():
                # new material
                mtlData = mtlDict[materialName]
                mat = MaxPlus.Factory.CreateDefaultStdMat()
                mat.Diffuse = MaxPlus.Color(mtlData.diffuse[0], mtlData.diffuse[1], mtlData.diffuse[2])
                mat.Specular = MaxPlus.Color(mtlData.specular[0], mtlData.specular[1], mtlData.specular[2])
                mat.Ambient = MaxPlus.Color(mtlData.ambient[0], mtlData.ambient[1], mtlData.ambient[2])
                mat.Shiness = mtlData.power
                
                if len(mtlData.textureMap) > 0:
                    mat.SetEnableMap(1, True)
                    texturePath = os.path.join(abc, mtlData.textureMap)
                    tex = MaxPlus.Factory.CreateDefaultBitmapTex()
                    tex.SetMapName(texturePath)
                    tex.ActivateTexDisplay(True)
                    submap = MaxPlus.ISubMap._CastFrom(mat)
                    submap.SetSubTexmap(1, tex)
                    mat.SetActiveTexmap(tex)
                    
                target.Material = mat

def execute():
    directory = MaxPlus.Core.EvalMAXScript('getSavePath caption:"Select MMDBridge\'s out directory"')
    if directory.Type == 44:
		return
    abc = os.path.normpath(directory.Get())

    if not os.path.isdir(abc):
        return

    files = os.listdir(abc)
    
    if len(files) <= 0:
        return

    mtl = ""
    # find first mtl
    for file in files:
        root, ext = os.path.splitext(file)
        if ext == ".mtl":
            mtl = os.path.join(abc, file)
            break
    
    if mtl == "":
        return
    
    mtlDict = {}
    relationDict = {}
    import_mtl(mtl, mtlDict, relationDict)
    assignMaterial(abc, mtlDict, relationDict)

execute()
