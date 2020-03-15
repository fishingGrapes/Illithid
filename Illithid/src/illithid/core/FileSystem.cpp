#include "ilpch.h"
#include "FileSystem.h"

#include "Log.h"
#include <filesystem>

namespace itd
{
	std::string FileSystem::BaseDirectory_ = "";
	std::string FileSystem::DirectorySeparator_ = "\\";
	const char FileSystem::DefaultSeparator_ = '/';

	void FileSystem::Initialize( )
	{
		DirectorySeparator_ = static_cast<char>( std::filesystem::path::preferred_separator );
		BaseDirectory_ = std::filesystem::current_path( ).string( ) + DirectorySeparator_;
		IL_CORE_INFO( "Current directory is {0}", BaseDirectory_ );
	}


	std::string FileSystem::ReadAsText( const std::string& path )
	{
		std::string text;
		std::string qualifiedPath = GetAbsolutePath( path );

		if (std::filesystem::exists( qualifiedPath ))
		{
			std::ifstream inputStream;
			inputStream.exceptions( std::ifstream::failbit | std::ifstream::badbit );

			try
			{
				std::stringstream stringStream;

				inputStream.open( path );
				stringStream << inputStream.rdbuf( );
				inputStream.close( );

				text = stringStream.str( );

			}
			catch (std::ifstream::failure e)
			{
				IL_CORE_ERROR( "Exception thrown when reading file {0}", path );
			}
		}

		return text;
	}

	std::string FileSystem::GetAbsolutePath( const std::string& relativePath )
	{
		std::string qualifiedPath = BaseDirectory_ + relativePath;

		size_t position;
		while (( position = qualifiedPath.find( DefaultSeparator_ ) ) != std::string::npos)
		{
			qualifiedPath.replace( position, 1, DirectorySeparator_ );
		}

		return qualifiedPath;
	}


}