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


const char maps[]={
	"oooooooooooooooo"
	"ol             o"
	"oooooooooooooo o"
	"o            o o"
	"o oooooooooo o o"
	"o o        o o o"
	"o o oooooo o o o"
	"o o o    o o o o"
	"o o o o. o o o o"
	"o o o oooo o o o"
	"o o o      o o o"
	"o o oooooooo o o"
	"o o          o o"
	"o oooooooooooo o"
	"o              o"
	"oooooooooooooooo"
	};


double vision[]={0.0,-1.0 , 1.0,0.0 , 0.0,1.0 , -1.0,0.0};
int rotation=0;
int horizontal=0.0;
int vertical=0.0;
int global=0;
int global2=0;
int status;
float xx=1.0;
float yy=1.0;
float x0=1.0;
float y0=1.0;
float ox=0.9;
float oy=0.9;
float oz=10.0;
float cx=1.0;
float cy=1.0;
int key=1;
int lastkey=2;
float ccx=1.0;
float ccy=1.0;
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
void on_exit();
void on_refresh();
void on_steps();
void drawBlock(float x,float z);
char getcor(const char *c,float x,float y);
void formap();
void cursor();



void render() {
 glClearColor(0.5, 0.5, 0.5, 1.0);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-1., 1., -1., 1., 1., 20.);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();


 gluLookAt(cx, 1.3,cy,ccx, 0.9, ccy, 0., 1., 0.);
 
 
formap();
cursor();
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


 XStoreName(dpy, win, "maze sample 3D");
 
 glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
 glXMakeCurrent(dpy, win, glc);
 
 glEnable(GL_DEPTH_TEST); 
 on_steps();
 on_refresh();		

 
   while (1) {

      XNextEvent(dpy, &e);
		

      if (e.type == Expose){
	  }
		  
          if (e.type == KeyPress){
			  printf("key\n");
			char buf[128] = {0};
			const char *cc;
			cc=&maps[0];
			KeySym keysym;
			int len = XLookupString(&e.xkey, buf, sizeof buf, &keysym, NULL);
	
			if (keysym == XK_Escape){
				on_exit();
				break;
			}
			if(keysym == XK_Left || keysym == XK_Right || keysym == XK_Up){
				key=0;

				if (keysym == XK_Left){
					rotation=rotation-1;
				}
				
				if (keysym == XK_Right) {
					rotation=rotation+1;
				}
				
				if (rotation>3)rotation=0;
				if (rotation<0)rotation=3;


				if (keysym == XK_Up){
					horizontal=vision[rotation*2];
					vertical=vision[rotation*2+1];
					xx=x0+horizontal;
					yy=y0+vertical;
				}
				
	
				if (getcor(cc,xx,yy)==' '){
					x0=xx;
					y0=yy;
				}else{
					if (getcor(cc,xx,yy)=='.') printf("you win!!!\n");
					xx=x0;
					yy=y0;
				}
					
				on_steps();
				}
				
			
				printf("%d\n",key);



				
				
			}
			
	
		on_refresh();
	
		if ((e.type == ClientMessage) && 
        (static_cast<unsigned int>(e.xclient.data.l[0]) == WM_DELETE_WINDOW))
			{
				on_exit();
				break;

			}
			

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


ccx=((15-x0)*0.2)-1.6;
ccy=((15-y0)*0.2)-1.6;
if(rotation==0){
	ccx=ccx+0.1;
	ccy=ccy+0.1;
	cx=ccx;
	cy=ccy-0.1;
}

if(rotation==1){
	ccx=ccx+0.0;
	ccy=ccy+0.1;
	cx=ccx+0.1;
	cy=ccy;
}


if(rotation==2){
	ccx=ccx+0.1;
	ccy=ccy+0.0;
	cx=ccx;
	cy=ccy+0.1;
}


if(rotation==3){
	ccx=ccx+0.0;
	ccy=ccy+0.1;
	cx=ccx-0.1;
	cy=ccy;
}


printf("rotation %d\n",rotation);
printf(">%f,%f,%f,%f,%f,%f,%f,%f\n",x0,y0,xx,yy,cx,cy,ccx,ccy);
}


