// T3DLIB1.H - Header file for T3DLIB1.CPP game engine library

// watch for multiple inclusions
#ifndef T3DLIB1
#define T3DLIB1

// DEFINES ////////////////////////////////////////////////

// default screen size
#define SCREEN_WIDTH        800  // size of screen
#define SCREEN_HEIGHT       600
#define SCREEN_BPP          8    // bits per pixel
#define MAX_COLORS_PALETTE  256

// bitmap defines
#define BITMAP_ID            0x4D42 // universal id for a bitmap
#define BITMAP_STATE_DEAD    0
#define BITMAP_STATE_ALIVE   1
#define BITMAP_STATE_DYING   2 
#define BITMAP_ATTR_LOADED   128

#define BITMAP_EXTRACT_MODE_CELL  0
#define BITMAP_EXTRACT_MODE_ABS   1

// defines for BOBs
#define BOB_STATE_DEAD         0    // this is a dead bob
#define BOB_STATE_ALIVE        1    // this is a live bob
#define BOB_STATE_DYING        2    // this bob is dying
#define BOB_STATE_ANIM_DONE    1    // done animation state
#define MAX_BOB_FRAMES         64   // maximum number of bob frames
#define MAX_BOB_ANIMATIONS     16   // maximum number of animation sequeces

#define BOB_ATTR_SINGLE_FRAME   1   // bob has single frame
#define BOB_ATTR_MULTI_FRAME    2   // bob has multiple frames
#define BOB_ATTR_MULTI_ANIM     4   // bob has multiple animations
#define BOB_ATTR_ANIM_ONE_SHOT  8   // bob will perform the animation once
#define BOB_ATTR_VISIBLE        16  // bob is visible
#define BOB_ATTR_BOUNCE         32  // bob bounces off edges
#define BOB_ATTR_WRAPAROUND     64  // bob wraps around edges
#define BOB_ATTR_LOADED         128 // the bob has been loaded
#define BOB_ATTR_CLONE          256 // the bob is a clone

// screen transition commands
#define SCREEN_DARKNESS  0         // fade to black
#define SCREEN_WHITENESS 1         // fade to white
#define SCREEN_SWIPE_X   2         // do a horizontal swipe
#define SCREEN_SWIPE_Y   3         // do a vertical swipe
#define SCREEN_DISOLVE   4         // a pixel disolve
#define SCREEN_SCRUNCH   5         // a square compression
#define SCREEN_BLUENESS  6         // fade to blue
#define SCREEN_REDNESS   7         // fade to red
#define SCREEN_GREENNESS 8         // fade to green

// defines for Blink_Colors
#define BLINKER_ADD           0    // add a light to database  
#define BLINKER_DELETE        1    // delete a light from database
#define BLINKER_UPDATE        2    // update a light
#define BLINKER_RUN           3    // run normal

const double PI = 3.1415926535;

// fixed point mathematics constants
#define FIXP16_SHIFT     16
#define FIXP16_MAG       65536
#define FIXP16_DP_MASK   0x0000ffff
#define FIXP16_WP_MASK   0xffff0000
#define FIXP16_ROUND_UP  0x00008000

// MACROS /////////////////////////////////////////////////

// these read the keyboard asynchronously
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// this builds a 16 bit color value in 5.5.5 format (1-bit alpha mode)
#define _RGB16BIT555(r,g,b) ((b%32) + ((g%32) << 5) + ((r%32) << 10))

// this builds a 16 bit color value in 5.6.5 format (green dominate mode)
#define _RGB16BIT565(r,g,b) ((b%32) + ((g%64) << 6) + ((r%32) << 11))

// bit manipulation macros
#define SET_BIT(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define RESET_BIT(word,bit_flag) ((word)=((word) & (~bit_flag)))

// initializes a direct draw struct, basically zeros it and sets the dwSize field
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

