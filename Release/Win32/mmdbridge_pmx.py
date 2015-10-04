import mmdbridge
from mmdbridge import *
import mmdbridge_pmx
from mmdbridge_pmx import *
import os
import math
from math import *
import time

# export mode
# 0 = physics + ik
# 1 = physics only
# 2 = all (buggy)
export_mode = 0

outpath = get_base_path().replace("\\", "/") + "out/"
texture_export_dir = outpath
start_frame = get_start_frame()
end_frame = get_end_frame()

framenumber = get_frame_number()
if (framenumber == start_frame):
	copy_textures(texture_export_dir.replace("/", "\\"))
	export_uncopied_textures(texture_export_dir.replace("/", "\\"), "png")
	messagebox("pmx export started")
	start_pmx_export("", "pmx_export")

if (framenumber >= start_frame or framenumber <= end_frame):
	execute_pmx_export(framenumber)

if (framenumber == end_frame):
	messagebox("pmx export ended at " + str(framenumber))
	end_pmx_export()
