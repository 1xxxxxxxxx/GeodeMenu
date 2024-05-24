#include "IconEffects.h"

ccColor3B EffectUI::getColourForSelected(int mode, bool player2) // bri`ish
{
    // 0 : primary, 1 : secondary : 2 : glow : 3 : trail : 4 : wave trail

    int sel = Mod::get()->getSavedValue<int>(fmt::format("selColour{}", mode), 0);

    if (mode == 0)
    {
        if (sel == 0)
            return GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value());
    }
    else if (mode == 1)
    {
        if (sel == 0)
            return GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value());
    }
    else if (mode == 2)
    {
        if (sel == 0)
            return GameManager::get()->colorForIdx(GameManager::get()->m_playerGlowColor.value());
    }
    else
    {
        if (sel == 0)
            return GameManager::get()->colorForIdx(Mod::get()->getSavedValue<bool>("same-dual") ? GameManager::get()->m_playerColor.value() : (player2 ? GameManager::get()->m_playerColor2.value() : GameManager::get()->m_playerColor.value()));
    }

    if (sel == 1)
        return ColourUtility::getChromaColour();

    if (sel == 2)
        return ColourUtility::getPastelColour();

    if (sel == 3)
    {
        std::stringstream fadeIn;
        fadeIn << "fadeColour1";
        fadeIn << mode;

        std::stringstream fadeOut;
        fadeOut << "fadeColour2";
        fadeOut << mode;

        ccColor3B in = Mod::get()->getSavedValue<ccColor3B>(fadeIn.str(), {0, 0, 0});
        ccColor3B out = Mod::get()->getSavedValue<ccColor3B>(fadeOut.str(), {255, 255, 255});

        return ColourUtility::lerpColour(in, out, (sinf(ColourUtility::va * 3) + 1) / 2);
        //fade
    }

    if (sel == 4)
    {
        //custom colour
    }

    return {0, 0, 0};
}

class $modify (GJBaseGameLayer)
{
    virtual void update(float p0)
    {
        if (m_player1)
        {
            m_player1->setColor(EffectUI::getColourForSelected(0));
            m_player1->setSecondColor(EffectUI::getColourForSelected(1));
            m_player1->m_glowColor = EffectUI::getColourForSelected(2);
            m_player1->updateGlowColor();
            m_player1->m_regularTrail->setColor(EffectUI::getColourForSelected(3));
            m_player1->m_waveTrail->setColor(EffectUI::getColourForSelected(4));
        }

        if (m_player2)
        {
            if (!Mod::get()->getSavedValue<bool>("same-dual"))
            {
                m_player2->setColor(EffectUI::getColourForSelected(1));
                m_player2->setSecondColor(EffectUI::getColourForSelected(0));
                m_player2->m_glowColor = EffectUI::getColourForSelected(2);
                m_player2->updateGlowColor();
            }
            else
            {
                m_player2->setColor(EffectUI::getColourForSelected(0));
                m_player2->setSecondColor(EffectUI::getColourForSelected(1));
                m_player2->m_glowColor = EffectUI::getColourForSelected(2);
                m_player2->updateGlowColor();
            }

            m_player1->m_regularTrail->setColor(EffectUI::getColourForSelected(3, true));
            m_player2->m_waveTrail->setColor(EffectUI::getColourForSelected(4, true));
        }

        GJBaseGameLayer::update(p0);
    }

    static void onModify(auto& self) {
        self.setHookPriority("GJBaseGameLayer::update", -69420);
    }
};