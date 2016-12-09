#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	cage.loadModel("FinalBaseMesh_max_green.obj", true);
	model.loadModel("FinalBaseMesh.obj");
	testCage.loadModel("FinalBaseMesh_max_green2.obj", true);
	auto lines = ofSplitString(ofBufferFromFile("cageVertices.txt").getText(), "\n");

	GC.initCage(cage.getMesh(0));
	GC.initModel(model.getMesh(0));

	testMesh.setMode(OF_PRIMITIVE_POINTS);
	auto testMapResult = GC.modelPartsVerticesMap.find("cage finger");
	auto modelVertices = testMapResult->second;
	for (size_t i = 0; i < modelVertices.size(); i++)
	{
		ofVec3f vertex = modelVertices[i];
		testMesh.addVertex(vertex);
	}

//	checkCage();
	checkCageParts();

	mesh = model.getMesh(0);
	testCageMesh = testCage.getMesh(0);
	xSize = 2.5;
	ySize = 2.5;

	box.setDepth(4);
	box.setHeight(ySize);
	box.setWidth(xSize);
	xBox = 0;
	yBox = 0;
	zBox = 0;

	//add color to mesh
	std::vector<ofIndexType> allIndices = mesh.getIndices();

	for (size_t i = 0; i < mesh.getNumVertices(); i++)
	{
		mesh.addColor(ofFloatColor(0, 0, 255));
	}

	tempCageMesh.setMode(OF_PRIMITIVE_POINTS);

	for (size_t i = 0; i < lines.size(); i++)
	{
		float x, y, z;
		auto values = ofSplitString(lines[i], " ");
		istringstream(values[0]) >> x;
		istringstream(values[1]) >> y;
		istringstream(values[2]) >> z;

		tempCageMesh.addVertex(ofVec3f(x,y,z));
		tempCageMesh.addColor(ofFloatColor(255, 0, 0));
	}

	ofDisableArbTex();
	ofEnableDepthTest();
//	easyCam.setPosition(0, 10, 36);

	/*
	image.load("stars.png");
	image.resize(200, 200);

	mesh.setMode(OF_PRIMITIVE_LINES);
	mesh.enableColors();
	mesh.enableIndices();

	float intensityThreshold = 150.0;
	int w = image.getWidth();
	int h = image.getHeight();

	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			ofColor c = image.getColor(x, y);
			float intensity = c.getLightness();
			if (intensity > intensityThreshold)
			{
				float saturation = c.getSaturation();
				float z = ofMap(saturation, 0, 255, -100, 100);
				ofVec3f pos(x * 4, y * 4, z);
				mesh.addVertex(pos);
				mesh.addColor(c);
			}
		}
	}

	// Let's add some lines!
	float connectionDistance = 30;
	int numVerts = mesh.getNumVertices();
	for (int a=0; a<numVerts; ++a) {
		ofVec3f verta = mesh.getVertex(a);
		for (int b=a+1; b<numVerts; ++b) {
			ofVec3f vertb = mesh.getVertex(b);
			float distance = verta.distance(vertb);
			if (distance <= connectionDistance) {
				// In OF_PRIMITIVE_LINES, every pair of vertices or indices will be
				// connected to form a line
				mesh.addIndex(a);
				mesh.addIndex(b);
			}
		}
	}
	*/
//	easyCam.setOrientation(ofVec3f(90, -180, 0));
//	easyCam.setGlobalPosition(0, 10, 0);
}

//--------------------------------------------------------------
void ofApp::update(){
	box.setPosition(xBox, yBox, zBox);
	if (ySize <= 0)
	{
		ySize = 0.25;
	}
	box.setHeight(ySize);
	if (xSize <= 0)
	{
		xSize = 0.25;
	}
	box.setWidth(xSize);
}

//--------------------------------------------------------------
void ofApp::draw(){
	/*
	ofColor centerColor = ofColor(85, 78, 68);
	ofColor edgeColor(0, 0, 0);
	ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);

	easyCam.begin();
	ofPushMatrix();
	ofTranslate(-ofGetWidth() / 2, -ofGetHeight() / 2);
	mesh.draw();
	ofPopMatrix();
	easyCam.end();
	*/
//	easyCam.setPosition(0, -400, -185);
	if (toggleLock)
	{
		easyCam.setPosition(0, 10, 36);
	}

	easyCam.setTarget(ofVec3f(0, 10, 6));

	easyCam.begin();
//	model.drawFaces();

	tempCageMesh.drawFaces();
//	testCageMesh.drawWireframe();
	if (!toggleShowWire)
	{
		mesh.drawFaces();
//		testMesh.drawFaces();
	}
	else
	{
		mesh.drawWireframe();
	}
	box.draw();
	for (size_t i = 0; i < polyLines.size(); i++)
	{
		polyLines[i].draw();
	}
	for (size_t i = 0; i < normalFaceLines.size(); i++)
	{
		normalFaceLines[i].draw();
	}

	easyCam.end();
	ofQuaternion orientation = easyCam.getGlobalOrientation();
	ofVec3f position = easyCam.getGlobalPosition();
	ofDrawBitmapString("Camera Information:", ofPoint(10, 10));
	ofDrawBitmapString("X angle: " + ofToString(orientation.getEuler().x,3), ofPoint(20, 20));
	ofDrawBitmapString("Y angle: " + ofToString(orientation.getEuler().y,3), ofPoint(20, 30));
	ofDrawBitmapString("Z angle: " + ofToString(orientation.getEuler().z,3), ofPoint(20, 40));
	ofDrawBitmapString("X Position: " + ofToString(position.x, 3), ofPoint(20, 60));
	ofDrawBitmapString("Y Position: " + ofToString(position.y, 3), ofPoint(20, 70));
	ofDrawBitmapString("Z Position: " + ofToString(position.z, 3), ofPoint(20, 80));

	ofDrawBitmapString("Box X Position: " + ofToString(box.getPosition().x, 3), ofPoint(10, 100));
	ofDrawBitmapString("Box Y Position: " + ofToString(box.getPosition().y, 3), ofPoint(10, 110));
	ofDrawBitmapString("Box Z Position: " + ofToString(box.getPosition().z, 3), ofPoint(10, 120));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
		case ' ':
			toggleLock = !toggleLock;
			break;
		case OF_KEY_UP:
			yBox+=0.1;
			break;
		case OF_KEY_DOWN:
			yBox-= 0.02;
			break;
		case OF_KEY_RIGHT:
			xBox+= 0.02;
			break;
		case OF_KEY_LEFT:
			xBox-= 0.125;
			break;
		case OF_KEY_HOME:
			zBox-= 0.125;
			break;
		case OF_KEY_END:
			zBox+= 0.125;
			break;
		case 'w':
			ySize += 0.25;
			break;
		case 's':
			ySize -= 0.25;
			break;
		case 'a':
			xSize -= 0.25;
			break;
		case 'd':
			xSize += 0.25;
			break;
		case 'p':
			save("finger");
		case 'o':
			save("palm");
		case 'i':
			save("fore-arm");
		case 'u':
			save("upper-arm");
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
}

