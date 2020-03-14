#pragma once
#include <optional>

namespace itd
{
	class FileSystem
	{
	public:
		static void Initialize( );
		static std::string LoadAsText( const std::string& path );
		static std::string GetAbsolutePath( const std::string& relativePath );
	private:
		static std::string BaseDirectory_;
		static std::string DirectorySeparator_;
		static const char DefaultSeparator_;
	};
}
