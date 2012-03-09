import mmdbridge
from mmdbridge import *

import os
import math
from math import *
import time
from xml.dom import minidom, Node

#octanepath = 'C://Program Files/Refractive Software/OctaneRender 1.024 beta2.46b CUDA 3.2/octane.exe'
octanepath = 'F:/work/OctaneRender_TEST_1025_beta255_win_x64/octane.exe'


class OCSObjImportParams:
	doc = None
	element = None

	overrideDuringMeshImport = None
	importMtlMaterials = None
	importSmooth = None
	useSuppliedVertexNormals = None
	useSuppliedSmoothGroupsAsBoundaries = None
	importImageTextures = None
	diffuse = None
	glossy = None
	specular = None
	roughness = None
	index = None
	textureDiffuse = None
	textureSpecular = None
	textureRoughness = None
	textureBump = None
	textureOpacity = None
	textureDiffuseAsFloatImage = None
	textureSpecularAsFloatImage = None
	textureRoughnessAsFloatImage = None
	textureBumpUseScale = None
	textureBumpAsFloatImage = None
	textureOpacityInvert = None
	textureOpacityAsFloatImage = None
	textureAutoAlphaImage = None
	glossySpecularScale = None
	useMetaCameraObject = None
	cameraMetaObjectName = None
	scaleUnitType = None

	def getElement(self):
		if(self.overrideDuringMeshImport != None):
			self.element.appendChild(self.overrideDuringMeshImport)
		if(self.importMtlMaterials != None):
			self.element.appendChild(self.importMtlMaterials)
		if(self.importSmooth != None):
			self.element.appendChild(self.importSmooth)
		if(self.useSuppliedVertexNormals != None):
			self.element.appendChild(self.useSuppliedVertexNormals)
		if(self.useSuppliedSmoothGroupsAsBoundaries != None):
			self.element.appendChild(self.useSuppliedSmoothGroupsAsBoundaries)
		if(self.importImageTextures != None):
			self.element.appendChild(self.importImageTextures)
		if(self.diffuse != None):
			self.element.appendChild(self.diffuse)
		if(self.glossy != None):
			self.element.appendChild(self.glossy)
		if(self.specular != None):
			self.element.appendChild(self.specular)
		if(self.roughness != None):
			self.element.appendChild(self.roughness)
		if(self.index != None):
			self.element.appendChild(self.index)
		if(self.textureDiffuse != None):
			self.element.appendChild(self.textureDiffuse)
		if(self.textureSpecular != None):
			self.element.appendChild(self.textureSpecular)
		if(self.textureRoughness != None):
			self.element.appendChild(self.textureRoughness)
		if(self.textureBump != None):
			self.element.appendChild(self.textureBump)
		if(self.textureOpacity != None):
			self.element.appendChild(self.textureOpacity)
		if(self.textureDiffuseAsFloatImage != None):
			self.element.appendChild(self.textureDiffuseAsFloatImage)
		if(self.textureSpecularAsFloatImage != None):
			self.element.appendChild(self.textureSpecularAsFloatImage)
		if(self.textureRoughnessAsFloatImage != None):
			self.element.appendChild(self.textureRoughnessAsFloatImage)
		if(self.textureBumpUseScale != None):
			self.element.appendChild(self.textureBumpUseScale)
		if(self.textureBumpAsFloatImage != None):
			self.element.appendChild(self.textureBumpAsFloatImage)
		if(self.textureOpacityInvert != None):
			self.element.appendChild(self.textureOpacityInvert)
		if(self.textureOpacityAsFloatImage != None):
			self.element.appendChild(self.textureOpacityAsFloatImage)
		if(self.textureAutoAlphaImage != None):
			self.element.appendChild(self.textureAutoAlphaImage)
		if(self.glossySpecularScale != None):
			self.element.appendChild(self.glossySpecularScale)
		if(self.useMetaCameraObject != None):
			self.element.appendChild(self.useMetaCameraObject)
		if(self.cameraMetaObjectName != None):
			self.element.appendChild(self.cameraMetaObjectName)
		if(self.scaleUnitType != None):
			self.element.appendChild(self.scaleUnitType)
		return self.element

	def __init__(self, doc):
		self.doc = doc
		self.element = doc.createElement('objmeshimportparams')
		self.overrideDuringMeshImport = None
		self.importMtlMaterials = None
		self.importSmooth = None
		self.useSuppliedVertexNormals = None
		self.useSuppliedSmoothGroupsAsBoundaries = None
		self.importImageTextures = None
		self.diffuse = None
		self.glossy = None
		self.specular = None
		self.roughness = None
		self.index = None
		self.textureDiffuse = None
		self.textureSpecular = None
		self.textureRoughness = None
		self.textureBump = None
		self.textureOpacity = None
		self.textureDiffuseAsFloatImage = None
		self.textureSpecularAsFloatImage = None
		self.textureRoughnessAsFloatImage = None
		self.textureBumpUseScale = None
		self.textureBumpAsFloatImage = None
		self.textureOpacityInvert = None
		self.textureOpacityAsFloatImage = None
		self.textureAutoAlphaImage = None
		self.glossySpecularScale = None
		self.useMetaCameraObject = None
		self.cameraMetaObjectName = None
		self.scaleUnitType = None

	def setOverrideDuringMeshImport(self, value):
		if self.overrideDuringMeshImport == None:
			self.overrideDuringMeshImport = self.doc.createElement('overrideDuringMeshImport')
		self.overrideDuringMeshImport.appendChild( self.doc.createTextNode(value) )
	def setImportMtlMaterials(self, value):
		if self.importMtlMaterials == None:
			self.importMtlMaterials = self.doc.createElement('importMtlMaterials')
		self.importMtlMaterials.appendChild( self.doc.createTextNode(value) )
	def setImportSmooth(self, value):
		if self.importSmooth == None:
			self.importSmooth = self.doc.createElement('importSmooth')
		self.importSmooth.appendChild( self.doc.createTextNode(value) )
	def setUseSuppliedVertexNormals(self, value):
		if self.useSuppliedVertexNormals == None:
			self.useSuppliedVertexNormals = self.doc.createElement('useSuppliedVertexNormals')
		self.useSuppliedVertexNormals.appendChild( self.doc.createTextNode(value) )
	def setUseSuppliedSmoothGroupsAsBoundaries(self, value):
		if self.useSuppliedSmoothGroupsAsBoundaries == None:
			self.useSuppliedSmoothGroupsAsBoundaries = self.doc.createElement('useSuppliedSmoothGroupsAsBoundaries')
		self.useSuppliedSmoothGroupsAsBoundaries.appendChild( self.doc.createTextNode(value) )
	def setImportImageTextures(self, value):
		if self.importImageTextures == None:
			self.importImageTextures = self.doc.createElement('importImageTextures')
		self.importImageTextures.appendChild( self.doc.createTextNode(value) )
	def setDiffuse(self, value):
		if self.diffuse == None:
			self.diffuse = self.doc.createElement('diffuse')
		self.diffuse.appendChild( self.doc.createTextNode(value) )
	def setGlossy(self, value):
		if self.glossy == None:
			self.glossy = self.doc.createElement('glossy')
		self.glossy.appendChild( self.doc.createTextNode(value) )
	def setSpecular(self, value):
		if self.specular == None:
			self.specular = self.doc.createElement('specular')
		self.specular.appendChild( self.doc.createTextNode(value) )
	def setRoughness(self, value):
		if self.roughness == None:
			self.roughness = self.doc.createElement('roughness')
		self.roughness.appendChild( self.doc.createTextNode(value) )
	def setIndex(self, value):
		if self.index == None:
			self.index = self.doc.createElement('index')
		self.index.appendChild( self.doc.createTextNode(value) )
	def setTextureDiffuse(self, value):
		if self.textureDiffuse == None:
			self.textureDiffuse = self.doc.createElement('textureDiffuse')
		self.textureDiffuse.appendChild( self.doc.createTextNode(value) )
	def setTextureSpecular(self, value):
		if self.textureSpecular == None:
			self.textureSpecular = self.doc.createElement('textureSpecular')
		self.textureSpecular.appendChild( self.doc.createTextNode(value) )
	def setTextureRoughness(self, value):
		if self.textureRoughness == None:
			self.textureRoughness = self.doc.createElement('textureRoughness')
		self.textureRoughness.appendChild( self.doc.createTextNode(value) )
	def setTextureBump(self, value):
		if self.textureBump == None:
			self.textureBump = self.doc.createElement('textureBump')
		self.textureBump.appendChild( self.doc.createTextNode(value) )
	def setTextureOpacity(self, value):
		if self.textureOpacity == None:
			self.textureOpacity = self.doc.createElement('textureOpacity')
		self.textureOpacity.appendChild( self.doc.createTextNode(value) )
	def setTextureDiffuseAsFloatImage(self, value):
		if self.textureDiffuseAsFloatImage == None:
			self.textureDiffuseAsFloatImage = self.doc.createElement('textureDiffuseAsFloatImage')
		self.textureDiffuseAsFloatImage.appendChild( self.doc.createTextNode(value) )
	def setTextureSpecularAsFloatImage(self, value):
		if self.textureSpecularAsFloatImage == None:
			self.textureSpecularAsFloatImage = self.doc.createElement('textureSpecularAsFloatImage')
		self.textureSpecularAsFloatImage.appendChild( self.doc.createTextNode(value) )
	def setTextureRoughnessAsFloatImage(self, value):
		if self.textureRoughnessAsFloatImage == None:
			self.textureRoughnessAsFloatImage = self.doc.createElement('textureRoughnessAsFloatImage')
		self.textureRoughnessAsFloatImage.appendChild( self.doc.createTextNode(value) )
	def setTextureBumpUseScale(self, value):
		if self.textureBumpUseScale == None:
			self.textureBumpUseScale = self.doc.createElement('textureBumpUseScale')
		self.textureBumpUseScale.appendChild( self.doc.createTextNode(value) )
	def setTextureBumpAsFloatImage(self, value):
		if self.textureBumpAsFloatImage == None:
			self.textureBumpAsFloatImage = self.doc.createElement('textureBumpAsFloatImage')
		self.textureBumpAsFloatImage.appendChild( self.doc.createTextNode(value) )
	def setTextureOpacityInvert(self, value):
		if self.textureOpacityInvert == None:
			self.textureOpacityInvert = self.doc.createElement('textureOpacityInvert')
		self.textureOpacityInvert.appendChild( self.doc.createTextNode(value) )
	def setTextureOpacityAsFloatImage(self, value):
		if self.textureOpacityAsFloatImage == None:
			self.textureOpacityAsFloatImage = self.doc.createElement('textureOpacityAsFloatImage')
		self.textureOpacityAsFloatImage.appendChild( self.doc.createTextNode(value) )
	def setTextureAutoAlphaImage(self, value):
		if self.textureAutoAlphaImage == None:
			self.textureAutoAlphaImage = self.doc.createElement('textureAutoAlphaImage')
		self.textureAutoAlphaImage.appendChild( self.doc.createTextNode(value) )
	def setGlossySpecularScale(self, value):
		if self.glossySpecularScale == None:
			self.glossySpecularScale = self.doc.createElement('glossySpecularScale')
		self.glossySpecularScale.appendChild( self.doc.createTextNode(str(value)) )
	def setUseMetaCameraObject(self, value):
		if self.useMetaCameraObject == None:
			self.useMetaCameraObject = self.doc.createElement('useMetaCameraObject')
		self.useMetaCameraObject.appendChild( self.doc.createTextNode(value) )
	def setCameraMetaObjectName(self, value):
		if self.cameraMetaObjectName == None:
			self.cameraMetaObjectName = self.doc.createElement('cameraMetaObjectName')
		self.cameraMetaObjectName.appendChild( self.doc.createTextNode(value) )
	def setScaleUnitType(self, value):
		if self.scaleUnitType == None:
			self.scaleUnitType = self.doc.createElement('scaleUnitType')
		self.scaleUnitType.appendChild( self.doc.createTextNode(str(value)) )


