#include <iostream>
#include <pangolin/pangolin.h>

using namespace pangolin;
using namespace std;

void setImageData(float * imageArray, int width, int height){
  for(int i = 0 ; i < width*height;i++) {
    imageArray[i] = (float)rand()/RAND_MAX;
  }
}

int main( int /*argc*/, char* argv[] )
{
  // Load configuration data
  pangolin::ParseVarsFile("app.cfg");

  // Create OpenGL window in single line thanks to GLUT
  pangolin::CreateGlutWindowAndBind("Main",640,480);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Issue specific OpenGl we might need
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Define Camera Render Object (for view / scene browsing)
  pangolin::OpenGlRenderState s_cam;
  s_cam.Set(ProjectionMatrix(640,480,420,420,320,240,0.1,1000));
  s_cam.Set(IdentityMatrix(GlModelViewStack));

  pangolin::OpenGlRenderState s_cam2;
  s_cam2.Set(ProjectionMatrix(640,480,420,420,320,240,0.1,1000));
  s_cam2.Set(IdentityMatrix(GlModelViewStack));

  // Add named OpenGL viewport to window and provide 3D Handler
  View& d_cam1 = pangolin::Display("cam1")
    .SetAspect(640.0f/480.0f)
    .SetHandler(new Handler3D(s_cam));

  View& d_cam2 = pangolin::Display("cam2")
    .SetAspect(640.0f/480.0f)
    .SetHandler(new Handler3D(s_cam2));

  View& d_cam3 = pangolin::Display("cam3")
    .SetAspect(640.0f/480.0f)
    .SetHandler(new Handler3D(s_cam));

  View& d_cam4 = pangolin::Display("cam4")
    .SetAspect(640.0f/480.0f)
    .SetHandler(new Handler3D(s_cam2));

  View& d_img1 = pangolin::Display("img1")
    .SetAspect(640.0f/480.0f);

  View& d_img2 = pangolin::Display("img2")
    .SetAspect(640.0f/480.0f);


  // Add named Panel and bind to variables beginning 'ui'
  // A Panel is just a View with a default layout and input handling
  View& d_panel = pangolin::CreatePanel("ui")
      .SetBounds(0.0, 1.0, 0.0, Attach::Pix(200));

  // LayoutEqual is an EXPERIMENTAL feature - it requires that all sub-displays
  // share the same aspect ratio, placing them in a raster fasion in the
  // viewport so as to maximise display size.
  pangolin::Display("multi")
      .SetBounds(1.0, 0.0, Attach::Pix(200), 1.0)
      .SetLayout(LayoutEqual)
      .AddDisplay(d_cam1)
      .AddDisplay(d_img1)
      .AddDisplay(d_cam2)
      .AddDisplay(d_img2)
      .AddDisplay(d_cam3)
      .AddDisplay(d_cam4);

  const int width =  64;
  const int height = 48;
  float * imageArray = new float[width*height];
  GlTexture imageTexture(width,height,GL_LUMINANCE32F_ARB);
  imageTexture.SetNearestNeighbour();

  // Default hooks for exiting (Esc) and fullscreen (tab).
  while( !pangolin::ShouldQuit() )
  {
    if(HasResized())
      DisplayBase().ActivateScissorAndClear();

    // Safe and efficient binding of named variables.
    // Specialisations mean no conversions take place for exact types
    // and conversions between scalar types are cheap.
    static Var<bool> a_button("ui.A Button",false,false);

    if( Pushed(a_button) )
      cout << "You Pushed a button!" << endl;

    // Generate random image and place in texture memory for display
    setImageData(imageArray,width,height);
    imageTexture.Upload(imageArray,GL_LUMINANCE,GL_FLOAT);

    // (3D Handler requires depth testing to be enabled)
    glEnable(GL_DEPTH_TEST);
    glColor3f(1.0,1.0,1.0);

    d_cam1.ActivateScissorAndClear(s_cam);
    glutWireTeapot(1.0);

    d_cam2.ActivateScissorAndClear(s_cam2);
    glutWireTeapot(1.0);

    d_cam3.ActivateScissorAndClear(s_cam);
    glutWireTeapot(1.0);

    d_cam4.ActivateScissorAndClear(s_cam2);
    glutWireTeapot(1.0);

    d_img1.ActivateScissorAndClear();
    imageTexture.RenderToViewport();

    d_img2.ActivateScissorAndClear();
    imageTexture.RenderToViewport();

    // Render our UI panel when we receive input
    if(HadInput())
      d_panel.Render();

    // Swap frames and Process Events
    glutSwapBuffers();
    glutMainLoopEvent();
  }

  delete imageArray;

  return 0;
}
