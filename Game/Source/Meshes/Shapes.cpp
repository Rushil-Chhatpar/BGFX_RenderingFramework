//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "VertexFormats.h"
#include "Shapes.h"

//=======================
// Triangle
//=======================
static const VertexFormat_PosColor g_TriangleVerts[] =
{
    { vec2( 0.0f, 0.5f), 255, 0, 0, 255 }, // Top
    { vec2(-0.5f,-0.5f), 0, 255, 0, 255 }, // Bottom-left
    { vec2( 0.5f,-0.5f), 0, 0, 255, 255 }, // Bottom-right
};
static const uint16 g_TriangleIndices[] =
{
    0, 2, 1,
};
fw::Mesh* CreateTriangleMesh()
{
    return new fw::Mesh( VertexFormat_PosColor::format, g_TriangleVerts, sizeof(g_TriangleVerts), g_TriangleIndices, sizeof(g_TriangleIndices) );
}

//=======================
// Square
//=======================
static const VertexFormat_PosColor g_SquareVerts[] =
{
    { vec2(-0.5f, 0.5f), 0, 255, 0, 255 }, // Top-left
    { vec2(-0.5f,-0.5f), 0,   0, 0, 255 }, // Bottom-left
    { vec2( 0.5f,-0.5f), 0,   0, 0, 255 }, // Bottom-right

    { vec2(-0.5f, 0.5f), 0, 255, 0, 255 }, // Top-left
    { vec2( 0.5f,-0.5f), 0,   0, 0, 255 }, // Bottom-right
    { vec2( 0.5f, 0.5f), 0, 255, 0, 255 }, // Top-right
};
static const uint16 g_SquareIndices[] =
{
    0,2,1, 3,5,4
};
fw::Mesh* CreateSquareMesh()
{
    return new fw::Mesh( VertexFormat_PosColor::format, g_SquareVerts, sizeof(g_SquareVerts), g_SquareIndices, sizeof(g_SquareIndices) );
}