class OCSParameters:
	doc = None
	element = None

	value = None
	valuexyz = None
	minvalue = None
	maxvalue = None
	usetextureui = None
	usetexturealphaui = None
	isloglincapable = None
	uselogscale = None
	modified = None

	def getElement(self):
		if (self.value != None):
			self.element.appendChild(self.value)
		if (self.valuexyz != None):
			self.element.appendChild(self.valuexyz)
		if (self.minvalue != None):
			self.element.appendChild(self.minvalue)
		if (self.maxvalue != None):
			self.element.appendChild(self.maxvalue)
		if (self.usetextureui != None):
			self.element.appendChild(self.usetextureui)
		if (self.usetexturealphaui != None):
			self.element.appendChild(self.usetexturealphaui)
		if (self.isloglincapable != None):
			self.element.appendChild(self.isloglincapable)
		if (self.modified != None):
			self.element.appendChild(self.modified)
		return self.element

	def __init__(self, doc):
		self.doc = doc
		self.element = doc.createElement('parameters')
		self.value = None
		self.valuexyz = None
		self.minvalue = None
		self.maxvalue = None
		self.usetextureui = None
		self.usetexturealphaui = None
		self.isloglincapable = None
		self.modified = None

	def setValue(self, value):
		self.value = self.doc.createElement('value')
		self.value.appendChild( self.doc.createTextNode(str(value)) )

	def setValueXyz(self, value):
		self.valuexyz = self.doc.createElement('valuexyz')
		self.valuexyz.appendChild( self.doc.createTextNode(str(value)) )

	def setMinValue(self, value):
		self.minvalue = self.doc.createElement('minvalue')
		self.minvalue.appendChild( self.doc.createTextNode(str(value)) )

	def setMaxValue(self, value):
		self.maxvalue = self.doc.createElement('maxvalue')
		self.maxvalue.appendChild( self.doc.createTextNode(str(value)) )

	def setUseTextureUi(self, flagstr):
		self.usetextureui = self.doc.createElement('usetextureui')
		self.usetextureui.appendChild( self.doc.createTextNode(str(flagstr).lower()) )

	def setUseTextureAlphaUi(self, flagstr):
		self.usetexturealphaui = self.doc.createElement('usetexturealphaui')
		self.usetexturealphaui.appendChild( self.doc.createTextNode(flagstr) )

	def setIsLogLinCapable(self, flagstr):
		self.isloglincapable = self.doc.createElement('isloglincapable')
		self.isloglincapable.appendChild( self.doc.createTextNode(flagstr) )

	def setUseLogScale(self, flagstr):
		self.uselogscale = self.doc.createElement('uselogscale')
		self.uselogscale.appendChild( self.doc.createTextNode(flagstr) )

	def setModified(self, flagstr):
		self.modified = self.doc.createElement('modified')
		self.modified.appendChild( self.doc.createTextNode(str(flagstr).lower()) )

class OCSNodePin:
	doc = None
	element = None

	typename= None
	id = None
	pintype = None
	hasinternalnodegraph = None
	basenodeid = None
	basenodepinid = None
	internalnodegraph = None

	def getElement(self):
		self.element.appendChild(self.typename)
		self.element.appendChild(self.id)
		self.element.appendChild(self.pintype)
		self.element.appendChild(self.hasinternalnodegraph)
		if (self.basenodeid != None):
			self.element.appendChild(self.basenodeid)
		if (self.basenodepinid != None):
			self.element.appendChild(self.basenodepinid)
		if (self.internalnodegraph != None):
			self.element.appendChild(self.internalnodegraph)
		return self.element

	def __init__(self, doc):
		self.doc = doc
		self.element = doc.createElement('NodePin')
		self.typename = doc.createElement('typename')
		self.id = doc.createElement('id')
		self.pintype = doc.createElement('pintype')
		self.hasinternalnodegraph = doc.createElement('hasinternalnodegraph')
		self.basenodeid = None
		self.basenodepinid = None
		self.internalnodegraph = None

	def setTypeName(self, typename):
		self.typename.appendChild( self.doc.createTextNode( typename ) )

	def setId(self, id):
		self.id.appendChild( self.doc.createTextNode( str(id) ) )

	def setPinType(self, pintype):
		self.pintype.appendChild( self.doc.createTextNode( str(pintype) ) )

	def setHasInternalNodeGraph(self, hasnodegraph):
		self.hasinternalnodegraph.appendChild(self.doc.createTextNode( hasnodegraph ) )

	def setBaseNodeId(self, basenodeid):
		if (basenodeid != None):
			if (self.basenodeid == None):
				self.basenodeid = self.doc.createElement('basenodeid')
			self.basenodeid.appendChild(self.doc.createTextNode( str(basenodeid) ) )

	def setBaseNodePinId(self, basenodepinid):
		if (basenodepinid != None):
			if (self.basenodepinid == None):
				self.basenodepinid = self.doc.createElement('basenodepinid')
			self.basenodepinid.appendChild(self.doc.createTextNode( str(basenodepinid) ) )

	def addInternalNodeGraph(self, nodegraph):
		if (nodegraph != None):
			if (self.internalnodegraph == None):
				self.internalnodegraph = self.doc.createElement('internalnodegraph')
			self.internalnodegraph.appendChild(nodegraph)

