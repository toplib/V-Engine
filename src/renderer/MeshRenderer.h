#pragma once
#include "material/Material.h"
#include "mesh/Mesh.h"

namespace Rendering {
   struct MeshRenderer {
      Mesh::Mesh mesh;
      Material::Material material;

   }
}
