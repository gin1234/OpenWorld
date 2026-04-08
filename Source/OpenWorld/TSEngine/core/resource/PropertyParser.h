#pragma once
#include <string>
#include <map>
#include <sstream>
#include "Engine/EngineTypes.h"

namespace TSEngine {
namespace PropertyParser {

// 解析向量 (格式: "X,Y,Z")
inline FVector ParseVector(const std::string& Value) {
    FVector Result = FVector::ZeroVector;
    std::stringstream ss(Value);
    std::string Item;
    int Index = 0;
    while (std::getline(ss, Item, ',') && Index < 3) {
        Result[Index++] = std::stof(Item);
    }
    return Result;
}

// 解析旋转变换 (格式: "Pitch,Yaw,Roll")
inline FRotator ParseRotator(const std::string& Value) {
    FRotator Result = FRotator::ZeroRotator;
    std::stringstream ss(Value);
    std::string Item;
    int Index = 0;
    while (std::getline(ss, Item, ',') && Index < 3) {
        Result[Index++] = std::stof(Item);
    }
    return Result;
}

// 解析可见性
inline EVisibility ParseVisibility(const std::string& Value) {
    if (Value == "Visible") return EVisibility::Visible;
    if (Value == "Hidden") return EVisibility::Hidden;
    if (Value == "Collapsed") return EVisibility::Collapsed;
    if (Value == "HitTestInvisible") return EVisibility::HitTestInvisible;
    return EVisibility::Visible;
}

// 解析动画参数
struct FAnimationParams {
    FName AnimationName;
    float BlendTime = 0.25f;
    float PlayRate = 1.0f;
    bool bLooping = false;
};

inline FAnimationParams ParseAnimationParams(const std::map<std::string, std::string>& Params) {
    FAnimationParams Result;

    auto it = Params.find("AnimationName");
    if (it != Params.end()) {
        Result.AnimationName = FName(UTF8_TO_TCHAR(it->second.c_str()));
    }

    it = Params.find("BlendTime");
    if (it != Params.end()) {
        Result.BlendTime = std::stof(it->second);
    }

    it = Params.find("PlayRate");
    if (it != Params.end()) {
        Result.PlayRate = std::stof(it->second);
    }

    it = Params.find("bLooping");
    if (it != Params.end()) {
        Result.bLooping = (it->second == "true" || it->second == "1");
    }

    return Result;
}

// 解析声音参数
struct FSoundParams {
    float VolumeMultiplier = 1.0f;
    float PitchMultiplier = 1.0f;
    bool bLooping = false;
    float StartTime = 0.0f;
};

inline FSoundParams ParseSoundParams(const std::map<std::string, std::string>& Params) {
    FSoundParams Result;

    auto it = Params.find("VolumeMultiplier");
    if (it != Params.end()) {
        Result.VolumeMultiplier = std::stof(it->second);
    }

    it = Params.find("PitchMultiplier");
    if (it != Params.end()) {
        Result.PitchMultiplier = std::stof(it->second);
    }

    it = Params.find("bLooping");
    if (it != Params.end()) {
        Result.bLooping = (it->second == "true" || it->second == "1");
    }

    it = Params.find("StartTime");
    if (it != Params.end()) {
        Result.StartTime = std::stof(it->second);
    }

    return Result;
}

} // namespace PropertyParser
} // namespace TSEngine
