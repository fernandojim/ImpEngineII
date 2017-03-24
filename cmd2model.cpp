/*
 * cmd2model.cpp
 *
 *  Created on: 23 de dic. de 2016
 *      Author: fjimartinez
 */

#include "cmd2model.h"

#include "utils.h"

using namespace ansiCToUnicode;
using namespace TextureManager;

ofstream salida;

bool first = true;

//----------------------------------------
// Constructor
//----------------------------------------
CMd2model::CMd2model(const string s_filename)  : CGameObject()
{
	m_iFrame = 0;
	m_iTexelBuffer = 0;

	m_AnimateState.startframe = 0;       // first frame
	m_AnimateState.endframe = 0;         // last frame
	m_AnimateState.fps = 0;              // frame per second for this animation
	m_AnimateState.curr_time = 0.0;      // current time
	m_AnimateState.old_time = 0.0;       // old time
	m_AnimateState.interpolation = 0.0 ; // percent of interpolation
	m_AnimateState.type = 0;             // animation type
	m_AnimateState.curr_frame = 0;       // current frame
	m_AnimateState.next_frame = 0;       // next frame

	frame_vnBuffer.clear();              // Clear the list of frame - vertex and normal buffer

	setState(md2_state_t::STAND1);

	//Geometry initializations
	m_angle = glm::radians(0.0);
	m_speed = 0.0;
	m_angles = glm::vec3(0.0, 0.0, 0.0);
	m_qrot = glm::quat(m_angles);
	m_qrotTo = glm::quat(m_angles);

	loadMD2Model(s_filename);

	m_walkingTo = false;

	salida.open("salida.txt", std::ifstream::binary|std::ifstream::trunc);
}

