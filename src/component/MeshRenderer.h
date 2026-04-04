#pragma once
#include "Component.h"
#include "material/Material.h"
#include "mesh/Mesh.h"

namespace Component {
   class MeshRenderer : Component {
      public:
      MeshRenderer();
      ~MeshRenderer() override;

      Material::Material& getMaterial();
      Mesh::Mesh& getMesh();

      void start() override;
      void update() override;

      void setMaterial(Material::Material material);
      void setMesh(Mesh::Mesh mesh);
      private:
      Material::Material m_material;
      Mesh::Mesh m_mesh;
   };
}
