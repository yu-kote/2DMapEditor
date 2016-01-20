#pragma once
#include "EnvSingleton.h"
#include "Block.h"
#include "Model_MapEditor.h"


class MapEditor{
private:
	// �I������u���b�N
	Texture arrowbutton = Texture("res/Texture/Buttonarrow.png");
	Texture button = Texture("res/Texture/Button.png");

	// �w�i
	Texture bg = Texture("res/Texture/sky_cloud3.png");

	// �u���b�N��Texture�̔z��
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

	Vec2f offset_mousepos;			// �}�E�X�̈ʒu�␳�l
	bool is_setting = false;		// true�̎��̓Z�b�e�B���O��ʂ��o��
	bool is_setting_hide = false;	// true�̎���

	// �w�i�`��
	void bgDraw();
	
	// �u���b�N�ϊ��֘A
	void blockChange();

	// �u���b�N���₷
	void blockIncrease();

	// �}�b�v�`��֘A
	void mapDraw();

	// �}�b�v�ړ��֘A
	void mapMove();
	Vec2f camera = Vec2f(-WIDTH / 2, -HEIGHT / 2);
	int cameraspeed = 30;

	//�}�b�v�̊g��k��
	Vec2f zooming = Vec2f(0.5, 0.5); 
	float zoomspeed = 0.01f;
	
	// ui�֘A
	void uiUpdate();
	void uiDraw();
	bool is_slide_left = false;
	bool is_slide_right = false;

	// �u���b�N�I��
	void selectBlock();
	Vec2f select_block_size = Vec2f(WIDTH, 130);

	// ���x�T�C�Y�ƃZ���N�g�u���b�N�̋��ʂ��T�C�Y
	Vec2f slide_size = Vec2f(80, select_block_size.y());

	// �Z�b�e�B���O��ʊ֘A
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

	// txt�t�@�C���ꗗ�`��
	void fileView();

	// �Z�[�u�֘A
	void save(std::string filename = "res/StageText/stage1.txt");
	void saveUpdate();
	void saveDraw();
	void saveExecution();
	bool is_savesetting = false;	// �Z�[�u����X�e�[�W�ԍ��I�Ԃ��
	bool is_savedraw = false;	// �Z�[�u���܂����o���z
	int okbutton_delay_max = 60;
	int okbutton_button_delay = okbutton_delay_max;
	int filesavecount = 1;		// �X�e�[�W�i���o�[


	// �ҏW�r���̓ǂݍ��݊֘A
	void fileopen(std::string filename = "res/StageText/stage1.txt");
	bool tryFileopen(std::string filename);
	void fileopenUpdate();
	void fileopenDraw();
	void fileopenExecution();
	bool is_fileopensetting = false;	// �t�@�C���J���X�e�[�W�ԍ��I�Ԃ��
	bool is_fileopendraw = false;
	int fileopencount = 1;

	// ���̃G�f�B�^�[�̏�Ԃ�����֐�
	void statusUpdate();
	void statusDraw();
	bool is_statusdraw = false;

	// ���蔻�蒲�ׂ���
	void checkHitBox(Vec2f pos, Vec2f size, Color color = Color::white);
	void checkHitFillBox(Vec2f pos, Vec2f size,Color color);

public:
	MapEditor();

	void setup(int ,int);
	void update();
	void draw();

	Scene shift();


};