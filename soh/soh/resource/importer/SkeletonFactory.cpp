#include "soh/resource/importer/SkeletonFactory.h"
#include "soh/resource/type/Skeleton.h"
#include <spdlog/spdlog.h>
#include <libultraship/bridge.h>

namespace Ship {
std::shared_ptr<Resource> SkeletonFactory::ReadResource(uint32_t version, std::shared_ptr<BinaryReader> reader)
{
	auto resource = std::make_shared<Skeleton>();
	std::shared_ptr<ResourceVersionFactory> factory = nullptr;

	switch (version)
	{
	case 0:
		factory = std::make_shared<SkeletonFactoryV0>();
		break;
	}

	if (factory == nullptr)
	{
		SPDLOG_ERROR("Failed to load Skeleton with version {}", version);
		return nullptr;
	}

	factory->ParseFileBinary(reader, resource);

	return resource;
}

std::shared_ptr<Resource> SkeletonFactory::ReadResourceXML(uint32_t version, tinyxml2::XMLElement* reader) 
{
    auto resource = std::make_shared<Skeleton>();
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch ((Version)version) {
        case Version::Deckard:
            factory = std::make_shared<SkeletonFactoryV0>();
            break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load Skeleton with version {}", version);
        return nullptr;
    }

    factory->ParseFileXML(reader, resource);

    return resource;
}

void SkeletonFactoryV0::ParseFileBinary(std::shared_ptr<BinaryReader> reader,
                                        std::shared_ptr<Resource> resource)
{
    std::shared_ptr<Skeleton> skeleton = std::static_pointer_cast<Skeleton>(resource);
    ResourceVersionFactory::ParseFileBinary(reader, skeleton);

	skeleton->type = (SkeletonType)reader->ReadInt8();
	skeleton->limbType = (LimbType)reader->ReadInt8();
    skeleton->limbCount = reader->ReadUInt32();
    skeleton->dListCount = reader->ReadUInt32();
	skeleton->limbTableType = (LimbType)reader->ReadInt8();
	skeleton->limbTableCount = reader->ReadUInt32();

    skeleton->limbTable.reserve(skeleton->limbTableCount);
    for (uint32_t i = 0; i < skeleton->limbTableCount; i++) {
        std::string limbPath = reader->ReadString();

        skeleton->limbTable.push_back(limbPath);
    }

    if (skeleton->type == Ship::SkeletonType::Curve) {
		skeleton->skeletonData.skelCurveLimbList.limbCount = skeleton->limbCount;
		skeleton->curveLimbArray.reserve(skeleton->skeletonData.skelCurveLimbList.limbCount);
    } else if (skeleton->type == Ship::SkeletonType::Flex) {
		skeleton->skeletonData.flexSkeletonHeader.dListCount = skeleton->dListCount;
    }

    if (skeleton->type == Ship::SkeletonType::Normal) {
        skeleton->skeletonData.skeletonHeader.limbCount = skeleton->limbCount;
		skeleton->standardLimbArray.reserve(skeleton->skeletonData.skeletonHeader.limbCount);
    } else if (skeleton->type == Ship::SkeletonType::Flex) {
        skeleton->skeletonData.flexSkeletonHeader.sh.limbCount = skeleton->limbCount;
		skeleton->standardLimbArray.reserve(skeleton->skeletonData.flexSkeletonHeader.sh.limbCount);
    }

    for (size_t i = 0; i < skeleton->limbTable.size(); i++) {
        std::string limbStr = skeleton->limbTable[i];
        auto limb = GetResourceDataByName(limbStr.c_str(), true);
        skeleton->skeletonHeaderSegments.push_back(limb);
    }

    if (skeleton->type == Ship::SkeletonType::Normal) {
        skeleton->skeletonData.skeletonHeader.segment = (void**)skeleton->skeletonHeaderSegments.data();
    } else if (skeleton->type == Ship::SkeletonType::Flex) {
        skeleton->skeletonData.flexSkeletonHeader.sh.segment = (void**)skeleton->skeletonHeaderSegments.data();
    } else if (skeleton->type == Ship::SkeletonType::Curve) {
        skeleton->skeletonData.skelCurveLimbList.limbs = (SkelCurveLimb**)skeleton->skeletonHeaderSegments.data();
    } else {
        SPDLOG_ERROR("unknown skeleton type {}", (uint32_t)skeleton->type);
    }
}
void SkeletonFactoryV0::ParseFileXML(tinyxml2::XMLElement* reader, std::shared_ptr<Resource> resource) 
{
    std::shared_ptr<Skeleton> skeleton = std::static_pointer_cast<Skeleton>(resource);

    std::string skeletonType = reader->Attribute("Type");
    // std::string skeletonLimbType = reader->Attribute("LimbType");
    int numLimbs = reader->IntAttribute("LimbCount");
    int numDLs = reader->IntAttribute("DisplayListCount");

    if (skeletonType == "Flex")
        skeleton->type = SkeletonType::Flex;
    else if (skeletonType == "Curve")
        skeleton->type = SkeletonType::Curve;
    else if (skeletonType == "Normal")
        skeleton->type = SkeletonType::Normal;

    skeleton->type = SkeletonType::Flex;
    skeleton->limbType = LimbType::LOD;

    skeleton->limbTableType = LimbType::LOD;
    skeleton->limbTableCount = numLimbs;

    auto child = reader->FirstChildElement();

    skeleton->limbCount = numLimbs;
    skeleton->dListCount = numDLs;

    if (skeleton->type == Ship::SkeletonType::Curve) {
        skeleton->skeletonData.skelCurveLimbList.limbCount = skeleton->limbCount;
        skeleton->curveLimbArray.reserve(skeleton->skeletonData.skelCurveLimbList.limbCount);
    } else if (skeleton->type == Ship::SkeletonType::Flex) {
        skeleton->skeletonData.flexSkeletonHeader.dListCount = skeleton->dListCount;
    }

    if (skeleton->type == Ship::SkeletonType::Normal) {
        skeleton->skeletonData.skeletonHeader.limbCount = skeleton->limbCount;
        skeleton->standardLimbArray.reserve(skeleton->skeletonData.skeletonHeader.limbCount);
    } else if (skeleton->type == Ship::SkeletonType::Flex) {
        skeleton->skeletonData.flexSkeletonHeader.sh.limbCount = skeleton->limbCount;
        skeleton->standardLimbArray.reserve(skeleton->skeletonData.flexSkeletonHeader.sh.limbCount);
    }

    while (child != nullptr) {
        std::string childName = child->Name();

        if (childName == "SkeletonLimb") {
            std::string limbName = child->Attribute("Path");
            skeleton->limbTable.push_back(limbName);
        }

        child = child->NextSiblingElement();
    }

    for (size_t i = 0; i < skeleton->limbTable.size(); i++) {
        std::string limbStr = skeleton->limbTable[i];
        auto limb = GetResourceDataByName(limbStr.c_str(), true);
        skeleton->skeletonHeaderSegments.push_back(limb);
    }
}

} // namespace Ship
