#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 700;

class Force{
    private:
    public:
};

class Moment{
    public:
    private:
    float value;
};


class Disk{
    public:
    Disk();
    //initializes future texture
    ~Disk();
    //deallocate memory
    private:
    float angle;
    float angular_velocity;
    float angular_acc;
    float x;
    float y;
    float vx;
    float vy;
    float ax;
    float ay;
    float mass;
};

class DiskTexture{
    public:
    DiskTexture();
    ~DiskTexture();
    bool loadFromFile( std::string path );
    void free();
    void setColor(Uint8 red, Uint8 green, Uint8 blue );
    //To set blend mode 
    void setBlendMode( SDL_BlendMode blending );
    //No blend mode
    void render( int x, int y, double angle = 0.0, SDL_Rect* clip = NULL, SDL_Point* center = NULL, 
    SDL_RendererFlip flip = SDL_FLIP_NONE );
    int getWidth();
    int getHeight();
    private:
    SDL_Texture* dTexture;
    int dWidth;
    int dHeight;
};

DiskTexture gDiskTexture;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL; 

//Initialization
DiskTexture::DiskTexture(){
    dTexture = NULL;
    dWidth = 0;
    dHeight = 0;
}

//Destroy
DiskTexture::~DiskTexture(){
    free();
}

//Deallocate memory
void DiskTexture::free(){
    if( dTexture != NULL){
        SDL_DestroyTexture( dTexture );
        dTexture = NULL;
        dWidth = 0;
        dHeight = 0;
    }
}

//Loading from file
bool DiskTexture::loadFromFile( std::string path ){
    //get rid of the existing texture
    free();
    // the final texture
    SDL_Texture* newTexture = NULL;
    //Load from the specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str());
    if( loadedSurface == NULL){
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else{
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format,
        0,0,0));
    
    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface);
    if( newTexture == NULL ){
        printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }
    else{
        dWidth = loadedSurface->w;
        dHeight = loadedSurface->h;
    }
    //Get rid of the old loaded surface
    SDL_FreeSurface( loadedSurface);
    }
    //Return sucess
    dTexture = newTexture;
    return dTexture !=  NULL;
    }

void DiskTexture::setColor( Uint8 red, Uint8 green, Uint8 blue){
    SDL_SetTextureColorMod( dTexture, red, green, blue );
}

void DiskTexture::setBlendMode( SDL_BlendMode blending ){
    SDL_SetTextureBlendMode( dTexture, blending );
}

void DiskTexture::render( int x, int y, double angle, SDL_Rect* clip,
SDL_Point* center, SDL_RendererFlip flip){

    SDL_Rect renderQuad = { x, y, dWidth, dHeight };
    if( clip != NULL){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    //Render to screen
    SDL_RenderCopyEx( gRenderer, dTexture, clip, &renderQuad, angle, center, flip);

}

int DiskTexture::getHeight(){
    return dHeight;
}

int DiskTexture::getWidth(){
    return dWidth;
}

bool init(){
    //Initialization flag
    bool success = true;
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0){
        printf("SDL could not be initialize ! SDL Error : %s\n", SDL_GetError());
        success = false;
    }
    else{
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1")){
            printf( "Warning: Linear Texture Filtering not enabled!");
        }
        //Create window
        gWindow = SDL_CreateWindow("0",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if( gWindow == NULL ){
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else{
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL ){
                printf( " Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else{
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF);
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !(IMG_Init( imgFlags ) & imgFlags) ){
                    printf( "SDL2/SDL_image could not be initialize! SDL2/SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

bool loadMedia(){
    //Loading success flag
    bool success = true;
    if( !gDiskTexture.loadFromFile( "./disk0.bmp" ) ){
        printf(" Failed to load disk texture! \n" );
        success = false;
    }
    return success;
}

void close(){
    //Free loaded texture
    gDiskTexture.free();
    //destroy windows
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gRenderer = NULL;
    gWindow = NULL;
    //Quit SDL subsystem
    IMG_Quit();
    SDL_Quit();

}

int main(int argc, char* args[]){
    //Start up and create window
    if( !init() ){
        printf("Failed to initialize!\n");
    }
    else{
        if( !loadMedia() ){
            printf(" Failed to load media!\n");
        }
        else{
            //Main loop flag
            bool quit = false;
            //Event handler
            SDL_Event e;
            //while application is running
            while( !quit ){
                //Handle event on queue
                while( SDL_PollEvent( &e ) != 0){
                    if( e.type == SDL_QUIT){
                        quit = true;
                    }
                }
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF);
                SDL_RenderClear( gRenderer );
                gDiskTexture.render(100,100, fmod(0.5* SDL_GetTicks(), 360.0));
                SDL_RenderPresent( gRenderer );
            }

        }
    }
    close();
    return 0;
}