void drawBlock(float x,float z){
float xx=x;
float zz=z;
 xx=((15-xx)*0.2)-1.6;
 zz=((15-zz)*0.2)-1.6;
 glBegin(GL_QUADS);
  glColor4f(0.0, 0.0, 1.0,1.0);
  glVertex3f(-.1+xx, -.1, .1+zz);
  glColor4f(0.0,1.0,0.0,0.1);
  glVertex3f( .1+xx, -.1, .1+zz);
  glColor4f(1.0,0.0,0.0,0.1);
  glVertex3f( .1+xx,  .1, .1+zz);
  glColor4f(1.0,1.0,1.0,0.1);
  glVertex3f(-.1+xx, .1, .1+zz);
 glEnd();
 
 glBegin(GL_QUADS);
  glColor4f(0.0, 0.0, 1.0,1.0);
  glVertex3f(-.1+xx, -.1, -.1+zz);
  glColor4f(0.0,1.0,0.0,0.1);
  glVertex3f( .1+xx, -.1, -.1+zz);
  glColor4f(1.0,0.0,0.0,0.1);
  glVertex3f( .1+xx,  .1, -.1+zz);
  glColor4f(1.0,1.0,1.0,0.1);
  glVertex3f(-.1+xx, .1, -.1+zz);
 glEnd();

 glBegin(GL_QUADS);
  glColor4f(0.0, 0.0, 1.0,1.0);
  glVertex3f(.1+xx, .1, .1+zz);
  glColor4f(0.0,1.0,0.0,0.1);
  glVertex3f( .1+xx, .1, -.1+zz);
  glColor4f(1.0,0.0,0.0,0.1);
  glVertex3f( .1+xx,  -.1, -.1+zz);
  glColor4f(1.0,1.0,1.0,0.1);
  glVertex3f(.1+xx, -.1, .1+zz);
 glEnd();

 glBegin(GL_QUADS);
  glColor4f(0.0, 0.0, 1.0,1.0);
  glVertex3f(-.1+xx, .1, .1+zz);
  glColor4f(0.0,1.0,0.0,0.1);
  glVertex3f(-.1+xx, .1, -.1+zz);
  glColor4f(1.0,0.0,0.0,0.1);
  glVertex3f(-.1+xx,  -.1, -.1+zz);
  glColor4f(1.0,1.0,1.0,0.1);
  glVertex3f(-.1+xx, -.1, .1+zz);
 glEnd();

glBegin(GL_QUADS);
  glColor3f(1.0, 1.0, 0.0);
  glVertex3f(.1+xx, -.1, .1+zz);
  glVertex3f(.1+xx, -.1, -.1+zz);
  glVertex3f(-.1+xx,  -.1, -.1+zz);
  glVertex3f(-.1+xx, -.1, .1+zz);
 glEnd();


 glBegin(GL_QUADS);
  glColor3f(0.0, 1.0, 1.0);
  glVertex3f(.1+xx, .1, .1+zz);
  glVertex3f(.1+xx, .1, -.1+zz);
  glVertex3f(-.1+xx,  .1, -.1+zz);
  glVertex3f(-.1+xx, .1, .1+zz);
 glEnd();



}


char getcor(const char *c,float x,float y){
int xx=(int) x;
int yy=(int) y;
yy=yy*16;
xx=xx+yy;
return c[xx];
}

void formap(){
float x;
float y;
char c;
const char *cc;
		cc=&maps[0];

	for(y=0;y<16.0;y=y+1.0){
		for(x=0;x<16.0;x=x+1.0){
			c=getcor(cc,x,y);
			if(c=='o')drawBlock(x,y);
		}			
	}
	
}

void cursor(){
float cccx=((15-x0)*0.2)-1.6;
float cccy=((15-y0)*0.2)-1.6;

glBegin(GL_QUADS);
  glColor3f(1.0, 1.0, 0.0);
  glVertex3f(.1+cccx, -.1, .1+cccy);
  glVertex3f(.1+cccx, -.1, -.1+cccy);
  glVertex3f(-.1+cccx,  -.1, -.1+cccy);
  glVertex3f(-.1+cccx, -.1, .1+cccy);
 glEnd();


}










