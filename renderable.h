#ifndef RENDERABLE_CLASS_H
#define RENDERABLE_CLASS_H

#include "renderer.h"
#include "lattice.h"
#include "utility"

class Renderable {
	public:
		virtual  std::pair<size_t, size_t> draw(Renderer& renderer) const = 0;
		virtual void update(Renderer& renderer) const = 0;
	private:
};

#endif // RENDERABLE_CLASS_H
