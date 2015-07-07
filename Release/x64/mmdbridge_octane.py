import mmdbridge
from mmdbridge import *

import os
import math
from math import *
import time

octanepath = 'C://Program Files/Refractive Software/OctaneRender 1.024 beta2.46b CUDA 3.2/octane.exe'

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
					texname, ext = os.path.splitext(texture)
					if (ext is not "bmp") and (ext is not "png") and (ext is not "tif") and \
							(ext is not "BMP") and (ext is not "PNG") and (ext is not "TIF"):
						export_path = get_base_path() + "tmp\\" + texname + ".png"
						export_texture(buf, mat, export_path)
						mtlfile.write("map_d "+texname + ".png"+"\n")
					else:
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
				objfile.write("vt "+str(uv[0])+" "+str(-uv[1])+"\n") # invert
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
				f1 = str(last_findex + f[1])
				f2 = str(last_findex + f[2])

				# righthand coords
				objfile.write("f "+f1+"/"+f1+"/"+f1+" "+f0+"/"+f0+"/"+f0+" "+f2+"/"+f2+"/"+f2+"\n")
		last_findex = max_findex_in_buf

def execute_octane(outpath, octanepath, objpath, mtlpath, samples):
	at = get_camera_at()
	up = get_camera_up()
	eye = get_camera_eye()
	aspect = get_camera_aspect()

	camera_fov = get_camera_fovy()
	fov = math.degrees(2*atan(tan(camera_fov/2)*(aspect)))

	light = [0.5, 0.5, 0.5]
	if (get_vertex_buffer_size() > 0):
		light = get_light(0)

	win_command_flag='start /b /normal /WAIT \"\" '
	octane = '\"' +octanepath + '\"'
	octane += ' -m \"mmdbridge\"'
	octane += ' -l \"%s\"' % (objpath)
	octane += " -s " + str(samples)
	octane += " --cam-pos-x " + str(eye[0])
	octane += " --cam-pos-y " + str(eye[1])
	octane += " --cam-pos-z " + str(-eye[2])
	octane += " --cam-target-x " + str(at[0])
	octane += " --cam-target-y " + str(at[1])
	octane += " --cam-target-z " + str(-at[2])
	octane += " --cam-up-x " + str(up[0])
	octane += " --cam-up-y " + str(up[1])
	octane += " --cam-up-z " + str(-up[2])
	octane += " --cam-fov " + str(fov)
	octane += " --film-width " + str(get_frame_width())
	octane += " --film-height " + str(get_frame_height())
	if len(light) > 0:
		octane += " --daylight-sundir-x " + str(-light[0])
		octane += " --daylight-sundir-y " + str( 0.3 )
		octane += " --daylight-sundir-z " + str(light[2])
	#octane += " -q"
	octane += " -o " + '\"' + outpath + '%05d' % get_frame_number() + ".png" + '\"' 
	octane += " -e "

	#messagebox(octane)

	os.system(win_command_flag + octane)

tmppath = get_base_path().replace("\\", "/") + "tmp/"
outpath = get_base_path().replace("\\", "/") + "out/"
objpath = tmppath + "out.obj"
mtlpath = tmppath + "out.mtl"
mtlfilename = 'out.mtl'
texture_export_dir = tmppath

export_mtl(mtlpath)
copy_textures(texture_export_dir.replace("/", "\\"))
export_uncopied_textures(texture_export_dir.replace("/", "\\"), "png")
export_obj(objpath, mtlfilename)
execute_octane(outpath, octanepath, objpath, mtlpath, 128)