// used to compute the min and max of two expresions
#define MIN(a, b)  (((a) < (b)) ? (a) : (b)) 
#define MAX(a, b)  (((a) > (b)) ? (b) : (a)) 

// used for swapping algorithm
#define SWAP(a,b,t) {t=a; a=b; b=t;}

// some math macros
#define DEG_TO_RAD(ang) ((ang)*PI/180)
#define RAD_TO_DEG(rads) ((rads)*180/PI)

// TYPES //////////////////////////////////////////////////

// basic unsigned types
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

// container structure for bitmaps .BMP file
typedef struct BITMAP_FILE_TAG
        {
        BITMAPFILEHEADER bitmapfileheader;  // this contains the bitmapfile header
        BITMAPINFOHEADER bitmapinfoheader;  // this is all the info including the palette
        PALETTEENTRY     palette[256];      // we will store the palette here
        UCHAR            *buffer;           // this is a pointer to the data

        } BITMAP_FILE, *BITMAP_FILE_PTR;

// the blitter object structure BOB
typedef struct BOB_TYP
        {
        int state;          // the state of the object (general)
        int anim_state;     // an animation state variable, up to you
        int attr;           // attributes pertaining to the object (general)
        int x,y;            // position bitmap will be displayed at
        int xv,yv;          // velocity of object
        int width, height;  // the width and height of the bob
        int width_fill;     // internal, used to force 8*x wide surfaces
        int counter_1;      // general counters
        int counter_2;
        int max_count_1;    // general threshold values;
        int max_count_2;
        int varsI[16];      // stack of 16 integers
        float varsF[16];    // stack of 16 floats
        int curr_frame;     // current animation frame
        int num_frames;     // total number of animation frames
        int curr_animation; // index of current animation
        int anim_counter;   // used to time animation transitions
        int anim_index;     // animation element index
        int anim_count_max; // number of cycles before animation
        int *animations[MAX_BOB_ANIMATIONS]; // animation sequences

        LPDIRECTDRAWSURFACE4 images[MAX_BOB_FRAMES]; // the bitmap images DD surfaces
 
        } BOB, *BOB_PTR;

// the simple bitmap image
typedef struct BITMAP_IMAGE_TYP
        {
        int state;          // state of bitmap
        int attr;           // attributes of bitmap
        int x,y;            // position of bitmap
        int width, height;  // size of bitmap
        int num_bytes;      // total bytes of bitmap
        UCHAR *buffer;      // pixels of bitmap

        } BITMAP_IMAGE, *BITMAP_IMAGE_PTR;

// blinking light structure
typedef struct BLINKER_TYP
               {
               // user sets these
               int color_index;         // index of color to blink
               PALETTEENTRY on_color;   // RGB value of "on" color
               PALETTEENTRY off_color;  // RGB value of "off" color
               int on_time;             // number of frames to keep "on" 
               int off_time;            // number of frames to keep "off"

               // internal member
               int counter;             // counter for state transitions
               int state;               // state of light, -1 off, 1 on, 0 dead
               } BLINKER, *BLINKER_PTR;

// a 2D vertex
typedef struct VERTEX2DI_TYP
        {
        int x,y; // the vertex
        } VERTEX2DI, *VERTEX2DI_PTR;

// a 2D vertex
typedef struct VERTEX2DF_TYP
        {
        float x,y; // the vertex
        } VERTEX2DF, *VERTEX2DF_PTR;


// a 2D polygon
typedef struct POLYGON2D_TYP
        {
        int state;        // state of polygon
        int num_verts;    // number of vertices
        int x0,y0;        // position of center of polygon  
        int xv,yv;        // initial velocity
        DWORD color;      // could be index or PALETTENTRY
        VERTEX2DF *vlist; // pointer to vertex list
 
        } POLYGON2D, *POLYGON2D_PTR;


// matrix defines
typedef struct MATRIX3X3_TYP
        {
        float M[3][3]; // data storage
        } MATRIX3X3, *MATRIX3X3_PTR;

