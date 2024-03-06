#include "GamePlayScene.h"
#include "Field.h"
#include "CameraRotateAround.h"
#include "Light.h"
#include "Player.h"
#include "StageManager.h"
#include "Assets.h"
#include <GSeffect.h>
#include "Screen.h"
#include "GameProgress.h"

// �W���V�F�[�_�[�̐ݒ�
//#define GS_ENABLE_AUX_LIGHT                 // �⏕���C�g��L���ɂ���
#define GS_ENABLE_BAKED_LIGHTMAP_SHADOW     // �x�C�N�������C�g�}�b�v�ɉe�𗎂Ƃ�
#define GS_ENABLE_SOFT_SHADOW               // �\�t�g�V���h�E�i�e�̗֊s���ڂ����j
#define GS_ENABLE_RIM_LIGHT                 // �������C�g��L���ɂ���
#include <GSstandard_shader.h>

void GamePlayScene::start(){
    // �I���t���O��������
    is_end_ = false;
    is_play_se_ = false;
    // ������J�����O��L���ɂ���
    gsEnable(GS_FRUSTUM_CULLING);
    // �f�t�H���g�V�F�[�_�[�̏������i���b�V���t�@�C����ǂݍ��ޑO�ɗL���ɂ���j
    gsInitDefaultShader();
    // �G�t�F�N�g�̏�����
    gsInitEffect();
    // ���C�g�}�b�v�̓ǂݍ���(0�Ԃɓǂݍ��߂Ύ����I�ɓK�p�����j
    gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt");
    // ���t���N�V�����v���[�u�̓ǂݍ���
    gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt");
    // �⏕���C�g�̓ǂݍ���
    //gsLoadAuxLight(0, "Assets/AuxLight/AuxLight.txt");
    // �V���h�E�}�b�v�̍쐬
    static const GSuint shadow_map_size[] = { 2048, 2048 };
    gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
    // �V���h�E�}�b�v��K�p���鋗��(���_����̋����j
    gsSetShadowMapDistance(60.0f);
    // �J�X�P�[�h�V���h�E�}�b�v�̕����ʒu�𒲐��i�f�t�H���g��0.5�j
    gsSetShadowMapCascadeLamda(0.7f);
    // �V���h�E�̔Z����ݒ�(0.0:�Z���`1.0:����)
    gsSetShadowMapAttenuation(0.6f);
    // �V���h�E�}�b�v�o�C�A�X
    gsSetShadowMapBias(0.0f);

    gsLoadTexture(Image_HpFrame, "Assets/Image/hp_frame.png");
    gsLoadTexture(Image_HpBarGrey, "Assets/Image/hp_bar_grey.png");
    gsLoadTexture(Image_HpBarRed, "Assets/Image/hp_bar_red.png");
    gsLoadTexture(Image_SpBarGrey, "Assets/Image/sp_bar_grey.png");
    gsLoadTexture(Image_SpBarOrange, "Assets/Image/sp_bar_orange.png");
    gsLoadTexture(Image_TextureNumber, "Assets/Image/texture_number.png");
    gsLoadTexture(Image_TextureScore, "Assets/Image/texture_score.png");
    gsLoadTexture(Image_TextureTime, "Assets/Image/texture_time.png");
    gsLoadTexture(Image_QuestStageOneEN, "Assets/Image/quest_stage1_en.png");
    gsLoadTexture(Image_QuestStageOneJP, "Assets/Image/quest_stage1_jp.png");
    gsLoadTexture(Image_QuestStageTwoEN, "Assets/Image/quest_stage2_en.png");
    gsLoadTexture(Image_QuestStageTwoJP, "Assets/Image/quest_stage2_jp.png");
    gsLoadTexture(Image_QuestStageThreeEN, "Assets/Image/quest_stage3_en.png");
    gsLoadTexture(Image_QuestStageThreeJP, "Assets/Image/quest_stage3_jp.png");
    gsLoadTexture(Image_Colon, "Assets/Image/colon.png");
    gsLoadTexture(Image_Slash, "Assets/Image/slash.png");
    gsLoadTexture(Image_Skybox, "Assets/Skybox/skybox.dds");

    // ���b�V���̓ǂݍ���
    gsLoadSkinMesh(Mesh_Player, "Assets/Player/player.msh");
    gsLoadSkeleton(Skel_Player, "Assets/Player/player.sklb");
    gsLoadAnimation(Anim_Player, "Assets/Player/player.anmb");

    gsLoadSkinMesh(Mesh_EnemyMelee, "Assets/EnemyMelee/enemy_melee.mshb");
	gsLoadSkeleton(Skel_EnemyMelee, "Assets/EnemyMelee/enemy_melee.sklb");
	gsLoadAnimation(Anim_EnemyMelee, "Assets/EnemyMelee/enemy_melee.anmb");

    gsLoadSkinMesh(Mesh_EnemyRanged, "Assets/EnemyRanged/enemy_ranged.mshb");
    gsLoadSkeleton(Skel_EnemyRanged, "Assets/EnemyRanged/enemy_ranged.sklb");
    gsLoadAnimation(Anim_EnemyRanged, "Assets/EnemyRanged/enemy_ranged.anmb");

    gsLoadMesh(Mesh_WeaponRifle, "Assets/Weapon/Rifle/rifle.mshb");
    gsLoadMesh(Mesh_WeaponPistol, "Assets/Weapon/Pistol/pistol.mshb");
    gsLoadMesh(Mesh_WeaponShotgun, "Assets/Weapon/Shotgun/shotgun.mshb");
    gsLoadMesh(Mesh_WeaponShotgun, "Assets/Weapon/Shotgun/shotgun.mshb");
    gsLoadMesh(Mesh_WeaponAxe, "Assets/Weapon/Axe/axe.mshb");
    gsLoadMesh(Mesh_WeaponSword, "Assets/Weapon/Sword/sword.mshb");
    gsLoadMesh(Mesh_WeaponShield, "Assets/Weapon/Shield/shield.mshb");
    gsLoadMesh(Mesh_WeaponGreatsword, "Assets/Weapon/Greatsword/greatsword.mshb");
    gsLoadMesh(Mesh_BulletPistol, "Assets/Bullet/BulletPistol/bullet_pistol.mshb");
    gsLoadMesh(Mesh_BulletRifle, "Assets/Bullet/BulletRifle/bullet_rifle.mshb");
    gsLoadMesh(Mesh_BulletShotgun, "Assets/Bullet/BulletShotgun/bullet_shotgun.mshb");
    gsLoadMesh(Mesh_HealthPotion, "Assets/Item/HealthPotion/health_potion.mshb");
    gsLoadMesh(Mesh_KeyItem, "Assets/Item/KeyItem/key.mshb");
    //gsLoadMeshFixed(Mesh_Skybox, "Assets/Skybox/skybox.dds");

    // �I�N�g���[�̓ǂݍ���
    //gsLoadOctree(Octree_Stage, "Assets/Octree/stage.oct");
    //gsLoadOctree(Octree_Collider, "Assets/Octree/stage_collider.oct");
    //gsLoadOctree(Octree_Stage, "Assets/Octree/Stage1/stage1.oct");
    //gsLoadOctree(Octree_Collider, "Assets/Octree/Stage1/stage1_collider.oct");

    // �G�t�F�N�g�t�@�C���̓ǂݍ���
    gsLoadEffect(Effect_MuzzleFlash1, "Assets/Effect/MuzzleFlash01/MuzzleFlash01.efkefc");
    gsLoadEffect(Effect_MuzzleFlash2, "Assets/Effect/MuzzleFlash02/MuzzleFlash02.efkefc");
    gsLoadEffect(Effect_SlashRed, "Assets/Effect/Slash02/Slash02_red.efkefc");
    gsLoadEffect(Effect_SlashBlue, "Assets/Effect/Slash02/Slash02_blue.efkefc");
    gsLoadEffect(Effect_SlashDark, "Assets/Effect/Slash02/Slash02_dark.efkefc");
    gsLoadEffect(Effect_Hit, "Assets/Effect/Hit02/Hit02-random-direction.efkefc");
    gsLoadEffect(Effect_Guard, "Assets/Effect/Guard/Guard03.efkefc");
    gsLoadEffect(Effect_AuraYellow, "Assets/Effect/Aura02/Aura02_Yellow.efkefc");
    gsLoadEffect(Effect_BigArrowDark, "Assets/Effect/Arrow02/Arrow02_Dark.efkefc");
    gsLoadEffect(Effect_LootBeamRed, "Assets/Effect/LootBeam/LootBeam_Red.efkefc");
    gsLoadEffect(Effect_LootBeamYellow, "Assets/Effect/LootBeam/LootBeam_Yellow.efkefc");
    gsLoadEffect(Effect_Goal, "Assets/Effect/MagicCircle/MagicCircle01.efkefc");

    gsLoadBGM(BGM_GamePlay, "Assets/Sound/bgmGameplay.ogg", GS_TRUE);
    gsLoadSE(SE_Pistol, "Assets/Sound/sePistol.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Rifle, "Assets/Sound/seRifle.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Shotgun, "Assets/Sound/seShotgun.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Axe1, "Assets/Sound/seAxe1.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Axe2, "Assets/Sound/seAxe2.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Axe3, "Assets/Sound/seAxe3.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Greatsword1, "Assets/Sound/seGreatsword1.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Greatsword2, "Assets/Sound/seGreatsword2.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Greatsword3, "Assets/Sound/seGreatsword3.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Sword1, "Assets/Sound/seSword1.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Sword2, "Assets/Sound/seSword2.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Sword3, "Assets/Sound/seSword3.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Guard, "Assets/Sound/seGuard.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Jump, "Assets/Sound/seJump.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Evade, "Assets/Sound/seEvade.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_PlayerDie, "Assets/Sound/sePlayerDie.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_PlayerDamage, "Assets/Sound/sePlayerDamage.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_EnemyDie, "Assets/Sound/seEnemyDie.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_EnemyDamage, "Assets/Sound/seEnemyDamage.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_EnemyRanged, "Assets/Sound/seEnemyRanged.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_EnemyMelee, "Assets/Sound/seEnemyMelee.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_ChangeWeapon, "Assets/Sound/seChangeWeapon.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Walk, "Assets/Sound/seWalk.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_Key, "Assets/Sound/seKey.wav", 1, GWAVE_DEFAULT);

    // ��Ղ̒ǉ�
    world_.add_actor(new StageManager{ &world_});
    
    world_.add_field(new Field{ Octree_Stage, Octree_Collider, Image_Skybox });
    world_.add_camera(new CameraRotateAround{&world_, 
        GSvector3{0.0f, 3.2f, -4.8f}, GSvector3{0.0f, 1.92, 0.0f} });
    world_.add_light(new Light{ &world_ });

    player_ = world_.find_actor("Player");
    stage_manager_ = world_.find_actor("StageManager");

    pause_ = new PauseScreen();
    pause_->start();
    gsPlayBGM(BGM_GamePlay);

    fade_ = new Fade();
    fade_->start_fade_in(60.0f);
}

void GamePlayScene::update(float delta_time){
    fade_->update(delta_time);
    if (fade_->end_scene()) is_end_ = true;
    if (fade_->is_fading()) return;

    if (stage_manager_->game_clear()) {
        GameProgress::score_stage = world_.score() + world_.time() * 0.1f;
        GameProgress::time_stage = world_.spent_time();
        next_ = "ResultScene";
        gsStopBGM();
        fade_->start_fade_out(90.0f);
        //is_end_ = true;
    }
    if (player_->is_dead() || stage_manager_->game_over()) {
        next_ = "GameOverScene";
        gsStopBGM();
        fade_->start_fade_out(90.0f);
        //is_end_ = true;
    }

    if (is_paused_) {
        if (gsGetKeyTrigger(GKEY_P)) is_paused_ = false;
        if (pause_->is_end()) {
            next_ = "TitleScene";
            gsStopBGM();
            is_paused_ = false;
            fade_->start_fade_out(90.0f);
            //is_end_ = true;
        }
        pause_->update(delta_time);
        return;
    } 
    world_.update(delta_time);
    if (gsGetKeyTrigger(GKEY_P)) is_paused_ = true;
}

void GamePlayScene::draw() const{
    world_.draw();
    if (is_paused_) {
        pause_->draw();
    }
    fade_->draw();
}

bool GamePlayScene::is_end() const{
    return is_end_;
}

std::string GamePlayScene::next() const{
    return next_;
}

void GamePlayScene::end(){
    // ���[���h������
    world_.clear();
    // �S�ẴG�t�F�N�g���~����
    gsStopAllEffects();
    gsDeleteTexture(Image_HpFrame);
    gsDeleteTexture(Image_HpBarGrey);
    gsDeleteTexture(Image_HpBarRed);
    gsDeleteTexture(Image_SpBarGrey);
    gsDeleteTexture(Image_SpBarOrange);
    gsDeleteTexture(Image_TextureNumber);
    gsDeleteTexture(Image_Skybox);
    gsDeleteTexture(Image_TextureScore);
    gsDeleteTexture(Image_TextureTime);
    gsDeleteTexture(Image_QuestStageOneEN);
    gsDeleteTexture(Image_QuestStageOneJP);
    gsDeleteTexture(Image_QuestStageTwoEN);
    gsDeleteTexture(Image_QuestStageTwoJP);
    gsDeleteTexture(Image_Colon);
    gsDeleteTexture(Image_Slash);
    // ���b�V���̍폜
    gsDeleteSkinMesh(Mesh_Player);
    gsDeleteSkinMesh(Mesh_EnemyMelee);
    gsDeleteSkinMesh(Mesh_EnemyRanged);
    gsDeleteMesh(Mesh_WeaponRifle);
    gsDeleteMesh(Mesh_WeaponPistol);
    gsDeleteMesh(Mesh_WeaponShotgun);
    gsDeleteMesh(Mesh_WeaponAxe);
    gsDeleteMesh(Mesh_WeaponSword);
    gsDeleteMesh(Mesh_WeaponShield);
    gsDeleteMesh(Mesh_WeaponGreatsword);
    gsDeleteMesh(Mesh_BulletPistol);
    gsDeleteMesh(Mesh_BulletRifle);
    gsDeleteMesh(Mesh_BulletShotgun);
    gsDeleteMesh(Mesh_HealthPotion);
    gsDeleteSkeleton(Skel_Player);
    gsDeleteSkeleton(Skel_EnemyMelee);
    gsDeleteSkeleton(Skel_EnemyRanged);
    gsDeleteAnimation(Anim_Player);
    gsDeleteAnimation(Anim_EnemyMelee);
    gsDeleteAnimation(Anim_EnemyRanged);
    // �I�N�g���[�̍폜
    gsDeleteOctree(Octree_Stage);
    gsDeleteOctree(Octree_Collider);
    // ���C�g�}�b�v�̍폜
    gsDeleteLightmap(0);
    // ���t���N�V�����v���[�u�̍폜
    gsDeleteReflectionProbe(0);
    // �⏕���C�g�̍폜
    //gsDeleteAuxLight(0);
    // �G�t�F�N�g�̍폜
    gsDeleteEffect(Effect_MuzzleFlash1);
    gsDeleteEffect(Effect_MuzzleFlash2);
    gsDeleteEffect(Effect_SlashRed);
    gsDeleteEffect(Effect_SlashBlue);
    gsDeleteEffect(Effect_SlashDark);
    gsDeleteEffect(Effect_Hit);
    gsDeleteEffect(Effect_Guard);
    gsDeleteEffect(Effect_AuraYellow);
    gsDeleteEffect(Effect_BigArrowDark);
    gsDeleteEffect(Effect_LootBeamRed);
    gsDeleteEffect(Effect_LootBeamYellow);
    gsDeleteEffect(Effect_Goal);
    // �G�t�F�N�g�̏I��
    gsFinishEffect();
    pause_->end();
    gsDeleteBGM(BGM_GamePlay);
    gsDeleteSE(SE_Pistol);
    gsDeleteSE(SE_Rifle);
    gsDeleteSE(SE_Shotgun);
    gsDeleteSE(SE_Axe1);
    gsDeleteSE(SE_Axe2);
    gsDeleteSE(SE_Axe3);
    gsDeleteSE(SE_Greatsword1);
    gsDeleteSE(SE_Greatsword2);
    gsDeleteSE(SE_Greatsword3);
    gsDeleteSE(SE_Sword1);
    gsDeleteSE(SE_Sword2);
    gsDeleteSE(SE_Sword3);
    gsDeleteSE(SE_Guard);
    gsDeleteSE(SE_Jump);
    gsDeleteSE(SE_Evade);
    gsDeleteSE(SE_PlayerDie);
    gsDeleteSE(SE_PlayerDamage);
    gsDeleteSE(SE_EnemyDie);
    gsDeleteSE(SE_EnemyDamage);
    gsDeleteSE(SE_EnemyRanged);
    gsDeleteSE(SE_EnemyMelee);
    gsDeleteSE(SE_ChangeWeapon);
    gsDeleteSE(SE_Walk);
    gsDeleteSE(SE_Key);
}


