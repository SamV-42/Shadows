//only include in other files
#ifndef UTILS_HPP
#define UTILS_HPP
GLuint TextureFromFile(std::string name, std::string directory) {
	GLuint text;
	glGenTextures(1, &text);

	glBindTexture(GL_TEXTURE_2D, text);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
    std::string location = directory + "/" + name;    //yeah whatever
	unsigned char* image = SOIL_load_image(location.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
    if(!image) {
        std::cout << "ERROR:: failed to load texture from " << location << std::endl;
    }
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	glBindTexture(GL_TEXTURE_2D, 0);
	return text;
}
#endif
