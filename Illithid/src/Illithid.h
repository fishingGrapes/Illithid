#pragma once

#include "illithid/core/Macros.h"
#include "illithid/core/Application.h"
#include "illithid/core/Time.h"
#include "illithid/core/Screen.h"
#include "illithid/core/Log.h"
#include "illithid/core/Input.h"

#include "illithid/core/KeyCodes.h"
#include "illithid/events/KeyEvents.h"
#include "illithid/events/MouseEvents.h"

#include "illithid/components/Transform.h"
#include "illithid/components/Camera.h"
#include "illithid/components/Light.h"

#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/quaternion.hpp"
#include "nlohmann/json.hpp"

#include "illithid/renderer/Graphics.h"
#include "illithid/scene/GameObject.h"

#include "illithid/components/MeshRenderer.h"
#include "illithid/components/Transform.h"

#include "illithid/utils/Array3D.h"
#include "illithid/utils/ptr_ref.h"
#include "illithid/memory/GrowingBlockAllocator.h"

#include "illithid/core/LogExtensions.h"