//=======================
// Sprite
//=======================
static const VertexFormat_PosUV g_SpriteVerts[] =
{
    { vec2(-0.5f, 0.5f), vec2(0,1) }, // Top-left
    { vec2(-0.5f,-0.5f), vec2(0,0) }, // Bottom-left
    { vec2( 0.5f,-0.5f), vec2(1,0) }, // Bottom-right

    { vec2(-0.5f, 0.5f), vec2(0,1) }, // Top-left
    { vec2( 0.5f,-0.5f), vec2(1,0) }, // Bottom-right
    { vec2( 0.5f, 0.5f), vec2(1,1) }, // Top-right
};
static const uint16 g_SpriteIndices[] =
{
    0,2,1, 3,5,4
};
fw::Mesh* CreateSpriteMesh()
{
    return new fw::Mesh( VertexFormat_PosUV::format, g_SpriteVerts, sizeof(g_SpriteVerts), g_SpriteIndices, sizeof(g_SpriteIndices) );
}
//=======================
// Cube
//=======================
static const VertexFormat_Pos3dColor g_CubeVerts[] =
{
    { vec3(-0.5f, 0.5f, -0.5f), 255, 0, 0, 255 }, // Top-left
    { vec3(-0.5f,-0.5f, -0.5f), 0, 255, 0, 255  }, // Bottom-left
    { vec3(0.5f,-0.5f, -0.5f), 255, 0, 255, 255  }, // Bottom-right
    { vec3(0.5f, 0.5f, -0.5f), 0, 0, 255, 255  }, // Top-right

    { vec3(-0.5f, 0.5f, 0.5f), 0, 255, 0, 255  }, // Top-left
    { vec3(-0.5f,-0.5f, 0.5f), 255, 255, 0, 255  }, // Bottom-left
    { vec3(0.5f,-0.5f, 0.5f), 255, 0, 255, 255  }, // Bottom-right
    { vec3(0.5f, 0.5f, 0.5f), 255, 255, 255, 255  }, // Top-right
};
static const uint16 g_CubeIndices[] =
{
    0, 2, 1, 0, 3, 2, // front
    4, 5, 6, 4, 6, 7, //back
    4, 7, 3, 4, 3, 0, //top
    1, 2, 6, 1, 6, 5, // bottom
    4, 0, 1, 4, 1, 5, // left
    3, 7, 6, 3, 6, 2, //right
    

};
fw::Mesh* CreateCubeMesh()
{
    return new fw::Mesh(VertexFormat_Pos3dColor::format, g_CubeVerts, sizeof(g_CubeVerts), g_CubeIndices, sizeof(g_CubeIndices));
}
//=======================
// Cube with texture
//=======================
static const VertexFormat_Pos3dUV g_DieVerts[] = //1-6, 2-5, 3-4
{
    //front (1)
    { vec3(-0.5f, 0.5f, -0.5f), vec2(0, 1)},    //top left                             //0
    { vec3(0.5f, 0.5f, -0.5f), vec2(0.33f, 1)},     //top right                          //1
    { vec3(0.5f, -0.5f, -0.5f), vec2(0.33f, 0.5f)},  //bottom right                       //2
    { vec3(-0.5f, -0.5f, -0.5f), vec2(0, 0.5f)},   //bottom left                        //3
                                                                                       
    //back (6)                                                                         
    { vec3(0.5f, 0.5f, 0.5f), vec2(0.66f, 0.5f)}, // Top-left                             //4
    { vec3(-0.5f, 0.5f, 0.5f), vec2(1, 0.5f)}, // top right                             //5
    { vec3(-0.5f,-0.5f,0.5f), vec2(1, 0)}, // Bottom-right                             //6
    { vec3(0.5f,-0.5f, 0.5f), vec2(0.66f, 0)}, // bottom left                            //7
                                                                                       
    //top (4)                                                                          
    { vec3(-0.5f, 0.5f, 0.5f), vec2(0, 0.5f)}, //top left                               //8
    { vec3(0.5f, 0.5f, 0.5f), vec2(0.33f, 0.5f)},  //top right                            //9
    { vec3(0.5f, 0.5f, -0.5f), vec2(0.33f, 0)}, //bottom right                           //10
    { vec3(-0.5f, 0.5f, -0.5f), vec2(0, 0)}, //bottom left                             //11
                                                                                       
    //bottom(3)                                                                        
    { vec3(-0.5f, -0.5f, -0.5f), vec2(0.66f, 1)}, //top left                             //12
    { vec3(0.5f, -0.5f, -0.5f), vec2(1, 1)}, //top right                               //13
    { vec3(0.5f, -0.5f, 0.5f), vec2(1, 0.5f)}, //bottom right                           //14
    { vec3(-0.5f, -0.5f, 0.5f), vec2(0.66f, 0.5f)}, //bottom left                         //15
                                                                                       
    //left(2)                                                                          
    { vec3(-0.5f, 0.5f, 0.5f), vec2(0.33f, 1)}, //top left                               //16
    { vec3(-0.5f, 0.5f, -0.5f), vec2(0.66f, 1)}, //top right                             //17
    { vec3(-0.5f, -0.5f, -0.5f), vec2(0.66f, 0.5f)}, //bottom right                       //18
    { vec3(-0.5f, -0.5f, 0.5f), vec2(0.33f, 0.5f)}, //bottom left                         //19
                                                                                       
    //right(5)                                                                         
    { vec3(0.5f, 0.5f, -0.5f), vec2(0.33f, 0.5f)}, //top left                             //20
    { vec3(0.5f, 0.5f, 0.5f), vec2(0.66f, 0.5f)}, //top right                             //21
    { vec3(0.5f, -0.5f, 0.5f), vec2(0.66f, 0)}, //bottom right                           //22
    { vec3(0.5f, -0.5f, -0.5f), vec2(0.33f, 0)} //bottom left                            //23

};
static const uint16 g_DieIndices[] =
{
    0, 1, 2, 0, 2, 3,               //front
    4, 5, 6, 4, 6, 7,               //back
    8, 9, 10, 8, 10, 11,            //top
    12, 13, 14, 12, 14, 15,         //bottom
    16, 17, 18, 16, 18, 19,         //left
    20, 21, 22, 20, 22, 23          //right
};

