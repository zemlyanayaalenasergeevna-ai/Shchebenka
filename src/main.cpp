#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <random>

using namespace geode::prelude;

class $modify(ShchebenkaPlayLayer, PlayLayer) {
    struct Fields {
        float nextEvent = 15.f;
        float rockTime = 0.f;
        bool rockMode = false;
        bool initialized = false;
    };

    float randomTime(float min, float max) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }

    void update(float dt) {
        PlayLayer::update(dt);

        if (!m_player1 || m_player1->m_isDead || m_hasCompletedLevel)
            return;

        if (!m_fields->initialized) {
            m_fields->initialized = true;
            m_fields->nextEvent = randomTime(10.f, 35.f);
        }

        if (m_fields->rockMode) {
            m_fields->rockTime -= dt;

            if (m_player1->m_iconSprite) {
                m_player1->m_iconSprite->setColor({105, 105, 105});
                m_player1->m_iconSprite->setOpacity(255);
            }

            if (m_fields->rockTime <= 0.f) {
                stopRock();
                m_fields->nextEvent = randomTime(12.f, 40.f);
            }
            return;
        }

        m_fields->nextEvent -= dt;
        if (m_fields->nextEvent <= 0.f)
            startRock();
    }

    void startRock() {
        if (m_fields->rockMode || !m_player1)
            return;

        // Temporary built-in sound until the real voice line is supplied.
        FMODAudioEngine::sharedEngine()->playEffect("gameSound_01.ogg");

        m_fields->rockMode = true;
        m_fields->rockTime = randomTime(3.f, 7.f);

        if (m_player1->m_iconSprite) {
            m_player1->m_iconSprite->setColor({105, 105, 105});
            m_player1->m_iconSprite->setOpacity(255);
        }
    }

    void stopRock() {
        if (m_player1 && m_player1->m_iconSprite) {
            m_player1->m_iconSprite->setColor({255, 255, 255});
            m_player1->m_iconSprite->setOpacity(255);
        }

        m_fields->rockMode = false;
        m_fields->rockTime = 0.f;
    }

    void onExit() {
        stopRock();
        PlayLayer::onExit();
    }
};
