import mmdbridge
from mmdbridge import *

framenumber = get_frame_number()
start_frame = 1
end_frame = 700

if (framenumber == start_frame):
	messagebox("alembic export started")
	start_alembic_export("")

if (framenumber >= start_frame or framenumber <= end_frame):
	execute_alembic_export(framenumber)

if (framenumber >= end_frame):
	messagebox("alembic export ended at " + str(framenumber))
	end_alembic_export()

