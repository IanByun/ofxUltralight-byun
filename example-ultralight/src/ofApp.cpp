#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	web_loader = make_shared<ofxUltralight>();

	web_loader->requestCreateView(960, 540, "https://m.search.naver.com/search.naver?query=%EB%84%A4%EC%9D%B4%EB%B2%84+%EC%8B%9C%EA%B3%84");
	web_loader->requestCreateView(960, 540, "https://m.weather.naver.com/m/main.nhn?regionCode=09140104&lang=en");
	web_loader->requestCreateView(960, 540, "https://www.google.com/search?q=calculator");
	web_loader->requestCreateView(960, 540, "https://www.apple.com/kr/iphone/");

	of_tex.allocate(width, height, GL_RGB8, ofGetUsingArbTex(), GL_RGBA, GL_UNSIGNED_BYTE);

	of_img.loadImage(R"(C:\Users\junghyun\Pictures\2altered.PNG)");
}

//--------------------------------------------------------------
void ofApp::update() {
	//of_img.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofClear(ofColor::black);

	web_loader->update();
	CHECK_GL();

	auto web_assets = web_loader->getViewAssets();
	auto driver = dynamic_pointer_cast<GPUDriverGL>(web_loader->gpu_driver);
	
	auto frame_map = driver->GetFrameMap();
	auto texture_map = driver->GetTextureMap();

	if (frame_map.size() != 0) {
		auto render_target = web_assets[0].view->render_target();
		GLint fbo_id = frame_map[render_target.render_buffer_id];
		GLint tex_id = texture_map[render_target.texture_id];

		ReadTextureToPBO(tex_id, pbo_id, mat_rgba);
		CHECK_GL();
		
		CopyTextureFromFBO(fbo_id, of_tex);
		CHECK_GL();

		of_tex.draw(0, 0, width, height);

		cv::cvtColor(mat_rgba, mat_bgr, cv::COLOR_RGBA2BGR);

		cv::imshow("mat_bgr", mat_bgr);
		cv::waitKey(1);
	}

	/*if (web_assets.size() == 4) {
		web_assets[0].tex.draw(0, 0, 960, 540);
		web_assets[1].tex.draw(960, 0, 960, 540);
		web_assets[2].tex.draw(0, 540, 960, 540);
		web_assets[3].tex.draw(960, 540, 960, 540);
	}*/
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