typedef struct MATRIX1X3_TYP
        {
        float M[3]; // data storage
        } MATRIX1X3, *MATRIX1X3_PTR;

typedef struct MATRIX3X2_TYP
        {
        float M[3][2]; // data storage
        } MATRIX3X2, *MATRIX3X2_PTR;

typedef struct MATRIX1X2_TYP
        {
        float M[2]; // data storage
        } MATRIX1X2, *MATRIX1X2_PTR;


// PROTOTYPES /////////////////////////////////////////////

// DirectDraw functions
int DDraw_Init(int width, int height, int bpp);
int DDraw_Shutdown(void);
LPDIRECTDRAWCLIPPER DDraw_Attach_Clipper(LPDIRECTDRAWSURFACE4 lpdds, int num_rects, LPRECT clip_list);
LPDIRECTDRAWSURFACE4 DDraw_Create_Surface(int width, int height, int mem_flags);
int DDraw_Flip(void);
int DDraw_Wait_For_Vsync(void);
int DDraw_Fill_Surface(LPDIRECTDRAWSURFACE4 lpdds,int color);
UCHAR *DDraw_Lock_Surface(LPDIRECTDRAWSURFACE4 lpdds,int *lpitch);
int DDraw_Unlock_Surface(LPDIRECTDRAWSURFACE4 lpdds);
UCHAR *DDraw_Lock_Primary_Surface(void);
int DDraw_Unlock_Primary_Surface(void);
UCHAR *DDraw_Lock_Back_Surface(void);
int DDraw_Unlock_Back_Surface(void);

// BOB functions
int Create_BOB(BOB_PTR bob,int x, int y,int width, int height,int num_frames,int attr,int mem_flags);              
int Clone_BOB(BOB_PTR source, BOB_PTR dest);
int Destroy_BOB(BOB_PTR bob);
//int Draw_BOB(BOB_PTR bob, LPDIRECTDRAWSURFACE4 dest);
int Draw_BOB(BOB_PTR bob, LPDIRECTDRAWSURFACE4 dest, int transparent=1);
int Draw_BOB(BOB_PTR bob, LPDIRECTDRAWSURFACE4 dest, int transparent, int x, int y);
int Draw_Scaled_BOB(BOB_PTR bob, int swidth, int sheight,LPDIRECTDRAWSURFACE4 dest);
int Load_Frame_BOB(BOB_PTR bob, BITMAP_FILE_PTR bitmap, int frame, int cx,int cy,int mode);                   // cx,cy are absolute coords
int Animate_BOB(BOB_PTR bob);
int Move_BOB(BOB_PTR bob);
int Load_Animation_BOB(BOB_PTR bob, int anim_index, int num_frames, int *sequence);
int Set_Pos_BOB(BOB_PTR bob, int x, int y);
int Set_Vel_BOB(BOB_PTR bob,int xv, int yv);
int Set_Anim_Speed_BOB(BOB_PTR bob,int speed);
int Set_Animation_BOB(BOB_PTR bob, int anim_index);
int Hide_BOB(BOB_PTR bob);
int Show_BOB(BOB_PTR bob);
int Collision_BOBS(BOB_PTR bob1, BOB_PTR bob2);


// general utility functions
DWORD Get_Clock(void);
DWORD Start_Clock(void);
DWORD Wait_Clock(DWORD count);

int Collision_Test(int x1, int y1, int w1, int h1, 
                   int x2, int y2, int w2, int h2); 

int Color_Scan(int x1, int y1, int x2, int y2, 
               UCHAR scan_start, UCHAR scan_end, 
               UCHAR *scan_buffer, int scan_lpitch);

