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
    if (!FLAlertLayer::init(150)) return false;

    m_levelKey = levelKey;
    m_callback = callback;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    float popW = 300.f;
    float popH = 220.f;

    auto bg = CCScale9Sprite::create("GJ_square01.png");
    bg->setContentSize({ popW, popH });
    bg->setPosition(winSize / 2);
    m_mainLayer->addChild(bg, -1);

    auto title = CCLabelBMFont::create("Level Note", "goldFont.fnt");
    title->setScale(0.7f);
    title->setPosition({ winSize.width / 2, winSize.height / 2 + 88.f });
    m_mainLayer->addChild(title);

    float cx = winSize.width / 2;
    float cy = winSize.height / 2;

    auto inputBg = CCScale9Sprite::create("square02b_001.png");
    inputBg->setContentSize({ 250.f, 100.f });
    inputBg->setColor({ 0, 0, 0 });
    inputBg->setOpacity(80);
    inputBg->setPosition({ cx, cy + 15.f });
    m_mainLayer->addChild(inputBg);

    m_input = CCTextInputNode::create(230.f, 90.f, "Write your note here...", "chatFont.fnt");
    m_input->setMaxLabelWidth(230.f);
    m_input->setLabelPlaceholderColor({ 150, 150, 150 });
    m_input->setLabelPlaceholderScale(0.45f);
    m_input->setMaxLabelScale(0.45f);
    m_input->setDelegate(this);
    m_input->setPosition({ cx, cy + 15.f });
    m_mainLayer->addChild(m_input, 10);

    auto saved = Mod::get()->getSavedValue<std::string>(m_levelKey, "");
    if (!saved.empty()) {
        m_input->setString(saved.c_str());
    }

    auto hint = CCLabelBMFont::create("Max 200 characters", "chatFont.fnt");
    hint->setScale(0.35f);
    hint->setColor({ 180, 180, 180 });
    hint->setPosition({ cx, cy - 62.f });
    m_mainLayer->addChild(hint);

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

    auto closeBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"),
        this,
        menu_selector(NotePopup::keyBackClicked)
    );
    closeBtn->setScale(0.8f);

    auto btnMenu = CCMenu::create();
    btnMenu->addChild(saveBtn);
    btnMenu->addChild(clearBtn);
    btnMenu->setLayout(RowLayout::create()->setGap(10.f));
    btnMenu->setContentWidth(260.f);
    btnMenu->updateLayout();
    btnMenu->setPosition({ cx, cy - 83.f });
    m_mainLayer->addChild(btnMenu);

    auto closeMenu = CCMenu::create();
    closeMenu->addChild(closeBtn);
    closeMenu->setPosition({ winSize.width / 2 - popW / 2 + 3.f, winSize.height / 2 + popH / 2 - 3.f });
    m_mainLayer->addChild(closeMenu);

    this->show();
    return true;
}

void NotePopup::registerWithTouchDispatcher() {
    CCTouchDispatcher::get()->addTargetedDelegate(this, -504, true);
}

void NotePopup::keyBackClicked() {
    this->setKeypadEnabled(false);
    this->removeFromParent();
}

void NotePopup::onSave(CCObject*) {
    std::string text = m_input->getString();

    if (text.size() > 200) {
        text = text.substr(0, 200);
    }

    Mod::get()->setSavedValue<std::string>(m_levelKey, text);

    if (m_callback) {
        m_callback(text);
    }

    Notification::create("Note saved!", NotificationIcon::Success, 1.5f)->show();
    this->keyBackClicked();
}

void NotePopup::onClear(CCObject*) {
    m_input->setString("");
    Mod::get()->setSavedValue<std::string>(m_levelKey, "");

    if (m_callback) {
        m_callback("");
    }

    Notification::create("Note cleared!", NotificationIcon::None, 1.5f)->show();
}