
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
using namespace std;

class Sprite_Sheet
{
public:
 Sprite_Sheet();
 void new_sheet(int width, int height, const string &filename);
 void new_sprite(const string &name);
 void new_series(int series_num);
 void new_item(const string &filename);
 void move_to(int x, int y);
 void finish();
 void set_src_pathname(const string &name);
 void save();

private:
 enum Item_Type { IMAGE, NEW_SERIES, NEW_DERIVED_SERIES, NEW_SPRITE };
 
 SDL_Surface *sheet_;
 string src_pathname_, filename_, sprite_name_;
 Item_Type item_type_;
 int series_num_;
 SDL_Point key_pixel_, next_key_pixel_;

 SDL_Surface *load_source_image(const string &filename);
 void put_pixel(SDL_Point pt, Uint32 val);
 void put_sprite_name(SDL_Point pt);
};