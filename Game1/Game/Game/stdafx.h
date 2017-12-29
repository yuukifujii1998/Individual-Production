#pragma once

#include "../osumaruEngine/engineStdafx.h"
#include "../osumaruEngine/GameObject/GameObject.h"
#include "../osumaruEngine/GameObject/GameObjectManager.h"
#include "../osumaruEngine/Engine.h"
#include "../osumaruEngine/Graphics/Sprite.h"
#include "../osumaruEngine/Graphics/Primitive.h"
#include "../osumaruEngine/Graphics/SkinModel/SkinModel.h"
#include "../osumaruEngine/Graphics/SkinModel/SkinModelData.h"
#include "../osumaruEngine/Graphics/SkinModel/Animation.h"
#include "../osumaruEngine/Camera.h"
#include "../osumaruEngine/Graphics/Light.h"
#include "../osumaruEngine/Physics/Physics.h"
#include "../osumaruEngine/Physics/RigidBody.h"
#include "../osumaruEngine/Physics/Collider/CapsuleCollider.h"
#include "../osumaruEngine/Physics/Collider/MeshCollider.h"
#include "../osumaruEngine/Physics/Collider/BoxCollider.h"
#include "../osumaruEngine/Physics/Collider/SphereCollider.h"
#include "../osumaruEngine/Physics/Collider/ICollider.h"
#include "../osumaruEngine/Physics/CharacterController.h"
#include "../osumaruEngine/Physics/CollisionDetection.h"
#include "../osumaruEngine/Physics/CollisionAttr.h"
#include "../osumaruEngine/Input/Pad.h"
#include "../osumaruEngine/Sound/SoundEngine.h"
#include "../osumaruEngine/Sound/SoundSource.h"
#include "../osumaruEngine/Particle/Particle.h"
#include "../osumaruEngine/Particle/ParticleEmitter.h"
#include "../osumaruEngine/Font/Font.h"

const int playerPriority = 1;
const int stageGimmickPriority = 0;
const int cameraPriority = 2;
const int lastPriority = 3;