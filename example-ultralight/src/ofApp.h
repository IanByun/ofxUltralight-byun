#pragma once

#include <ofMain.h>
#include <opencv.hpp>
#include <ofxUltralight.hpp>

class ofApp : public ofBaseApp {

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
		
		ofxUltralight::Ptr web_loader;

		function<bool(int, int, ofRectangle)> in_range = [](int x, int y, ofRectangle rect)->bool {
			return x >= rect.x && x <= (rect.x + rect.width)
				&& y >= rect.y && y <= (rect.y + rect.height);
		};

		vector<ofRectangle> regions = {
			ofRectangle(0, 0, 960, 540),
			ofRectangle(960, 0, 960, 540),
			ofRectangle(0, 540, 960, 540),
			ofRectangle(960, 540, 960, 540)
		};
};
