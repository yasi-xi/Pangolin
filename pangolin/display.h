/* This file is part of the Pangolin Project.
* http://github.com/stevenlovegrove/Pangolin
*
* Copyright (c) 2011 Steven Lovegrove, Richard Newcombe
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef PANGOLIN_DISPLAY_H
#define PANGOLIN_DISPLAY_H

#include "platform.h"
#include <limits.h>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <iostream>

#ifdef HAVE_GLUT
#include <GL/freeglut_std.h>
#include <GL/freeglut_ext.h>
#endif

#ifdef HAVE_TOON
#include <cstring>
#include <TooN/TooN.h>
#include <TooN/se3.h>
#endif

#ifdef _WIN_
#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#else
#include <GL/gl.h>
#endif

#define GLUT_KEY_ESCAPE 27
#define GLUT_KEY_TAB 9

namespace pangolin
{

	//! @brief Give this OpenGL context a name or switch contexts
	//! This is required to initialise Pangolin for use with an
	//! externally defined OpenGL context. You needn't call it
	//! if you have used CreateGlutWindowAndBind() to create a GLUT
	//! window
	LIBRARY_API void BindToContext(std::string name);

	//! @brief Request that the program exit
	LIBRARY_API void Quit();

	//! @brief Returns true if user has requested to close OpenGL window.
	LIBRARY_API bool ShouldQuit();

	//! @brief Returns true if user has interacted with the window since this was last called
	LIBRARY_API bool HadInput();

	//! @brief Returns true if user has pressed mouse buttons since this was last called
	LIBRARY_API bool HadMousePress();

	//! @brief Returns true if user has resized the window
	LIBRARY_API bool HasResized();

	namespace process
	{
		//! @brief Tell pangolin to process input to drive display
		//! You will need to call this manually if you haven't let
		//! Pangolin register callbacks from your windowing system
		LIBRARY_API void Keyboard( unsigned char key, int x, int y);

		LIBRARY_API void KeyboardUp(unsigned char key, int x, int y);

		//! @brief Tell pangolin base window size has changed
		//! You will need to call this manually if you haven't let
		//! Pangolin register callbacks from your windowing system
		LIBRARY_API void Resize(int width, int height);

		LIBRARY_API void Mouse( int button, int state, int x, int y);

		LIBRARY_API void MouseMotion( int x, int y);
	}

#ifdef HAVE_GLUT

	//! @brief Create GLUT window and bind Pangolin to it.
	//! All GLUT initialisation is taken care of. This prevents you
	//! from needing to call BindToContext() and TakeGlutCallbacks().
	LIBRARY_API void CreateGlutWindowAndBind(std::string window_title, int w = 640, int h = 480, unsigned int mode = GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//! @brief Allow Pangolin to take GLUT callbacks for its own uses
	//! Not needed if you instantiated a window through CreateWindowAndBind().
	LIBRARY_API void TakeGlutCallbacks();

#endif

	//! @brief Unit for measuring quantities
	enum Unit {
		Fraction,
		Pixel,
		ReversePixel
	};

	//! @brief Defines typed quantity
	//! Constructors distinguised by whole pixels, or floating
	//! fraction in interval [0,1]
	struct Attach {
		Attach() : unit(Fraction), p(0) {}
		Attach(Unit unit, GLfloat p) : unit(unit), p(p) {}

		Attach(GLfloat p) : unit(Fraction), p(p) {
			if( p < 0 || 1.0 < p ) {
				std::cerr << "Pangolin API Change: Display::SetBounds must be used with Attach::Pix or Attach::ReversePix to specify pixel bounds relative to an edge. See the code samples for details." << std::endl;
				throw std::exception();
			}
		}

		//    Attach(GLdouble p) : unit(Fraction), p(p) {}

		static Attach Pix(int p) {
			return Attach(p >=0 ? Pixel : ReversePixel, std::abs((float)p));
		}
		static Attach ReversePix(int p) {
			return Attach(ReversePixel, p);
		}
		static Attach Frac(float frac) {
			return Attach(frac);
		}

		Unit unit;
		GLfloat p;

		//  protected:
		//    Attach(int p) {}
	};


	enum Lock {
		LockLeft = 0,
		LockBottom = 0,
		LockCenter = 1,
		LockRight = 2,
		LockTop = 2
	};

	//! @brief Encapsulates OpenGl Viewport
	struct LIBRARY_API Viewport
	{
		Viewport() {}
		Viewport(GLint l,GLint b,GLint w,GLint h);
		void Activate() const;
		void Scissor() const;
		void ActivateAndScissor() const;
		bool Contains(int x, int y) const;

		Viewport Inset(int i) const;
		Viewport Inset(int horiz, int vert) const;

		static void DisableScissor();

		GLint r() const;
		GLint t() const;
		GLfloat aspect() const;
		GLint l,b,w,h;
	};

	//! @brief Capture OpenGL matrix types in enum to typing
	enum OpenGlStack {
		GlProjectionStack = GL_PROJECTION,
		GlModelViewStack = GL_MODELVIEW,
		GlTextureStack = GL_TEXTURE
	};

	struct CameraSpec {
		GLdouble forward[3];
		GLdouble up[3];
		GLdouble right[3];
		GLdouble img_up[2];
		GLdouble img_right[2];
	};

	const static CameraSpec CameraSpecOpenGl = {{0,0,-1},{0,1,0},{1,0,0},{0,1},{1,0}};
	const static CameraSpec CameraSpecYDownZForward = {{0,0,1},{0,-1,0},{1,0,0},{0,-1},{1,0}};

	//! @brief Object representing OpenGl Matrix
	struct LIBRARY_API OpenGlMatrixSpec {
		// Load matrix on to OpenGl stack
		void Load() const;

		void Multiply() const;

		void SetIdentity();

		// Specify which stack this refers to
		OpenGlStack type;

		// Column major Internal buffer
		GLdouble m[16];
	};

	//! @brief Object representing attached OpenGl Matrices / transforms
	//! Only stores what is attached, not entire OpenGl state (which would
	//! be horribly slow). Applying state is efficient.
	struct LIBRARY_API OpenGlRenderState
	{
		static void ApplyIdentity();
		static void ApplyWindowCoords();

		void Apply() const;    
		OpenGlRenderState& Set(OpenGlMatrixSpec spec);
		std::map<OpenGlStack,OpenGlMatrixSpec> stacks;
	};

	enum Layout
	{
		LayoutOverlay,
		LayoutVertical,
		LayoutHorizontal,
		LayoutEqual
	};

	// Forward declaration
	struct LIBRARY_API Handler;

	//! @brief A Display manages the location and resizing of an OpenGl viewport.
	struct LIBRARY_API View
	{
		View()
			: aspect(0.0), top(1.0),left(0.0),right(1.0),bottom(0.0), hlock(LockCenter),vlock(LockCenter),
			layout(LayoutOverlay), scroll_offset(0), show(1), handler(0) {}

		virtual ~View() {}

		//! Activate Displays viewport for drawing within this area
		void Activate() const;

		//! Activate Displays and set State Matrices
		void Activate(const OpenGlRenderState& state ) const;

		//! Activate Displays viewport and Scissor for drawing within this area
		void ActivateAndScissor() const;

		//! Activate Displays viewport and Scissor for drawing within this area
		void ActivateScissorAndClear() const;

		//! Activate Display and set State Matrices
		void ActivateAndScissor(const OpenGlRenderState& state ) const;

		//! Activate Display and set State Matrices
		void ActivateScissorAndClear(const OpenGlRenderState& state ) const;

		//! Given the specification of Display, compute viewport
		virtual void Resize(const Viewport& parent);

		//! Instruct all children to resize
		virtual void ResizeChildren();

		//! Perform any automatic rendering for this View.
		//! Default implementation simply instructs children to render themselves.
		virtual void Render();

		//! Instruct all children to render themselves if appropriate
		virtual void RenderChildren();

		//! Set this view as the active View to receive input
		View& SetFocus();

		//! Set bounds for the View using mixed fractional / pixel coordinates (OpenGl view coordinates)
		View& SetBounds(Attach bottom, Attach top, Attach left, Attach right, bool keep_aspect = false);

		//! Set bounds for the View using mixed fractional / pixel coordinates (OpenGl view coordinates)
		View& SetBounds(Attach bottom, Attach top, Attach left, Attach right, double aspect);

		View& SetHandler(Handler* handler);
		View& SetAspect(double aspect);
		View& SetLock(Lock horizontal, Lock vertical );
		View& SetLayout(Layout layout);
		View& AddDisplay(View& view);

		//! Check whether this View is current focused
		bool isFocused() const;

		// Desired width / height aspect (0 if dynamic)
		double aspect;

		// Bounds to fit display within
		Attach top, left, right, bottom;
		Lock hlock;
		Lock vlock;
		Layout layout;

		int scroll_offset;

		// Cached client area (space allocated from parent)
		Viewport vp;

		// Cached absolute viewport (recomputed on resize - respects aspect)
		Viewport v;

		// Should this view be displayed?
		bool show;

		// Input event handler (if any)
		Handler* handler;

		// Map for sub-displays (if any)
		std::vector<View*> views;

	private:
		// Private copy constructor
		View(View&) { /* Do Not copy - take reference instead*/ }
	};

	enum MouseButton
	{
		MouseButtonLeft = 1,
		MouseButtonMiddle = 2,
		MouseButtonRight = 4,
		MouseWheelUp = 8,
		MouseWheelDown = 16
	};

	//! @brief Input Handler base class with virtual methods which recurse
	//! into sub-displays
	struct LIBRARY_API Handler
	{
		virtual ~Handler() {}
		virtual void Keyboard(View&, unsigned char key, int x, int y, bool pressed);
		virtual void Mouse(View&, MouseButton button, int x, int y, bool pressed, int button_state);
		virtual void MouseMotion(View&, int x, int y, int button_state);
	};
	static Handler StaticHandler;

	struct LIBRARY_API HandlerScroll : Handler
	{
		void Mouse(View&, MouseButton button, int x, int y, bool pressed, int button_state);
	};
	static HandlerScroll StaticHandlerScroll;

	struct LIBRARY_API Handler3D : Handler
	{

		Handler3D(OpenGlRenderState& cam_state, float trans_scale=0.01f)
			: cam_state(&cam_state), /*hwin(3),*/ tf(trans_scale), cameraspec(CameraSpecOpenGl), last_z(1.0) {}

		void SetOpenGlCamera();
		void Mouse(View&, MouseButton button, int x, int y, bool pressed, int button_state);
		void MouseMotion(View&, int x, int y, int button_state);

		OpenGlRenderState* cam_state;
		const static int hwin = 3;
		float tf;
		CameraSpec cameraspec;
		GLfloat last_z;
		GLint last_pos[2];
		GLdouble rot_center[3];
	};

	LIBRARY_API View& DisplayBase();
	LIBRARY_API View& Display(const std::string& name);

	LIBRARY_API OpenGlMatrixSpec ProjectionMatrixRUB_BottomLeft(int w, int h, double fu, double fv, double u0, double v0, double zNear, double zFar );
	LIBRARY_API OpenGlMatrixSpec ProjectionMatrixRDF_TopLeft(int w, int h, double fu, double fv, double u0, double v0, double zNear, double zFar );
	LIBRARY_API OpenGlMatrixSpec ProjectionMatrixRDF_BottomLeft(int w, int h, double fu, double fv, double u0, double v0, double zNear, double zFar );

	// Use OpenGl's default frame RUB_BottomLeft
	LIBRARY_API OpenGlMatrixSpec ProjectionMatrix(int w, int h, double fu, double fv, double u0, double v0, double zNear, double zFar );

	LIBRARY_API OpenGlMatrixSpec IdentityMatrix(OpenGlStack type);
	LIBRARY_API OpenGlMatrixSpec negIdentityMatrix(OpenGlStack type);