// graphics functions
int Draw_Clip_Line(int x0,int y0, int x1, int y1,UCHAR color,UCHAR *dest_buffer, int lpitch);
int Clip_Line(int &x1,int &y1,int &x2, int &y2);
int Draw_Line(int xo, int yo, int x1,int y1, UCHAR color,UCHAR *vb_start,int lpitch);
//inline int Draw_Pixel(int x, int y,int color,UCHAR *video_buffer, int lpitch);
int Draw_Rectangle(int x1, int y1, int x2, int y2, int color,LPDIRECTDRAWSURFACE4 lpdds);
void HLine(int x1,int x2,int y,int color, UCHAR *vbuffer, int lpitch);
void VLine(int y1,int y2,int x,int color, UCHAR *vbuffer, int lpitch);
void Screen_Transitions(int effect, UCHAR *vbuffer, int lpitch);
int Draw_Pixel(int x, int y,int color,UCHAR *video_buffer, int lpitch);


// palette functions
int Set_Palette_Entry(int color_index, LPPALETTEENTRY color);
int Get_Palette_Entry(int color_index, LPPALETTEENTRY color);
int Load_Palette_From_File(char *filename, LPPALETTEENTRY palette);
int Save_Palette_To_File(char *filename, LPPALETTEENTRY palette);
int Save_Palette(LPPALETTEENTRY sav_palette);
int Set_Palette(LPPALETTEENTRY set_palette);
int Rotate_Colors(int start_index, int end_index);
int Blink_Colors(int command, BLINKER_PTR new_light, int id);
inline void Copy_Palette(LPPALETTEENTRY dest, LPPALETTEENTRY source)
{ memcpy(dest, source, 256*sizeof(PALETTEENTRY)); }

// simple bitmap image functions
int Create_Bitmap(BITMAP_IMAGE_PTR image, int x, int y, int width, int height);
int Destroy_Bitmap(BITMAP_IMAGE_PTR image);
int Draw_Bitmap(BITMAP_IMAGE_PTR source_bitmap,UCHAR *dest_buffer, int lpitch, int transparent);
int Load_Image_Bitmap(BITMAP_IMAGE_PTR image,BITMAP_FILE_PTR bitmap,int cx,int cy,int mode);               
int Scroll_Bitmap(void); // ni
int Copy_Bitmap(void); // ni
int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height);

// bitmap file functions
int Load_Bitmap_File(BITMAP_FILE_PTR bitmap, char *filename);
int Unload_Bitmap_File(BITMAP_FILE_PTR bitmap);

// gdi functions
int Draw_Text_GDI(char *text, int x,int y,COLORREF color, LPDIRECTDRAWSURFACE4 lpdds);
int Draw_Text_GDI(char *text, int x,int y,int color, LPDIRECTDRAWSURFACE4 lpdds);

// error functions
int Open_Error_File(char *filename);
int Close_Error_File(void);
int Write_Error(char *string, ...);

// 2d 8-bit triangle rendering
void Draw_Top_Tri(int x1,int y1,int x2,int y2, int x3,int y3,int color,UCHAR *dest_buffer, int mempitch);

void Draw_Bottom_Tri(int x1,int y1, int x2,int y2, int x3,int y3,int color,UCHAR *dest_buffer, int mempitch);

void Draw_Top_TriFP(int x1,int y1,int x2,int y2, int x3,int y3,int color,UCHAR *dest_buffer, int mempitch);

void Draw_Bottom_TriFP(int x1,int y1, int x2,int y2, int x3,int y3,int color,UCHAR *dest_buffer, int mempitch);

void Draw_Triangle_2D(int x1,int y1,int x2,int y2,int x3,int y3,
                      int color,UCHAR *dest_buffer, int mempitch);

void Draw_TriangleFP_2D(int x1,int y1,int x2,int y2,int x3,int y3,
                        int color,UCHAR *dest_buffer, int mempitch);

inline void Draw_QuadFP_2D(int x0,int y0,int x1,int y1,
                           int x2,int y2,int x3, int y3,
                           int color,UCHAR *dest_buffer, int mempitch);


