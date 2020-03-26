#pragma once

#include <ofMain.h>
#include <ofxUltralight.hpp>

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseScrolled(int x, int y, float scrollX, float scrollY);

	ultralight::Config ultralight_config;
	ofxUltralight::Ptr web_loader;

	function<bool(int, int, ofRectangle)> in_range = [](int x, int y, ofRectangle rect)->bool {
		return x >= rect.x && x <= (rect.x + rect.width)
			&& y >= rect.y && y <= (rect.y + rect.height);
	};

	ofRectangle screen;

};
