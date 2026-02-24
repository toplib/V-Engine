#pragma once
#include "material/Material.h"
#include "mesh/Mesh.h"

namespace Rendering {
   class MeshRenderer {
      public:
      MeshRenderer();
      ~MeshRenderer();

      private:
      Material::Material m_material;
      Mesh::Mesh m_mesh;
   }
}
