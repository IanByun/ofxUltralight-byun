#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	web_loader = make_shared<ofxUltralight>();

	//web_loader->requestCreateView(960, 540, "https://m.search.naver.com/search.naver?query=%EB%84%A4%EC%9D%B4%EB%B2%84+%EC%8B%9C%EA%B3%84");
	//web_loader->requestCreateView(960, 540, "https://m.weather.naver.com/m/main.nhn?regionCode=09140104&lang=en");
	web_loader->requestCreateView(960, 540, "https://www.google.com/search?q=calculator");
	//web_loader->requestCreateView(960, 540, "https://www.apple.com/kr/iphone/");

	// screen quad VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * quadVertices.size(), quadVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

}

//--------------------------------------------------------------
void ofApp::update() {
	//web_loader->update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofClear(ofColor::black);

	//for (int i = 0; i < 100; i++)
		web_loader->update();

	auto web_assets = web_loader->getViewAssets();

	auto driver = static_cast<GPUDriverGL*>(web_loader->gpu_driver);

	auto buffer_map = driver->GetFrameBuffer();

	if (buffer_map.size() != 0) {
		GLint fbo_id = buffer_map.begin()->first;
		GLint tex_id = buffer_map.begin()->second;

		if (false) { //easy, naive, slow
			glBindTexture(GL_TEXTURE_2D, tex_id);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)frame_data.data());
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else {
			ReadTexture(tex_id, pbo_id, frame_data);
		}

		cv::Mat bmat = cv::Mat(540, 960, CV_8UC4, (void*)frame_data.data());
		cv::cvtColor(bmat, bmat, cv::COLOR_RGBA2BGR);

		cv::imshow("ast.mat", bmat);
		cv::waitKey(1);

		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

		//// clear all relevant buffers
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		//glClear(GL_COLOR_BUFFER_BIT);

		//glBindVertexArray(quadVAO);
		//glBindTexture(GL_TEXTURE_2D, tex_id);	// use the color attachment texture as the texture of the quad plane
		//glDrawArrays(GL_TRIANGLES, 0, 6);
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
