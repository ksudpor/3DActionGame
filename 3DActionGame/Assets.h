#ifndef ASSETS_H_
#define ASSETS_H_

// メッシュ
enum {
	Mesh_None,
	Mesh_Player,
	Mesh_EnemyMelee,
	Mesh_EnemyRanged,
	Mesh_WeaponPistol,
	Mesh_WeaponRifle,
	Mesh_WeaponShotgun,
	Mesh_WeaponAxe,
	Mesh_WeaponSword,
	Mesh_WeaponShield,
	Mesh_WeaponGreatsword,
	Mesh_BulletPistol,
	Mesh_BulletRifle,
	Mesh_BulletShotgun,
	Mesh_HealthPotion,
	Mesh_KeyItem
};

enum {
	Skel_None,
	Skel_Player,
	Skel_EnemyMelee,
	Skel_EnemyRanged
};

enum {
	Anim_None,
	Anim_Player,
	Anim_EnemyMelee,
	Anim_EnemyRanged
};

// オクトリー
enum {
	Octree_Stage,
	Octree_Collider
};

// エフェクト
enum {
	Effect_MuzzleFlash1,
	Effect_MuzzleFlash2,
	Effect_SlashRed,
	Effect_SlashBlue,
	Effect_SlashDark,
	Effect_Hit,
	Effect_Guard,
	Effect_AuraYellow,
	Effect_BigArrowDark,
	Effect_LootBeamRed,
	Effect_LootBeamYellow,
	Effect_Goal
};

// 画像
enum {
	Image_Background,
	Image_Title,
	Image_EnglishOption,
	Image_JapaneseOption,
	Image_PlayIconEN,
	Image_PlayIconJP,
	Image_OptionIconEN,
	Image_OptionIconJP,
	Image_QuitIconEN,
	Image_QuitIconJP,
	Image_ConfirmEN,
	Image_ConfirmJP,
	Image_WeaponOneEN,
	Image_WeaponOneJP,
	Image_WeaponTwoEN,
	Image_WeaponTwoJP,
	Image_StageOneEN,
	Image_StageOneJP,
	Image_StageTwoEN,
	Image_StageTwoJP,
	Image_StageThreeEN,
	Image_StageThreeJP,
	Image_QuestStageOneEN,
	Image_QuestStageOneJP,
	Image_QuestStageTwoEN,
	Image_QuestStageTwoJP,
	Image_QuestStageThreeEN,
	Image_QuestStageThreeJP,
	Image_SelectWeaponEN,
	Image_SelectWeaponJP,
	Image_SelectRewardEN,
	Image_SelectRewardJP,
	Image_SameWeaponEN,
	Image_SameWeaponJP,
	Image_OptionEN,
	Image_OptionJP,
	Image_ResolutionEN,
	Image_ResolutionJP,
	Image_ScreenModeEN,
	Image_ScreenModeJP,
	Image_BgmEN,
	Image_BgmJP,
	Image_SeEN,
	Image_SeJP,
	Image_SoundBar,
	Image_LanguageEN,
	Image_LanguageJP,
	Image_CheckControlEN,
	Image_CheckControlJP,
	Image_640x480,
	Image_800x600,
	Image_1024x768,
	Image_1280x720,
	Image_1600x900,
	Image_1920x1080,
	Image_WindowEN,
	Image_WindowJP,
	Image_FullscreenEN,
	Image_FullscreenJP,
	Image_ApplyEN,
	Image_ApplyJP,
	Image_RetryEN,
	Image_RetryJP,
	Image_RetryResetEN,
	Image_RetryResetJP,
	Image_NextEN,
	Image_NextJP,
	Image_ResultEN,
	Image_ResultJP,
	Image_TimeSpentEN,
	Image_TimeSpentJP,
	Image_TotalScoreEN,
	Image_TotalScoreJP,
	Image_ReturnTitleEN,
	Image_ReturnTitleJP,
	Image_GameOverEN,
	Image_GameOverJP,
	Image_GameClearEN,
	Image_GameClearJP,
	Image_HpFrame,
	Image_HpBarGrey,
	Image_HpBarRed,
	Image_SpBarGrey,
	Image_SpBarOrange,
	Image_TextureNumber,
	Image_TextureScore,
	Image_TextureTime,
	Image_Colon,
	Image_Slash,
	Image_Axe,
	Image_SwordShield,
	Image_GreatSword,
	Image_Pistol,
	Image_Rifle,
	Image_Shotgun,
	Image_HealthRewardEN,
	Image_StaminaRewardEN,
	Image_SpeedRewardEN,
	Image_AttackRewardEN,
	Image_HealthRewardJP,
	Image_StaminaRewardJP,
	Image_SpeedRewardJP,
	Image_AttackRewardJP,
	Image_Skybox
};

//BGM
enum {
	BGM_GamePlay,
	BGM_Over,
	BGM_Clear
};

//SE
enum {
	SE_Select,
	SE_Confirm,
	SE_Walk,
	SE_Guard,
	SE_Jump,
	SE_Evade,
	SE_Pistol,
	SE_Rifle,
	SE_Shotgun,
	SE_Axe1,
	SE_Axe2,
	SE_Axe3,
	SE_Greatsword1,
	SE_Greatsword2,
	SE_Greatsword3,
	SE_Sword1,
	SE_Sword2,
	SE_Sword3,
	SE_PlayerDie,
	SE_PlayerDamage,
	SE_EnemyRanged,
	SE_EnemyMelee,
	SE_EnemyDie,
	SE_EnemyDamage,
	SE_ChangeWeapon,
	SE_Key,
	SE_Count
};
#endif