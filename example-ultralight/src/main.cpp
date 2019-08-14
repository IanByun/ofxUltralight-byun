#include "ofMain.h"
#include "ofApp.h"

int main() {
	ofGLFWWindowSettings main_settings;
	main_settings.width = 1920;
	main_settings.height = 1080;
	main_settings.windowMode = OF_WINDOW;
	main_settings.setGLVersion(4, 5);
	auto mainWindow = ofCreateWindow(main_settings);

	shared_ptr<ofApp> mainApp = make_shared<ofApp>();

	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();

	/*ofSetupOpenGL(1920, 1080, OF_WINDOW);
	ofRunApp(new ofApp());*/

	/*ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1920, 1080, OF_WINDOW);
	ofRunApp(new ofApp());*/
}