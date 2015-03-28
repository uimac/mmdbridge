import mmdbridge
from mmdbridge import *
import mmdbridge_abc
from mmdbridge_abc import *
import os
import math
from math import *
import time

# settings
export_normals = True
export_uvs = True
is_use_euler_rotation_for_camera = True
is_use_ogawa = True

# 0 = create buffer every marerials, fixed vertex index for face
# 1 = create buffer every objects, original vertex index for face
# 2 = create buffer every marerials, direct vertex index for face
export_mode = 0

txmakepath = "txmake"

def export_rib(ribpath, framenumber):
	if os.path.isfile(ribpath):
		os.remove(ribpath)

	win_command_flag='start /b /normal /WAIT \"\" '
	ribfile = open(ribpath, 'a')

	eye = get_camera_eye()
	up = get_camera_up()
	aspect = get_camera_aspect()
	camera_fov = get_camera_fovy()
	fov = math.degrees(camera_fov)

	winv = get_world(0)
	axis = [winv[6] - winv[9], winv[8] - winv[2], winv[1] - winv[4]]
	trace = winv[0] + winv[5] + winv[10] + winv[15] - 2
	rot = math.degrees(acos(trace/2.0))

	light = [0.5, 0.5, 0.5]
	if (get_vertex_buffer_size() > 0):
		light = get_light(0)

	ribfile.write("##RenderMan RIB-Structure 1.0\n")
	ribfile.write("Display "+'\"'+str(get_frame_number())+".tif"+'\"'+" \"file\" \"rgb\"\n")
	ribfile.write("Format "+str(get_frame_width())+" "+str(get_frame_height())+" 1\n")
	ribfile.write("Projection \"perspective\" \"fov\" " + '[' +str(fov) + ']' + "\n")
	ribfile.write("PixelSamples 1 1\n")

	ribfile.write("Rotate "+str(rot)+" "+str(axis[0])+" "+str(axis[1])+" "+str(axis[2])+"\n")
	ribfile.write("Translate "+str(-eye[0])+" "+str(-eye[1])+" "+str(-eye[2])+"\n")
	ribfile.write("WorldBegin\n")
	ribfile.write("Attribute \"visibility\"\n")

	ribfile.write("LightSource \"ambientlight\" 1 \"intensity\" [0.3]\n")
	distantlight = "LightSource \"distantlight\" 2 \"string shadowname\" [\"raytrace\"] \"intensity\" [1.5] \"from\" [{0:0.6f} {1:0.6f} {2:0.6f}] \"lightcolor\" [1.0 1.0 1.0]\n"
	ribfile.write(distantlight.format(light[0], light[1] , light[2]))

	for buf in range(get_vertex_buffer_size()):
		w = get_world(buf)

		for mat in range(get_material_size(buf)):
			material_name = "material_" + str(buf) + "_" + str(mat)
			object_name = "xform_"+str(buf) + "_" + "material_" + str(mat)

			ambient = get_ambient(buf, mat)
			diffuse = get_diffuse(buf, mat)
			specular = get_specular(buf, mat)
			emissive = get_emissive(buf, mat)
			power = get_power(buf, mat)
			texture = get_texture(buf, mat)

			ribfile.write("AttributeBegin\n")
			ribfile.write("Attribute \"identifier\" \"name\" \"" + material_name +"\"\n")
			

			if len(texture) > 0:	
				texname, ext = os.path.splitext(texture)

				# outtex = get_base_path() + "tmp\\" + texname + ".tif" #3Delight
				outtex = get_base_path() + "tmp\\" + texname + ".ltx" #lucille

				if not (os.path.isfile(outtex)):
					intex = get_base_path() + "tmp\\" + texture

					"""
					#3Delight
					if (ext is not "tif") or (ext is not "jpg") or (ext is not "gif") or (ext is not "tga") or (ext is not "psd"):
						export_path = get_base_path() + "tmp\\" + texname + ".tga"
						if export_texture(buf, mat, export_path):
							intex = export_path

					outtex = get_base_path() + "tmp\\" + texname + ".tif"
					tdlmake = '\"' + tdlmakepath + '\"' + " " + '\"' + intex + '\"' + " " + '\"' + outtex + '\"'
					os.system(win_command_flag + tdlmake)
					"""

					"""
					#lucille
					if (ext is not "jpg") or (ext is not "png") or (ext is not "hdr"):
						export_path = get_base_path() + "tmp\\" + texname + ".png"
						if export_texture(buf, mat, export_path):
							intex = export_path

					ltxmake = '\"' + ltxmakepath + '\"' + " " + '\"' + intex + '\"' + " " + '\"' + outtex + '\"'
					os.system(win_command_flag + ltxmake)
					"""

					#renderman
					txmake = '\"' + txmakepath + '\"' + " " + '\"' + intex + '\"' + " " + '\"' + outtex + '\"'
					os.system(win_command_flag + txmake)
				ribfile.write("Surface \"paintedplastic\" " + '\"'+"texturename"+'\"'+" "+'\"'+texname + ".tex"+'\"'+"\n")
			else:
				ribfile.write("Surface \"paintedplastic\"\n" )
			
			ribfile.write("\"Ka\" [ 0.5 ]\n")
			ribfile.write("\"Kd\" [ 0.7 ]\n")
			ribfile.write("\"Ks\" [ 0.3 ]\n")
			ribfile.write("Color [ "+str(diffuse[0])+" "+str(diffuse[1])+" "+str(diffuse[2])+" ]"+"\n")

			# calculate bbox
			box_min = [float("inf"), float("inf"), float("inf")]
			box_max = [-float("inf"), -float("inf"), -float("inf")]
			for findex in range(get_face_size(buf, mat)):
				f = get_face(buf, mat, findex)
				v0 = get_vertex(buf, f[0]-1)
				v1 = get_vertex(buf, f[1]-1)
				v2 = get_vertex(buf, f[2]-1)
				for i in range(3):
					box_min[i] = min(v0[i], v1[i], v2[i], box_min[i])
					box_max[i] = min(v0[i], v1[i], v2[i], box_max[i])

			ribfile.write("Procedural \"DynamicLoad\" [ \"" + alembic_dll_path
							+"\" -filename alembic_file.abc -fps 30 -frame " + str(framenumber)\
							+" -objectpath /" + object_name + "\" ] "\
							+"[ "\
							+ str(box_min[0]) +" "+ str(box_max[0] +" "\
							+ str(box_min[1]) +" "+ str(box_max[1] +" "\
							+ str(box_min[2]) +" "+ str(box_max[2] +" ]")

			ribfile.write("AttributeEnd\n")

	ribfile.write("WorldEnd \n\n\n")

def export_mtl(mtlpath, export_mode):
	if os.path.isfile(mtlpath):
		os.remove(mtlpath)

	mtlfile = open(mtlpath, 'a')

	mtlfile.write("# mode "+str(export_mode)+"\n")

	for buf in range(get_vertex_buffer_size()):
		for mat in range(get_material_size(buf)):
			material_name = "material_" + str(buf) + "_" + str(mat)
			mtlfile.write("newmtl "+material_name+"\n")

			if is_accessory(buf):
				mtlfile.write("# is_accessory"+"\n")

			if export_mode == 1:
				face_size = get_face_size(buf, mat)
				mtlfile.write("# face_size "+str(face_size)+"\n")

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
			if diffuse[0] < 0 or diffuse[1] < 0 or diffuse[2] < 0:
				diffuse[0] = 1
				diffuse[1] = 1
				diffuse[2] = 1

			if specular[0] < 0 or specular[1] < 0 or specular[2] < 0:
				specular[0] = 0
				specular[1] = 0
				specular[2] = 0

			mtlfile.write("Kd "+str(diffuse[0])+" "+str(diffuse[1])+" "+str(diffuse[2])+"\n")
			mtlfile.write("Ks "+str(specular[0])+" "+str(specular[1])+" "+str(specular[2])+"\n")
			if (diffuse[3] < 1):
				mtlfile.write("d "+str(diffuse[3])+"\n")				
			mtlfile.write("Ns "+str(power)+"\n")
			#mtlfile.write("Ni 1.33\n")
			# lum = 1 no specular highlights, lum = 2 light normaly
			mtlfile.write("lum 1\n")
			if len(texture) > 0:
				texname, ext = os.path.splitext(texture)
				if (ext is not ".bmp") and (ext is not ".png") and (ext is not ".tif") and \
						(ext is not ".BMP") and (ext is not ".PNG") and (ext is not ".TIF"):
					export_path = get_base_path() + "out\\" + texname + ".png"
					if export_texture(buf, mat, export_path):
						mtlfile.write("map_Kd "+texname + ".png"+"\n")
						if (diffuse[3] < 1):
							mtlfile.write("map_d "+texname + ".png"+"\n")
				else:
					mtlfile.write("map_Kd "+texture+"\n")
					if (diffuse[3] < 1):
						mtlfile.write("map_d "+texture+"\n")


outpath = get_base_path().replace("\\", "/") + "out/"
mtlpath = outpath + "alembic_file" ".mtl"
ribpath = outpath + "alembic_file" ".rib"
texture_export_dir = outpath
start_frame = get_start_frame()
end_frame = get_end_frame()

framenumber = get_frame_number()
if (framenumber == start_frame):
	messagebox("alembic export started")
	export_mtl(mtlpath, export_mode)
	copy_textures(texture_export_dir.replace("/", "\\"))
	start_alembic_export("", export_mode, export_normals, export_uvs, is_use_euler_rotation_for_camera, is_use_ogawa)

if (framenumber >= start_frame or framenumber <= end_frame):
	execute_alembic_export(framenumber)
	export_rib(ribpath, framenumber)

if (framenumber == end_frame):
	messagebox("alembic export ended at " + str(framenumber))
	end_alembic_export()
