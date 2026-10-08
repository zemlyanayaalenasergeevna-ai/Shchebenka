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

        auto& f = *m_fields;

        if (!f.initialized) {
            f.initialized = true;
            f.nextEvent = randomTime(10.f, 35.f);
        }

        if (f.rockMode) {
            f.rockTime -= dt;

            if (m_player1->m_iconSprite) {
                m_player1->m_iconSprite->setColor({105, 105, 105});
                m_player1->m_iconSprite->setOpacity(255);
            }

            if (f.rockTime <= 0.f) {
                stopRock();
                f.nextEvent = randomTime(12.f, 40.f);
            }
            return;
        }

        f.nextEvent -= dt;
        if (f.nextEvent <= 0.f)
            startRock();
    }

    void startRock() {
        auto& f = *m_fields;
        if (f.rockMode || !m_player1)
            return;

        // Temporary built-in sound until the real voice line is supplied.
        FMODAudioEngine::sharedEngine()->playEffect("gameSound_01.ogg");

        f.rockMode = true;
        f.rockTime = randomTime(3.f, 7.f);

        if (m_player1->m_iconSprite) {
            m_player1->m_iconSprite->setColor({105, 105, 105});
            m_player1->m_iconSprite->setOpacity(255);
        }
    }

    void stopRock() {
        auto& f = *m_fields;

        if (m_player1 && m_player1->m_iconSprite) {
            m_player1->m_iconSprite->setColor({255, 255, 255});
            m_player1->m_iconSprite->setOpacity(255);
        }

        f.rockMode = false;
        f.rockTime = 0.f;
    }

    void onExit() {
        stopRock();
        PlayLayer::onExit();
    }
};