fw::Mesh* CreateDieMesh()
{
    return new fw::Mesh(VertexFormat_Pos3dUV::format, g_DieVerts, sizeof(g_DieVerts), g_DieIndices, sizeof(g_DieIndices));
}

//=======================
// Coin BS
//=======================

static const VertexFormat_Pos3dUV g_CoinVerts[] = //1-6, 2-5, 3-4
{

    //front (1)
    { vec3(-0.5f, 0.5f, -0.01f), vec2(0.5f, 0.98f)},    //top left                             //0
    { vec3(0.5f, 0.5f, -0.01f), vec2(1, 0.98f)},     //top right                          //1
    { vec3(0.5f, -0.5f, -0.01f), vec2(1, 0.0f)},  //bottom right                       //2
    { vec3(-0.5f, -0.5f, -0.01f), vec2(0.5f, 0.0f)},   //bottom left                        //3

    //back (6)                                                                         
    { vec3(0.5f, 0.5f, 0.01f), vec2(0, 0.98f)}, // Top-left                             //4
    { vec3(-0.5f, 0.5f, 0.01f), vec2(0.5f, 0.98f)}, // top right                             //5
    { vec3(-0.5f,-0.5f,0.01f), vec2(0.5f, 0)}, // Bottom-right                             //6
    { vec3(0.5f,-0.5f, 0.01f), vec2(0, 0)}, // bottom left                            //7

};

static const uint16 g_CoinIndices[] =
{
    0, 1, 2, 0, 2, 3,               //front
    4, 5, 6, 4, 6, 7,               //back
};

fw::Mesh* CreateCoinMesh()
{
    return new fw::Mesh(VertexFormat_Pos3dUV::format, g_CoinVerts, sizeof(g_CoinVerts), g_CoinIndices, sizeof(g_CoinIndices));
}


static const VertexFormat_Pos3dUV g_ImageVerts[] = //1-6, 2-5, 3-4
{

    //front (1)
    { vec3(-0.5f, 0.5f, -0.01f), vec2(0, 1)},    //top left                             //0
    { vec3(0.5f, 0.5f, -0.01f), vec2(1, 1)},     //top right                          //1
    { vec3(0.5f, -0.5f, -0.01f), vec2(1, 0)},  //bottom right                       //2
    { vec3(-0.5f, -0.5f, -0.01f), vec2(0, 0)},   //bottom left                        //3

    //back (6)                                                                         
    { vec3(0.5f, 0.5f, 0.01f), vec2(0, 1)}, // Top-left                             //4
    { vec3(-0.5f, 0.5f, 0.01f), vec2(1, 1)}, // top right                             //5
    { vec3(-0.5f,-0.5f,0.01f), vec2(1, 0)}, // Bottom-right                             //6
    { vec3(0.5f,-0.5f, 0.01f), vec2(0, 0)}, // bottom left                            //7

};

static const uint16 g_ImageIndices[] =
{
    0, 1, 2, 0, 2, 3,               //front
    4, 5, 6, 4, 6, 7,               //back
};

fw::Mesh* CreateImageMesh()
{
    return new fw::Mesh(VertexFormat_Pos3dUV::format, g_ImageVerts, sizeof(g_ImageVerts), g_ImageIndices, sizeof(g_ImageIndices));
}

//=======================
// Collide (Or Don't)
//=======================


