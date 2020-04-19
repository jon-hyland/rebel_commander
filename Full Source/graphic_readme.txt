Notes on graphics and sprites:

This is a 256 color game that uses a specific 256 color palette.  I got the specific colors from Starcraft since that game is 256 as well.  The game runs in 640x480 mode.  All graphic files must be converted to this exact palette or the colors will be indexed wrong.  When drawing graphics in this mode it is sometimes easier to make the sprite in full color (lots of effects and filters don't work in 256) and then convert the sprite to the color palette afterwards.  The included "game_palette.pal" is the game palette.  It can be imported with Paint Shop Pro.

Sprites pages:  The graphic bmp's contain frames of a specifc sprite.  Each sprite has a certain height, width, number of frames, etc.  Changes can be made to these (changing the size of a sprite, frame count, etc) but it requires a simple code change for each.

The source is included with this package and requires Visual C++ 6 with SP5, and the DirectX 8.1 SDK to compile.

