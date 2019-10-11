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
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>



const char files[]="house.3d";
char varvalue[64000];
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
int key=2;
float ccx=2.0;
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
void on_timer();
void on_exit();
void on_refresh();
void on_steps();
void drawBlock(float x,float y,float z,float red,float green,float blue);
char getcor(const char *c,float x,float y,float z);
void formap();
ssize_t on_load(const char *file,char *var);



void render() {
 glClearColor(0.5, 0.5, 0.5, 1.0);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-1., 1., -1., 1., 1., 20.);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();


 gluLookAt(ox, 7.0,oz,0.0, 0.0, 0., 0., 1., 0.);
 
 
formap();
} 

void on_timer(){
		usleep(300);
		on_steps();
		on_refresh();		
		
		
}


 
int main(int argc, char *argv[]) {
const char *f=&files[0];
char *v=&varvalue[0];
ssize_t counter=on_load(f,v);
int n=0;



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


 XStoreName(dpy, win, "view 3D");
 
 glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
 glXMakeCurrent(dpy, win, glc);
 
 glEnable(GL_DEPTH_TEST); 
 
 for (n=0;n<10;n++)printf("%d=%d\n",n,varvalue[n]);
 
 on_timer();
 	

 
   while (1) {
     if(XPending(dpy)){
      XNextEvent(dpy, &e);
		

      if (e.type == Expose){
	  }
		  
          if (e.type == KeyPress){
			  printf("key\n");
			char buf[128] = {0};
			const char *cc;
			KeySym keysym;
			int len = XLookupString(&e.xkey, buf, sizeof buf, &keysym, NULL);
	
			if (keysym == XK_Escape){
				on_exit();
				break;
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







void drawBlock(float x,float y,float z,float red,float green,float blue){
float xx=x;
float yy=y;
float zz=z;
 xx=((15-xx)*0.2)-1.6;
 yy=((15-yy)*0.2)-1.6;
 zz=((15-zz)*0.2)-1.6;
 glBegin(GL_QUADS);
  glColor3f(red, green, blue);
  glVertex3f(-.1+xx, -.1+yy, .1+zz);
  glVertex3f( .1+xx, -.1+yy, .1+zz);
  glVertex3f( .1+xx,  .1+yy, .1+zz);
  glVertex3f(-.1+xx, .1+yy, .1+zz);
 glEnd();
 
 glBegin(GL_QUADS);
  glColor3f(red, green, blue);
  glVertex3f(-.1+xx, -.1+yy, -.1+zz);
  glVertex3f( .1+xx, -.1+yy, -.1+zz);
  glVertex3f( .1+xx,  .1+yy, -.1+zz);
  glVertex3f(-.1+xx, .1+yy, -.1+zz);
 glEnd();

 glBegin(GL_QUADS);
  glColor3f(red, green, blue);
  glVertex3f(.1+xx, .1+yy, .1+zz);
  glVertex3f( .1+xx, .1+yy, -.1+zz);
  glVertex3f( .1+xx,  -.1+yy, -.1+zz);
  glVertex3f(.1+xx, -.1+yy, .1+zz);
 glEnd();

 glBegin(GL_QUADS);
  glColor3f(red, green, blue);
  glVertex3f(-.1+xx, .1+yy, .1+zz);
  glVertex3f(-.1+xx, .1+yy, -.1+zz);
  glVertex3f(-.1+xx,  -.1+yy, -.1+zz);
  glVertex3f(-.1+xx, -.1+yy, .1+zz);
 glEnd();

glBegin(GL_QUADS);
  glColor3f(red, green, blue);
  glVertex3f(.1+xx, -.1+yy, .1+zz);
  glVertex3f(.1+xx, -.1+yy, -.1+zz);
  glVertex3f(-.1+xx,  -.1+yy, -.1+zz);
  glVertex3f(-.1+xx, -.1+yy, .1+zz);
 glEnd();


 glBegin(GL_QUADS);
  glColor3f(red, green, blue);
  glVertex3f(.1+xx, .1+yy, .1+zz);
  glVertex3f(.1+xx, .1+yy, -.1+zz);
  glVertex3f(-.1+xx,  .1+yy, -.1+zz);
  glVertex3f(-.1+xx, .1+yy, .1+zz);
 glEnd();



}


char getcor(const char *c,float x,float y,float z){
int xx=(int) x;
int yy=(int) y;
int zz=(int) z;
zz=64*zz;
yy=yy*8;
xx=xx+yy+zz+6;
return c[xx];
}

void formap(){
float x;
float y;
float z;
char c;
const char *cc;
		cc=&varvalue[0];

for(z=0;z<8.0;z=z+1.0){
	for(y=0;y<8.0;y=y+1.0){
		for(x=0;x<8;x=x+1.0){
			c=getcor(cc,x,y,z);
			if(c==15)drawBlock(x,y,z,1.0,1.0,1.0);
			if(c==1)drawBlock(x,y,z,0.0,0.0,0.0);
			if(c==2)drawBlock(x,y,z,0.0,0.0,1.0);
			if(c==3)drawBlock(x,y,z,0.0,1.0,0.0);
			if(c==4)drawBlock(x,y,z,1.0,0.0,0.0);
			if(c==5)drawBlock(x,y,z,1.0,1.0,0.0);
			if(c==6)drawBlock(x,y,z,1.0,0.0,1.0);
			if(c==7)drawBlock(x,y,z,0.0,1.0,1.0);
			if(c==8)drawBlock(x,y,z,0.5,0.5,0.5);
			if(c==9)drawBlock(x,y,z,0.5,1.0,1.0);
			if(c==10)drawBlock(x,y,z,0.5,0.5,1.0);
			if(c==11)drawBlock(x,y,z,0.5,1.0,0.5);
			if(c==12)drawBlock(x,y,z,1.0,0.5,0.5);
			if(c==13)drawBlock(x,y,z,1.0,1.0,0.5);
			if(c==14)drawBlock(x,y,z,1.0,0.0,1.0);
		}			
	}

}	
}


ssize_t on_load(const char *file,char *var){
	ssize_t counters;
	int f1;
	if (file!=NULL){
		f1=open(file,O_RDONLY);
		counters=read(f1,var,8*8*8+6);
		close(f1);
	}
	return counters;

}