// general 2D polygon rendering and transforming functions
void Draw_Filled_Polygon2D(POLYGON2D_PTR poly, UCHAR *vbuffer, int mempitch);
int Translate_Polygon2D(POLYGON2D_PTR poly, int dx, int dy);
int Rotate_Polygon2D(POLYGON2D_PTR poly, int theta);
int Scale_Polygon2D(POLYGON2D_PTR poly, float sx, float sy);
void Build_Sin_Cos_Tables(void);
int Translate_Polygon2D_Mat(POLYGON2D_PTR poly, int dx, int dy);
int Rotate_Polygon2D_Mat(POLYGON2D_PTR poly, int theta);
int Scale_Polygon2D_Mat(POLYGON2D_PTR poly, float sx, float sy);
int Draw_Polygon2D(POLYGON2D_PTR poly, UCHAR *vbuffer, int lpitch);

// math functions
int Fast_Distance_2D(int x, int y);
float Fast_Distance_3D(float x, float y, float z);

// collision detection functions
int Find_Bounding_Box_Poly2D(POLYGON2D_PTR poly, 
                             float &min_x, float &max_x, 
                             float &min_y, float &max_y);

int Mat_Mul1X2_3X2(MATRIX1X2_PTR ma, 
                   MATRIX3X2_PTR mb,
                   MATRIX1X2_PTR mprod);

int Mat_Mul1X3_3X3(MATRIX1X3_PTR ma, 
                   MATRIX3X3_PTR mb,
                   MATRIX1X3_PTR mprod);

int Mat_Mul3X3(MATRIX3X3_PTR ma, 
               MATRIX3X3_PTR mb,
               MATRIX3X3_PTR mprod);

inline int Mat_Init_3X2(MATRIX3X2_PTR ma, 
                        float m00, float m01,
                        float m10, float m11,
                        float m20, float m21);


// GLOBALS ////////////////////////////////////////////////

extern FILE *fp_error;                           // general error file

// notice that interface 4.0 is used on a number of interfaces
extern LPDIRECTDRAW4        lpdd;                 // dd object
extern LPDIRECTDRAWSURFACE4 lpddsprimary;         // dd primary surface
extern LPDIRECTDRAWSURFACE4 lpddsback;            // dd back surface
extern LPDIRECTDRAWPALETTE  lpddpal;              // a pointer to the created dd palette
extern LPDIRECTDRAWCLIPPER  lpddclipper;          // dd clipper
extern PALETTEENTRY         palette[256];         // color palette
extern PALETTEENTRY         save_palette[256];    // used to save palettes
extern DDSURFACEDESC2       ddsd;                 // a direct draw surface description struct
extern DDBLTFX              ddbltfx;              // used to fill
extern DDSCAPS2             ddscaps;              // a direct draw surface capabilities struct
extern HRESULT              ddrval;               // result back from dd calls
extern UCHAR                *primary_buffer;      // primary video buffer
extern UCHAR                *back_buffer;         // secondary back buffer
extern int                  primary_lpitch;       // memory line pitch
extern int                  back_lpitch;          // memory line pitch
extern BITMAP_FILE          bitmap8bit;           // a 8 bit bitmap file
extern BITMAP_FILE          bitmap16bit;          // a 16 bit bitmap file
extern BITMAP_FILE          bitmap24bit;          // a 24 bit bitmap file


extern DWORD                start_clock_count;    // used for timing
extern int                  windowed_mode;        // tracks if dd is windowed or not

// these defined the general clipping rectangle for software clipping
extern int min_clip_x,                             // clipping rectangle 
           max_clip_x,                  
           min_clip_y,     
           max_clip_y;                  

// these are overwritten globally by DD_Init()
extern int screen_width,                            // width of screen
           screen_height,                           // height of screen
           screen_bpp;                              // bits per pixel 

extern int window_client_x0;   // used to track the starting (x,y) client area for
extern int window_client_y0;   // for windowed mode directdraw operations

// storage for our lookup tables
extern float cos_look[360];
extern float sin_look[360];


#endif


