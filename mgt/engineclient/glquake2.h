/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
*	Copyright (C) 2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief

#pragma once

// disable data conversion warnings

#pragma warning(disable : 4244) // MIPS
#pragma warning(disable : 4136) // X86
#pragma warning(disable : 4051) // ALPHA

#include <windows.h>

#include <gl\gl.h>
#include <gl\glu.h>

void GL_BeginRendering(int *x, int *y, int *width, int *height);
void GL_EndRendering();

// Function prototypes for the Texture Object Extension routines
typedef GLboolean(APIENTRY *ARETEXRESFUNCPTR)(GLsizei, const GLuint *,
                                              const GLboolean *);
typedef void(APIENTRY *BINDTEXFUNCPTR)(GLenum, GLuint);
typedef void(APIENTRY *DELTEXFUNCPTR)(GLsizei, const GLuint *);
typedef void(APIENTRY *GENTEXFUNCPTR)(GLsizei, GLuint *);
typedef GLboolean(APIENTRY *ISTEXFUNCPTR)(GLuint);
typedef void(APIENTRY *PRIORTEXFUNCPTR)(GLsizei, const GLuint *,
                                        const GLclampf *);
typedef void(APIENTRY *TEXSUBIMAGEPTR)(int, int, int, int, int, int, int, int, void *);

extern BINDTEXFUNCPTR bindTexFunc;
extern DELTEXFUNCPTR delTexFunc;
extern TEXSUBIMAGEPTR TexSubImage2DFunc;

extern int texture_extension_number;
extern int texture_mode;

extern float gldepthmin, gldepthmax;

void GL_Upload32(unsigned *data, int width, int height, qboolean mipmap, qboolean alpha, qboolean modulate);
void GL_Upload8(byte *data, int width, int height, qboolean mipmap, qboolean alpha, qboolean modulate);
int GL_LoadTexture(char *identifier, int width, int height, byte *data, int mipmap, int alpha, int modulate);
int GL_FindTexture(char *identifier);

typedef struct
{
	float x, y, z;
	float s, t;
	float r, g, b;
} glvert_t;

extern glvert_t glv;

extern int glx, gly, glwidth, glheight;

extern PROC glArrayElementEXT;
extern PROC glColorPointerEXT;
extern PROC glTexturePointerEXT;
extern PROC glVertexPointerEXT;

// r_local.h -- private refresh defs

#define MAXALIASVERTS 2000 // TODO: tune this

#define ALIAS_BASE_SIZE_RATIO (1.0 / 11.0)
// normalizing factor so player model works out to about
//  1 pixel per triangle
#define MAX_LBM_HEIGHT 480

#define TILE_SIZE 128 // size of textures generated by R_GenTiledSurf

#define SKYSHIFT 7
#define SKYSIZE (1 << SKYSHIFT)
#define SKYMASK (SKYSIZE - 1)

#define BACKFACE_EPSILON 0.01

void R_TimeRefresh_f();
void R_ReadPointFile_f();
texture_t *R_TextureAnimation(texture_t *base);

typedef struct surfcache_s
{
	struct surfcache_s *next;
	struct surfcache_s **owner; // nullptr is an empty chunk of memory
	int lightadj[MAXLIGHTMAPS]; // checked for strobe flush
	int dlight;
	int size; // including header
	unsigned width;
	unsigned height; // DEBUG only needed for debug
	float mipscale;
	struct texture_s *texture; // checked for animating textures
	byte data[4];              // width*height elements
} surfcache_t;

typedef struct
{
	pixel_t *surfdat; // destination for generated surface
	int rowbytes;     // destination logical width in bytes
	msurface_t *surf; // description for surface to generate
	fixed8_t lightadj[MAXLIGHTMAPS];
	// adjust for lightmap levels for dynamic lighting
	texture_t *texture; // corrected for animating textures
	int surfmip;        // mipmapped ratio of surface texels / world pixels
	int surfwidth;      // in mipmapped texels
	int surfheight;     // in mipmapped texels
} drawsurf_t;


//====================================================

extern cl_entity_t r_worldentity;
extern qboolean r_cache_thrash; // compatability
extern vec3_t modelorg, r_entorigin;
extern cl_entity_t *currententity;
extern int r_visframecount; // ??? what difs?
extern int r_framecount;
extern mplane_t frustum[4];
extern int c_brush_polys, c_alias_polys;

//
// view origin
//
extern vec3_t vup;
extern vec3_t vpn;
extern vec3_t vright;
extern vec3_t r_origin;

//
// screen size info
//
extern refdef_t r_refdef;
extern mleaf_t *r_viewleaf, *r_oldviewleaf;
extern texture_t *r_notexture_mip;
extern int d_lightstylevalue[256]; // 8.8 fraction of base light value

extern qboolean envmap;
extern int currenttexture;
extern int particletexture;
extern int playertextures;

extern int skytexturenum; // index in cl.loadmodel, not gl texture object

extern cvar_t r_drawentities;
extern cvar_t r_drawworld;
extern cvar_t r_drawviewmodel;
extern cvar_t r_speeds;
extern cvar_t r_waterwarp;
extern cvar_t r_fullbright;
extern cvar_t r_lightmap;
extern cvar_t r_shadows;
extern cvar_t r_dynamic;

extern cvar_t gl_clear;
extern cvar_t gl_cull;
extern cvar_t gl_poly;
extern cvar_t gl_texsort;
extern cvar_t gl_smoothmodels;
extern cvar_t gl_affinemodels;
extern cvar_t gl_fogblend;
extern cvar_t gl_polyblend;
extern cvar_t gl_keeptjunctions;
extern cvar_t gl_reporttjunctions;

extern int gl_lightmap_format;
extern int gl_solid_format;
extern int gl_alpha_format;

void R_TranslatePlayerSkin(int playernum);
void GL_Bind(int texnum);