class OCSNodeGraph:
	doc = None
	element = None
	
	name = None
	currentnewnodeid = None
	currentnewnodepinconnectionid = None
	nodes = None
	nodepinconnections = None

	def getElement(self):
		self.element.appendChild(self.name)
		if (not self.currentnewnodeid.hasChildNodes()):
			self.currentnewnodeid.appendChild( self.doc.createTextNode( str(len(self.nodes.childNodes)+1) ) )
		if (not self.currentnewnodepinconnectionid.hasChildNodes()):
			self.currentnewnodepinconnectionid.appendChild( self.doc.createTextNode( str(len(self.nodepinconnections.childNodes)+1) ) )

		self.element.appendChild(self.currentnewnodeid)
		self.element.appendChild(self.currentnewnodepinconnectionid)
		self.element.appendChild(self.nodes)
		self.element.appendChild(self.nodepinconnections)
		return self.element

	def __init__(self, doc):
		self.doc = doc
		self.element = doc.createElement('NodeGraph')
		self.name = doc.createElement('name')
		self.currentnewnodeid = doc.createElement('currentnewnodeid')
		self.currentnewnodepinconnectionid = doc.createElement('currentnewnodepinconnectionid')
		self.nodes = doc.createElement('nodes')
		self.nodepinconnections = doc.createElement('nodepinconnections')

	def setName(self, name):
		self.name.appendChild( self.doc.createTextNode( name ) )

	def setCurrentNewNodeId(self, id):
		if (id != None):
			self.currentnewnodeid.appendChild( self.doc.createTextNode( str(id) ) )

	def setCurrentNewNodePinConnectionId(self, id):
		if (id != None):
			self.currentnewnodepinconnectionid.appendChild( self.doc.createTextNode( str(id) ) )

	def addNode(self, node):
		self.nodes.appendChild(node)

	def addNodePinConnection(self, connection):
		self.nodepinconnections.appendChild(connection)

class OCSNode:
	doc = None
	element = None

	name = None
	typename= None
	id = None
	position = None
	inputnodepins = None
	childgraph = None

	parameters = None
	objmeshimportparams = None
	linkedfilename = None

	def getElement(self):
		self.element.appendChild(self.name)
		self.element.appendChild(self.typename)
		self.element.appendChild(self.id)
		self.element.appendChild(self.position)
		if (self.parameters != None):
			self.element.appendChild(self.parameters)
		self.element.appendChild(self.inputnodepins)
		if (self.childgraph != None):
			self.element.appendChild(self.childgraph)
		if (self.objmeshimportparams != None):
			self.element.appendChild(self.objmeshimportparams)
		if (self.linkedfilename != None):
			self.element.appendChild(self.linkedfilename)
		return self.element

	def __init__(self, doc):
		self.doc = doc
		self.element = doc.createElement('Node')
		self.name = doc.createElement('name')
		self.typename = doc.createElement('typename')
		self.id = doc.createElement('id')
		self.position = doc.createElement('position')
		self.inputnodepins = doc.createElement('inputnodepins')
		self.childgraph = self.doc.createElement('childgraph')
		self.parameters = None
		self.childgraph = None
		self.objmeshimportparams = None
		self.linkedfilename = None

	def setName(self, name):
		self.name.appendChild( self.doc.createTextNode( name ) )

	def setTypeName(self, typename):
		self.typename.appendChild( self.doc.createTextNode( typename ) )

	def setId(self, id):
		self.id.appendChild( self.doc.createTextNode( str(id) ) )

	def setPosition(self, pos):
		self.position.appendChild( self.doc.createTextNode( str(pos[0])+" "+str(pos[1]) ) )

	def addInputNodePin(self, nodepin):
		self.inputnodepins.appendChild(nodepin)

	def addChildGraph(self, graph):
		if (self.childgraph == None):
			self.childgraph = self.doc.createElement('childgraph')
		self.childgraph.appendChild(graph)

	def addParameter(self, nodename, parameter):
		if (self.parameters == None):
			self.parameters = self.doc.createElement('parameters')
		node = doc.createElement(nodename)
		node.appendChild( self.doc.createTextNode( str(parameter).lower() ) )
		self.parameters.appendChild(  node )

	def addParameters(self, parameters):
		self.parameters = parameters

	def setObjMeshImportParams(self, importparameters):
		self.objmeshimportparams = importparameters

	def setLinkedFileName(self, str):
		if (self.linkedfilename == None):
			self.linkedfilename = self.doc.createElement('linkedfilename')
		self.linkedfilename.appendChild( self.doc.createTextNode( str ) )

class OCSScene:
	doc = None
	element = None

	# this is root of dom
	def toprettyxml(self, indent):
		self.doc.appendChild(self.element)
		return self.doc.toprettyxml(indent)

	def toxml(self):
		self.doc.appendChild(self.element)
		return self.doc.toxml()

	def __init__(self, doc):
		self.doc = doc
		self.element = doc.createElement('OCS_1_0_23_Scene')

	def addNode(self, node):
		self.element.appendChild(node)

def createNodeGraph(doc, name, newnodeid = None, newnodepinconnectionid = None):
	nodeGraph = OCSNodeGraph(doc)
	nodeGraph.setName(name)
	nodeGraph.setCurrentNewNodeId(newnodeid)
	nodeGraph.setCurrentNewNodePinConnectionId(newnodepinconnectionid)
	return nodeGraph

def createNode(doc, name, typename, id, position):
	node = OCSNode(doc)
	node.setName(name)
	node.setTypeName(typename)
	node.setId(id)
	node.setPosition(position)
	return node

def createNodePinConnectionElement(doc, sourceid, sourcepinid, destid, destpinid):
	element = doc.createElement("nodepinconnection")
	esourceid = doc.createElement('sourceid')
	esourcepinid = doc.createElement('sourcepinid')
	edestid = doc.createElement('destid')
	edestpinid = doc.createElement('destpinid')
	esourceid.appendChild( doc.createTextNode( str(sourceid) ) )
	esourcepinid.appendChild( doc.createTextNode( str(sourcepinid) ) )
	edestid.appendChild( doc.createTextNode( str(destid) ) )
	edestpinid.appendChild( doc.createTextNode( str(destpinid) ) )
	element.appendChild(esourceid)
	element.appendChild(esourcepinid)
	element.appendChild(edestid)
	element.appendChild(edestpinid)
	return element

def createNodePin(doc, typename, id, pintype, hasinternalnodegraph, basenodeid = None, basenodepinid = None):
	nodepin = OCSNodePin(doc)
	nodepin.setTypeName(typename)
	nodepin.setId(id)
	nodepin.setPinType(pintype)
	nodepin.setHasInternalNodeGraph(hasinternalnodegraph)
	nodepin.setBaseNodeId(basenodeid)
	nodepin.setBaseNodePinId(basenodepinid)
	return nodepin

def createSplitValueNode(doc, id, position, value, minvalue, maxvalue, textureui, alphaui, capable, logscale, modified):
	splitValue = createNode(doc, "Split Value", "float", id, position)
	splitValue.addParameter("value", value)
	splitValue.addParameter("minvalue", minvalue)
	splitValue.addParameter("maxvalue", maxvalue)
	splitValue.addParameter("usetextureui", textureui)
	splitValue.addParameter("usetexturealphaui", alphaui)
	splitValue.addParameter("isloglincapable", capable)
	splitValue.addParameter("uselogscale", logscale)
	splitValue.addParameter("modified", modified)
	return splitValue

def createSplitAxisNode(doc, id, position, value, modified):
	splitAxis = createNode(doc, "Split Axis", "bool", id, position)
	splitAxis.addParameter("value", value)
	splitAxis.addParameter("modified", modified)
	return splitAxis

