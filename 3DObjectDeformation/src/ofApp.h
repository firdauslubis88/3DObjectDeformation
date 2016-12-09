#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "GreenCoordinate.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void mouseScrolled(int x, int y, float scrollX, float scrollY);

		void save(string name);
		void checkCage();

		void checkCageParts();

		ofMesh mesh, tempCageMesh, testCageMesh, testMesh;
		ofImage image;
		ofEasyCam easyCam;

		ofxAssimpModelLoader model;
		ofxAssimpModelLoader cage;
		ofxAssimpModelLoader testCage;

		bool toggleShowWire = false, toggleLock = true;
		ofBoxPrimitive box;
		float xBox, yBox, zBox;
		float xSize, ySize;

		ofFile cageParts, modelParts;

		GreenCoordinate GC;
		vector<ofPolyline> polyLines, normalFaceLines;
};
