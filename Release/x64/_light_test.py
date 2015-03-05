import mmdbridge
from mmdbridge import *

light = get_light(0)
light_color = get_light_color(0)

light_str = "x:" + str(light[0]) + " y:" + str(light[1]) + " z:" +str(light[2])
messagebox(light_str)

light_color_str = "r:" + str(light_color[0]) + " g:" + str(light_color[1]) + " b:" +str(light_color[2]);
messagebox(light_color_str)