def createDefaultSplitPaneNode(doc, name, typename, id, position, maximized, splitvalue, rootpanenode):
	node = createNode(doc, name, typename, id, position)
	node.addParameter("maximized", maximized)
	node.addParameter("unmaxsplitvalue", splitvalue)
	node.addParameter("isrootpanenode", rootpanenode)
	node.addInputNodePin(createNodePin(doc, "childpaneleft", 0, 21000, "false").getElement())
	node.addInputNodePin(createNodePin(doc, "childpaneright", 1, 21000, "false").getElement())
	node.addInputNodePin(createNodePin(doc, "splitvalue", 2, 20001, "false").getElement())
	node.addInputNodePin(createNodePin(doc, "splitvertical", 3, 20003, "false").getElement())
	return node

def createDefaultWorkPaneNode(doc, id, position, ismaximized, isrootpanenode, toolbar_placement):
	node = createNode(doc, "%%WORKPANE NODE%%", "workpane", id, position)
	node.addParameter("ismaximized", ismaximized)
	node.addParameter("isrootpanenode", isrootpanenode)
	node.addParameter("toolbar_placement", toolbar_placement)
	node.addInputNodePin(createNodePin(doc, "defaultstate", 0, 21000, "false").getElement())
	node.addInputNodePin(createNodePin(doc, "nodegrapheditorstate", 1, 21000, "false").getElement())
	node.addInputNodePin(createNodePin(doc, "nodestackeditorstate", 2, 21000, "false").getElement())
	node.addInputNodePin(createNodePin(doc, "rendererstate", 3, 21000, "false").getElement())
	node.addInputNodePin(createNodePin(doc, "outlinerstate", 4, 21000, "false").getElement())
	return node

def createDefaultPreviewRenderTargetNode(doc, id, position):
	node = createNode(doc, "Mesh Preview RenderTarget", "rendertarget", id, position)
	node.addInputNodePin(createNodePin(doc, "camera", 0, 21000, "false").getElement())
	node.addInputNodePin(createNodePin(doc, "resolution", 1, 20002, "false").getElement())
	node.addInputNodePin(createNodePin(doc, "environment", 2, 21000, "false").getElement())
	node.addInputNodePin(createNodePin(doc, "imager", 3, 21000, "false").getElement())
	node.addInputNodePin(createNodePin(doc, "kernel", 4, 21000, "false").getElement())
	return node

def createParameters(doc, \
		value = None, minvalue = None, maxvalue = None, \
			textureui = None, texturealphaui = None, loglincapable = None, logscale = None, modified = None):
	params = OCSParameters(doc)
	params.setValue(value)
	params.setMinValue(minvalue)
	params.setMaxValue(maxvalue)
	params.setUseTextureUi(textureui)
	params.setUseTextureAlphaUi(texturealphaui)
	params.setIsLogLinCapable(loglincapable)
	params.setUseLogScale(logscale)
	params.setModified(modified)
	return params

def createDefaultPreviewCameraNode(doc, id, position):
	node = createNode(doc, "Mesh Preview Camera", "thinlens", id, position)
	fovPin = createNodePin(doc, "fov", 0, 20001, "true", 1, 0)
	fovPinGraph = createNodeGraph(doc, "fov")
	fovPinGraphNode = createNode(doc, "fov", "float", 1, [0,0])
	fovPinGraphNode.addParameters(createParameters(doc, 88.4571, 1, 180, "false", "false", "true", "true", "true").getElement())
	fovPinGraph.addNode(fovPinGraphNode.getElement())
	fovPin.addInternalNodeGraph(fovPinGraph.getElement())
	node.addInputNodePin(fovPin.getElement())

	aperturePin = createNodePin(doc, "aperture", 1, 20001, "true", 1, 0)
	aperturePinGraph = createNodeGraph(doc, "aperture")
	aperturePinNode = createNode(doc, "aperture", "float", 1, [0,0])
	aperturePinNode.addParameters(createParameters(doc, 1, 0.01, 100, "false", "false", "true", "true", "false").getElement())
	aperturePinGraph.addNode(aperturePinNode.getElement())
	aperturePin.addInternalNodeGraph(aperturePinGraph.getElement())
	node.addInputNodePin(aperturePin.getElement())

	posPin = createNodePin(doc, "pos", 2, 20001, "true", 1, 0)
	posPinGraph = createNodeGraph(doc, "pos")
	posPinNode = createNode(doc, "pos", "float3", 1, [0,0])
	posPinNode.addParameter("valuexyz", "-0.88 13.477 22.7")
	posPinNode.addParameter("minvalue", -100000)
	posPinNode.addParameter("maxvalue", 100000)
	posPinNode.addParameter("modified", True)
	posPinNode.addParameter("control", 1)
	posPinGraph.addNode(posPinNode.getElement())
	posPin.addInternalNodeGraph(posPinGraph.getElement())
	node.addInputNodePin(posPin.getElement())

	targetPin = createNodePin(doc, "target", 3, 20001, "true", 1, 0)
	targetPinGraph = createNodeGraph(doc, "target")
	targetPinNode = createNode(doc, "target", "float3", 1, [0,0])
	targetPinNode.addParameter("valuexyz", "-0.814457 9.80848 -43.9309")
	targetPinNode.addParameter("minvalue", -100000)
	targetPinNode.addParameter("maxvalue", 100000)
	targetPinNode.addParameter("modified", True)
	targetPinNode.addParameter("control", 1)
	targetPinGraph.addNode(targetPinNode.getElement())
	targetPin.addInternalNodeGraph(targetPinGraph.getElement())
	node.addInputNodePin(targetPin.getElement())

	upPin = createNodePin(doc, "up", 4, 20001, "true", 1, 0)
	upPinGraph = createNodeGraph(doc, "up")
	upPinNode = createNode(doc, "up", "float3", 1, [0,0])
	upPinNode.addParameter("valuexyz", "0 1 0")
	upPinNode.addParameter("minvalue", -1)
	upPinNode.addParameter("maxvalue", 1)
	upPinNode.addParameter("modified", True)
	upPinNode.addParameter("control", 1)
	upPinGraph.addNode(upPinNode.getElement())
	upPin.addInternalNodeGraph(upPinGraph.getElement())
	node.addInputNodePin(upPin.getElement())

	stereoPin = createNodePin(doc, "stereo", 5, 20003, "true", 1, 0)
	stereoPinGraph = createNodeGraph(doc, "stereo")
	stereoPinNode = createNode(doc, "stereo", "bool", 1, [0,0])
	stereoPinNode.addParameter("value", False)
	stereoPinNode.addParameter("modified", True)
	stereoPinGraph.addNode(stereoPinNode.getElement())
	stereoPin.addInternalNodeGraph(stereoPinGraph.getElement())
	node.addInputNodePin(stereoPin.getElement())

	stereoDistPin = createNodePin(doc, "stereodist", 6, 20001, "true", 1, 0)
	stereoDistPinGraph = createNodeGraph(doc, "stereodist")
	stereoDistPinNode = createNode(doc, "stereodist", "float", 1, [0,0])
	stereoDistPinNode.addParameter("value", 0.02)
	stereoDistPinNode.addParameter("minvalue", 0.001)
	stereoDistPinNode.addParameter("maxvalue", 2)
	stereoDistPinNode.addParameter("usetextureui", False)
	stereoDistPinNode.addParameter("usetexturealphaui", False)
	stereoDistPinNode.addParameter("isloglincapable", True)
	stereoDistPinNode.addParameter("uselogscale", True)
	stereoDistPinNode.addParameter("modified", False)
	stereoDistPinGraph.addNode(stereoDistPinNode.getElement())
	stereoDistPin.addInternalNodeGraph(stereoDistPinGraph.getElement())
	node.addInputNodePin(stereoDistPin.getElement())

	leftFilterPin = createNodePin(doc, "leftFilter", 7, 20000, "true", 1, 0)
	leftFilterPinGraph = createNodeGraph(doc, "leftFilter")
	leftFilterPinNode = createNode(doc, "leftFilter", "RGBspectrum", 1, [0,0])
	leftFilterPinNode.addParameter("rgbvalue", "1 0 0.812")
	leftFilterPinGraph.addNode(leftFilterPinNode.getElement())
	leftFilterPin.addInternalNodeGraph(leftFilterPinGraph.getElement())
	node.addInputNodePin(leftFilterPin.getElement())

	rightFilterPin = createNodePin(doc, "rightFilter", 8, 20000, "true", 1, 0)
	rightFilterPinGraph = createNodeGraph(doc, "rightFilter")
	rightFilterPinNode = createNode(doc, "rightFilter", "RGBspectrum", 1, [0,0])
	rightFilterPinNode.addParameter("rgbvalue", "1 0 0.188")
	rightFilterPinGraph.addNode(rightFilterPinNode.getElement())
	rightFilterPin.addInternalNodeGraph(rightFilterPinGraph.getElement())
	node.addInputNodePin(rightFilterPin.getElement())

	lensShiftPin = createNodePin(doc, "lensShift", 9, 20001, "true", 1, 0)
	lensShiftPinGraph = createNodeGraph(doc, "lensShift")
	lensShiftPinNode = createNode(doc, "lensShift", "float2", 1, [0,0])
	lensShiftPinNode.addParameter("valuexy", "0 0")
	lensShiftPinNode.addParameter("minvalue", -4)
	lensShiftPinNode.addParameter("maxvalue", 4)
	lensShiftPinNode.addParameter("modified", False)
	lensShiftPinGraph.addNode(lensShiftPinNode.getElement())
	lensShiftPin.addInternalNodeGraph(lensShiftPinGraph.getElement())
	node.addInputNodePin(lensShiftPin.getElement())

	return node

