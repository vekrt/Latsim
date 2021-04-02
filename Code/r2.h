#ifndef VEC_STRUCT_H
#define VEC_STRUCT_H

#include <cmath>

#define tan_60 tanf(M_PI/3.0)
#define tan_30 tanf(M_PI/6.0)

namespace r2 {


	template<typename T>
	struct vec2 {
		T x, y;
		vec2() = default;
		vec2(const T& u, const T& v) : x(u), y(v) {}
		
		template<typename U>
		vec2<U> map(U (*f)(T)) {
			return vec2<U>(f(x), f(y));
		}
		
		vec2<T>& operator+=(vec2<T> const& my_vec) {
			x += my_vec.x;
			y += my_vec.y;
			return *this;
		}

		vec2<T>& operator-=(vec2<T> const& my_vec) {
			x -= my_vec.x;
			y -= my_vec.y;
			return *this;
		}
	};
	
	template<typename T> 
	vec2<T> operator+(vec2<T> a, const vec2<T>& b) {
		a += b;
		return a;
	}

	template<typename T> 
	vec2<T> operator-(vec2<T> a, const vec2<T>& b) {
		a -= b;
		return a;
	}

	template<typename T>
	vec2<T> operator*(const vec2<T>& a, const T& v) {
		return vec2<T>(a.x*v, a.y*v);
	}

	template<typename T>
	vec2<T> operator*(const T& v, const vec2<T>& a) {
		return vec2<T>(a.x*v, a.y*v);
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const vec2<T> a) {
		out << a.x << "," << a.y;
		return out;
	}

	template<typename T>
	struct vec3 {
		T x, y, z;
		vec3() = default;
		vec3(const T& u, const T& v, const T& w) : x(u), y(v), z(w) {}
		
		template<typename U>
		vec3<U> map(U (*f)(T)) {
			return vec3<U>(f(x), f(y), f(z));
		}
		
		vec3<T>& operator+=(vec3<T> const& my_vec) {
			x += my_vec.x;
			y += my_vec.y;
			z += my_vec.z;
			return *this;
		}

		vec3<T>& operator-=(vec3<T> const& my_vec) {
			x -= my_vec.x;
			y -= my_vec.y;
			z -= my_vec.z;
			return *this;
		}

		double norm2() const {
			return x*x + y*y + z*z;
		}

		double norm() const {
			return sqrt(x*x + y*y + z*z);
		}
	};
	
	template<typename T> 
	vec3<T> operator+(vec3<T> a, const vec3<T>& b) {
		a += b;
		return a;
	}

	template<typename T> 
	vec3<T> operator-(vec3<T> a, const vec3<T>& b) {
		a -= b;
		return a;
	}

	template<typename T>
	vec3<T> operator*(const vec3<T>& a, const T& v) {
		return vec3<T>(a.x*v, a.y*v, a.z*v);
	}

	template<typename T>
	vec3<T> operator*(const T& v, const vec3<T>& a) {
		return vec3<T>(a.x*v, a.y*v, a.z*v);
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const vec3<T> a) {
		out << a.x << "," << a.y << "," << a.z;
		return out;
	}

	template<typename T>
	vec3<T> cross(const vec3<T>& a, const vec3<T>& b) {
		return vec3<T>(a.y*b.z-a.z*b.y, -a.x*b.z+a.z*b.x, a.x*b.y-a.y*b.x);
	}

	template<typename T>
	double dot(const vec3<T>& a, const vec3<T>& b) {
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}

	template<typename T>
	vec3<T> normalise(const vec3<T> a) {
		const double norm = a.norm();
		return vec3<float>(a.x/norm, a.y/norm, a.z/norm);
	}

	template<typename T = float>
	struct vec4 {
		T x, y, z, w;
		vec4() = default;
		vec4(const T& x_, const T& y_, const T& z_, const T& w_): 
					x(x_), y(y_), z(z_), w(w_)
					{}

		template<typename U>
		vec4<U> map(U (*f)(T)) {
			return vec4<U>(f(x), f(y), f(z), f(w));
		}
		
		vec4<T>& operator+=(vec4<T> const& my_vec) {
			x += my_vec.x;
			y += my_vec.y;
			z += my_vec.z;
			w += my_vec.w;
			return *this;
		}

		vec4<T>& operator-=(vec4<T> const& my_vec) {
			x -= my_vec.x;
			y -= my_vec.y;
			z -= my_vec.z;
			w -= my_vec.w;
			return *this;
		}

		T& operator[](const size_t& idx) {
			if (idx == 0)
				return x;
			else if (idx == 1)
				return y;
			else if (idx == 2)
				return z;
			else if (idx == 3)
				return w;
			else
				throw std::invalid_argument("index out of bound");
		}
		
		const T& operator[](const size_t& idx) const {
			if (idx == 0)
				return x;
			else if (idx == 1)
				return y;
			else if (idx == 2)
				return z;
			else if (idx == 3)
				return w;
			else
				throw std::invalid_argument("index out of bound");
		}
	};

