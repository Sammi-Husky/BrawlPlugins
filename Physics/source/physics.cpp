#include <os/OSError.h>
#include <sy_core.h>
#include <modules.h>
#include <ft/fighter.h>

namespace Physics {
    void aerialTransitionFix(Fighter* fighter) {
        soStatusModule* statusModule = fighter->m_moduleAccesser->getStatusModule();
        soPostureModule* postureModule = fighter->m_moduleAccesser->getPostureModule();
        soGroundModule* groundModule = fighter->m_moduleAccesser->getGroundModule();

        if (groundModule->getCollStatus(0)->m_touchFlag7 && ((statusModule->getPrevStatusKind(0) == 0x33 && statusModule->getStatusKind() == 0xE) || (statusModule->getPrevStatusKind(0) == 0x45 && statusModule->getStatusKind() == 0x49))) {
            Vec3f newPos = postureModule->getPos();
            groundModule->setCorrect(2, 0);
            fighter->processAnim();
            fighter->processUpdate();
            fighter->processPreMapCorrection;
            fighter->processMapCorrection();
            fighter->processFixPosition();
            postureModule->initPos(&newPos);
        }
    }

    void smoothWavedashes(Fighter* fighter) {

    }

    void instantFastfall(Fighter* fighter) {

    }

    void fastfallTumble(Fighter* fighter) {

    }

    void gameplayFixes(Fighter* fighter) {
        aerialTransitionFix(fighter);
        smoothWavedashes(fighter);
        instantFastfall(fighter);
        fastfallTumble(fighter);
    }

    asm void postFighterProcessFixPosition()
    {
        nofralloc // don't need stack frame
        mr r3, r26
        bl gameplayFixes
        psq_l f31,0x68(r1),0,0  // original operation
    }

    void Init()
    {
        OSReport("Hello World\n");

        SyringeCore::sySimpleHookRel(0x12F998, reinterpret_cast<void*>(postFighterProcessFixPosition), Modules::SORA_MELEE);
    }

    void Destroy()
    {
        OSReport("Goodbye\n");
    }
}