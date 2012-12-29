/*
 * GraphicsEngine.cpp
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#include <boost/log/trivial.hpp>

#include "GraphicsEngine.h"

#include "GLWindow.h"

namespace oglre {

namespace engine {

/**
 *
 */
std::unique_ptr<IWindow> GraphicsEngine::createWindow(uint32 width, uint32 height, uint32 depth,
			bool fullscreen, bool vsync) {

	std::unique_ptr<IWindow> window(new GLWindow(width, height, "SFML Test"));
	
	if (window->initialize() < 0) {
		BOOST_LOG_TRIVIAL(warning) << "Window did not initialize successfully.";
	}
	
	// VERY wierd bug - I can call 'resize' all I want in the GLWindow class - the initial perspective
	// doesn't seem to get set unless I call it OUTSIDE of the GLWindow class...wtf?
	window->resize(width, height);
	
	return window;
}

/**
 *
 */
IOS* GraphicsEngine::createIOSObject() {
	IOS* retObj = 0;

	return retObj;
}

}

}
