#include "mesh.h"
#include "game.h"

#include <cassert>
#include "includes.h"
#include "shader.h"
#include "extra/textparser.h"

Mesh::Mesh()
{
	vertices_vbo_id = 0;
	normals_vbo_id = 0;
	uvs_vbo_id = 0;
	colors_vbo_id = 0;
    indices_vbo_id = 0;
    vao = 0;
}

Mesh::Mesh( const Mesh& m )
{
	vertices = m.vertices;
	normals = m.normals;
	uvs = m.uvs;
	colors = m.colors;
    indices = m.indices;
    vao = m.vao;
}

Mesh::~Mesh()
{
	if(vertices_vbo_id) glDeleteBuffers(1, &vertices_vbo_id);
	if(normals_vbo_id) glDeleteBuffers(1, &normals_vbo_id);
	if(uvs_vbo_id) glDeleteBuffers(1, &uvs_vbo_id);
	if(colors_vbo_id) glDeleteBuffers(1, &colors_vbo_id);
    if(colors_vbo_id) glDeleteBuffers(1, &indices_vbo_id);
    if(vao)glDeleteVertexArrays(1, &vao);
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
    indices.clear();
}

void Mesh::genBuffers() {
    assert(vertices.size() && "No vertices in this mesh");
    

    //delete old
    if(vertices_vbo_id) glDeleteBuffers(1, &vertices_vbo_id);
    if(normals_vbo_id) glDeleteBuffers(1, &normals_vbo_id);
    if(uvs_vbo_id) glDeleteBuffers(1, &uvs_vbo_id);
    if(colors_vbo_id) glDeleteBuffers(1, &colors_vbo_id);
    
    //bind vao to store state
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers( 1, &vertices_vbo_id ); //generate one handler (id)
    glBindBuffer( GL_ARRAY_BUFFER, vertices_vbo_id ); //bind the handler
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW ); //upload data
    
    //shader must a_vertex attribute, error if not
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(VERTEX_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    
    if(normals.size())
    {
        glGenBuffers( 1, &normals_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, normals_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(float), &normals[0], GL_STATIC_DRAW ); //upload data
        
        //shader must have a_normal attribute
        glEnableVertexAttribArray(NORMAL_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(NORMAL_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );

    }
    
    if(uvs.size())
    {
        glGenBuffers( 1, &uvs_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, uvs_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, uvs.size() * 2 * sizeof(float), &uvs[0], GL_STATIC_DRAW ); //upload data
        
        //shader must have a_uv attribute
        glEnableVertexAttribArray(UV_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(UV_ATTRIBUTE_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );
        
    }
    
    if(colors.size())
    {
        glGenBuffers( 1, &colors_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, colors_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, uvs.size() * 4 * sizeof(float), &colors[0], GL_STATIC_DRAW ); //upload data

        //shader must have a_color attribute
        glEnableVertexAttribArray(COLOR_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(COLOR_ATTRIBUTE_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );
        
    }
    
    if (indices.size() == 0){
        //if there is no index buffer, make an 'emergency' serial one here
        for (size_t i = 0; i < vertices.size(); i++) {
            indices.push_back((unsigned int)i);
        }
    }
    
    //create index array
    glGenBuffers(1, &indices_vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0); //unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer
    
}

void Mesh::render( int primitive, Shader* sh )
{
    
    glBindVertexArray(vao);
    
    glDrawElements(primitive, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    
}



void Mesh::createPlane(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
	indices.clear();

	//create four vertices for corner of quad

	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(-size, 0, size));
	vertices.push_back( Vector3(-size,0,-size) );
	vertices.push_back( Vector3(size,0,-size) );
	

	//all of them have the same normal
	normals.push_back( Vector3(0,1,1) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );


	//texture coordinates
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(0,1) );
	uvs.push_back( Vector2(0,0) );
	uvs.push_back( Vector2(1,0) );
    
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(1);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);


    
    this->genBuffers();
}
//loadASE function 
bool Mesh::loadASE(const char* filename) {

	//clear stuff just in case
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
	indices.clear();

	//init TextParser
	TextParser t;
	if (t.create(filename) == false) {
		std::cout << "Could not open file" << std::endl;
		return false;
	}

	//get numverts and faces
	t.seek("*MESH_NUMVERTEX");
	int num_verts = t.getint();
	t.seek("*MESH_NUMFACES");
	int num_faces = t.getint();

	//resize our arrays
	vertices.resize(num_faces * 3);
	normals.resize(num_faces * 3);
	uvs.resize(num_faces * 3);

	//create unique positions array and fill it with positions
	std::vector<Vector3> unique_positions;
	unique_positions.resize(num_verts);
	int index;
	float x, y, z;
	for (int i = 0; i < num_verts; i++) {
		t.seek("*MESH_VERTEX");
		index = t.getint();
		x = (float)t.getfloat();
		y = (float)t.getfloat();
		z = (float)t.getfloat();

		unique_positions[index] = Vector3(-x, z, y);
	}

	//for all faces
	int aID, bID, cID;
	for (int i = 0; i < num_faces; i++) {
		//search for face
		t.seek("*MESH_FACE");
		//get ids
		t.seek("A:");
		aID = t.getint();
		t.seek("B:");
		bID = t.getint();
		t.seek("C:");
		cID = t.getint();
		//fill vertices array with Vector3 positions
		vertices[i * 3 + 0] = unique_positions[aID];
		vertices[i * 3 + 1] = unique_positions[bID];
		vertices[i * 3 + 2] = unique_positions[cID];

	}

	t.seek("*MESH_NUMTVERTEX");
	int num_tex = t.getint();
	std::vector<Vector2> unique_tex;
	unique_tex.resize(num_tex);
	float vtxU, vtxV;
	for (int i = 0; i < num_tex; i++) {
		t.seek("*MESH_TVERT");
		index = t.getint();
		vtxU = t.getfloat();
		vtxV = t.getfloat();

		unique_tex[i] = Vector2(vtxU, vtxV);
	}

	//now get texture faces
	t.seek("*MESH_TFACELIST");
	int aUV, bUV, cUV;
	for (int i = 0; i < num_faces; i++) {
		t.seek("*MESH_TFACE");
		index = t.getint();
		aUV = t.getint();
		bUV = t.getint();
		cUV = t.getint();

		uvs[i * 3 + 0] = unique_tex[aUV];
		uvs[i * 3 + 1] = unique_tex[bUV];
		uvs[i * 3 + 2] = unique_tex[cUV];
	}

	//now do same for norms
	int aId;
	float nX, nY, nZ;
	for (int i = 0; i < num_faces; i++)
	{
		t.seek("*MESH_VERTEXNORMAL");
		aId = t.getint();
		nX = (float)t.getfloat();
		nY = (float)t.getfloat();
		nZ = (float)t.getfloat();
		normals[i * 3] = Vector3(-nX, nZ, nY);
		t.seek("*MESH_VERTEXNORMAL");
		aId = t.getint();
		nX = (float)t.getfloat();
		nY = (float)t.getfloat();
		nZ = (float)t.getfloat();
		normals[i * 3 + 1] = Vector3(-nX, nZ, nY);
		t.seek("*MESH_VERTEXNORMAL");
		aId = t.getint();
		nX = (float)t.getfloat();
		nY = (float)t.getfloat();
		nZ = (float)t.getfloat();
		normals[i * 3 + 2] = Vector3(-nX, nZ, nY);
	}


	// send to the GPU - easy to forget!
	this->genBuffers();

	return true;

}
