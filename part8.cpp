//g++ -o out glu.cpp  -lGL  -lX11 -lGLU

#include<stdio.h>
#include<stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include<string.h>
#include<unistd.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>

int global=0;
int global2=0;
int status;
float ox=0.9;
float oy=0.9;
float oz=10.0;
int step;
Display                 *dpy;
Window                  root;
GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo             *vi;
Colormap                cmap;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XWindowAttributes       gwa;
XEvent                  e;
void *on_timer(void *argts);
void on_exit();
void on_refresh();
void on_steps();
void drawBlock(float x,float z);

void on_timer(){
		usleep(3000);
		on_steps();
		on_refresh();		
		
		
}



void render() {
 glClearColor(0.5, 0.5, 0.5, 1.0);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-1., 1., -1., 1., 1., 20.);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();


 gluLookAt(ox, 1.0,oz,0.0, 0.0, 0., 0., 1., 0.);
 
 drawBlock(7.0,7.0);
 drawBlock(11.0,7.0);
 drawBlock(7.0,11.0);
 drawBlock(8.0,8.0);
 drawBlock(10.0,8.0);
 drawBlock(9.0,9.0);
 drawBlock(8.0,10.0);
 drawBlock(10.0,10.0);
 drawBlock(11.0,11.0);
} 



 
int main(int argc, char *argv[]) {

 dpy = XOpenDisplay(NULL);
 
 if(dpy == NULL) {
 
        exit(0);
 }
        
 root = DefaultRootWindow(dpy);

 vi = glXChooseVisual(dpy, 0, att);

 if(vi == NULL) {
        exit(0);
 } 
 else {

 }


 cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

 swa.colormap = cmap;
 swa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask;
 
 win = XCreateWindow(dpy, root, 0, 0, 630, 330, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

 XMapWindow(dpy, win);
 Atom WM_DELETE_WINDOW = XInternAtom(dpy, "WM_DELETE_WINDOW", False); 
  XSetWMProtocols(dpy, win, &WM_DELETE_WINDOW, 1);  


 XStoreName(dpy, win, "panel sample");
 
 glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
 glXMakeCurrent(dpy, win, glc);
 
 glEnable(GL_DEPTH_TEST); 

	on_refresh();
 
   while (1) {
     if(XPending(dpy)){
      XNextEvent(dpy, &e);
		

      if (e.type == Expose){
		  on_refresh();
          if (e.type == KeyPress){
			char buf[128] = {0};
			KeySym keysym;
			int len = XLookupString(&e.xkey, buf, sizeof buf, &keysym, NULL);
	
			if (keysym == XK_Escape){
				on_exit();
				break;
			}else{
				on_refresh();
				
			}
	}

}
		if ((e.type == ClientMessage) && 
        (static_cast<unsigned int>(e.xclient.data.l[0]) == WM_DELETE_WINDOW))
			{
				on_exit();
				break;

			}
			

}
on_timer();
}
}


void on_exit(){
      glXMakeCurrent(dpy, None, NULL);
      glXDestroyContext(dpy, glc);
      XDestroyWindow(dpy, win);
      XCloseDisplay(dpy);
	global=2;
    

}



void on_refresh(){

                XGetWindowAttributes(dpy, win, &gwa);
                glViewport(0, 0, gwa.width, gwa.height);
				render();
                glXSwapBuffers(dpy, win);

}


void on_steps(){
  if (step==3){
		oz=oz+0.6;
		if (oz>10.0){
			 ox=0.9;
			 oz=10.0;
			 step++;
		 }
	}




	if (step==2){
		ox=ox+0.1;

		if (ox>0.9){
			 ox=0.9;
			 oz=-10.0;
			 step++;
		 }

	}


  if (step==1){
		oz=oz-0.6;
		if (oz<-10.0){
			 ox=-0.9;
			 oz=-10.0;
			 step++;
		 }

	}
	
	if (step==0){
		ox=ox-0.1;

		if (ox<-0.9){
			 ox=-0.9;
			 oz=10.0;
			 step++;
		 }


	}
	
	if (step>3) step=0;
}


void drawBlock(float x,float z){
float xx=x;
float zz=z;
 xx=(xx*0.2)-1.6;
 zz=(zz*0.2)-1.6;
 glBegin(GL_QUADS);
  glColor4f(0.0, 0.0, 1.0,0.1);
  glVertex3f(-.1+xx, -.1, .1+zz);
  glColor4f(0.0,1.0,0.0,0.1);
  glVertex3f( .1+xx, -.1, .1+zz);
  glColor4f(1.0,0.0,0.0,0.1);
  glVertex3f( .1+xx,  .1, .1+zz);
  glColor4f(1.0,1.0,1.0,0.1);
  glVertex3f(-.1+xx, .1, .1+zz);
 glEnd();
 
 glBegin(GL_QUADS);
  glColor4f(0.0, 0.0, 1.0,0.1);
  glVertex3f(-.1+xx, -.1, -.1+zz);
  glColor4f(0.0,1.0,0.0,0.1);
  glVertex3f( .1+xx, -.1, -.1+zz);
  glColor4f(1.0,0.0,0.0,0.1);
  glVertex3f( .1+xx,  .1, -.1+zz);
  glColor4f(1.0,1.0,1.0,0.1);
  glVertex3f(-.1+xx, .1, -.1+zz);
 glEnd();

 glBegin(GL_QUADS);
  glColor4f(0.0, 0.0, 1.0,0.1);
  glVertex3f(.1+xx, .1, .1+zz);
  glColor4f(0.0,1.0,0.0,0.1);
  glVertex3f( .1+xx, .1, -.1+zz);
  glColor4f(1.0,0.0,0.0,0.1);
  glVertex3f( .1+xx,  -.1, -.1+zz);
  glColor4f(1.0,1.0,1.0,0.1);
  glVertex3f(.1+xx, -.1, .1+zz);
 glEnd();

 glBegin(GL_QUADS);
  glColor4f(0.0, 0.0, 1.0,0.1);
  glVertex3f(-.1+xx, .1, .1+zz);
  glColor4f(0.0,1.0,0.0,0.1);
  glVertex3f(-.1+xx, .1, -.1+zz);
  glColor4f(1.0,0.0,0.0,0.1);
  glVertex3f(-.1+xx,  -.1, -.1+zz);
  glColor4f(1.0,1.0,1.0,0.1);
  glVertex3f(-.1+xx, -.1, .1+zz);
 glEnd();



}



