/*
 * cmd2model.h
 *
 *  Created on: 23 de dic. de 2016
 *      Author: fjimartinez
 */

#ifndef CMD2MODEL_H_
#define CMD2MODEL_H_

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>

#include "glm\vec3.hpp"
#include "glm\vec2.hpp"
#include "glm\mat4x4.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp" //Header for glm::perspective, etc...
#include "glm\gtx\vector_angle.hpp"
#include "glm\gtc\constants.hpp"
#include "cshader.h"
#include "cmaterial.h"
#include "cworld.h"
#include "cfiledef.h"
#include "cgameobject.h"
#include "texturemanager.h"
#include "materialmanager.h"
#include "anorms.h"

using namespace std;

/* MD2 header */
typedef struct
{
  int ident;                  /* magic number: "IDP2" */
  int version;                /* version: must be 8 */

  int skinwidth;              /* texture width */
  int skinheight;             /* texture height */

  int framesize;              /* size in bytes of a frame */

  int num_skins;              /* number of skins */
  int num_vertices;           /* number of vertices per frame */
  int num_st;                 /* number of texture coordinates */
  int num_tris;               /* number of triangles */
  int num_glcmds;             /* number of opengl commands */
  int num_frames;             /* number of frames */

  int offset_skins;           /* offset skin data */
  int offset_st;              /* offset texture coordinate data */
  int offset_tris;            /* offset triangle data */
  int offset_frames;          /* offset frame data */
  int offset_glcmds;          /* offset OpenGL command data */
  int offset_end;             /* offset end of file */

} md2_header_t;

typedef struct
{
	char name[64];

} md2_skin_t;

typedef struct
{
	short s;
	short t;

} md2_texcoord_t;

typedef struct
{
	unsigned short vertex[3];   /* vertex indices of the triangle */
	unsigned short st[3];       /* tex. coord. indices */

} md2_triangle_t;

/* Compressed vertex */
typedef struct
{
  unsigned char v[3];         /* position */
  unsigned char normalIndex;  /* normal vector index */

} md2_vertex_t;

/* Vector */
typedef float vec3_t[3];

typedef struct
{
	vec3_t scale;               /* scale factor */
	vec3_t translate;           /* translation vector */
	char name[16];              /* frame name */
	md2_vertex_t *verts; /* list of frame's vertices */

} md2_frame_t;

typedef struct
{
   float       	scale[3];       // scale values
   float       	translate[3];   // translation vector
   char        	name[16];       // frame name
   md2_vertex_t verts[1];       // first vertex of this frame

} frame_t;

typedef struct
{
	char x;
	char y;
	char z;

} vec2_t;

typedef struct
{
	GLuint frame;   // num fram
	GLuint vBuffer; // Vertex buffer
	GLuint nBuffer; // Normal buffer
} fr_vnBuffer;

//Struct for animation
typedef struct
{
   int startframe;     // first frame
   int endframe;       // last frame
   int fps;            // frame per second for this animation

   float curr_time;    // current time
   float old_time;     // old time
   float interpolation;     // percent of interpolation

   int type;           // animation type

   int curr_frame;     // current frame
   int next_frame;     // next frame

} animState_t;

typedef enum  {STAND1 = 0, STAND2, WALK, ATTACK, PAIN, DIE } md2_state_t;

class CWorld;

/*******************
 * CMd2model class *
 *******************/
class CMd2model : public CGameObject
{
public:
	md2_header_t m_header;
	md2_skin_t *m_skins;
	md2_texcoord_t *m_texcoords;
	md2_triangle_t *m_triangles;
	md2_frame_t *m_frames;

	animState_t m_AnimateState;

	GLuint m_vertsPerFrame;
	GLuint m_iFrame;

	GLuint m_idTexture;

	std::vector<fr_vnBuffer> frame_vnBuffer;
	GLuint m_iTexelBuffer;

	md2_state_t m_currState;

	bool m_walkingTo;

public:
	bool loadMD2Model(const string s_filename);

	void startAnimation();
	void stopAnimation();
	void pauseAnimation();

	void setState(md2_state_t state);

	void UpdateAnimation(float dt);

	void setActiveFrame(int f);
	int getActiveFrame();

	void WalkTo(glm::vec2 newPosition);

	CMd2model(const string s_filename);
	virtual ~CMd2model();

};

#endif /* CMD2MODEL_H_ */
