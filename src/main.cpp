#include <iostream>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <map>
#include "../include/display_music.h"
#include "../include/world.h"
#undef main


int main(){
	// Init graphics setings.
  int sWidth = 1280;
  int sHeight = 720;

	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
    }
	else
	{

		window = SDL_CreateWindow("GuitarToSheet", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sWidth, sHeight , SDL_WINDOW_SHOWN);

    if (window == NULL)
      {
        printf("Failed to create window. SDL_Error: %s\n", SDL_GetError());
      }
		else
		{
      int crotchetFlags = IMG_INIT_PNG;
      bool success = true;
      if( !( IMG_Init( crotchetFlags ) & crotchetFlags ) )
        {
          printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
          success = false;
        }

      if(success){
        // Event handler
        SDL_Event e;

        // Probably want to update the user-input component along with draw rate at a diff speed to polling the python program output
        Uint32 framesPerSecond = 60;
        Uint32 timePerFrame = 1000 / framesPerSecond;

        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        screenSurface = SDL_GetWindowSurface(window);

        Resources resources = loadResources("images/legend", renderer);
        World world = initWorld(&resources);

        bool quit = false;
        Uint32 prevTime = SDL_GetTicks();
        while(quit == false){
            while(SDL_PollEvent(&e) != 0){
                if(e.type == SDL_QUIT){
                    quit = true;
                }
                else if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP){
                  // printf("keypress: %d", e.key.keysym.sym);
                }
                else{
                    // printf("Unrecognized SDL event %d \n", e.type);
                }
              }
            Uint32 curTime = SDL_GetTicks();
            Uint32 deltaTime = curTime - prevTime;
            if ( (deltaTime) > timePerFrame)
              {
                prevTime = curTime;

                // update
                readNotesToWorld("note_output", &world);

                // draw
                SDL_Color bgColor = { 255,255,255,255 };
                SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
                SDL_RenderClear(renderer);

                renderWorld(renderer, &world);

                // Note cStemUp = resources.notes["crotchetUp"];
                // cout << cStemUp.heightOffset << "\n";
                // cStemUp.dis.x += startX;
                // cStemUp.dis.y += firstBarY;
                //renderDisplayable(crotchetStemUp.dis, renderer);


                // Note n1 = cStemUp;
                // Note n2 = n1;
                // n2.dis.x += xInc;
                // n2.dis.y += 3*yStep;

                // renderDisplayable(n1.dis, renderer);
                // renderDisplayable(n2.dis, renderer);

                SDL_RenderPresent(renderer);
              }
          }
        // cleanup
      }
    }
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;

}
