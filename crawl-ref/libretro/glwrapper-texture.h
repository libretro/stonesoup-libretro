#ifndef RETROGL_GL_WRAPPER_TEXTURE_H
#define RETROGL_GL_WRAPPER_TEXTURE_H

struct FBTexture
{
   unsigned int width;
   unsigned int height;
   unsigned int* pixels;

   FBTexture() : width(0), height(0), pixels(0)
   {

   }

   ~FBTexture()
   {
      delete[] pixels;
   }

   void set_size(unsigned int width_, unsigned int height_)
   {
      if (width == width_ && height == height_)
         return;

      width = width_;
      height = height_;

      delete[] pixels;
      pixels = new unsigned int[width * height];
   }

   const unsigned int* get_pixel_ptr(float u, float v) const
   {
      if (!pixels) return 0;

      if (u < 0) u = 0;
      if (u > 1) u = 1;
      if (v < 0) v = 0;
      if (v > 1) v = 1;

      unsigned int x = (float)width * u;
      unsigned int y = (float)height * v;

      if (x == width) x -= 1;
      if (y == height) y -= 1;

      return &pixels[y * width + x];
   }

   unsigned int get_pixel(float u, float v) const
   {
      if (!pixels) return 0;
      return *get_pixel_ptr(u, v);
   }

   static unsigned int get_reversed_colour(unsigned int colour)
   {
      unsigned int result = colour & 0xFF00FF00;
      result |= (colour & 0xFF) << 16;
      result |= (colour >> 16) & 0xFF;
      return result;
   }

   void load_pixels(unsigned char *data, unsigned int _width, unsigned int _height,
         int xoffset, int yoffset)
   {
      const unsigned int* const px = (unsigned int*)data;

      if (xoffset < 0 || yoffset < 0)
      {
         set_size(_width, _height);

         if (data)
            for (unsigned i = 0; i != _width * _height; i ++)
               pixels[i] = get_reversed_colour(px[i]);
         else
            memset(pixels, 0, _width * _height * 4);
      }
      else
      {
         if (pixels == 0)
            abort();

         for (unsigned int i = 0; i != _height; i ++)
         {
            auto ty = yoffset + i;
            if (ty >= height)
               break;

            for (unsigned int j = 0; j != _width; j ++)
            {
               auto tx = xoffset + j;
               if (tx >= width)
                  break;

               pixels[ty * width + tx] = get_reversed_colour(px[i * _width + j]);
            }
         }
      }
   }
};

#endif