	template<typename T> 
	vec4<T> operator+(vec4<T> a, const vec4<T>& b) {
		a += b;
		return a;
	}

	template<typename T> 
	vec4<T> operator-(vec4<T> a, const vec4<T>& b) {
		a -= b;
		return a;
	}

	template<typename T>
	vec4<T> operator*(const vec4<T>& a, const T& v) {
		return vec4<T>(a.x*v, a.y*v, a.z*v, a.w*v);
	}

	template<typename T>
	vec4<T> operator*(const T& v, const vec4<T>& a) {
		return vec4<T>(a.x*v, a.y*v, a.z*v, a.w*v);
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const vec4<T>& a) {
		out << a.x << "," << a.y << "," << a.z << a.w;
		return out;
	}

	template<typename T = float>
	struct mat4 {
		vec4<T> c1, c2, c3, c4;
		mat4() = default;
		mat4(const vec4<T>& x, const vec4<T>& y, const vec4<T>& z, const vec4<T>& w):
				c1(x),
				c2(y),
				c3(z),
				c4(w)
				{}

		const vec4<T>& operator[](const size_t& idx) const {
			if (idx == 0)
				return c1;
			else if (idx == 1)
				return c2;
			else if (idx == 2)
				return c3;
			else if (idx == 3)
				return c4;
			else {
				throw std::invalid_argument("index out of bound");
			}
		}
		
		vec4<T>& operator[](const size_t& idx) {
			if (idx == 0)
				return c1;
			else if (idx == 1)
				return c2;
			else if (idx == 2)
				return c3;
			else if (idx == 3)
				return c4;
			else {
				throw std::invalid_argument("index out of bound");
			}
		}
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const mat4<T>& a) {
		for (size_t i=0; i<4; ++i) {
			out << a.c1[i] << "," << a.c2[i] << "," 
			    << a.c3[i] << "," << a.c4[i] << std::endl;
		}
		return out;
	}

	template<typename T>
	mat4<T> identity4() {
		mat4<T> res;
		res.c1 = {1.0f, 0.0f, 0.0f, 0.0f};
		res.c2 = {0.0f, 1.0f, 0.0f, 0.0f};
		res.c3 = {0.0f, 0.0f, 1.0f, 0.0f};
		res.c4 = {0.0f, 0.0f, 0.0f, 1.0f};
	
		return res;
	}

	template<typename T>
	mat4<T> translation(vec2<T> p) {
		mat4<T> res;
		res.c1 = {1.0f, 0.0f, 0.0f, 0.0f};
		res.c2 = {0.0f, 1.0f, 0.0f, 0.0f};
		res.c3 = {0.0f, 0.0f, 1.0f, 0.0f};
		res.c4 = {p.x , p.y , 1.0f, 1.0f};
	
		return res;
	}

	template<typename T>
	mat4<T> translation(const float& p1, const float& p2) {
		mat4<T> res;
		res.c1 = {1.0f, 0.0f, 0.0f, 0.0f};
		res.c2 = {0.0f, 1.0f, 0.0f, 0.0f};
		res.c3 = {0.0f, 0.0f, 1.0f, 0.0f};
		res.c4 = {p1  , p2  , 1.0f, 1.0f};
	
		return res;
	}

	template<typename T>
	mat4<T> scale(const float& s) {
		mat4<T> res;
		res.c1 = {s   , 0.0f, 0.0f, 0.0f};
		res.c2 = {0.0f, s   , 0.0f, 0.0f};
		res.c3 = {0.0f, 0.0f, s   , 0.0f};
		res.c4 = {0.0f, 0.0f, 0.0f, s   };
	
		return res;	
	}

	template<typename T>
	mat4<T> zoom(const float& p1, const float& p2, const float& s) {
		mat4<T> res;

		res.c1 = {s   , 0.0f, 0.0f, 0.0f};
		res.c2 = {0.0f, s   , 0.0f, 0.0f};
		res.c3 = {0.0f, 0.0f, s   , 0.0f};
		res.c4 = {p1*(1-s), p2*(1-s), 0.0f, 1.0f };
	
		return res;	
	}

	template<typename T>
	struct Vertex {
		vec2<T> position;
		//vec3<T> colour;
		//vec2<T> texture;
		//float tex_idx;

		Vertex() = default;
		Vertex(const vec2<T>& p/*, const vec3<T>& c*/): position(p)/*, colour(c)*/ {}
		//Vertex(const vec2<T>& p, const vec3<T>& c, const vec2<T>& tex, const float& idx): position(p), colour(c), texture(tex), tex_idx(idx) {}
		Vertex<T> shift(const vec2<T>& sh) const {
			Vertex<T> res = *this;
			res.position += sh;
			return res;
		}
		Vertex<T> shift_x(const float& shx) const {
			Vertex<T> res = *this;
			res.position.x += shx;
			return res;
		}
		Vertex<T> shift_y(const float& shy) const {
			Vertex<T> res = *this;
			res.position.y += shy;
			return res;
		}
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const Vertex<T>& vert) {
		out << vert.position << " " << vert.colour;
		return out;
	}


