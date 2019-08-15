#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	web_loader = make_shared<ofxUltralight>();

	web_loader->createViewAsset(regions[0].width, regions[0].height, "https://m.search.naver.com/search.naver?query=%EB%84%A4%EC%9D%B4%EB%B2%84+%EC%8B%9C%EA%B3%84");
	web_loader->createViewAsset(regions[1].width, regions[1].height, "https://m.weather.naver.com/m/main.nhn?regionCode=09140104&lang=en");
	web_loader->createViewAsset(regions[2].width, regions[2].height, "https://www.google.com/search?q=calculator");
	web_loader->createViewAsset(regions[3].width, regions[3].height, "https://www.apple.com/kr/iphone/");
	
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
		web_assets[0].tex->draw(regions[0]);
		web_assets[1].tex->draw(regions[1]);
		web_assets[2].tex->draw(regions[2]);
		web_assets[3].tex->draw(regions[3]);

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
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	ultralight::MouseEvent mouse_event;
	mouse_event.type = ultralight::MouseEvent::kType_MouseDown;
	mouse_event.button = ultralight::MouseEvent::Button(OF_MOUSE_BUTTON_LEFT + 1);
	mouse_event.x = x;
	mouse_event.y = y;

	auto web_assets = web_loader->getViewAssets();

	for (int i = 0; i < regions.size();i++) {
		const auto& region = regions[i];

		if (in_range(x, y, region)) {

			mouse_event.x -= region.x;
			mouse_event.y -= region.y;

			web_assets[i].view->FireMouseEvent(mouse_event);
			break;
		}
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

	for (int i = 0; i < regions.size(); i++) {
		const auto& region = regions[i];

		if (in_range(x, y, region)) {

			mouse_event.x -= region.x;
			mouse_event.y -= region.y;

			web_assets[i].view->FireMouseEvent(mouse_event);
			break;
		}
	}
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

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
	ultralight::ScrollEvent scroll_event;
	scroll_event.type = ultralight::ScrollEvent::kType_ScrollByPixel;
	scroll_event.delta_x = scrollX * 30;
	scroll_event.delta_y = scrollY * 30;

	auto web_assets = web_loader->getViewAssets();

	for (int i = 0; i < regions.size(); i++) {
		const auto& region = regions[i];

		if (in_range(x, y, region)) {

			web_assets[i].view->FireScrollEvent(scroll_event);
			break;
		}
	}
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}
