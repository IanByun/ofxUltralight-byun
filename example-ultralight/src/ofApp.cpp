#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	web_loader = make_shared<ofxUltralight>();

	web_loader->requestCreateView(960, 540, "https://m.search.naver.com/search.naver?query=%EB%84%A4%EC%9D%B4%EB%B2%84+%EC%8B%9C%EA%B3%84");
	web_loader->requestCreateView(960, 540, "https://m.weather.naver.com/m/main.nhn?regionCode=09140104&lang=en");
	web_loader->requestCreateView(960, 540, "https://www.google.com/search?q=calculator");
	web_loader->requestCreateView(960, 540, "https://www.apple.com/kr/iphone/");
	
	//유튜브는 여전히 안나오는군
	//web_loader->requestCreateView(960, 540, "https://www.youtube.com/embed/UOxkGD8qRB4?autoplay=1");
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
		web_assets[0].tex.draw(0, 0, 960, 540);
		web_assets[1].tex.draw(960, 0, 960, 540);
		web_assets[2].tex.draw(0, 540, 960, 540);
		web_assets[3].tex.draw(960, 540, 960, 540);

		cv::imshow("web_assets[0].mat_bgr", web_assets[0].mat_bgr);
		cv::imshow("web_assets[1].mat_bgr", web_assets[1].mat_bgr);
		cv::imshow("web_assets[2].mat_bgr", web_assets[2].mat_bgr);
		cv::imshow("web_assets[3].mat_bgr", web_assets[3].mat_bgr);
	}
	cv::waitKey(1);
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