//----------------------------------------
// loadMD2Model method : public
//----------------------------------------
bool CMd2model::loadMD2Model(const string s_filename)
{
	CFileDef *file = NULL;
	glm::vec3 v;
	glm::vec3 n;
	glm::vec2 t;
	GLuint id_buffer = 0;
	fr_vnBuffer fvn;

	//Open the file
	file = new CFileDef(s_filename, true);

	if (file->getIsOpen())
	{
		//Reads the header of MD2 model
		file->m_Filedef.read((char*)&m_header, sizeof(md2_header_t));

		if ((m_header.ident != 844121161) || (m_header.version != 8))
		{
			//Error
		    cout << "Error: bad MD2 version or identifier\n";

		    return false;
		}

		//Alloc memory
		m_skins = new md2_skin_t[m_header.num_skins];
		m_texcoords = new md2_texcoord_t[m_header.num_st];
		m_triangles = new md2_triangle_t[m_header.num_tris];
		m_frames = new md2_frame_t[m_header.num_frames];

		//Read skins info
		file->m_Filedef.seekg(m_header.offset_skins, ios::beg);
		file->m_Filedef.read((char*)&m_skins[0], sizeof(md2_skin_t) * m_header.num_skins);

		//Calcule num vertex per frame
		m_vertsPerFrame = m_header.num_tris * 3;

		//Read the texture
		//m_idTexture = ::getTextureManager().loadTexture("texturas\\" + m_skins[0].name);
		m_idTexture = ::getTextureManager().loadTexture("texturas\\Imp.jpg");

		//Read texture coordinates info
		file->m_Filedef.seekg(m_header.offset_st, ios::beg);
		file->m_Filedef.read((char*)&m_texcoords[0], sizeof(md2_texcoord_t) * m_header.num_st);

		//Read triangles info
		file->m_Filedef.seekg(m_header.offset_tris, ios::beg);
		file->m_Filedef.read((char*)&m_triangles[0], sizeof(md2_triangle_t) * m_header.num_tris);

		//And finally read frames info
		file->m_Filedef.seekg(m_header.offset_frames, ios::beg);
		for (int i = 0; i< m_header.num_frames;i++)
		{
			m_frames[i].verts = new md2_vertex_t[m_header.num_vertices];
			file->m_Filedef.read((char*)&m_frames[i].scale, sizeof(vec3_t) * 1);
			file->m_Filedef.read((char*)&m_frames[i].translate, sizeof(vec3_t) * 1);
			file->m_Filedef.read((char*)&m_frames[i].name, sizeof(char) * 16);
			file->m_Filedef.read((char*)&m_frames[i].verts[0], sizeof(md2_vertex_t) * m_header.num_vertices);
		}

		file->m_Filedef.close();
	}
	else
		return false;

	//Read the texture coordinates
	for (int i = 0;i<m_header.num_tris;i++)
	{
		//3-verts per triangle
		for (int k = 0;k<3;k++)
		{
			t.x = (float)m_texcoords[m_triangles[i].st[k]].s / m_header.skinwidth;
			t.y = (float)m_texcoords[m_triangles[i].st[k]].t / m_header.skinheight;
			m_Texel.push_back(t);
		}
	}

	//Traverses the entire list of frames and its vertex, texel and normals info
	for (int j = 0;j<m_header.num_frames;j++)
	{
		//Create the Vertex, texel and Normal buffers traduced from md2 info
		for (int i = 0;i<m_header.num_tris;i++)
		{
			//3-verts per triangle
			for (int k = 0;k<3;k++)
			{
				//Vertex buffer
				v.x = (float)(m_frames[j].scale[0] * m_frames[j].verts[m_triangles[i].vertex[k]].v[0]) + m_frames[j].translate[0];
				v.y = (float)(m_frames[j].scale[2] * m_frames[j].verts[m_triangles[i].vertex[k]].v[2]) + m_frames[j].translate[2];
				v.z = (float)(m_frames[j].scale[1] * m_frames[j].verts[m_triangles[i].vertex[k]].v[1]) + m_frames[j].translate[1];
				m_Vertex.push_back(v);

				n.x = predefined_normal[m_frames[j].verts[m_triangles[i].vertex[k]].normalIndex].nx;
				n.y = predefined_normal[m_frames[j].verts[m_triangles[i].vertex[k]].normalIndex].ny;
				n.z = predefined_normal[m_frames[j].verts[m_triangles[i].vertex[k]].normalIndex].nz;
				m_Normal.push_back(n);
			}
		}

		//Get the frame-j
		fvn.frame = j;

		//Creates buffers for each frame
		// vertex buffer
		id_buffer = CreateArrayBuffer(&m_Vertex[0], m_vertsPerFrame * sizeof(float) * 3, GL_STATIC_DRAW);
		fvn.vBuffer = id_buffer;
		m_Vertex.clear();

		// Normal buffer
		id_buffer = CreateArrayBuffer(&m_Normal[0], m_vertsPerFrame * sizeof(float) * 3, GL_STATIC_DRAW);
		fvn.nBuffer = id_buffer;
		m_Normal.clear();

		frame_vnBuffer.push_back(fvn);
	}

	//Creates the buffer to Texel
	m_iTexelBuffer = CreateArrayBuffer(&m_Texel[0],  m_vertsPerFrame * sizeof(float) * 2, GL_STATIC_DRAW);
	m_Texel.clear();

	m_bIsValid = true;

	return true;
}

void CMd2model::setActiveFrame(int f)
{
	m_iFrame = f;
}

int CMd2model::getActiveFrame()
{
	return m_iFrame;
}

