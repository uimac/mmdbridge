#thread

import mmdbridge
from mmdbridge import *
import sys
import threading

use_separate_thread = True

__command_queue = []
__command_queue_lock = threading.Lock()
__command_queue_event = threading.Event()

__pyqt_thread = None
def queueCommand(callable, arguments=()):
	if use_separate_thread == False:
		callable(*arguments)
		return

	global __pyqt_thread
	if __pyqt_thread is None:
		__pyqt_thread = threading.Thread(target=__pyQtThreadMain)
		__pyqt_thread.start()

	__command_queue_lock.acquire()
	__command_queue.append((callable, arguments))
	__command_queue_lock.release()

	# Signal the PyQt thread to run the task.
	__command_queue_event.set()

def __pyQtThreadMain():
	from PyQt4 import QtCore
	from PyQt4 import QtGui

	while True:
		__command_queue_event.wait()
		__command_queue_lock.acquire()
		command = __command_queue.pop()
		__command_queue_event.clear()
		__command_queue_lock.release()

		# Run the command.
		command[0].__call__(*command[1])

__pyqt_app = None
def getApplication():
	from PyQt4 import QtCore
	from PyQt4 import QtGui

	global __pyqt_app
	if __pyqt_app is None:
		__pyqt_app = QtGui.QApplication(['mmdbridge'])
	return __pyqt_app







def runPyQtTestInThread():

	from PyQt4 import QtCore, QtGui

	try:
		_fromUtf8 = QtCore.QString.fromUtf8
	except AttributeError:
		_fromUtf8 = lambda s: s

	class Ui_MainWindow(object):
		def setupUi(self, MainWindow):
			MainWindow.setObjectName(_fromUtf8("MainWindow"))
			MainWindow.resize(480, 640)
			self.centralwidget = QtGui.QWidget(MainWindow)
			self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
			self.testRenderButton = QtGui.QPushButton(self.centralwidget)
			self.testRenderButton.setGeometry(QtCore.QRect(160, 570, 75, 23))
			self.testRenderButton.setObjectName(_fromUtf8("testRenderButton"))
			self.materialWidget = QtGui.QListWidget(self.centralwidget)
			self.materialWidget.setGeometry(QtCore.QRect(20, 10, 211, 531))
			self.materialWidget.setObjectName(_fromUtf8("materialWidget"))
			self.okButton = QtGui.QPushButton(self.centralwidget)
			self.okButton.setGeometry(QtCore.QRect(380, 570, 75, 23))
			self.okButton.setObjectName(_fromUtf8("okButton"))
			MainWindow.setCentralWidget(self.centralwidget)
			self.menubar = QtGui.QMenuBar(MainWindow)
			self.menubar.setGeometry(QtCore.QRect(0, 0, 480, 24))
			self.menubar.setObjectName(_fromUtf8("menubar"))
			MainWindow.setMenuBar(self.menubar)
			self.statusbar = QtGui.QStatusBar(MainWindow)
			self.statusbar.setObjectName(_fromUtf8("statusbar"))
			MainWindow.setStatusBar(self.statusbar)

			self.retranslateUi(MainWindow)
			QtCore.QObject.connect(self.testRenderButton, QtCore.SIGNAL(_fromUtf8("clicked()")), MainWindow.testRenderButtonPressed)
			QtCore.QObject.connect(self.okButton, QtCore.SIGNAL(_fromUtf8("clicked()")), MainWindow.okButtonPressed)
			QtCore.QObject.connect(self.materialWidget, QtCore.SIGNAL(_fromUtf8("itemPressed(QListWidgetItem*)")), MainWindow.materialSelected)
			QtCore.QMetaObject.connectSlotsByName(MainWindow)


		def retranslateUi(self, MainWindow):
			MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "MainWindow", None, QtGui.QApplication.UnicodeUTF8))
			self.testRenderButton.setText(QtGui.QApplication.translate("MainWindow", "Test Render", None, QtGui.QApplication.UnicodeUTF8))
			self.okButton.setText(QtGui.QApplication.translate("MainWindow", "OK", None, QtGui.QApplication.UnicodeUTF8))


	class HelloWindow(QtGui.QMainWindow):
		material_dict = {}

		def __init__(self, win_parent = None):
			QtGui.QMainWindow.__init__(self, win_parent)
			self.ui = Ui_MainWindow()
			self.ui.setupUi(self)

			for buf in range(get_vertex_buffer_size()):
				for mat in range(get_material_size(buf)):
					mat_name = "buf:" + str(buf) + " mat:" + str(mat)
					self.material_dict[mat_name] = [buf, mat]
					self.ui.materialWidget.addItem(QtGui.QListWidgetItem(mat_name))

		def testRenderButtonPressed(self):
			messagebox("not implemented")
			app.exit();

		def okButtonPressed(self):
			app.exit();

		def materialSelected(self, item):
			buf_mat = self.material_dict[item.text()]
			set_current_material(buf_mat[0], buf_mat[1])

	app = getApplication()
	main_window = HelloWindow()
	main_window.show()
	app.exec_()

queueCommand(runPyQtTestInThread)