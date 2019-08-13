#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	web_loader = make_shared<ofxUltralight>();

	web_loader->requestCreateView(
		1280, 720, "https://m.search.naver.com/search.naver?query=%EB%84%A4%EC%9D%B4%EB%B2%84+%EC%8B%9C%EA%B3%84");
	web_loader->requestCreateView(
		1280, 720, "https://m.weather.naver.com/m/main.nhn?regionCode=09140104&lang=en");
	web_loader->requestCreateView(
		1280, 720, "https://www.google.com/search?q=calculator");
	web_loader->requestCreateView(
		1280, 720, "https://www.apple.com/kr/iphone/");
}

//--------------------------------------------------------------
void ofApp::update() {
	web_loader->update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofClear(ofColor::black);

	auto web_assets = web_loader->getViewAssets();

	if (web_assets.size() == 4) {
		web_assets[0].tex.draw(0, 0, 640, 360);
		web_assets[1].tex.draw(640, 0, 640, 360);
		web_assets[2].tex.draw(0, 360, 640, 360);
		web_assets[3].tex.draw(640, 360, 640, 360);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key)
	{
	case OF_KEY_RETURN: web_loader->update();
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}
