#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <random>
#include <string>

using namespace geode::prelude;

namespace {
    constexpr int kCatCount = 330;

    std::string catFileName(int index) {
        char name[32] = {};
        if (index <= 60) {
            std::snprintf(name, sizeof(name), "cat_%02d.png", index);
        } else {
            std::snprintf(name, sizeof(name), "cat_%03d.png", index);
        }
        return name;
    }

    std::string catPath(int index) {
        return Mod::get()->expandSpriteName(catFileName(index));
    }

    float catTargetSize() {
        return static_cast<float>(Mod::get()->getSettingValue<double>("size"));
    }

    GLubyte catOpacity() {
        auto opacity = static_cast<float>(Mod::get()->getSettingValue<double>("opacity"));
        opacity = std::clamp(opacity, 0.f, 1.f);
        return static_cast<GLubyte>(std::round(opacity * 255.f));
    }
}

class $modify(CatRandomizerLayer, GJBaseGameLayer) {
    struct Fields {
        CCSprite* m_catSprite = nullptr;
        int m_currentCat = 0;
        std::mt19937 m_rng { std::random_device{}() };
    };

    void setupLevelStart(LevelSettingsObject* settings) {
        GJBaseGameLayer::setupLevelStart(settings);
        this->showRandomCat();
    }

    void handleButton(bool down, int button, bool isPlayer1) {
        GJBaseGameLayer::handleButton(down, button, isPlayer1);

        if (down) {
            this->showRandomCat();
        }
    }

    void showRandomCat() {
        if (!Mod::get()->getSettingValue<bool>("enabled")) {
            this->removeCat();
            return;
        }

        if (!m_uiLayer) return;

        std::uniform_int_distribution<int> dist(1, kCatCount);
        auto nextCat = dist(m_fields->m_rng);
        if (kCatCount > 1) {
            while (nextCat == m_fields->m_currentCat) {
                nextCat = dist(m_fields->m_rng);
            }
        }
        m_fields->m_currentCat = nextCat;

        this->removeCat();

        auto path = catPath(nextCat);
        auto sprite = CCSprite::create(path.c_str());
        if (!sprite) {
            log::warn("Could not load cat image {}", path);
            return;
        }

        auto size = sprite->getContentSize();
        auto target = catTargetSize();
        auto longestSide = std::max(size.width, size.height);
        if (longestSide > 0.f) {
            sprite->setScale(target / longestSide);
        }

        sprite->setID("cat-randomizer-cat");
        sprite->setAnchorPoint({ 0.f, 0.f });
        sprite->setPosition({ 8.f, 8.f });
        sprite->setOpacity(catOpacity());
        sprite->setZOrder(9999);

        m_uiLayer->addChild(sprite);
        m_fields->m_catSprite = sprite;
    }

    void removeCat() {
        if (m_fields->m_catSprite) {
            m_fields->m_catSprite->removeFromParent();
            m_fields->m_catSprite = nullptr;
        }
    }
};
