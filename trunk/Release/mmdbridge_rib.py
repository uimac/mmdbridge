import mmdbridge
from mmdbridge import *

import os
import math
from math import *
import time

#tdlmakepath = "C:/Program Files/3Delight/bin/tdlmake.exe"
ltxmakepath = "F:/um/luccile/0.9.1-alpha-windows-x64/lucille-alpha-64bit/bin/ltxmake.exe"

def export_rib(ribpath):
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
				outtex = get_base_path() + "tmp\\" + texname + ".ltx" #liccile

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

					if (ext is not "jpg") or (ext is not "png") or (ext is not "hdr"):
						export_path = get_base_path() + "tmp\\" + texname + ".png"
						if export_texture(buf, mat, export_path):
							intex = export_path

					ltxmake = '\"' + ltxmakepath + '\"' + " " + '\"' + intex + '\"' + " " + '\"' + outtex + '\"'
					os.system(win_command_flag + ltxmake)

				ribfile.write("Surface \"paintedplastic\" " + '\"'+"texturename"+'\"'+" "+'\"'+texname + ".ltx"+'\"'+"\n")
			else:
				ribfile.write("Surface \"paintedplastic\"\n" )
			
			ribfile.write("\"Ka\" [ 0.5 ]\n")
			ribfile.write("\"Kd\" [ 0.7 ]\n")
			ribfile.write("\"Ks\" [ 0.3 ]\n")
			ribfile.write("Color [ "+str(diffuse[0])+" "+str(diffuse[1])+" "+str(diffuse[2])+" ]"+"\n")

			for findex in range(get_face_size(buf, mat)):
				ribfile.write("Polygon ")
				f = get_face(buf, mat, findex)
				v0 = get_vertex(buf, f[0]-1)
				v1 = get_vertex(buf, f[1]-1)
				v2 = get_vertex(buf, f[2]-1)
				verts = "\"P\" [ {0:0.6f} {1:0.6f} {2:0.6f} {3:0.6f} {4:0.6f} {5:0.6f} {6:0.6f} {7:0.6f} {8:0.6f}] "
				ribfile.write(verts.format(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2]))

				n0 = get_normal(buf, f[0]-1)
				n1 = get_normal(buf, f[1]-1)
				n2 = get_normal(buf, f[2]-1)
				normals = "\"N\" [ {0:0.6f} {1:0.6f} {2:0.6f} {3:0.6f} {4:0.6f} {5:0.6f} {6:0.6f} {7:0.6f} {8:0.6f}] "
				ribfile.write(normals.format(n0[0], n0[1], n0[2], n1[0], n1[1], n1[2], n2[0], n2[1], n2[2]))

				uv0 = [0,0]
				uv1 = [0,0]
				uv2 = [0,0]
				if get_uv_size(buf) > 0:
					uv0 = get_uv(buf, f[0]-1)
					uv1 = get_uv(buf, f[1]-1)
					uv2 = get_uv(buf, f[2]-1)
				uvs = "\"st\" [ {0:0.6f} {1:0.6f} {2:0.6f} {3:0.6f} {4:0.6f} {5:0.6f} ] "
				ribfile.write(uvs.format(uv0[0], uv0[1], uv1[0], uv1[1], uv2[0], uv2[1]))

			ribfile.write("AttributeEnd\n")

	ribfile.write("WorldEnd \n")

	messagebox("done")

tmppath = get_base_path().replace("\\", "/") + "tmp/"
outpath = get_base_path().replace("\\", "/") + "out/"
ribpath = tmppath + "out.rib"
texture_export_dir = tmppath
copy_textures(texture_export_dir.replace("/", "\\"))
export_rib(ribpath)

