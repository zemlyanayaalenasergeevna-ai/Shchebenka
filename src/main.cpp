#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <random>

using namespace geode::prelude;

class $modify(ShchebenkaPlayLayer, PlayLayer) {
    struct Fields {
        float nextEvent = 12.f;
        float rockTime = 0.f;
        bool turned = false;
        CCSprite* rock = nullptr;
        bool initialized = false;
    };

    void update(float dt) {
        PlayLayer::update(dt);

        if (!m_player1 || m_player1->m_isDead || m_hasCompletedLevel)
            return;

        auto& f = *m_fields;

        if (!f.initialized) {
            f.initialized = true;
            f.nextEvent = randomRange(10.f, 35.f);
        }

        if (f.turned) {
            f.rockTime -= dt;

            if (f.rock) {
                f.rock->setPosition(m_player1->getPosition());
                f.rock->setRotation(m_player1->getRotation());
            }

            if (f.rockTime <= 0.f) {
                unRock();
                f.nextEvent = randomRange(12.f, 40.f);
            }
            return;
        }

        f.nextEvent -= dt;
        if (f.nextEvent <= 0.f) {
            becomeRock();
        }
    }

    void becomeRock() {
        auto& f = *m_fields;
        if (f.turned || !m_player1)
            return;

        FMODAudioEngine::sharedEngine()->playEffect("gameSound_01.ogg");

        f.rock = CCSprite::createWithSpriteFrameName("GJ_square01.png");

        if (!f.rock)
            return;

        f.rock->setScale(0.72f);
        f.rock->setPosition(m_player1->getPosition());
        f.rock->setRotation(m_player1->getRotation());
        f.rock->setOpacity(255);
        this->addChild(f.rock, 10000);

        m_player1->setVisible(false);
        f.turned = true;
        f.rockTime = randomRange(3.f, 7.f);
    }

    void unRock() {
        auto& f = *m_fields;

        if (m_player1)
            m_player1->setVisible(true);

        if (f.rock) {
            f.rock->removeFromParentAndCleanup(true);
            f.rock = nullptr;
        }

        f.turned = false;
        f.rockTime = 0.f;
    }

    float randomRange(float low, float high) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(low, high);
        return dist(rng);
    }

    void onExit() {
        unRock();
        PlayLayer::onExit();
    }
};
