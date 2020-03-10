#pragma once

#define SET_ILLITHID_APPLICATION(x) itd::Application* itd::CreateApplication( ){return new x( );}
#define BIND_EVENT_FUNCTION(function, object) std::bind( &function, object, std::placeholders::_1 )