void CMd2model::setState(md2_state_t state)
{
	if (m_currState != state)
	{
		m_currState = state;

		switch (state)
		{
			case md2_state_t::STAND1:
				m_AnimateState.startframe   = 0;
				m_AnimateState.endframe     = 0;
				m_AnimateState.next_frame   = m_AnimateState.startframe;
				m_AnimateState.fps          = 5;
			break;

			case md2_state_t::STAND2:
				m_AnimateState.startframe   = 1;
				m_AnimateState.endframe     = 1;
				m_AnimateState.next_frame   = m_AnimateState.startframe;
				m_AnimateState.fps          = 5;
			break;

			case md2_state_t::WALK:
				m_AnimateState.startframe   = 2;
				m_AnimateState.endframe     = 5;
				m_AnimateState.next_frame   = m_AnimateState.startframe + 1;
				m_AnimateState.fps          = 5;
			break;

			case md2_state_t::ATTACK:
				m_AnimateState.startframe   = 6;
				m_AnimateState.endframe     = 10;
				m_AnimateState.next_frame   = m_AnimateState.startframe + 1;
				m_AnimateState.fps          = 5;
			break;

			case md2_state_t::PAIN:
				m_AnimateState.startframe   = 11;
				m_AnimateState.endframe     = 13;
				m_AnimateState.next_frame   = m_AnimateState.startframe + 1;
				m_AnimateState.fps          = 5;
			break;

			case md2_state_t::DIE:
				m_AnimateState.startframe   = 14;
				m_AnimateState.endframe     = 20;
				m_AnimateState.next_frame   = m_AnimateState.startframe + 1;
				m_AnimateState.fps          = 5;
			break;

			default:
				m_AnimateState.startframe   = 0;
				m_AnimateState.endframe     = 0;
				m_AnimateState.next_frame   = m_AnimateState.startframe;
				m_AnimateState.fps          = 5;
			break;
		}
	}
}

/*
 * ACTIONS
 */

//
// WALK TO A GIVEN POSITION
//
void CMd2model::WalkTo(glm::vec2 newPosition)
{
	/*float ang = 0.0;

	glm::vec2 vto = newPosition - glm::vec2(m_position.x, m_position.z);

	if ( glm::length(vto) > 3.0)
	{
		if (!m_walkingTo)
		{
			if (newPosition.y > m_position.z)
				ang = -glm::angle(glm::normalize(glm::vec2(m_facing.x, m_facing.z)), glm::normalize(vto));
			else
				ang = glm::angle(glm::normalize(glm::vec2(m_facing.x, m_facing.z)), glm::normalize(vto));

			m_angles = glm::vec3(0.0, ang , 0.0);
			m_qrotTo = glm::quat(m_angles);

			m_walkingTo = true;
			m_speed = 16.0;
		}

		m_qrot = glm::slerp(m_qrot, m_qrotTo, 0.05f);
		if (ang < 0.0)
			m_angle = -glm::angle(m_qrot);
		else
			m_angle = glm::angle(m_qrot);
	}
	else
	{
		m_walkingTo = false;
		m_speed = 0.0;
	}*/

	salida << "m_position=(" << m_position.x << "," << m_position.z << ") angle=" << m_angle << "\n";
}


void CMd2model::UpdateAnimation(float dt)
{
	m_position.x += m_velocity.x * dt;
	m_position.z += m_velocity.z * dt;

	if (m_speed > 0.0)
	{
		setState(md2_state_t::WALK);
		m_AnimateState.fps = m_speed * 0.35;
	}
	else
		setState(md2_state_t::STAND1);

	m_AnimateState.curr_time += dt;

	if(m_AnimateState.curr_time - m_AnimateState.old_time > (1.0f / float(m_AnimateState.fps)))
	{
		m_AnimateState.old_time = m_AnimateState.curr_time;

		m_AnimateState.curr_frame = m_AnimateState.next_frame;
		m_AnimateState.next_frame++;

		if(m_AnimateState.next_frame > m_AnimateState.endframe)
		{
			m_AnimateState.next_frame = m_AnimateState.startframe;
		}
	}

	m_AnimateState.interpolation = float(m_AnimateState.fps) * (m_AnimateState.curr_time - m_AnimateState.old_time);
}

//----------------------------------------
// Destructor
//----------------------------------------
CMd2model::~CMd2model()
{
	clear();
	salida.close();
}

