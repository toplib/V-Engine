#pragma once
#include "material/Material.h"
#include "mesh/Mesh.h"

namespace Rendering {
   class MeshRenderer {
      public:
      MeshRenderer();
      ~MeshRenderer();

      Material::Material getMaterial();
      Mesh::Mesh getMesh();

      void setMaterial(Material::Material material);
      void setMesh(Mesh::Mesh mesh);
      private:
      Material::Material m_material;
      Mesh::Mesh m_mesh;
   };
}