def createDefaultPreviewResolutionNode(doc, id, position):
	node = createNode(doc, "Mesh Preview Resolution", "int2resolution", id, position)
	node.addParameter("valuexy", "800 600")
	node.addParameter("presetid", 403)
	node.addParameter("minvalue", 4)
	node.addParameter("maxvalue", 8192)
	node.addParameter("modified", True)
	return node

def createDefaultMeshPreviewImager(doc, id, position):
	node = createNode(doc, "Mesh Preview Imager", "camera", id, position)

	exposurePin = createNodePin(doc, "exposure", 0, 20001, "true", 1, 0)
	exposurePinGraph = createNodeGraph(doc, "exposure")
	exposurePinNode = createNode(doc, "exposure", "float", 1, [0,0])
	exposurePinNode.addParameter("value", 1)
	exposurePinNode.addParameter("minvalue", 0.001)
	exposurePinNode.addParameter("maxvalue", 4096)
	exposurePinNode.addParameter("usetextureui", False)
	exposurePinNode.addParameter("usetexturealphaui", False)
	exposurePinNode.addParameter("isloglincapable", True)
	exposurePinNode.addParameter("uselogscale", True)
	exposurePinNode.addParameter("modified", False)
	exposurePinGraph.addNode(exposurePinNode.getElement())
	exposurePin.addInternalNodeGraph(exposurePinGraph.getElement())
	node.addInputNodePin(exposurePin.getElement())

	fstopPin = createNodePin(doc, "fstop", 1, 20001, "true", 1, 0)
	fstopPinGraph = createNodeGraph(doc, "fstop")
	fstopPinNode = createNode(doc, "fstop", "float", 1, [0,0])
	fstopPinNode.addParameter("value", 2.8)
	fstopPinNode.addParameter("minvalue", 1)
	fstopPinNode.addParameter("maxvalue", 64)
	fstopPinNode.addParameter("usetextureui", False)
	fstopPinNode.addParameter("usetexturealphaui", False)
	fstopPinNode.addParameter("isloglincapable", True)
	fstopPinNode.addParameter("uselogscale", True)
	fstopPinNode.addParameter("modified", False)
	fstopPinGraph.addNode(fstopPinNode.getElement())
	fstopPin.addInternalNodeGraph(fstopPinGraph.getElement())
	node.addInputNodePin(fstopPin.getElement())

	isoPin = createNodePin(doc, "ISO", 2, 20001, "true", 1, 0)
	isoPinGraph = createNodeGraph(doc, "ISO")
	isoPinNode = createNode(doc, "ISO", "float", 1, [0,0])
	isoPinNode.addParameter("value", 100)
	isoPinNode.addParameter("minvalue", 1)
	isoPinNode.addParameter("maxvalue", 800)
	isoPinNode.addParameter("usetextureui", False)
	isoPinNode.addParameter("usetexturealphaui", False)
	isoPinNode.addParameter("isloglincapable", True)
	isoPinNode.addParameter("uselogscale", False)
	isoPinNode.addParameter("modified", False)
	isoPinGraph.addNode(isoPinNode.getElement())
	isoPin.addInternalNodeGraph(isoPinGraph.getElement())
	node.addInputNodePin(isoPin.getElement())

	gammaPin = createNodePin(doc, "gamma", 3, 20001, "true", 1, 0)
	gammaPinGraph = createNodeGraph(doc, "gamma")
	gammaPinNode = createNode(doc, "gamma", "float", 1, [0,0])
	gammaPinNode.addParameter("value", 1)
	gammaPinNode.addParameter("minvalue", 0.1)
	gammaPinNode.addParameter("maxvalue", 32)
	gammaPinNode.addParameter("usetextureui", False)
	gammaPinNode.addParameter("usetexturealphaui", False)
	gammaPinNode.addParameter("isloglincapable", True)
	gammaPinNode.addParameter("uselogscale", True)
	gammaPinNode.addParameter("modified", False)
	gammaPinGraph.addNode(gammaPinNode.getElement())
	gammaPin.addInternalNodeGraph(gammaPinGraph.getElement())
	node.addInputNodePin(gammaPin.getElement())

	responsePin = createNodePin(doc, "response", 4, 21000, "true", 1, 0)
	responsePinGraph = createNodeGraph(doc, "response")
	responsePinNode = createNode(doc, "response", "camera response", 1, [0,0])
	responsePinNode.addParameter("currentcurveid", 105)
	responsePinNode.addParameter("modified", False)
	responsePinGraph.addNode(responsePinNode.getElement())
	responsePin.addInternalNodeGraph(responsePinGraph.getElement())
	node.addInputNodePin(responsePin.getElement())

	vignetPin = createNodePin(doc, "vignetting", 5, 20001, "true", 1, 0)
	vignetPinGraph = createNodeGraph(doc, "vignetting")
	vignetPinNode = createNode(doc, "vignetting", "float", 1, [0,0])
	vignetPinNode.addParameter("value", 0.3)
	vignetPinNode.addParameter("minvalue", 0)
	vignetPinNode.addParameter("maxvalue", 1)
	vignetPinNode.addParameter("usetextureui", False)
	vignetPinNode.addParameter("usetexturealphaui", False)
	vignetPinNode.addParameter("isloglincapable", False)
	vignetPinNode.addParameter("uselogscale", False)
	vignetPinNode.addParameter("modified", False)
	vignetPinGraph.addNode(vignetPinNode.getElement())
	vignetPin.addInternalNodeGraph(vignetPinGraph.getElement())
	node.addInputNodePin(vignetPin.getElement())

	saturationPin = createNodePin(doc, "saturation", 6, 20001, "true", 1, 0)
	saturationPinGraph = createNodeGraph(doc, "saturation")
	saturationPinNode = createNode(doc, "saturation", "float", 1, [0,0])
	saturationPinNode.addParameter("value", 1)
	saturationPinNode.addParameter("minvalue", 0)
	saturationPinNode.addParameter("maxvalue", 4)
	saturationPinNode.addParameter("usetextureui", False)
	saturationPinNode.addParameter("usetexturealphaui", False)
	saturationPinNode.addParameter("isloglincapable", False)
	saturationPinNode.addParameter("uselogscale", False)
	saturationPinNode.addParameter("modified", False)
	saturationPinGraph.addNode(saturationPinNode.getElement())
	saturationPin.addInternalNodeGraph(saturationPinGraph.getElement())
	node.addInputNodePin(saturationPin.getElement())

	hotpixelPin = createNodePin(doc, "hotpixel_removal", 7, 20001, "true", 1, 0)
	hotpixelPinGraph = createNodeGraph(doc, "hotpixel_removal")
	hotpixelPinNode = createNode(doc, "hotpixel_removal", "float", 1, [0,0])
	hotpixelPinNode.addParameter("value", 1)
	hotpixelPinNode.addParameter("minvalue", 0)
	hotpixelPinNode.addParameter("maxvalue", 1)
	hotpixelPinNode.addParameter("usetextureui", False)
	hotpixelPinNode.addParameter("usetexturealphaui", False)
	hotpixelPinNode.addParameter("isloglincapable", False)
	hotpixelPinNode.addParameter("uselogscale", False)
	hotpixelPinNode.addParameter("modified", False)
	hotpixelPinGraph.addNode(hotpixelPinNode.getElement())
	hotpixelPin.addInternalNodeGraph(hotpixelPinGraph.getElement())
	node.addInputNodePin(hotpixelPin.getElement())

	preAlphaPin = createNodePin(doc, "premultiplied_alpha", 8, 20003, "true", 1, 0)
	preAlphaPinGraph = createNodeGraph(doc, "premultiplied_alpha")
	preAlphaPinNode = createNode(doc, "premultiplied_alpha", "bool", 1, [0,0])
	preAlphaPinNode.addParameter("value", False)
	preAlphaPinNode.addParameter("modified", True)
	preAlphaPinGraph.addNode(preAlphaPinNode.getElement())
	preAlphaPin.addInternalNodeGraph(preAlphaPinGraph.getElement())
	node.addInputNodePin(preAlphaPin.getElement())

	return node

