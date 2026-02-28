#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class NotePopup : public Popup {
public:
    static NotePopup* create(std::string levelKey, std::function<void(std::string)> callback);

protected:
    std::string m_levelKey;
    std::function<void(std::string)> m_callback;
    CCTextInputNode* m_input = nullptr;

    bool init(std::string levelKey, std::function<void(std::string)> callback);

    void onSave(CCObject*);
    void onClear(CCObject*);
};