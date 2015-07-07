import mmdbridge
from mmdbridge import *

import os
import math
from math import *
import time

def export_mtl(mtlpath):
	if os.path.isfile(mtlpath):
		os.remove(mtlpath)

	mtlfile = open(mtlpath, 'a')

	for buf in range(get_vertex_buffer_size()):
		for mat in range(get_material_size(buf)):
			material_name = "material_" + str(buf) + "_" + str(mat)
			mtlfile.write("newmtl "+material_name+"\n")

			ambient = get_ambient(buf, mat)
			diffuse = get_diffuse(buf, mat)
			specular = get_specular(buf, mat)
			emissive = get_emissive(buf, mat)
			power = get_power(buf, mat)
			texture = get_texture(buf, mat)
			if len(texture) == 0:
				texture = get_exported_texture(buf, mat)
				if len(texture) > 0:
					texture = texture + ".png"

			mtlfile.write("Ka "+str(ambient[0])+" "+str(ambient[1])+" "+str(ambient[2])+"\n")
			mtlfile.write("Kd "+str(diffuse[0])+" "+str(diffuse[1])+" "+str(diffuse[2])+"\n")
			mtlfile.write("Ks "+str(specular[0])+" "+str(specular[1])+" "+str(specular[2])+"\n")
			if (diffuse[3] < 1):
				mtlfile.write("d "+str(diffuse[3])+"\n")				
			mtlfile.write("Ns "+str(power)+"\n")
			#mtlfile.write("Ni 1.33\n")
			# lum = 1 no specular highlights, lum = 2 light normaly
			mtlfile.write("lum 1\n")
			if len(texture) > 0:
				mtlfile.write("map_Kd "+texture+"\n")
				if (diffuse[3] < 1):
					mtlfile.write("map_d "+texture+"\n")

def export_obj(objpath, material_file_name):
	if os.path.isfile(objpath):
		os.remove(objpath)

	objfile = open(objpath, 'a')

	objfile.write("mtllib "+material_file_name+"\n")

	for buf in range(get_vertex_buffer_size()):
		for vindex in range(get_vertex_size(buf)):
			v = get_vertex(buf, vindex)
			objfile.write("v "+str(v[0])+" "+str(v[1])+" "+str(-v[2])+"\n")

	for buf in range(get_vertex_buffer_size()):
		for nindex in range(get_normal_size(buf)):
			n = get_normal(buf, nindex)
			objfile.write("vn "+str(n[0])+" "+str(n[1])+" "+str(-n[2])+"\n")

	for buf in range(get_vertex_buffer_size()):
		if (get_uv_size(buf) > 0):
			for uvindex in range(get_uv_size(buf)):
				uv = get_uv(buf, uvindex)
				objfile.write("vt "+str(uv[0])+" "+str(1.0 - uv[1])+"\n")
		else:
			for uvindex in range(get_vertex_size(buf)):
				objfile.write("vt 0 0\n")

	last_findex = 0
	for buf in range(get_vertex_buffer_size()):
		max_findex_in_buf = 0
		for mat in range(get_material_size(buf)):
			material_name = "material_" + str(buf) + "_" + str(mat)
			objfile.write("usemtl "+material_name+"\n")
			objfile.write("s "+str(mat+1)+"\n")

			for findex in range(get_face_size(buf, mat)):
				f = get_face(buf, mat, findex)
				if max_findex_in_buf < (last_findex + max(f)):
					max_findex_in_buf = (last_findex + max(f))

				f0 = str(last_findex + f[0])
				f1 = str(last_findex + f[2])
				f2 = str(last_findex + f[1])

				objfile.write("f "+f0+"/"+f0+"/"+f0+" "+f1+"/"+f1+"/"+f1+" "+f2+"/"+f2+"/"+f2+"\n")
		last_findex = max_findex_in_buf

framenumber = '%05d' % get_frame_number()
tmppath = get_base_path().replace("\\", "/") + "tmp/"
outpath = get_base_path().replace("\\", "/") + "out/"
objpath = outpath + framenumber + ".obj"
mtlpath = outpath + framenumber + ".mtl"
mtlfilename = framenumber + ".mtl"
texture_export_dir = outpath

export_mtl(mtlpath)
copy_textures(texture_export_dir.replace("/", "\\"))
export_uncopied_textures(texture_export_dir.replace("/", "\\"), "png")
export_obj(objpath, mtlfilename)
