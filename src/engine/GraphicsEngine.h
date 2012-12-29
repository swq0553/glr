/*
 * GraphicsEngine.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef GRAPHICSENGINE_H_
#define GRAPHICSENGINE_H_

#include <memory>

#include "IWindow.h"
#include "IOS.h"

// we require a C++ compiler.
#if !defined(__cplusplus)
#error C++ compiler required.
#endif

namespace oglre {

namespace engine {

class GraphicsEngine {
public:
	static std::unique_ptr<IWindow> createWindow(glm::detail::uint32 width = 800, glm::detail::uint32 height = 600, glm::detail::uint32 depth = 24,
			bool fullscreen = false, bool vsync = false);

	static IOS* createIOSObject();

private:
	/**
	 * Private.
	 */
	GraphicsEngine() {
	}
	;

	/**
	 * Private.
	 */
	GraphicsEngine(const GraphicsEngine& iceGE) {
	}
	;

	/**
	 * Private.
	 */
	virtual ~GraphicsEngine() {
	}
	;
};

}

}

#endif /* GRAPHICSENGINE_H_ */
