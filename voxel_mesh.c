InitVoxelMesh(int triangles, bool useColors = false){

}

PushVertex(Vector3& vertex, float xOffset = 0, float yOffset = 0, float zOffset = 0){

}

AddCube(Vector3&& position, bool faces[6], int block){

}

inline void SetNormal(Vector3& value) { Normal = value; }
inline void SetNormal(float x, float y, float z) { Normal = Vector3{ x,y,z }; }
inline void SetSetUV(Vector2& value) { UV = value; }
inline void SetSetUV(float x, float y ) { UV = Vector2{ x,y }; }

inline void SetColor(Color& value) { VertColor = value; }