#ifdef HAVE_TOON
	LIBRARY_API OpenGlMatrixSpec FromTooN(const TooN::SE3<>& T_cw);
	LIBRARY_API OpenGlMatrixSpec FromTooN(OpenGlStack type, const TooN::Matrix<4,4>& M);
	LIBRARY_API TooN::Matrix<4,4> ToTooN(const OpenGlMatrixSpec& ms);
	LIBRARY_API TooN::SE3<> ToTooN_SE3(const OpenGlMatrixSpec& ms);
#endif


}

// Inline definitions
namespace pangolin
{

	inline Viewport::Viewport(GLint l,GLint b,GLint w,GLint h) : l(l),b(b),w(w),h(h) {}
	inline GLint Viewport::r() const { return l+w;}
	inline GLint Viewport::t() const { return b+h;}
	inline GLfloat Viewport::aspect() const { return (GLfloat)w / (GLfloat)h; }

#ifdef HAVE_TOON

	inline OpenGlMatrixSpec FromTooN(const TooN::SE3<>& T_cw)
	{
		TooN::Matrix<4,4,double,TooN::ColMajor> M;
		M.slice<0,0,3,3>() = T_cw.get_rotation().get_matrix();
		M.T()[3].slice<0,3>() = T_cw.get_translation();
		M[3] = TooN::makeVector(0,0,0,1);

		OpenGlMatrixSpec P;
		P.type = GlModelViewStack;
		std::memcpy(P.m, &(M[0][0]),16*sizeof(double));
		return P;
	}

	inline OpenGlMatrixSpec FromTooN(OpenGlStack type, const TooN::Matrix<4,4>& M)
	{
		// Read in remembering col-major convension for our matrices
		OpenGlMatrixSpec P;
		P.type = type;
		int el = 0;
		for(int c=0; c<4; ++c)
			for(int r=0; r<4; ++r)
				P.m[el++] = M[r][c];
		return P;
	}

	inline TooN::Matrix<4,4> ToTooN(const OpenGlMatrixSpec& ms)
	{
		TooN::Matrix<4,4> m;
		int el = 0;
		for( int c=0; c<4; ++c )
			for( int r=0; r<4; ++r )
				m(r,c) = ms.m[el++];
		return m;
	}

	inline TooN::SE3<> ToTooN_SE3(const OpenGlMatrixSpec& ms)
	{
		TooN::Matrix<4,4> m = ToTooN(ms);
		const TooN::SO3<> R(m.slice<0,0,3,3>());
		const TooN::Vector<3> t = m.T()[3].slice<0,3>();
		return TooN::SE3<>(R,t);
	}


#endif

}

#endif // PANGOLIN_DISPLAY_H

