/*
  Fast Circle Algorithm

  Here's one I've used. It draws an outlined circle with centre
  at point (xc,yc) and given radius. This is in pseudo C code and
  assumes an aspect ratio of 1:1 with no clipping and no
  explanation - suffice to say it's fast!
*/

   x = 0;
   y = radius;
   k = radius >> 3;
   do
   {
      plot (xc+x,yc-y);   /** mirror point about 45 degrees **/
      plot (xc+y,yc-x);
      plot (xc+y,yc+x);
      plot (xc+x,yc+y);
      plot (xc-x,yc+y);
      plot (xc-y,yc+x);
      plot (xc-y,yc-x);
      plot (xc-x,yc-y);
      k -= x++;
      if (k < 0) k += y--;
   }
   while (x <= y);
