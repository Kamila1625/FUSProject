#pragma once
class ModelGL
{
public:
    ModelGL();
    ~ModelGL();

    void init();                                    // initialize OpenGL states
    void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
    void setViewport(int width, int height);
    void resizeWindow(int width, int height);
    void draw();

    void setMouseLeft(bool flag) { mouseLeftDown = flag; };
    void setMouseRight(bool flag) { mouseRightDown = flag; };
    void setMousePosition(int x, int y) { mouseX = x; mouseY = y; };
    void setDrawMode(int mode);
    void animate(bool flag) { animateFlag = flag; };

    void rotateCamera(int x, int y);
    void zoomCamera(int dist);

    void setBackgroundRed(float value);             // change background colour
    void setBackgroundGreen(float value);
    void setBackgroundBlue(float value);


	bool createContext(HWND handle, int colorBits, int depthBits, int stencilBits);  // create OpenGL rendering context
	void closeContext(HWND handle);
	void swapBuffers();

	HDC getDC() const { return hdc; };
	HGLRC getRC() const { return hglrc; };

protected:

private:
    // member functions
    void initLights();                              // add a white light ti scene
    unsigned int createEarthDL();
    unsigned int loadTextureBmp(const char *filename);

    // members
    int windowWidth;
    int windowHeight;
    bool animateFlag;                               // on/off animation
    bool changeDrawMode;
    int drawMode;
    unsigned int listId;                            // display list ID
    bool mouseLeftDown;
    bool mouseRightDown;
    int mouseX;
    int mouseY;
    float cameraAngleX;
    float cameraAngleY;
    float cameraDistance;
    float bgColor[4];
    bool bgFlag;
    bool windowResized;
    unsigned char* frameBuffer;                     // framebuffer to store RGBA color
    int bufferSize;                                 // framebuffer size in bytes

													// member functions
	static bool setPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits);
	static int findPixelFormat(HDC hdc, int colorbits, int depthBits, int stencilBits); // return best matched format ID

	HDC hdc;                                        // handle to device context
	HGLRC hglrc;                                    // handle to OpenGL rendering context
};

