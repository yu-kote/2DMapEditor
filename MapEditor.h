#pragma once
#include "EnvSingleton.h"
#include "Block.h"
#include "Model_MapEditor.h"


class MapEditor{
private:
	// 選択するブロック
	Texture arrowbutton = Texture("res/Texture/Buttonarrow.png");
	Texture button = Texture("res/Texture/Button.png");

	// 背景
	Texture bg = Texture("res/Texture/sky_cloud3.png");

	// ブロックのTextureの配列
	std::vector<Texture> tex;

	// Font
	Font font = Font("res/KodomoRounded.otf");
	Font savefont = Font("res/KodomoRounded.otf");
	Font modeselectfont = Font("res/KodomoRounded.otf");
	Font openfont = Font("res/KodomoRounded.otf");
	int fontsize = 50;

	typedef std::vector<std::vector<std::shared_ptr<Block>>> pvetBlock;
	pvetBlock block;

	Block selectblock[BLOCK_MAX];
	Block nowselectblock;
	Vec2i blocknum = Vec2i(WIDE,LENGTH);

	int slide_selectblock = 0;
	int delay_max = 15;
	int button_delay = delay_max;
	BlockType now_select_block = BlockType::AIR_BLOCK;

	Vec2f pos = Vec2f(0.0f, 0.0f);
	Vec2f size = Vec2f(150, 150);

	Vec2f offset_mousepos;			// マウスの位置補正値
	bool is_setting = false;		// trueの時はセッティング画面を出す
	bool is_setting_hide = false;	// trueの時は

	// 背景描画
	void bgDraw();
	
	// ブロック変換関連
	void blockChange();

	// ブロック増やす
	void blockIncrease();

	// マップ描画関連
	void mapDraw();

	// マップ移動関連
	void mapMove();
	Vec2f camera = Vec2f(-WIDTH / 2, -HEIGHT / 2);
	int cameraspeed = 30;

	//マップの拡大縮小
	Vec2f zooming = Vec2f(0.5, 0.5); 
	float zoomspeed = 0.01f;
	
	// ui関連
	void uiUpdate();
	void uiDraw();
	bool is_slide_left = false;
	bool is_slide_right = false;

	// ブロック選び
	void selectBlock();
	Vec2f select_block_size = Vec2f(WIDTH, 130);

	// 矢印xサイズとセレクトブロックの共通ｙサイズ
	Vec2f slide_size = Vec2f(80, select_block_size.y());

	// セッティング画面関連
	void settingUpdate();
	void settingDraw();
	Vec2f settingwindow_pos = Vec2f::Zero();
	Vec2f settingwindow_size = Vec2f::Zero();
	Vec2f settingwindow_size_max = Vec2f(-WIDTH / 1.3, -HEIGHT / 1.3);
	Vec2f setting_pos = Vec2f(WIDTH / 2 - fontsize * 5, HEIGHT / 2 - fontsize);
	Vec2f setting_size = Vec2f(fontsize * 4, fontsize);
	float easing_time = 0.0f;
	bool filesave_mode = false;
	bool fileopen_mode = false;
	bool now_status = false;
	bool is_modechange = false;
	bool isSettingMode();

	// txtファイル一覧描画
	void fileView();

	// セーブ関連
	void save(std::string filename = "res/StageText/stage1.txt");
	void saveUpdate();
	void saveDraw();
	void saveExecution();
	bool is_savesetting = false;	// セーブするステージ番号選ぶやつ
	bool is_savedraw = false;	// セーブしました出す奴
	int okbutton_delay_max = 60;
	int okbutton_button_delay = okbutton_delay_max;
	int filesavecount = 1;		// ステージナンバー


	// 編集途中の読み込み関連
	void fileopen(std::string filename = "res/StageText/stage1.txt");
	bool tryFileopen(std::string filename);
	void fileopenUpdate();
	void fileopenDraw();
	void fileopenExecution();
	bool is_fileopensetting = false;	// ファイル開くステージ番号選ぶやつ
	bool is_fileopendraw = false;
	int fileopencount = 1;

	// 今のエディターの状態を見る関数
	void statusUpdate();
	void statusDraw();
	bool is_statusdraw = false;

	// 当り判定調べるやつ
	void checkHitBox(Vec2f pos, Vec2f size, Color color = Color::white);
	void checkHitFillBox(Vec2f pos, Vec2f size,Color color);

public:
	MapEditor();

	void setup(int ,int);
	void update();
	void draw();

	Scene shift();


};