void ofApp::save(string name)
{
	cageParts.open("cage " + name, ofFile::WriteOnly);
	for (size_t i = 0; i < tempCageMesh.getNumVertices(); i++)
	{
		float x, y, z;
		float xLower, xUpper;
		x = tempCageMesh.getVertex(i).x;
		y = tempCageMesh.getVertex(i).y;
		z = tempCageMesh.getVertex(i).z;
		xLower = box.getPosition().x - box.getWidth() / 2;
		xUpper = box.getPosition().x + box.getWidth() / 2;
		if (x >= xLower && x <= xUpper)
		{
			float yLower, yUpper;
			yLower = box.getPosition().y - box.getHeight() / 2;
			yUpper = box.getPosition().y + box.getHeight() / 2;
			if (y >= yLower && y <= yUpper)
			{
				cageParts << ofToString(x) << " " << ofToString(y) << " " << ofToString(z) << " " << endl;
			}
		}
	}
	/*
	modelParts.open("model " + name, ofFile::WriteOnly);
	for (size_t i = 0; i < mesh.getNumVertices(); i++)
	{
		float x, y, z;
		float xLower, xUpper;
		x = mesh.getVertex(i).x;
		y = mesh.getVertex(i).y;
		z = mesh.getVertex(i).z;
		xLower = box.getPosition().x - box.getWidth() / 2;
		xUpper = box.getPosition().x + box.getWidth() / 2;
		if (x >= xLower && x <= xUpper)
		{
			float yLower, yUpper;
			yLower = box.getPosition().y - box.getHeight() / 2;
			yUpper = box.getPosition().y + box.getHeight() / 2;
			if (y >= yLower && y <= yUpper)
			{
				modelParts << ofToString(i) << endl;
			}
		}
	}
	*/
}

void ofApp::checkCage()
{
	/*
	int faceNormalCount = 0;
	for (size_t i = 0; i < faceNormals.size(); i+=3)
	{
	cout << faceNormals[i] << endl;
	//		faceNormalCount++;
	}
	//	cout << faceNormalCount << endl;
	*/

	for (size_t i = 0; i < GC.numMeshFaces; i++)
	{
		ofMeshFace meshFace = GC.meshFaces[i];
		ofVec3f a = meshFace.getVertex(0);
		ofVec3f b = meshFace.getVertex(1);
		ofVec3f c = meshFace.getVertex(2);
		ofVec3f ba = b - a;
		ofVec3f ca = c - a;
		ofVec3f d = ba.cross(ca);
		cout << "Manual Calculation for index " << i << "\t:" << d.normalized() << endl;
		cout << "From faceNormals for index " << i << "\t\t:" << GC.meshFacesNormal[i] << endl;
		/*
		for (size_t j = 0; j < 3; j++)
		{
		cout << meshFace.getVertex(j) << endl;
		}
		*/
	}
	//	cout << cageMesh.getUniqueFaces().size() << endl;
}

void ofApp::checkCageParts()
{
	std::map<string,vector<pair<ofMeshFace,ofPoint>>> fingerMap = GC.cagePartsVerticesMap;
	auto testMapResult = fingerMap.find("cage finger");
	auto meshFaces = testMapResult->second;
	for (size_t i = 0; i < meshFaces.size(); i++)
	{
		ofMeshFace meshFace = meshFaces[i].first;
		ofVec3f a = meshFace.getVertex(0);
		ofVec3f b = meshFace.getVertex(1);
		ofVec3f c = meshFace.getVertex(2);
		ofVec3f ba = b - a;
		ofVec3f ca = c - a;
		ofVec3f d = ba.cross(ca);
//		cout << "Manual Calculation for index " << i << "\t:" << d.normalized() << endl;
//		if (i == 0)
//		{
			ofPolyline polyLine;
			polyLine.addVertex(a);
			polyLine.addVertex(b);
			polyLine.addVertex(c);
			polyLine.close();
			polyLines.push_back(polyLine);

			ofPolyline normalPolyLine;
			normalPolyLine.addVertex(a);
			normalPolyLine.addVertex(a + meshFaces[i].second);
			normalPolyLine.close();
			normalFaceLines.push_back(normalPolyLine);

//			cout << meshFaces[i].second << endl;
//		}
	}

//	cout << "In ofApp:\t" << meshFaces.size() << endl;

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
