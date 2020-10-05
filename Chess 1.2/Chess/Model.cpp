
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <iostream>


#include "stb_image.h"


#include "Model.h"
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif // GLUT Library

#include "Model.h"


Model::Model(std::string objFilePath)
{
	m_textureWhite = 0;
	m_textureBlack = 0;
    std::vector<float> position_vector;
    std::vector<float> normal_vector;
    std::vector<float> texture_coord;

    std::fstream obj;
	std::string input;
	std::vector<std::string> split;
	obj.open(objFilePath);
	if(obj.is_open())
	{
		while(getline(obj, input))
		{
			split = splitString(input, ' ');
			int sz = split.size();
			if(sz==0)continue;
			if(split[0]=="v")
            {
                position_vector.push_back(getFloat(split[1]));
                position_vector.push_back(getFloat(split[2]));
                position_vector.push_back(getFloat(split[3]));
            }
			else if(split[0]=="vt")
            {
                texture_coord.push_back(getFloat(split[1]));
                texture_coord.push_back(getFloat(split[2]));
            }
			else if(split[0]=="vn")
            {
                normal_vector.push_back(getFloat(split[1]));
                normal_vector.push_back(getFloat(split[2]));
                normal_vector.push_back(getFloat(split[3]));
            }
            else if(split[0]=="f")
            {
                std::vector<std::string> inSplit;
                for(int i=1; i<=3; i++)
                {
                    inSplit = splitString(split[i], '/');
                    int index = 3 * (getInt(inSplit[0])-1);
                    position_texture_normal.push_back(position_vector[index]);
                    position_texture_normal.push_back(position_vector[index+1]);
                    position_texture_normal.push_back(position_vector[index+2]);
                    index = 2 * (getInt(inSplit[1])-1);
                    position_texture_normal.push_back(texture_coord[index]);
                    position_texture_normal.push_back(texture_coord[index+1]);
                    index = 3 * (getInt(inSplit[2])-1);
                    position_texture_normal.push_back(normal_vector[index]);
                    position_texture_normal.push_back(normal_vector[index+1]);
                    position_texture_normal.push_back(normal_vector[index+2]);
                }
            }
		}
	}
    sz = position_texture_normal.size() / 8;
}

void Model::Initialise(const char* szTextureNameBlack, const char* szTextureNameWhite)
{
	loadTextureWhite(szTextureNameWhite);
	loadTextureBlack(szTextureNameBlack);
}

void Model::loadTextureWhite(const char* szTextureFileName)
{
	GLenum err = glGetError();
	
	glGenTextures(1, &m_textureWhite);
	err = glGetError();
	glBindTexture(GL_TEXTURE_2D, m_textureWhite);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(szTextureFileName, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Model::loadTextureBlack(const char* szTextureFileName)
{
	GLenum err = glGetError();

	glGenTextures(1, &m_textureBlack);
	err = glGetError();
	glBindTexture(GL_TEXTURE_2D, m_textureBlack);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(szTextureFileName, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Model::Draw(bool white)
{
    int index;
	glEnable(GL_TEXTURE_2D);
	if(!white)
		glBindTexture(GL_TEXTURE_2D, m_textureBlack);
	else
		glBindTexture(GL_TEXTURE_2D, m_textureWhite);

    glPushMatrix();
	GLfloat fScale = 50;
	glScalef(fScale, fScale, fScale);
        glBegin(GL_TRIANGLES);
            for(int i=0; i<sz; i++)
            {
                index = 8 * i;
                glNormal3f(position_texture_normal[index+5], position_texture_normal[index+6], position_texture_normal[index+7]);
                glTexCoord2f(position_texture_normal[index+3], position_texture_normal[index+4]);
                glVertex3f(position_texture_normal[index], position_texture_normal[index+1], position_texture_normal[index+2]);
            }
        glEnd();
    glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

Model::~Model()
{
    position_texture_normal.clear();
}

std::vector<std::string> Model::splitString(std::string str, char delimiter)
{
	std::vector<std::string> split;
	int sz = str.length(), lastIdx = -1, i;

	for(i=0; i<sz; i++)
	{
		if(str[i]==delimiter)
		{
			split.push_back(str.substr(lastIdx+1, i-lastIdx-1));
			lastIdx = i;
		}
	}
	if(i==sz && lastIdx!=i-1) split.push_back(str.substr(lastIdx+1));

	return split;
}

float Model::getFloat(std::string input)
{
    std::istringstream ssinput(input);
	float check;

	ssinput >> check;
	return check;
}

int Model::getInt(std::string input)
{
    std::istringstream ssinput(input);
	int check;

	ssinput >> check;
	return check;
}
