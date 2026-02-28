#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "NotePopup.hpp"

using namespace geode::prelude;

static std::string getLevelKey(GJGameLevel* level) {
    return "note_" + std::to_string(level->m_levelID.value());
}

static bool hasNote(GJGameLevel* level) {
    return !Mod::get()->getSavedValue<std::string>(getLevelKey(level), "").empty();
}

class $modify(MyLevelInfoLayer, LevelInfoLayer) {

    struct Fields {
        CCSprite* dot = nullptr;
    };

    bool init(GJGameLevel * level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        auto btnSpr = CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png");
        btnSpr->setScale(0.85f);

        auto dot = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
        dot->setScale(0.3f);
        dot->setColor({ 255, 200, 0 });
        dot->setPosition({ btnSpr->getContentWidth() - 3.f, btnSpr->getContentHeight() - 3.f });
        dot->setVisible(hasNote(level));
        btnSpr->addChild(dot, 1);
        m_fields->dot = dot;

        auto btn = CCMenuItemSpriteExtra::create(
            btnSpr,
            this,
            menu_selector(MyLevelInfoLayer::onNoteButton)
        );

        auto leftMenu = this->getChildByID("left-side-menu");
        if (leftMenu) {
            leftMenu->addChild(btn);
            leftMenu->updateLayout();
        }
        else {
            auto menu = CCMenu::create();
            menu->addChild(btn);
            menu->setPosition({ 25.f, 25.f });
            this->addChild(menu, 10);
        }

        return true;
    }

    void onNoteButton(CCObject*) {
        auto dot = m_fields->dot;
        NotePopup::create(getLevelKey(m_level), [dot](std::string note) {
            if (dot) dot->setVisible(!note.empty());
            })->show();
    }
};