def createDefaultMeshPreviewKernel(doc, id, position):
	node = createNode(doc, "Mesh Preview Kernel", "directlighting", id, position)

	specularDepthPin = createNodePin(doc, "speculardepth", 0, 20002, "true", 1, 0)
	specularDepthPinGraph = createNodeGraph(doc, "speculardepth")
	specularDepthPinNode = createNode(doc, "speculardepth", "int", 1, [0,0])
	specularDepthPinNode.addParameter("value", 5)
	specularDepthPinNode.addParameter("minvalue", 1)
	specularDepthPinNode.addParameter("maxvalue", 1024)
	specularDepthPinNode.addParameter("isloglincapable", True)
	specularDepthPinNode.addParameter("uselogscale", True)
	specularDepthPinNode.addParameter("modified", False)
	specularDepthPinGraph.addNode(specularDepthPinNode.getElement())
	specularDepthPin.addInternalNodeGraph(specularDepthPinGraph.getElement())
	node.addInputNodePin(specularDepthPin.getElement())

	glossyDepthPin = createNodePin(doc, "glossydepth", 1, 20002, "true", 1, 0)
	glossyDepthPinGraph = createNodeGraph(doc, "glossydepth")
	glossyDepthPinNode = createNode(doc, "glossydepth", "int", 1, [0,0])
	glossyDepthPinNode.addParameter("value", 1)
	glossyDepthPinNode.addParameter("minvalue", 1)
	glossyDepthPinNode.addParameter("maxvalue", 1024)
	glossyDepthPinNode.addParameter("isloglincapable", True)
	glossyDepthPinNode.addParameter("uselogscale", True)
	glossyDepthPinNode.addParameter("modified", False)
	glossyDepthPinGraph.addNode(glossyDepthPinNode.getElement())
	glossyDepthPin.addInternalNodeGraph(glossyDepthPinGraph.getElement())
	node.addInputNodePin(glossyDepthPin.getElement())

	aoDistPin = createNodePin(doc, "aodist", 2, 20001, "true", 1, 0)
	aoDistPinGraph = createNodeGraph(doc, "aodist")
	aoDistPinNode = createNode(doc, "aodist", "float", 1, [0,0])
	aoDistPinNode.addParameter("value", 3)
	aoDistPinNode.addParameter("minvalue", 0.01)
	aoDistPinNode.addParameter("maxvalue", 1024)
	aoDistPinNode.addParameter("usetextureui", False)
	aoDistPinNode.addParameter("usetexturealphaui", False)
	aoDistPinNode.addParameter("isloglincapable", True)
	aoDistPinNode.addParameter("uselogscale", True)
	aoDistPinNode.addParameter("modified", False)
	aoDistPinGraph.addNode(aoDistPinNode.getElement())
	aoDistPin.addInternalNodeGraph(aoDistPinGraph.getElement())
	node.addInputNodePin(aoDistPin.getElement())

	rayepsilonPin = createNodePin(doc, "rayepsilon", 3, 20001, "true", 1, 0)
	rayepsilonPinGraph = createNodeGraph(doc, "rayepsilon")
	rayepsilonPinNode = createNode(doc, "rayepsilon", "float", 1, [0,0])
	rayepsilonPinNode.addParameter("value", 0.0001)
	rayepsilonPinNode.addParameter("minvalue", 1e-006)
	rayepsilonPinNode.addParameter("maxvalue", 0.1)
	rayepsilonPinNode.addParameter("usetextureui", False)
	rayepsilonPinNode.addParameter("usetexturealphaui", False)
	rayepsilonPinNode.addParameter("isloglincapable", True)
	rayepsilonPinNode.addParameter("uselogscale", True)
	rayepsilonPinNode.addParameter("modified", False)
	rayepsilonPinGraph.addNode(rayepsilonPinNode.getElement())
	rayepsilonPin.addInternalNodeGraph(rayepsilonPinGraph.getElement())
	node.addInputNodePin(rayepsilonPin.getElement())

	maxSamplePin = createNodePin(doc, "maxsamples", 4, 20002, "true", 1, 0)
	maxSamplePinGraph = createNodeGraph(doc, "maxsamples")
	maxSamplePinNode = createNode(doc, "maxsamples", "int", 1, [0,0])
	maxSamplePinNode.addParameter("value", "128")
	maxSamplePinNode.addParameter("minvalue", "1")
	maxSamplePinNode.addParameter("maxvalue", "64000")
	maxSamplePinNode.addParameter("isloglincapable", True)
	maxSamplePinNode.addParameter("uselogscale", False)
	maxSamplePinNode.addParameter("modified", True)
	maxSamplePinGraph.addNode(maxSamplePinNode.getElement())
	maxSamplePin.addInternalNodeGraph(maxSamplePinGraph.getElement())
	node.addInputNodePin(maxSamplePin.getElement())

	filterSizePin = createNodePin(doc, "filtersize", 5, 20001, "true", 1, 0)
	filterSizePinGraph = createNodeGraph(doc, "filtersize")
	filterSizePinNode = createNode(doc, "filtersize", "float", 1, [0,0])
	filterSizePinNode.addParameter("value", "1.5")
	filterSizePinNode.addParameter("minvalue", "1")
	filterSizePinNode.addParameter("maxvalue", "8")
	filterSizePinNode.addParameter("usetextureui", False)
	filterSizePinNode.addParameter("usetexturealphaui", False)
	filterSizePinNode.addParameter("isloglincapable", False)
	filterSizePinNode.addParameter("uselogscale", False)
	filterSizePinNode.addParameter("modified", False)
	filterSizePinGraph.addNode(filterSizePinNode.getElement())
	filterSizePin.addInternalNodeGraph(filterSizePinGraph.getElement())
	node.addInputNodePin(filterSizePin.getElement())

	alphaChannelPin = createNodePin(doc, "alphachannel", 6, 20003, "true", 1, 0)
	alphaChannelPinGraph = createNodeGraph(doc, "alphachannel")
	alphaChannelPinNode = createNode(doc, "alphachannel", "bool", 1, [0,0])
	alphaChannelPinNode.addParameter("value", False)
	alphaChannelPinNode.addParameter("modified", True)
	alphaChannelPinGraph.addNode(alphaChannelPinNode.getElement())
	alphaChannelPin.addInternalNodeGraph(alphaChannelPinGraph.getElement())
	node.addInputNodePin(alphaChannelPin.getElement())

	keepEnvironmentPin = createNodePin(doc, "keep_environment", 7, 20003, "true", 1, 0)
	keepEnvironmentPinGraph = createNodeGraph(doc, "keep_environment")
	keepEnvironmentPinNode = createNode(doc, "keep_environment", "bool", 1, [0,0])
	keepEnvironmentPinNode.addParameter("value", False)
	keepEnvironmentPinNode.addParameter("modified", True)
	keepEnvironmentPinGraph.addNode(keepEnvironmentPinNode.getElement())
	keepEnvironmentPin.addInternalNodeGraph(keepEnvironmentPinGraph.getElement())
	node.addInputNodePin(keepEnvironmentPin.getElement())

	return node


