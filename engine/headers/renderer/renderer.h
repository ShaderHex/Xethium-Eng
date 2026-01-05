
namespace Renderer {

class Renderer {

public:  
    void Init();
    void StartDrawing();
private:
    unsigned int shaderProgram;
    unsigned int VBO, VAO;
};

}
