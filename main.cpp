/**
Sprite sheets.
*/

#include "Sprite_Sheet.h"
#include <iomanip>


int main(int argc, char *argv[])
{
 try
 {
   Sprite_Sheet sheet;
   //sheet.set_src_pathname("graphics/source");
   sheet.new_sheet(800, 200, "sheet2.png");
   sheet.new_sprite("ABCDEFGHI");
   sheet.new_item("blue.png");
   sheet.new_series(7);
   sheet.new_item("green.png");
   sheet.move_to(204, 4);
   sheet.new_item("blue.png");
   //sheet.finish();
   sheet.save();
 }
 catch( runtime_error e )
 {
   cerr << "Sprite sheet error: " << e.what() << endl;
 }

 SDL_Surface *surfl = IMG_Load("sheet2.png");
 SDL_Surface *surf = SDL_ConvertSurfaceFormat(surfl, SDL_PIXELFORMAT_RGBA8888, 0);
 Uint32 v, prev;
 int ct = 0;
 for( int r=0; r != 16; r++)
 {
   for(int c=0; c!=surf->w; c++)
   {
     v = *((Uint32*)(surf->pixels) + c + r*(surf->pitch)/4);
     if( c == 0 || v != prev )
     {
       if( ct > 1 )
         cout << "(x" << ct << ") ";
       cout << hex << setw(8) << v << dec << " ";
       prev = v;
       ct = 1;
     }
     else 
     {
       ++ct;    
     }
   }
   if( ct > 1 )
     cout << "(x" << ct << ")";
   cout << "\n";
   ct = 0;
 }

 
/*
 SDL_Surface *b = SDL_CreateRGBSurfaceWithFormat(0, 60, 10, 32, SDL_PIXELFORMAT_RGBA8888);
 SDL_FillRect(b, NULL, SDL_MapRGB(b->format, 0, 0, 0xff));
 IMG_SavePNG(b, "blue.png");
 */

 /*
 SDL_Window *window = SDL_CreateWindow("Sprite sheet", 100, 100, 400, 300, 0);
 SDL_Renderer *ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
 SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
 SDL_SetRenderDrawColor(ren, 0xff, 0xff, 0, 0xff);
 SDL_RenderClear(ren);
 SDL_Rect srcrect = { 0, 0, surf->w, surf->h };
 SDL_Rect dstrect = { 100, 100, surf->w, surf->h };
 SDL_RenderCopy(ren, texture, NULL, &dstrect);
 SDL_RenderPresent(ren);

 SDL_Event e;
 
 while( SDL_WaitEvent(&e) && e.type != SDL_QUIT )
	   ;
 */

 return 0;
}