	template<typename T>
	struct Triangle {
		Vertex<T> a, b, c;

		Triangle() = default;
		Triangle(const Vertex<T>& a, const Vertex<T>& b, const Vertex<T>& c): 
				a(a), b(b), c(c)
				{}

		//Triangle(const Vertex<T>& a, const Vertex<T>& b, const Vertex<T>& c, const float& idx): 
		//		a(a), b(b), c(c) {

		//		a.texture = {0.0f, 0.0f};
		//		b.texture = {1.0f, 0.0f};
		//		c.texture = {0.0f, 1.0f};
		//		a.tex_idx = idx;
		//		b.tex_idx = idx;
		//		c.tex_idx = idx;
		//}

		Triangle<T> translate(const vec2<T>& p) {
			a.position += p;
			b.position += p;
			c.position += p;

			return *this;
		}
	};

	template<typename T>
	struct Quad {
		Vertex<T> a, b, c, d;

		Quad() = default;
		Quad(const Vertex<T>& a, const Vertex<T>& b, 
		     const Vertex<T>& c, const Vertex<T>& d): 
		     a(a), b(b), c(c), d(d)
		     {}
		Quad(const Vertex<T>& a_, const float& size/*, const float& idx*/): 
				a(a_), b(a_.shift_x(size)), 
				c(a_.shift_y(size)), d(a_.shift({size,size}))
				{
		//			a.texture = {0.0f, 0.0f};
		//			b.texture = {1.0f, 0.0f};
		//			c.texture = {0.0f, 1.0f};
		//			d.texture = {1.0f, 1.0f};	
		//			a.tex_idx = idx;
		//			b.tex_idx = idx;
		//			c.tex_idx = idx;
		//			d.tex_idx = idx;				
		}

		//void change_tex() {
		//	a.tex_idx = 1.0f - a.tex_idx;
		//	b.tex_idx = 1.0f - b.tex_idx;
		//	c.tex_idx = 1.0f - c.tex_idx;
		//	d.tex_idx = 1.0f - d.tex_idx;
		//}
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const Quad<T>& quad) {
		out << quad.a << std::endl << quad.b << std::endl
		    << quad.c << std::endl << quad.d;
		return out;
	}

	template<typename T>
	struct Hex {
		Vertex<T> a, b, c,
		          d, e, f;

		Hex() = default;
		Hex(const Vertex<T>& a_, const float& width, const float& height): 
				a(a_), b(a_.shift_y(width/2.0f)), c(a_.shift({width/2.0f, 0.5f*width + height})),
				d(a_.shift({width, 0.5f*width})), e(a_.shift({width, 0.0f})), f(a_.shift({0.5f*width, -height}))
				{
					//a.texture = {0.0f, 0.0f};
					//b.texture = {0.0f, 0.0f};
					//c.texture = {0.5f, 0.0f};
					//d.texture = {0.0f, 0.0f};
					//e.texture = {0.0f, 0.0f};
					//f.texture = {0.5f, 0.0f};
					//a.tex_idx = a_.tex_idx;
					//b.tex_idx = a_.tex_idx;
					//c.tex_idx = a_.tex_idx;
					//d.tex_idx = a_.tex_idx;
					//e.tex_idx = a_.tex_idx;
					//f.tex_idx = a_.tex_idx;
				}

		//void change_tex() {
		//	a.tex_idx = 1.0f - a.tex_idx;
		//	b.tex_idx = 1.0f - b.tex_idx;
		//	c.tex_idx = 1.0f - c.tex_idx;
		//	d.tex_idx = 1.0f - d.tex_idx;
		//	e.tex_idx = 1.0f - e.tex_idx;
		//	f.tex_idx = 1.0f - f.tex_idx;
		//}

		
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const Hex<T>& hex) {
		out << hex.a << std::endl << hex.b << std::endl
		    << hex.c << std::endl << hex.d << std::endl
		    << hex.e << std::endl << hex.f << std::endl;
		return out;
	}

	template<typename T>
	struct Hex_half {
		Vertex<T> a, b, c, d;
		float width;

		Hex_half() = default;
		Hex_half(const Vertex<T>& a_, const float& width, const float& height) : 
				a(a_), b(a_.shift_y(width/2.0f)), c(a_.shift({width/2.0f, 0.5f*width + height})),
				d(a_.shift({0.5f*width, -height})), width(width) {}

		Hex_half<T> flip_lr() {
			c = c.shift_x(-width);
			d = d.shift_x(-width);

			return *this;
		}

		Hex_half<T> translate(const vec2<T>& p) {
			a.position += p;
			b.position += p;
			c.position += p;
			d.position += p;

			return *this;
		}
	};
}

#endif //VEC_STRUCT_H
