
#include "Sprite_Sheet.h"

/* ------------------------------------------------------------------------------------- */

Sprite_Sheet::Sprite_Sheet()
 : sheet_(NULL)
{
 if( !SDL_WasInit(SDL_INIT_VIDEO) )
   SDL_Init(SDL_INIT_VIDEO);
 IMG_Init(IMG_INIT_PNG);

 new_sheet(200, 100, "sheet.png");
}

/* ------------------------------------------------------------------------------------- */
/** Start a new sheet, deleting previous data. */

void Sprite_Sheet::new_sheet(int width, int height, const string &filename)
{
 if( sheet_ )
   SDL_FreeSurface(sheet_);
 sheet_ = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
 SDL_FillRect(sheet_, NULL, SDL_MapRGBA(sheet_->format, 0xff, 0xff, 0x7f, 0xff));

 filename_ = filename;
 key_pixel_ = { -1, -1 };
 next_key_pixel_ = { 0, 0 };

 new_sprite("sprite");
}

/* ------------------------------------------------------------------------------------- */
/** Start a new sprite. */

void Sprite_Sheet::new_sprite(const string &name)
{
 item_type_ = NEW_SPRITE;
 sprite_name_ = name;
 new_series(0);
}

/* ------------------------------------------------------------------------------------- */
/** Start a new series of images. */

void Sprite_Sheet::new_series(int series_num)
{
 if( item_type_ != NEW_SPRITE )
   item_type_ = NEW_SERIES;
 series_num_ = series_num;
}

/* ------------------------------------------------------------------------------------- */
/** Add an image to the sheet. */

void Sprite_Sheet::new_item(const string &filename)
{
 // Finish the previous item by adding its key pixel:
 if( key_pixel_.x >= 0 )
   put_pixel(key_pixel_, (next_key_pixel_.x - key_pixel_.x) << 16
                       | (next_key_pixel_.y - key_pixel_.y) & 0xffff);
 key_pixel_ = next_key_pixel_;
 
 // Add the image:
 SDL_Surface *image = load_source_image(filename);
 SDL_Rect img_rect = { key_pixel_.x + 1, key_pixel_.y, image->w, image->h };
 SDL_BlitSurface(image, NULL, sheet_, &img_rect);

 // Add the metadata/dimensions pixel:
 SDL_Point pixel = key_pixel_;  ++pixel.y;
 put_pixel(pixel,  (item_type_ & 2) << 30 | (image->w & 0x7fff) << 16
                 | (item_type_ & 1) << 15 | (image->h & 0x7fff) );

 // Add the series number pixel:
 ++pixel.y;
 put_pixel(pixel, series_num_);

 // Add the sprite name, if required:
 ++pixel.y;
 if( item_type_ == NEW_SPRITE )
   put_sprite_name(pixel);

 // Set default values for the type and location of the next item:
 next_key_pixel_.x += image->w + 1;
 item_type_ = IMAGE;
}

/* ------------------------------------------------------------------------------------- */
/** Set location for (the key pixel of) the next item. */

void Sprite_Sheet::move_to(int x, int y)
{
 next_key_pixel_.x = x;
 next_key_pixel_.y = y;
}

/* ------------------------------------------------------------------------------------- */
/** Finish off a sheet. */

void Sprite_Sheet::finish()
{
 // Write the final key pixel:
 if( key_pixel_.x >= 0 )
   put_pixel(key_pixel_, 0);

 // Return to starting position:
 key_pixel_ = { -1, -1 };
 next_key_pixel_ = { 0, 0 };
}

/* ------------------------------------------------------------------------------------- */
/** Set the pathname to the location of source images. */

void Sprite_Sheet::set_src_pathname(const string &name)
{
 src_pathname_ = name;

 if( !(name.back() == '/' || name.back() == '\\') )
   src_pathname_ += '/';
}

/* ------------------------------------------------------------------------------------- */

void Sprite_Sheet::save()
{
 finish();

 IMG_SavePNG(sheet_, filename_.c_str());
}

/* ------------------------------------------------------------------------------------- */
/** Load a source image. */

SDL_Surface *Sprite_Sheet::load_source_image(const string &filename)
{
 SDL_Surface *image = IMG_Load((src_pathname_ + filename).c_str());

 if( !image )
   throw runtime_error("source image file \"" + src_pathname_ + filename + "\" not found.");

 return image;
}

/* ------------------------------------------------------------------------------------- */
/** Put a data pixel on the sheet. */

void Sprite_Sheet::put_pixel(SDL_Point pt, Uint32 val)
{
 *((Uint32*)(sheet_->pixels) + pt.x + pt.y*(sheet_->pitch)/4) = val;
}

/* ------------------------------------------------------------------------------------- */
/** Put data pixels encoding the sprite name on the sheet. */

void Sprite_Sheet::put_sprite_name(SDL_Point pt)
{
 Uint32 val = 0;
 int shift = 24;
 const char *p = sprite_name_.c_str();
 do
 {
   val |= (*p & 0xff) << shift;
   if( shift == 0 || *p == '\0' )
   {
     put_pixel(pt, val);
     ++pt.y;
     val = 0;
   }
   shift = (shift + 24) % 32;
 } while( *p++ != '\0' );
}
