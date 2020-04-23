#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	//³ª´®°íµñ
	ultralight_config.font_family_standard = "Verdana";
	//Samsung Browser 7.2 on Android 8.1 Samsung Galaxy S9 Plus 
	ultralight_config.user_agent = "Mozilla/5.0 (Linux; Android 8.1.0; SM-G965F Build/OPM2.171019.029) AppleWebKit/537.36 (KHTML, like Gecko) SamsungBrowser/7.2 Chrome/59.0.3071.125 Mobile Safari/537.36";
	web_loader = make_shared<ofxUltralight>(&ultralight_config);
	screen = ofRectangle(0, 0, 1200, 800);
	web_loader->createViewAsset(screen.width, screen.height, "https://www.iwanson.de");
}

//--------------------------------------------------------------
void ofApp::update() {
	web_loader->update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofClear(ofColor::black);
	auto web_assets = web_loader->getViewAssets();
	if (web_assets.size()) {
		web_assets[0].tex->draw(screen);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	ultralight::MouseEvent mouse_event;
	mouse_event.type = ultralight::MouseEvent::kType_MouseDown;
	mouse_event.button = ultralight::MouseEvent::Button(OF_MOUSE_BUTTON_LEFT + 1);
	mouse_event.x = x;
	mouse_event.y = y;

	auto web_assets = web_loader->getViewAssets();
	if (in_range(x, y, screen)) {

		mouse_event.x -= screen.x;
		mouse_event.y -= screen.y;

		web_assets[0].view->FireMouseEvent(mouse_event);
	}

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	ultralight::MouseEvent mouse_event;
	mouse_event.type = ultralight::MouseEvent::kType_MouseUp;
	mouse_event.button = ultralight::MouseEvent::Button(OF_MOUSE_BUTTON_LEFT + 1);
	mouse_event.x = x;
	mouse_event.y = y;

	auto web_assets = web_loader->getViewAssets();

	if (in_range(x, y, screen)) {

		mouse_event.x -= screen.x;
		mouse_event.y -= screen.y;

		web_assets[0].view->FireMouseEvent(mouse_event);
	}

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
	ultralight::ScrollEvent scroll_event;
	scroll_event.type = ultralight::ScrollEvent::kType_ScrollByPixel;
	scroll_event.delta_x = scrollX * 30;
	scroll_event.delta_y = scrollY * 30;

	auto web_assets = web_loader->getViewAssets();

	if (in_range(x, y, screen)) {
		web_assets[0].view->FireScrollEvent(scroll_event);
	}

}