static const VertexFormat_Pos3dUV g_ColliderVerts[] = //1-6, 2-5, 3-4
{
    //front (1)
    { vec3(-0.5f, 0.5f, -0.5f), vec2(0, 1)},    //top left                             
    { vec3(0.5f, 0.5f, -0.5f), vec2(1, 1)},     //top right                          
    { vec3(0.5f, -0.5f, -0.5f), vec2(1, 0)},  //bottom right                      
    { vec3(-0.5f, -0.5f, -0.5f), vec2(0, 0)},   //bottom left                        

    //back (6)     
    { vec3(0.5f, 0.5f, 0.5f), vec2(0, 1)},     //top left                      
    { vec3(-0.5f, 0.5f, 0.5f), vec2(1, 1)},    //top right
    { vec3(-0.5f, -0.5f, 0.5f), vec2(1, 0)},   //bottom right  
    { vec3(0.5f, -0.5f, 0.5f), vec2(0, 0)},  //bottom left 

    //top (4)                                                                          
    { vec3(-0.5f, 0.5f, 0.5f), vec2(0, 1)},    //top left                             //0
    { vec3(0.5f, 0.5f, 0.5f), vec2(1, 1)},     //top right                          //1
    { vec3(0.5f, 0.5f, -0.5f), vec2(1, 0)},  //bottom right                       //2
    { vec3(-0.5f, 0.5f, -0.5f), vec2(0, 0)},   //bottom left                        //3

    //bottom(3)                                                                        
    { vec3(-0.5f, -0.5f, 0.5f), vec2(0, 1)},    //top left                             //0
    { vec3(0.5f, -0.5f, 0.5f), vec2(1, 1)},     //top right                          //1
    { vec3(0.5f, -0.5f, -0.5f), vec2(1, 0)},  //bottom right                       //2
    { vec3(-0.5f, -0.5f, -0.5f), vec2(0, 0)},   //bottom left                        //3

    //left(2)                                                                          
    { vec3(-0.5f, 0.5f, 0.5f), vec2(0, 1)},    //top left                             //0
    { vec3(-0.5f, 0.5f, -0.5f), vec2(0.0625, 1)},     //top right                          //1
    { vec3(-0.5f, -0.5f, -0.5f), vec2(0.0625, 0)},  //bottom right                       //2
    { vec3(-0.5f, -0.5f, 0.5f), vec2(0, 0)},   //bottom left                        //3

    ////right(5)                                                                         
    { vec3(0.5f, 0.5f, -0.5f), vec2(1, 1)},    //top left                             //0
    { vec3(0.5f, 0.5f, 0.5f), vec2(0.9375, 1)},     //top right                          //1
    { vec3(0.5f, -0.5f, 0.5f), vec2(0.9375, 0)},  //bottom right                       //2
    { vec3(0.5f, -0.5f, -0.5f), vec2(1, 0)},   //bottom left                        //3
};

static const uint16 g_ColliderIndices[] =
{
    0, 1, 2, 0, 2, 3,               //front
    4, 5, 6, 4, 6, 7,               //back
    8, 9, 10, 8, 10, 11,            //top
    12, 13, 14, 12, 14, 15,         //bottom
    16, 17, 18, 16, 18, 19,         //left
    20, 21, 22, 20, 22, 23          //right
};

fw::Mesh* CreateBorderMesh()
{
    return new fw::Mesh(VertexFormat_Pos3dUV::format, g_ColliderVerts, sizeof(g_ColliderVerts), g_ColliderIndices, sizeof(g_ColliderIndices));
}

//=======================
// become the ZOOMER
//=======================


static const VertexFormat_Pos3dUV g_BoostVerts[] = //1-6, 2-5, 3-4
{
    //top (4)                                                                          
    { vec3(-0.5f, 0.5f, 0.5f), vec2(0, 1)}, //top left                               //8
    { vec3(0.5f, 0.5f, 0.5f), vec2(1, 1) },  //top right                            //9
    { vec3(0.5f, 0.5f, -0.5f), vec2(1, 0) }, //bottom right                           //10
    { vec3(-0.5f, 0.5f, -0.5f), vec2(0, 0) }, //bottom left                             //11

    //bottom(3)                                                                        
    { vec3(-0.5f, -0.5f, -0.5f), vec2(0, 1) }, //top left                             //12
    { vec3(0.5f, -0.5f, -0.5f), vec2(1, 1) }, //top right                               //13
    { vec3(0.5f, -0.5f, 0.5f), vec2(1, 0) }, //bottom right                           //14
    { vec3(-0.5f, -0.5f, 0.5f), vec2(0, 0) }, //bottom left                         //15
};

