#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	ofGLFWWindowSettings main_settings;
	main_settings.setSize(1920, 1080);
	main_settings.windowMode = OF_WINDOW;
	main_settings.setGLVersion(4, 5);
	ofCreateWindow(main_settings);
	ofRunApp(new ofApp());
}
