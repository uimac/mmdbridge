import c4d
from c4d import gui
from c4d import documents
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
        
def main():
    #gui.MessageDialog('Hello World!')
    
    abc = c4d.storage.LoadDialog( \
        c4d.FILESELECTTYPE_ANYTHING, \
        "Select .abc Folder", \
        c4d.FILESELECT_DIRECTORY)

    if not os.path.isdir(abc):
        return
    
    files = os.listdir(abc)
    
    if len(files) <= 0:
        return
    
    doc = documents.GetActiveDocument()
    docName = doc.GetDocumentName()
    
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
    
    if mtl == "":
        return
    
    mtlDict = {}
    import_mtl(mtl, mtlDict)
    
    for obj in doc.GetObjects():
        #gui.MessageDialog(obj.GetName())
        name = obj.GetName()
        if 'xform' in name:
            materialName = name.replace('material_', '')
            materialName = materialName.replace('xform', 'material')
            if materialName in mtlDict.keys():
                # new material    
                mat = c4d.BaseMaterial(c4d.Mmaterial)
                mtlData = mtlDict[materialName]
                # assign texture
                if len(mtlData.textureMap) > 0:
                    texturePath = os.path.join(abc, mtlData.textureMap)
                    shaderTexture = c4d.BaseList2D(c4d.Xbitmap)
                    shaderTexture[c4d.BITMAPSHADER_FILENAME] = texturePath
                    mat[c4d.MATERIAL_COLOR_SHADER] = shaderTexture
                    mat.InsertShader(shaderTexture)
                    print("color map", texturePath)
                if len(mtlData.alphaMap) > 0:
                    mat[c4d.MATERIAL_ALPHA_IMAGEALPHA] = True
                    texturePath = os.path.join(abc, mtlData.alphaMap)
                    shaderTexture = c4d.BaseList2D(c4d.Xbitmap)
                    shaderTexture[c4d.BITMAPSHADER_FILENAME] = texturePath
                    mat[c4d.MATERIAL_ALPHA_SHADER] = shaderTexture
                    mat.InsertShader(shaderTexture)
                    print("alpha map:", texturePath,mtlData.name )
                # assign color
                mat.SetName(mtlData.name)

		if mtlData.isAccessory:
	                mat[c4d.MATERIAL_COLOR_COLOR] = c4d.Vector(\
	                    mtlData.diffuse[0] + 0.5 * mtlData.ambient[0], \
	                    mtlData.diffuse[1] + 0.5 * mtlData.ambient[1], \
	                    mtlData.diffuse[2] + 0.5 * mtlData.ambient[2])
		else:
	                mat[c4d.MATERIAL_COLOR_COLOR] = c4d.Vector(\
	                    mtlData.diffuse[0], \
	                    mtlData.diffuse[1], \
	                    mtlData.diffuse[2])
                    
                mat[c4d.MATERIAL_SPECULAR_COLOR] = c4d.Vector(\
                    mtlData.specular[0], \
                    mtlData.specular[1], \
                    mtlData.specular[2])
                                
                if mtlData.trans < 0.9999:
                    mat[c4d.MATERIAL_USE_ALPHA] = True
                    mat[c4d.MATERIAL_TRANSPARENCY_BRIGHTNESS] = mtlData.trans
                        
                mat[c4d.MATERIAL_SPECULAR_TEXTURESTRENGTH] = mtlData.power
                    
                mat[c4d.MATERIAL_USE_COLOR] = True
                mat[c4d.MATERIAL_USE_SPECULAR] = True
                mat[c4d.MATERIAL_USE_SPECULARCOLOR] = True
                
                mat.Update(True, True)
                    
                # assign material to object
                doc.InsertMaterial(mat)
                doc.SetActiveObject(obj)
                doc.SetActiveMaterial(mat)
                c4d.CallCommand(12169)

if __name__=='__main__':
    main()

