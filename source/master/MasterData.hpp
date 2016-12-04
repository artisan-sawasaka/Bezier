#pragma once

#include <string>
#include <vector>
#include <map>
#include "master/TitleImageList.hpp"
#include "master/TitleUI.hpp"

namespace MasterData
{
    extern std::map<std::string, TitleImageListData> TitleImageList;
    extern std::vector<TitleUIData> TitleUI;


    void Reload(const std::string& path);
}
