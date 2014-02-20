#include "main.h"

/* screen width height bit depth */
#define SWIDTH 1024                           
#define SHEIGHT 768                              
#define SDEPTH 16 

#define TRUE  1
#define FALSE 0

SDL_Surface *MainWindow;

float x = -0.1;
float y = 0.01;

float speed = 0.01;
float accel = 0.02;

/* 2 assignments booleans*/
bool impactCOM = FALSE;
bool impactOFFCOM = FALSE;

void Quit(int ret_val)
{
    SDL_Quit();
    exit(ret_val);                                        
}

/* Handle key Presses */
void handleKeyPress( SDL_keysym *keysym )
{
    switch ( keysym->sym )
	{
	case SDLK_ESCAPE:
	    Quit( 0 );
	    break;
	case SDLK_F1:
	    SDL_WM_ToggleFullScreen(MainWindow );
	    break;
	case SDLK_RIGHT:
	    impactCOM = TRUE;
	    break;
	case SDLK_LEFT:
	    impactOFFCOM = TRUE;
	    break;
	default:
	    break;
	}
    return;
}

/* OpenGL init */
int initGL( )
{

    glViewport( 0, 0, SWIDTH, SHEIGHT);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    /* Set perspective */
    gluPerspective( 45.0f, (GLfloat)SWIDTH/(GLfloat)SHEIGHT, 0.1f, 100.0f );
    glMatrixMode( GL_MODELVIEW );
    glShadeModel( GL_SMOOTH );

    /* bckgrnd black */
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    glClearDepth( 1.0f );

    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    return( TRUE );
}

void drawGLScene(float x, float y)
{

    static GLfloat recQuad;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glLoadIdentity( );

    glTranslatef(-1.5f,0.0f,-6.0f);	

    /* impact on centre of mass */
    if(impactCOM == TRUE){
	
	glBegin(GL_QUADS);
		glColor3f(0.5f,0.5f,1.0f);	
					
		glVertex3f(x, 1.0, 0.0f);				
		glVertex3f(y, 1.0, 0.0f);				
		glVertex3f(y, -1.0, 0.0f);				
		glVertex3f(x, -1.0, 0.0f);		
	glEnd();	
    }
     /* impact off centre of mass */
    else if(impactOFFCOM == TRUE){

	glRotatef( recQuad, 0.0f, 0.0f, 1.0f );

	glBegin( GL_QUADS ); 
		glColor3f(0.5f,0.5f,1.0f);
        
      		glVertex3f(  x,  1.0f,  0.0f ); 
      		glVertex3f(  y,  1.0f,  0.0f ); 
      		glVertex3f(  y, -1.0f,  0.0f ); 
      		glVertex3f(  x, -1.0f,  0.0f ); 
    	glEnd( );                            
    }	
											
    SDL_GL_SwapBuffers( );

    recQuad -= 0.25f;

}

void chkMove()
{
	if(impactCOM == TRUE){
		x = x + (speed + accel);
		y = y + (speed + accel);
		
		printf("%-1f\n" , x);
		printf("%-1f\n" , y);
		drawGLScene(x, y);
	}	
	else if(impactOFFCOM == TRUE){
		drawGLScene(x, y);
	}
}

int main( int argc, char **argv )
{

	/* init SDL Stuff from SDL docs*/

	int videoFlags;

	SDL_Event event;
	const SDL_VideoInfo *videoInfo;
    
    
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
	    fprintf( stderr, "Video initialization failed: %s\n",
		     SDL_GetError( ) );
	    Quit( 1 );
	}

    /* video info */
    videoInfo = SDL_GetVideoInfo( );

    if ( !videoInfo )
	{
	    fprintf( stderr, "Video query failed: %s\n",
		     SDL_GetError( ) );
	    Quit( 1 );
	}

    /* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          
    videoFlags |= SDL_GL_DOUBLEBUFFER; 
    videoFlags |= SDL_HWPALETTE;       
    videoFlags |= SDL_RESIZABLE;      

    /* This checks to see if surfaces can be stored in memory */
    if ( videoInfo->hw_available )
	videoFlags |= SDL_HWSURFACE;
    else
	videoFlags |= SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if ( videoInfo->blit_hw )
	videoFlags |= SDL_HWACCEL;

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    /* get a SDL surface */
    MainWindow = SDL_SetVideoMode( SWIDTH, SHEIGHT, SDEPTH, videoFlags );

    /* initialize OpenGL */
    if ( initGL( ) == FALSE )
	{
	    fprintf( stderr, "Could not initialize OpenGL.\n" );
	    Quit( 1 );
	}

    int finished = FALSE;
    /* wait for events */
    while ( !finished )
	{
	    while ( SDL_PollEvent( &event ) )
		{
		    switch( event.type )
			{
			case SDL_KEYDOWN:
			    /* handle key presses */
			    handleKeyPress( &event.key.keysym );
			    break;
			case SDL_QUIT:
			    /* handle quit requests */
			    finished= TRUE;
			    break;
			default:
			    break;
			}
		}

		printf("main\n" );
	    	chkMove();
		
	}

    Quit( 0 );

    return( 0 );
}
