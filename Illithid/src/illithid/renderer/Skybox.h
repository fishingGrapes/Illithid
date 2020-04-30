#pragma once

namespace itd
{
	class Material;
	class StaticMesh;

	class Skybox
	{
	public:
		explicit Skybox( const std::string& file );
		void Render( );
		~Skybox( );

	private:
		void LoadCubemap( const std::vector<std::string>& faces );

		uint32_t id_;
		std::unique_ptr<Material> material_;
		std::shared_ptr<StaticMesh> box_;
	};
}