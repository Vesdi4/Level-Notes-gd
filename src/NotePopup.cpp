#include "NotePopup.hpp"

using namespace geode::prelude;

NotePopup* NotePopup::create(std::string levelKey, std::function<void(std::string)> callback) {
    auto ret = new NotePopup();
    if (ret->init(levelKey, callback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool NotePopup::init(std::string levelKey, std::function<void(std::string)> callback) {
    if (!Popup::init(300.f, 180.f)) return false;

    m_levelKey = levelKey;
    m_callback = callback;

    this->setTitle("Level Note");

    m_input = TextInput::create(250.f, "Write your note here...", "chatFont.fnt");
    m_input->setMaxCharCount(200);
    m_mainLayer->addChildAtPosition(m_input, Anchor::Center, { 0.f, 10.f });

    auto saved = Mod::get()->getSavedValue<std::string>(m_levelKey, "");
    if (!saved.empty()) {
        m_input->setString(saved, false);
    }

    auto hint = CCLabelBMFont::create("Max 200 characters", "chatFont.fnt");
    hint->setScale(0.35f);
    hint->setColor({ 150, 150, 150 });
    m_mainLayer->addChildAtPosition(hint, Anchor::Bottom, { 0.f, 40.f });

    auto saveBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Save", "goldFont.fnt", "GJ_button_01.png", 0.8f),
        this,
        menu_selector(NotePopup::onSave)
    );

    auto clearBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Clear", "bigFont.fnt", "GJ_button_06.png", 0.6f),
        this,
        menu_selector(NotePopup::onClear)
    );

    auto menu = CCMenu::create();
    menu->addChild(saveBtn);
    menu->addChild(clearBtn);
    menu->setLayout(RowLayout::create()->setGap(10.f));
    menu->setContentWidth(260.f);
    menu->updateLayout();
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, { 0.f, 22.f });

    return true;
}

void NotePopup::onSave(CCObject*) {
    auto text = std::string(m_input->getString());

    Mod::get()->setSavedValue<std::string>(m_levelKey, text);
    if (m_callback) m_callback(text);

    Notification::create("Note saved!", NotificationIcon::Success, 1.5f)->show();
    this->onClose(nullptr);
}

void NotePopup::onClear(CCObject*) {
    m_input->setString("", false);
    Mod::get()->setSavedValue<std::string>(m_levelKey, "");
    if (m_callback) m_callback("");
    Notification::create("Note cleared!", NotificationIcon::None, 1.5f)->show();
}