def createDefaultPreviewEnvironment(doc, id, position):
	node = createNode(doc, "Mesh Preview Environment", "daylight", id, position)

	sundirPin = createNodePin(doc, "sundir", 0, 20001, "true", 1, 0)
	sundirPinGraph = createNodeGraph(doc, "sundir")
	sundirPinNode = createNode(doc, "sundir", "float3", 1, [0,0])
	sundirPinNode.addParameter("valuexyz", "-0.768835 0.110155 -0.629888")
	sundirPinNode.addParameter("minvalue", "-1")
	sundirPinNode.addParameter("maxvalue", "1")
	sundirPinNode.addParameter("modified", True)
	sundirPinNode.addParameter("control", "1")
	sundirPinGraph.addNode(sundirPinNode.getElement())
	sundirPin.addInternalNodeGraph(sundirPinGraph.getElement())
	node.addInputNodePin(sundirPin.getElement())

	turbidityPin = createNodePin(doc, "turbidity", 1, 20001, "true", 1, 0)
	turbidityPinGraph = createNodeGraph(doc, "turbidity")
	turbidityPinNode = createNode(doc, "turbidity", "float", 1, [0,0])
	turbidityPinNode.addParameter("value", "2.2")
	turbidityPinNode.addParameter("minvalue", "0.5")
	turbidityPinNode.addParameter("maxvalue", "16")
	turbidityPinNode.addParameter("usetextureui", False)
	turbidityPinNode.addParameter("usetexturealphaui", False)
	turbidityPinNode.addParameter("isloglincapable", True)
	turbidityPinNode.addParameter("uselogscale", True)
	turbidityPinNode.addParameter("modified", False)
	turbidityPinGraph.addNode(turbidityPinNode.getElement())
	turbidityPin.addInternalNodeGraph(turbidityPinGraph.getElement())
	node.addInputNodePin(turbidityPin.getElement())

	powerPin = createNodePin(doc, "power", 2, 20001, "true", 1, 0)
	powerPinGraph = createNodeGraph(doc, "power")
	powerPinNode = createNode(doc, "power", "float", 1, [0,0])
	powerPinNode.addParameter("value", "1")
	powerPinNode.addParameter("minvalue", "0.001")
	powerPinNode.addParameter("maxvalue", "1000")
	powerPinNode.addParameter("usetextureui", False)
	powerPinNode.addParameter("usetexturealphaui", False)
	powerPinNode.addParameter("isloglincapable", True)
	powerPinNode.addParameter("uselogscale", True)
	powerPinNode.addParameter("modified", False)
	powerPinGraph.addNode(powerPinNode.getElement())
	powerPin.addInternalNodeGraph(powerPinGraph.getElement())
	node.addInputNodePin(powerPin.getElement())

	northoffsetPin = createNodePin(doc, "northoffset", 3, 20001, "true", 1, 0)
	northoffsetPinGraph = createNodeGraph(doc, "northoffset")
	northoffsetPinNode = createNode(doc, "northoffset", "float", 1, [0,0])
	northoffsetPinNode.addParameter("value", "0")
	northoffsetPinNode.addParameter("minvalue", "-1")
	northoffsetPinNode.addParameter("maxvalue", "1")
	northoffsetPinNode.addParameter("usetextureui", False)
	northoffsetPinNode.addParameter("usetexturealphaui", False)
	northoffsetPinNode.addParameter("isloglincapable", False)
	northoffsetPinNode.addParameter("uselogscale", False)
	northoffsetPinNode.addParameter("modified", False)
	northoffsetPinGraph.addNode(northoffsetPinNode.getElement())
	northoffsetPin.addInternalNodeGraph(northoffsetPinGraph.getElement())
	node.addInputNodePin(northoffsetPin.getElement())

	return node

def createDefaultObjImportParams(doc):
	params = OCSObjImportParams(doc)
	params.setOverrideDuringMeshImport("false")
	params.setImportMtlMaterials("true")
	params.setImportSmooth("true")
	params.setUseSuppliedVertexNormals("true")
	params.setUseSuppliedSmoothGroupsAsBoundaries("false")
	params.setImportImageTextures("true")
	params.setDiffuse("true")
	params.setGlossy("true")
	params.setSpecular("true")
	params.setRoughness("true")
	params.setIndex("true")
	params.setTextureDiffuse("true")
	params.setTextureSpecular("true")
	params.setTextureRoughness("true")
	params.setTextureBump("true")
	params.setTextureOpacity("true")
	params.setTextureDiffuseAsFloatImage("false")
	params.setTextureSpecularAsFloatImage("false")
	params.setTextureRoughnessAsFloatImage("true")
	params.setTextureBumpUseScale("true")
	params.setTextureBumpAsFloatImage("true")
	params.setTextureOpacityInvert("false")
	params.setTextureOpacityAsFloatImage("true")
	params.setTextureAutoAlphaImage("true")
	params.setGlossySpecularScale(0.5)
	params.setUseMetaCameraObject("false")
	params.setCameraMetaObjectName("CAM_TRANSFORM")
	params.setScaleUnitType(4)
	return params

class MaterialNodePin:
	doc = None

	nodePin = None
	nodePinGraph = None

	def getElement(self):
		self.nodePin.addInternalNodeGraph(self.nodePinGraph.getElement())
		return self.nodePin.getElement()

	def __init__(self, doc, name, id, typename):
		self.doc = doc
		self.nodePin = createNodePin(doc, name, id, 20005, "true", 1, 0)
		self.nodePinGraph = createNodeGraph(doc, name)
		self.nodePinGraph.addNode(createNode(doc, name, typename, self.getNextNodeId(), [0, 0]).getElement())

	def getNextNodeId(self):
		return len(self.nodePinGraph.nodes.childNodes) + 1

	def setDiffuse(self, rgbValue = [1, 1, 1]):
		node = createNode(self.doc, 'diffuse', 'RGBspectrum', self.getNextNodeId(), [0, 0])
		node.addParameter("rgbvalue", str(rgbValue[0])+" "+str(rgbValue[1])+" "+str(rgbValue[2]))
		self.nodePinGraph.addNode(node.getElement())

	def setSmooth(self, issmooth):
		node = createNode(self.doc, 'smooth', 'bool', self.getNextNodeId(), [0, 0])
		node.addParameter("value", issmooth)
		node.addParameter("modified", True)
		self.nodePinGraph.addNode(node.getElement())

	def setBump(self, isbump):
		node = createNode(self.doc, 'bump', 'floattexture', self.getNextNodeId(), [0, 0])
		params = None
		if isbump:
			params = createParameters(doc, 1, 1, None, "false", "false", "false", "true")
		else:
			params = createParameters(doc, 0, 1, None, "false", "false", "false", "true")
		node.addParameters(params.getElement())
		node.addParameter("resolution", 0.001)
		self.nodePinGraph.addNode(node.getElement())

	def setOpacity(self, opacity):
		node = createNode(self.doc, 'opacity', 'floattexture', self.getNextNodeId(), [0, 0])
		params = createParameters(doc, opacity, 0, 1, None, "false", "false", "false", "true")
		node.addParameters(params.getElement())
		node.addParameter("resolution", 0.001)
		self.nodePinGraph.addNode(node.getElement())

