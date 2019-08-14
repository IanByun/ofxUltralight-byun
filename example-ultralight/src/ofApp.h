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

		const int width = 960, height = 540;
		const std::vector<BYTE> frame_data = std::vector<BYTE>(width * height * 4);
		
		GLuint GeneratePBOReader(int width, int height, int numChannels = 4) {
			int data_size = width*height * numChannels;
			GLuint  pbo;
			glGenBuffers(1, &pbo);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
			glBufferData(GL_PIXEL_PACK_BUFFER, width*height * 4, NULL, GL_STREAM_READ);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
			return pbo;
		}
		
		void ReadTextureToPBO(GLuint tex_id, GLuint pbo_id, OUT const std::vector<BYTE>& pixel_data) {
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
			glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		}

		void CopyTextureFromFBO(GLuint fbo_id, OUT const ofTexture& tex) {
			int tex_id = tex.texData.textureID;
			int tex_target = tex.texData.textureTarget;
			int tex_format = tex.texData.glInternalFormat;

			glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
			glBindTexture(tex_target, tex_id);
			//glCopyTexImage2D(tex_target, 0, tex_format, 0, 0, width, height, 0);
			glCopyTexSubImage2D(tex_target, 0, 0, 0, 0, 0, width, height);
			glBindTexture(tex_target, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		const GLuint pbo_id = GeneratePBOReader(width, height);

		ofTexture of_tex;
};
