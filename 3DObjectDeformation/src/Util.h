#pragma once

#include "ofMain.h"
class Util
{
public:
	Util();
	~Util();

	static bool upperLowerPlaneTest(ofVec3f testPoint, ofVec3f planeNormal, ofVec3f planePoint);
};

