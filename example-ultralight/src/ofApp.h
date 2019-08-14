#pragma once

#include <ofMain.h>
#include <opencv.hpp>
#include <ofxUltralight.hpp>

#include <Windows.h>
#define FATAL(x) { std::stringstream str; \
  str << "[ERROR] " << __FUNCSIG__ << " @ Line " << __LINE__ << ":\n\t" << x << std::endl; \
  OutputDebugStringA(str.str().c_str()); \
  std::cerr << str.str() << std::endl; \
  /*__debugbreak();*/ }

static inline char const* glErrorString(GLenum const err) noexcept
{
	switch (err)
	{
		// OpenGL 2.0+ Errors:
	case GL_NO_ERROR: return "GL_NO_ERROR";
	case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
	case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
	case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
		// OpenGL 3.0+ Errors
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
	default: return "UNKNOWN ERROR";
	}
}

#define CHECK_GL()  {if (GLenum err = glGetError()) FATAL(glErrorString(err)) }

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
		std::vector<BYTE> frame_data = std::vector<BYTE>(width * height * 4);
		
		GLuint GeneratePBOReader(int width, int height, int numChannels = 4) {
			int data_size = width * height * numChannels;
			GLuint  pbo;
			glGenBuffers(1, &pbo);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
			glBufferData(GL_PIXEL_PACK_BUFFER, data_size, NULL, GL_STREAM_READ);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
			return pbo;
		}
		
		void ReadTextureToPBO(GLuint tex_id, GLuint pbo_id, OUT std::vector<BYTE>& pixel_data) {
			int data_size = pixel_data.size(); //어차피 1byte 원소

			glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_id);
			glBindTexture(GL_TEXTURE_2D, tex_id);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				(GLvoid*)0 // buffer에서 시작하는 byte 단위 offset. cpu memory가 아니다!
			);
			glBufferData(GL_PIXEL_PACK_BUFFER, data_size, 0, GL_STREAM_READ); //GL_STREAM_READ 말고 GL_STATIC_READ 하면 깜빡깜빡!

			GLubyte* dma_ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
			if (dma_ptr) {
				memcpy((void*)pixel_data.data(), dma_ptr, data_size);
				glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		}

		void ReadTextureToPBO(GLuint tex_id, GLuint pbo_id, OUT cv::Mat& pixel_data) {
			int data_size = pixel_data.total() * pixel_data.elemSize();

			glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_id);
			glBindTexture(GL_TEXTURE_2D, tex_id);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				(GLvoid*)0 // buffer에서 시작하는 byte 단위 offset. cpu memory가 아니다!
			);
			glBufferData(GL_PIXEL_PACK_BUFFER, data_size, 0, GL_STREAM_READ); //GL_STREAM_READ 말고 GL_STATIC_READ 하면 깜빡깜빡!

			GLubyte* dma_ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
			if (dma_ptr) {
				memcpy((void*)pixel_data.data, dma_ptr, data_size);
				glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		}

		void CopyTextureFromFBO(GLuint fbo_id, OUT ofTexture& tex) {
			int tex_id = tex.texData.textureID;
			int tex_target = tex.texData.textureTarget;
			
			glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
			glBindTexture(tex_target, tex_id);
			glCopyTexSubImage2D(tex_target, 0, 0, 0, 0, 0, width, height); //sub가 더 빠르다?
			glBindTexture(tex_target, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		GLuint pbo_id = GeneratePBOReader(width, height);

		ofTexture of_tex;
		ofImage of_img;

		cv::Mat mat_rgba = cv::Mat::zeros(height, width, CV_8UC4);
		cv::Mat mat_bgr = cv::Mat::zeros(height, width, CV_8UC3);
};