#--------------------------------%%SYSTEM%% -----------------------------------
def createSystemNode(doc):
	systemNode = createNode(doc, "%%SYSTEM%%", "macro", 1, [0.1, 0.2])

	childGraph = createNodeGraph(doc, "%%SYSTEM%%", 31, 34)
	systemNode.addChildGraph(childGraph.getElement())

	workspaceManager = createNode(doc, "%%WORKSPACEMANAGER%%", "workspacemanager", 1, [0,0])
	workspaceManager.addInputNodePin(createNodePin(doc, "rootpane", 0, 21000, "false").getElement())
	childGraph.addNode(workspaceManager.getElement())

	childGraph.addNode(\
		createDefaultSplitPaneNode(doc, "rootsplitpane", "splitpane", 2, [0,0], \
			"false", 0.5, "true").getElement())

	childGraph.addNode(\
		createSplitValueNode(doc, 3, [0,0], 0.729167, 0, 1, \
			"false", "false", "true", "false", "true").getElement())

	childGraph.addNode(\
		createSplitAxisNode(doc, 4, [0,0], "false", "true").getElement())

	childGraph.addNode(\
		createDefaultSplitPaneNode(doc, "leftsplitpane", "splitpane", 5, [0,0], \
			"false", 0.5, "false").getElement())

	childGraph.addNode(\
		createSplitValueNode(doc, 6, [0,0], 0.185714, 0, 1, \
			"false", "false", "true", "false", "true").getElement())

	childGraph.addNode(\
		createSplitAxisNode(doc, 7, [0,0], "false", "true").getElement())

	childGraph.addNode(\
		createDefaultSplitPaneNode(doc, "centersplitpane", "splitpane", 8, [0,0], \
			"false", 0.5, "false").getElement())

	childGraph.addNode(\
		createSplitValueNode(doc, 9, [0,0], 0.610895, 0, 1, \
			"false", "false", "true", "false", "true").getElement())

	childGraph.addNode(\
		createSplitAxisNode(doc, 10, [0,0], "true", "true").getElement())

	childGraph.addNode(\
		createDefaultWorkPaneNode(doc, 11, [0,0], "false", "false", 3).getElement())

	childGraph.addNode(\
		createNode(doc, "%%NODEGRAPHEDITORPANESTATE NODE%%", "nodegrapheditorpanestate", 12, [0, 0]).getElement())

	childGraph.addNode(\
		createNode(doc, "%%NODESTACKEDITORPANESTATE NODE%%", "nodestackeditorpanestate", 13, [0, 0]).getElement())

	childGraph.addNode(\
		createNode(doc, "%%RENDERERPANESTATE NODE%%", "rendererpanestate", 14, [0, 0]).getElement())

	childGraph.addNode(\
		createNode(doc, "%%OUTLINERPANESTATE NODE%%", "outlinerpanestate", 15, [0, 0]).getElement())

	childGraph.addNode(\
		createDefaultWorkPaneNode(doc, 16, [0,0], "false", "false", 4).getElement())

	childGraph.addNode(\
		createNode(doc, "%%NODEGRAPHEDITORPANESTATE NODE%%", "nodegrapheditorpanestate", 17, [0, 0]).getElement())

	childGraph.addNode(\
		createNode(doc, "%%NODESTACKEDITORPANESTATE NODE%%", "nodestackeditorpanestate", 18, [0, 0]).getElement())

	childGraph.addNode(\
		createNode(doc, "%%RENDERERPANESTATE NODE%%", "rendererpanestate", 19, [0, 0]).getElement())

	childGraph.addNode(\
		createNode(doc, "%%OUTLINERPANESTATE NODE%%", "outlinerpanestate", 20, [0, 0]).getElement())

	childGraph.addNode(\
		createDefaultWorkPaneNode(doc, 21, [0,0], "false", "false", 1).getElement())

	childGraph.addNode(\
		createNode(doc, "%%NODEGRAPHEDITORPANESTATE NODE%%", "nodegrapheditorpanestate", 22, [0, 0]).getElement())

	childGraph.addNode(\
		createNode(doc, "%%NODESTACKEDITORPANESTATE NODE%%", "nodestackeditorpanestate", 23, [0, 0]).getElement())

	childGraph.addNode(\
		createNode(doc, "%%RENDERERPANESTATE NODE%%", "rendererpanestate", 24, [0, 0]).getElement())

	childGraph.addNode(\
		createNode(doc, "%%OUTLINERPANESTATE NODE%%", "outlinerpanestate", 25, [0, 0]).getElement())

	childGraph.addNode(\
		createDefaultWorkPaneNode(doc, 26, [0,0], "false", "false", 1).getElement())

	childGraph.addNode(\
		createNode(doc, "%%NODEGRAPHEDITORPANESTATE NODE%%", "nodegrapheditorpanestate", 27, [0, 0]).getElement())

	childGraph.addNode(\
		createNode(doc, "%%NODESTACKEDITORPANESTATE NODE%%", "nodestackeditorpanestate", 28, [0, 0]).getElement())

	childGraph.addNode(\
		createNode(doc, "%%RENDERERPANESTATE NODE%%", "rendererpanestate", 29, [0, 0]).getElement())

	childGraph.addNode(\
		createNode(doc, "%%OUTLINERPANESTATE NODE%%", "outlinerpanestate", 30, [0, 0]).getElement())

	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 3, 0, 2, 2 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 4, 0, 2, 3 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 2, 0, 1, 0 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 6, 0, 5, 2 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 7, 0, 5, 3 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 5, 0, 2, 0 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 9, 0, 8, 2 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 10, 0, 8, 3 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 8, 0, 5, 1 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 12, 0, 11, 1 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 13, 0, 11, 2 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 14, 0, 11, 3 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 15, 0, 11, 4 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 15, 0, 11, 0 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 11, 0, 5, 0 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 17, 0, 16, 1 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 18, 0, 16, 2 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 19, 0, 16, 3 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 20, 0, 16, 4 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 19, 0, 16, 0 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 16, 0, 8, 0 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 22, 0, 21, 1 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 23, 0, 21, 2 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 24, 0, 21, 3 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 25, 0, 21, 4 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 22, 0, 21, 0 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 21, 0, 8, 1 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 27, 0, 26, 1 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 28, 0, 26, 2 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 29, 0, 26, 3 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 30, 0, 26, 4 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 28, 0, 26, 0 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 26, 0, 2, 1 ))
	return systemNode

#nodeGraph.addNode(createSystemNode(doc).getElement())

#--------------------------Preview Configuration-------------------------------
def createPreviewConfigurationNode(doc):
	previewConfig = createNode(doc, "Preview Configuration", "macro", 2, [0.1, 0.1])

	childGraph = OCSNodeGraph(doc)
	childGraph.setName("Preview Configuration")
	childGraph.setCurrentNewNodeId(8)
	childGraph.setCurrentNewNodePinConnectionId(7)
	previewConfig.addChildGraph(childGraph.getElement())

	childGraph.addNode(createDefaultPreviewRenderTargetNode(doc, 1, [0.296, 1.0625]).getElement())
	childGraph.addNode(createDefaultPreviewCameraNode(doc, 2, [0.166, 0.68]).getElement())
	childGraph.addNode(createDefaultPreviewResolutionNode(doc, 4, [0.197, 0.5025]).getElement())
	childGraph.addNode(createDefaultMeshPreviewImager(doc, 5, [0.272, 0.8725]).getElement())
	childGraph.addNode(createDefaultMeshPreviewKernel(doc, 6, [0.461, 0.8725]).getElement())
	childGraph.addNode(createDefaultPreviewEnvironment(doc, 7, [0.403, 0.68]).getElement())

	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 2, 0, 1, 0 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 4, 0, 1, 1 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 5, 0, 1, 3 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 6, 0, 1, 4 ))
	childGraph.addNodePinConnection(createNodePinConnectionElement(doc, 7, 0, 1, 2 ))

	return previewConfig

#nodeGraph.addNode(createPreviewConfigurationNode(doc).getElement())



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
				texture = get_exported_texture(buf, mat) + ".png"

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


def createObjNode(doc, objrelativepath):
	objNode = createNode(doc, "out.obj", "mesh", 6, [0.558, 0.4325])

	objNode.setObjMeshImportParams(createDefaultObjImportParams(doc).getElement())
	objNode.setLinkedFileName(objrelativepath.replace("/","\\"))

	
	materialPin = MaterialNodePin(doc, "material_0_0", 0, "diffuse")
	materialPin.setDiffuse([1, 0, 0])
	materialPin.setSmooth(True)
	materialPin.setBump(False)
	materialPin.setOpacity(1)

	objNode.addInputNodePin(materialPin.getElement())
	return objNode


def export_ocs(ocspath, objrelativepath):
	if os.path.isfile(ocspath):
		os.remove(ocspath)
	
	ocsfile = open(ocspath, 'a')
	doc = minidom.Document()
	scene = OCSScene(doc)
	rootNode = createNode(doc, "mmdbridge_ocs", "projectroot", 0, [0, 0])
	nodeGraph = OCSNodeGraph(doc)
	nodeGraph.setName("mmdbridge_ocs")
	nodeGraph.setCurrentNewNodeId(7)
	nodeGraph.setCurrentNewNodePinConnectionId(1)
	rootNode.addChildGraph(nodeGraph.getElement())
	scene.addNode(rootNode.getElement())
	nodeGraph.addNode(createPreviewConfigurationNode(doc).getElement())
	nodeGraph.addNode(createSystemNode(doc).getElement())
	nodeGraph.addNode(createObjNode(doc, objrelativepath).getElement())

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
				texture = get_exported_texture(buf, mat) + ".png"

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

	ocsfile.write(scene.toxml())

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
objfilename = 'out.obj'
mtlfilename = 'out.mtl'
ocsfilename = 'out.ocs'
objpath = tmppath + objfilename
mtlpath = tmppath + mtlfilename
ocspath = tmppath + ocsfilename
objrelativepath = "tmp/" + objfilename
texture_export_dir = tmppath

export_mtl(mtlpath)
copy_textures(texture_export_dir.replace("/", "\\"))
export_uncopied_textures(texture_export_dir.replace("/", "\\"), "png")
export_obj(objpath, mtlfilename)
execute_octane(outpath, octanepath, objpath, mtlpath, 256)