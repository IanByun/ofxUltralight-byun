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
		
		ofxUltralight::Ptr web_loader;
		ofFbo		perspectiveFrame;

		unsigned int quadVAO, quadVBO;
		vector<float> quadVertices = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			0.0f,  500.0f,  0.0f, 1.0f,
			0.0f, 0.0f,  0.0f, 0.0f,
			1000.0f, 0.0f,  1.0f, 0.0f,

			0.0f,  500.0f,  0.0f, 1.0f,
			1000.0f, 0.0f,  1.0f, 0.0f,
			1000.0f,  500.0f,  1.0f, 1.0f
		};

		const int width = 960, height = 540;
		const std::vector<BYTE> frame_data = std::vector<BYTE>(width * height * 4);
		
		GLuint GenerateReadPBO(int width, int height) {
			GLuint  pbo;
			glGenBuffers(1, &pbo);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
			glBufferData(GL_PIXEL_PACK_BUFFER, width*height * 4, NULL, GL_STREAM_READ);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
			return pbo;
		}
		
		void ReadTexture(GLuint tex_id, GLuint pbo_id, OUT const std::vector<BYTE>& pixel_data) {
			glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_id);
			glBindTexture(GL_TEXTURE_2D, tex_id);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				(GLvoid*)0 // offset in bytes into "buffer", not pointer to client memory!
			);
			glBufferData(GL_PIXEL_PACK_BUFFER, pixel_data.size(), 0, GL_STREAM_READ); //GL_STREAM_READ ¸»°í GL_STATIC_READ ÇÏ¸é ±ôºý±ôºý!

			GLubyte* dma_ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
			if (dma_ptr) { 	// update data directly on the mapped buffer
				memcpy((void*)pixel_data.data(), dma_ptr, 960 * 540 * 4);
				glUnmapBuffer(GL_PIXEL_PACK_BUFFER); // release the mapped buffer
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		}

		const GLuint pbo_id = GenerateReadPBO(width, height);
};
