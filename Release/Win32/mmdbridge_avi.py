import mmdbridge
from mmdbridge import *
import mmdbridge_avi
from mmdbridge_avi import *
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
	messagebox("avi export started")
	start_avi_export("", export_mode)

if (framenumber >= start_frame or framenumber <= end_frame):
	execute_avi_export(framenumber)

if (framenumber == end_frame):
	messagebox("avi export ended at " + str(framenumber))
	end_avi_export()
