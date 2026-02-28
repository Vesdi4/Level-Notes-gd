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
    if (!Popup::init(320.f, 240.f)) return false;

    m_levelKey = levelKey;
    m_callback = callback;

    this->setTitle("Level Note");

    auto inputBg = CCScale9Sprite::create("square02b_001.png");
    inputBg->setContentSize({ 260.f, 110.f });
    inputBg->setColor({ 0, 0, 0 });
    inputBg->setOpacity(80);
    this->addChildAtPosition(inputBg, Anchor::Center, { 0, 10.f });

    m_input = CCTextInputNode::create(240.f, 100.f, "Write your note here...", "chatFont.fnt");
    m_input->setMaxLabelWidth(240.f);
    m_input->setLabelPlaceholderColor({ 150, 150, 150 });
    m_input->setLabelPlaceholderScale(0.45f);
    m_input->setMaxLabelScale(0.45f);
    CCTouchDispatcher::get()->addPrioTargetedDelegate(m_input, CCTouchDispatcher::get()->getTargetPrio() - 1, true);
    this->addChildAtPosition(m_input, Anchor::Center, { 0, 10.f }, false);

    auto saved = Mod::get()->getSavedValue<std::string>(m_levelKey, "");
    if (!saved.empty()) {
        m_input->setString(saved.c_str());
    }

    auto hint = CCLabelBMFont::create("Max 200 characters", "chatFont.fnt");
    hint->setScale(0.35f);
    hint->setColor({ 180, 180, 180 });
    this->addChildAtPosition(hint, Anchor::Bottom, { 0, 42.f });

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
    menu->setContentWidth(280.f);
    menu->updateLayout();
    this->addChildAtPosition(menu, Anchor::Bottom, { 0, 22.f });

    return true;
}

void NotePopup::onSave(CCObject*) {
    std::string text = m_input->getString();
    if (text.size() > 200) text = text.substr(0, 200);

    Mod::get()->setSavedValue<std::string>(m_levelKey, text);
    if (m_callback) m_callback(text);

    CCTouchDispatcher::get()->removeDelegate(m_input);
    Notification::create("Note saved!", NotificationIcon::Success, 1.5f)->show();
    this->onClose(nullptr);
}

void NotePopup::onClear(CCObject*) {
    m_input->setString("");
    Mod::get()->setSavedValue<std::string>(m_levelKey, "");
    if (m_callback) m_callback("");
    Notification::create("Note cleared!", NotificationIcon::None, 1.5f)->show();
}