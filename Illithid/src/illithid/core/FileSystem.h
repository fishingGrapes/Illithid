#pragma once
#include <optional>

namespace itd
{
	class FileSystem
	{
	public:
		static void Initialize( );
		static std::string LoadAsText( const std::string& path );

	private:
		static std::string BaseDirectory_;
		static std::string DirectorySeparator_;
		static const char DefaultSeparator_;
	};
}