static const uint16 g_BoostIndices[] =
{
    0, 1, 2, 0, 2, 3,               //front
    4, 5, 6, 4, 6, 7,               //back
};

fw::Mesh* CreateBoostMesh()
{
    return new fw::Mesh(VertexFormat_Pos3dUV::format, g_BoostVerts, sizeof(g_BoostVerts), g_BoostIndices, sizeof(g_BoostIndices));
}

//=======================
// Sky's The Limit
//=======================


static const VertexFormat_Pos3dUV g_BackgroundVerts[] = //1-6, 2-5, 3-4
{
    //front (1)
  { vec3(0.5f, 0.5f, -0.5f), vec2(1, 1)},     //top right    
  { vec3(-0.5f, 0.5f, -0.5f), vec2(0, 1)},    //top left                             
  { vec3(-0.5f, -0.5f, -0.5f), vec2(0, 0)},   //bottom left               
  { vec3(0.5f, -0.5f, -0.5f), vec2(1, 0)},  //bottom right                                       

  //back (6)     
  { vec3(-0.5f, 0.5f, 0.5f), vec2(0, 1)},    //top right
  { vec3(0.5f, 0.5f, 0.5f), vec2(1, 1)},     //top left  
  { vec3(0.5f, -0.5f, 0.5f), vec2(1, 0)},  //bottom left 
  { vec3(-0.5f, -0.5f, 0.5f), vec2(0, 0)},   //bottom right  

  //top (4)                                               
  { vec3(0.5f, 0.5f, 0.5f), vec2(1, 1)},     //top right                          //1
  { vec3(-0.5f, 0.5f, 0.5f), vec2(0, 1)},    //top left                             //0
  { vec3(-0.5f, 0.5f, -0.5f), vec2(0, 0)},   //bottom left                        //3
  { vec3(0.5f, 0.5f, -0.5f), vec2(1, 0)},  //bottom right                       //2

  //bottom(3)       
  { vec3(0.5f, -0.5f, 0.5f), vec2(1, 1)},     //top right                          //1
  { vec3(-0.5f, -0.5f, 0.5f), vec2(0, 1)},    //top left                             //0
  { vec3(-0.5f, -0.5f, -0.5f), vec2(0, 0)},   //bottom left                        //3
  { vec3(0.5f, -0.5f, -0.5f), vec2(1, 0)},  //bottom right                       //2

  //left(2)                         
  { vec3(-0.5f, 0.5f, -0.5f), vec2(1, 1)},     //top right                          //1
  { vec3(-0.5f, 0.5f, 0.5f), vec2(0, 1)},    //top left                             //0
  { vec3(-0.5f, -0.5f, 0.5f), vec2(0, 0)},   //bottom left                        //3
  { vec3(-0.5f, -0.5f, -0.5f), vec2(1, 0)},  //bottom right                       //2

  ////right(5)  
  { vec3(0.5f, 0.5f, 0.5f), vec2(1, 1)},     //top right                          //1
  { vec3(0.5f, 0.5f, -0.5f), vec2(0, 1)},    //top left                             //0
  { vec3(0.5f, -0.5f, -0.5f), vec2(0, 0)},   //bottom left                        //3
  { vec3(0.5f, -0.5f, 0.5f), vec2(1, 0)},  //bottom right                       //2
};

static const uint16 g_BackgroundIndices[] =
{
    0, 1, 2, 0, 2, 3,               //front
    4, 5, 6, 4, 6, 7,               //back
    8, 9, 10, 8, 10, 11,            //top
    12, 13, 14, 12, 14, 15,         //bottom
    16, 17, 18, 16, 18, 19,         //left
    20, 21, 22, 20, 22, 23          //right
};


fw::Mesh* CreateBackgroundMesh()
{
    return new fw::Mesh(VertexFormat_Pos3dUV::format, g_BackgroundVerts, sizeof(g_BackgroundVerts), g_BackgroundIndices, sizeof(g_BackgroundIndices));
}
