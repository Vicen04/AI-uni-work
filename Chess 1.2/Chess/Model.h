#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <GL/glut.h>

class Model
{
private:
    //Attribute
    std::vector<float> position_texture_normal;
    int sz;
    //Function
    std::vector<std::string> splitString(std::string str, char delimiter);
    float getFloat(std::string input);
    int getInt(std::string input);
	GLuint m_textureWhite;
	GLuint m_textureBlack;
public:
    Model(std::string objFilePath);
    ~Model();
    void Draw(bool white);
	void Initialise(const char* szTextureNameBlack, const char* szTextureNameWhite);

protected:
	void loadTextureWhite(const char* szTextureFileName);
	void loadTextureBlack(const char* szTextureFileName);
};

#endif
