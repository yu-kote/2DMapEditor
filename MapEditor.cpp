#include "MapEditor.h"
#include "Function_Group.h"

MapEditor::MapEditor() {
	font.size(fontsize);
	//block = pvetBlock(WIDE, std::vector<std::shared_ptr<Block>>(LENGTH,std::make_shared<Block>()));
	std::vector<std::shared_ptr<Block>> temp;

	for (int y = 0; y < blocknum.y(); y++)
	{
		temp.clear();
		for (int x = 0; x < blocknum.x(); x++)
		{
			temp.push_back(std::make_shared<Block>());
		}
		block.push_back(temp);
	}

	const char* texture_name[BLOCK_MAX]{
		"res/Texture/Button.png",					// 0 ここは判定されないので適当なブロックを置く
			"res/Texture/stripe.png",				// 1 どの状態でも乗れる
			"res/Texture/moveright_white.png",		// 2 白　黒の時右に動く
			"res/Texture/downwhite.png",			// 3 白　黒になったら落ちる
			"res/Texture/doublewhite.png",			// 4 白　吸うと白になる吐くと黒くなる
			"res/Texture/white.png",				// 5 白　黒しか乗れない
			"res/Texture/moveright_black.png",		// 6 黒　白の時動く
			"res/Texture/downblack.png",			// 7 黒　白になったら落ちる
			"res/Texture/doubleblack.png",			// 8 黒　吸うと黒になる
			"res/Texture/black.png",				// 9 黒　吸うと白になる吐くと黒になる
			"res/Texture/chara_1.png",				// 10 プレイヤースタート位置
			"res/Texture/movedown_white.png",		// 11 白　下に動く
			"res/Texture/moveleft_white.png",		// 12 白　左に動く
			"res/Texture/moveup_white.png",			// 13 白　上に動く
			"res/Texture/movedown_black.png",		// 14 黒　下に動く
			"res/Texture/moveleft_black.png",		// 15 黒　左に動く
			"res/Texture/moveup_black.png",			// 16 黒　上に動く
			"res/Texture/goal.png",				// 17 ゴール

	};
	for (int index = 0; index < BLOCK_MAX; index++)
	{
		tex.push_back(Texture(texture_name[index]));
	}

	for (unsigned y = 0; y < block.size(); y++)
	{
		for (unsigned x = 0; x < block[y].size(); x++)
		{
			block[y][x]->setPosInterval(pos, size, x, y);
			block[y][x]->setExElement(0, Vec2f(1.0f, 1.0f), size / 2);
			block[y][x]->setBlockType(BlockType::AIR_BLOCK);
			block[y][x]->setColor(Color::white);
		}
	}

	int interval_size = 200;

	// Textureを入れるfor文
	for (int i = 0; i < BlockType::BLOCK_MAX; i++)
	{
		selectblock[i].setSelectPos(
			Vec2f(-WIDTH / 2 + -interval_size * slide_selectblock + interval_size,
				  -HEIGHT / 2 + 30),
			Vec2f(slide_size.y() / 1.5f, slide_size.y() / 1.5f),
			i);
		selectblock[i].setTexture(&tex[i], Vec2f(0, 0));
	}

	offset_mousepos = Vec2f(-WIDTH / 2, -HEIGHT / 2);
}

void MapEditor::setup(int wide, int length) {
	//block = pvetBlock(wide, std::vector<std::shared_ptr<Block>>(length, std::make_shared<Block>()));
	blocknum = Vec2i(wide, length);
	block.clear();
	std::vector<std::shared_ptr<Block>> temp;

	for (int y = 0; y < blocknum.y(); y++)
	{
		temp.clear();
		for (int x = 0; x < blocknum.x(); x++)
		{
			temp.push_back(std::make_shared<Block>());
		}
		block.push_back(temp);
	}
	for (unsigned y = 0; y < block.size(); y++)
	{
		for (unsigned x = 0; x < block[y].size(); x++)
		{
			block[y][x]->setPosInterval(pos, size, x, y);
			block[y][x]->setExElement(0, Vec2f(1.0f, 1.0f), Vec2f(0.0f, 0.0f));
			block[y][x]->setBlockType(BlockType::AIR_BLOCK);
			block[y][x]->setColor(Color::white);
		}
	}
}

void MapEditor::update() {
	if (isSettingMode())
	{
		blockChange();
		blockIncrease();
	}

	saveUpdate();
	fileopenUpdate();
	settingUpdate();
	statusUpdate();
	mapMove();
}

void MapEditor::draw() {
	bgDraw();
	mapDraw();
	settingDraw();
	uiDraw();
	saveDraw();
	fileopenDraw();
	statusDraw();
}

// シーンチェンジ
Scene MapEditor::shift() {
	return Scene::MAP_EDITOR;
}

void MapEditor::bgDraw() {
	drawTextureBox(-WIDTH / 2, -HEIGHT / 2,
				   WIDTH, HEIGHT,
				   0, 0,
				   2048, 1024,
				   bg, Color::white);
}

// マップ描画 Translateはここだけ
void MapEditor::mapDraw() {
	Vec2f offset_zoompos = Vec2f(WIDTH / 2, HEIGHT / 2);

	glPushMatrix();
	glScaled(zooming.x(), zooming.y(), 0);
	glTranslated(camera.x(), camera.y(), 0);
	for (unsigned y = 0; y < block.size(); y++)
	{
		for (unsigned x = 0; x < block[y].size(); x++)
		{
			if (-camera.x() - offset_zoompos.x() / zooming.x() < block[y][x]->getPos().x() + size.x() &&
				-camera.y() - offset_zoompos.y() / zooming.y() < block[y][x]->getPos().y() + size.y() &&
				-camera.x() + offset_zoompos.x() / zooming.x() > block[y][x]->getPos().x() &&
				-camera.y() + offset_zoompos.y() / zooming.y() > block[y][x]->getPos().y())
			{

				block[y][x]->draw();
				block[y][x]->textureDraw();
			}
		}
	}
	glPopMatrix();
}

// マップ移動
void MapEditor::mapMove() {
	if (env.isPressKey('W')) { camera.y() -= cameraspeed; }
	if (env.isPressKey('S') && !env.isPressKey(GLFW_KEY_LEFT_CONTROL)) { camera.y() += cameraspeed; }
	if (env.isPressKey('A')) { camera.x() += cameraspeed; }
	if (env.isPressKey('D')) { camera.x() -= cameraspeed; }

	offset_mousepos.x() = (env.mousePosition().x() / zooming.x() - camera.x()) / size.x();
	offset_mousepos.y() = (env.mousePosition().y() / zooming.y() - camera.y()) / size.y();

	if (isSettingMode())
	{
		if (env.isPressKey(GLFW_KEY_UP)) { zooming += Vec2f(zoomspeed, zoomspeed); }
		if (env.isPressKey(GLFW_KEY_DOWN)) { zooming -= Vec2f(zoomspeed, zoomspeed); }
	}
	if (zooming.x() < 0.1 && zooming.y() < 0.1)
	{
		zooming += Vec2f(zoomspeed, zoomspeed);
	}
	if (zooming.x() >= 1.0f && zooming.y() >= 1.0f)
	{
		zooming -= Vec2f(zoomspeed, zoomspeed);
	}
}

// ブロック配置
void MapEditor::blockChange() {
	Vec2f edit_range_pos = Vec2f(-WIDTH / 2, -HEIGHT / 2 + HEIGHT / 6);
	Vec2f edit_range_size = Vec2f(WIDTH, HEIGHT - HEIGHT / 6 - 50);


	if (pointCollision(env.mousePosition(), edit_range_pos, edit_range_size))
	{
		for (unsigned y = 0; y < block.size(); y++)
		{
			for (unsigned x = 0; x < block[y].size(); x++)
			{
				if (pointCollision(Vec2f(offset_mousepos.x() * size.x(), offset_mousepos.y() * size.y()), block[y][x]->getPos(), block[y][x]->getSize()))
				{
					block[y][x]->setMouseHit(pointCollision(Vec2f(offset_mousepos.x() * size.x(), offset_mousepos.y() * size.y()),
															block[y][x]->getPos(),
															block[y][x]->getSize()));
					if (env.isPressButton(Mouse::LEFT))
					{
						block[y][x]->setBlockType(now_select_block);
						// 選択されているTextureが配置される
						block[y][x]->setTexture(&tex[now_select_block], Vec2f(0, 0));
					}
					else if (env.isPressButton(Mouse::RIGHT))
					{
						block[y][x]->setBlockType(BlockType::AIR_BLOCK);
					}
				}
				else
				{
					block[y][x]->setMouseHit(0);
				}
			}
		}
	}

}

// ブロック増減
void MapEditor::blockIncrease() {
	if (env.isPushKey('1'))
	{
		for (unsigned y = 0; y < block.size(); y++)
		{
			block[y].push_back(std::make_shared<Block>(Vec2f(blocknum.x() * size.x(), y * size.y()), size, Vec2f(0, 0), BlockType::AIR_BLOCK));
		}
		blocknum.x()++;
		/*for (unsigned y = 0; y < blocknum.y(); y++)
		{
			for (unsigned x = 0; x < blocknum.x(); x++)
			{
				block[y][x]->setExElement(0, Vec2f(1.0f, 1.0f), size / 2);
			}
		}*/
	}
	if (env.isPushKey('3'))
	{
		std::vector<std::shared_ptr<Block>> temp;
		for (int y = 0; y < blocknum.y(); y++)
		{
			temp.clear();
			for (int x = 0; x < blocknum.x(); x++)
			{
				temp.push_back(std::make_shared<Block>(Vec2f(x * size.x(), blocknum.y() * size.y()), size, Vec2f(0, 0), BlockType::AIR_BLOCK));
			}
			block.push_back(temp);
			break;
		}
		blocknum.y()++;
		/*for (unsigned y = 0; y < blocknum.y(); y++)
		{
			for (unsigned x = 0; x < blocknum.x(); x++)
			{
				block[y][x]->setExElement(0, Vec2f(1.0f, 1.0f), size / 2);
			}
		}*/
	}

	if (env.isPushKey('2'))
	{
		for (unsigned y = 0; y < block.size(); y++)
		{
			block[y].pop_back();
		}
		blocknum.x()--;
	}
	if (env.isPushKey('4'))
	{
		block.pop_back();
		blocknum.y()--;
	}

}

// ブロック選び
void MapEditor::selectBlock() {
	for (int i = 0; i < BlockType::BLOCK_MAX; i++)
	{
		selectblock[i].setBlockType(i);
		selectblock[i].setColor(Color(1, 1, 1, 1));
		if (pointCollision(env.mousePosition(), selectblock[i].getPos(), selectblock[i].getSize()))
		{
			selectblock[i].setColor(Color(0, 1, 1, 1));
			if (is_slide_left == false && is_slide_right == false)
			{
				if (env.isPushButton(Mouse::LEFT))
				{
					now_select_block = selectblock[i].getBlockType();
				}
			}
		}
		selectblock[i].textureDraw();
	}

	nowselectblock.setNowSelectPos(Vec2f(WIDTH / 2 - 100, -HEIGHT / 2 + 200), Vec2f(100, 100));
	nowselectblock.setBlockType(now_select_block);
	nowselectblock.draw();
	nowselectblock.setTexture(&tex[now_select_block], Vec2f(0, 0));
	nowselectblock.textureDraw();
}

// 特に使ってない
void MapEditor::uiUpdate() {

}

// UI描画
void MapEditor::uiDraw() {
	Vec2f select_block_pos = Vec2f(-WIDTH / 2, -HEIGHT / 2);
	Vec2f slide_pos = Vec2f(WIDTH / 2 - slide_size.x(), -HEIGHT / 2);
	Color selectblock_color_right = Color(1, 1, 1, 1);
	Color selectblock_color_left = Color(1, 1, 1, 1);
	Vec2f arrow_size = Vec2f(80, select_block_size.y());
	Color arrow_select_color = Color::yellow;

	if (button_delay == delay_max)
	{
		if (pointCollision(env.mousePosition(), select_block_pos, arrow_size))
		{
			selectblock_color_left = arrow_select_color;
			if (slide_selectblock > 0)
			{
				if (env.isPushButton(Mouse::LEFT))
				{
					slide_selectblock--;
					is_slide_left = true;
					for (int i = 0; i < BlockType::BLOCK_MAX; i++)
					{
						selectblock[i].setIsSlideLeft(is_slide_left);
					}
				}
			}
		}
		if (pointCollision(env.mousePosition(), slide_pos, arrow_size))
		{
			selectblock_color_right = arrow_select_color;
			if (slide_selectblock < BlockType::BLOCK_MAX - 6)
			{
				if (env.isPushButton(Mouse::LEFT))
				{
					slide_selectblock++;
					is_slide_right = true;
					for (int i = 0; i < BlockType::BLOCK_MAX; i++)
					{
						selectblock[i].setIsSlideRight(is_slide_right);
					}
				}
			}
		}
	}

	if (is_slide_left == true || is_slide_right == true)
	{
		button_delay--;
		if (button_delay <= 0)
		{
			button_delay = delay_max;
			is_slide_left = false;
			is_slide_right = false;
		}
	}

	int slide_speed = 40;
	for (int i = 0; i < BlockType::BLOCK_MAX; i++)
	{
		selectblock[i].setSlideLeft(slide_speed);
	}

	for (int i = 0; i < BlockType::BLOCK_MAX; i++)
	{
		selectblock[i].setSlideRight(slide_speed);
	}

	Vec2i block_total = Vec2i(blocknum.x(), blocknum.y());

	if (is_setting == false)
	{
		// ちょっと透明にしてる選択ブロック
		drawFillBox(select_block_pos.x(), select_block_pos.y(),
					select_block_size.x(), select_block_size.y(),
					Color(1.0f, 1.0f, 1.0f, 0.8f));

		selectBlock();

		// 左の矢印
		drawTextureBox(select_block_pos.x(), select_block_pos.y(),
					   arrow_size.x(), arrow_size.y(),
					   0, 0,
					   256, 256,
					   arrowbutton,
					   selectblock_color_left,
					   0, Vec2f(-1, 1), Vec2f(slide_size.x(), 1));

		// 右の矢印
		drawTextureBox(slide_pos.x(), slide_pos.y(),
					   arrow_size.x(), arrow_size.y(),
					   0, 0,
					   256, 256,
					   arrowbutton,
					   selectblock_color_right,
					   0, Vec2f(1, 1), Vec2f(1, 1));

		// 要素数表示
		//font.draw("wide" + std::to_string(block_total.x()), Vec2f(0, 0), Color::black);
		//font.draw("         length" + std::to_string(block_total.y()), Vec2f(0, 0), Color::black);

		// ズームのフォント表示
		int zoom_draw = zooming.x() * 100;
		font.draw(std::to_string(zoom_draw) + "%", Vec2f(-WIDTH / 2 + 30, HEIGHT / 2 - 850), Color::yellow);
	}
}


// Settingの中身処理
void MapEditor::settingUpdate() {
	if (env.isPushKey(GLFW_KEY_BACKSPACE))
	{
		is_setting = false;
		is_modechange = false;
	}

}

// Setting画面の表示
void MapEditor::settingDraw() {
	Color fontcolor = Color::white;
	Color buttoncolor = Color::gray;
	Color settingwindow_color = Color(1, 1, 1, 0.8f);
	int modeselectfont_size = 100;
	const int mode = 3;

	// Setting赤
	if (pointCollision(env.mousePosition(), setting_pos, setting_size))
	{
		fontcolor = Color::red;
	}
	// Setting画面
	drawFillBox(settingwindow_pos.x(), settingwindow_pos.y(),
				settingwindow_size.x(), settingwindow_size.y(),
				settingwindow_color);
	drawBox(settingwindow_pos.x(), settingwindow_pos.y(),
			settingwindow_size.x(), settingwindow_size.y(),
			5, Color::black);

	// SettingWindowPosの初期化＆SettingWindow閉じる
	if (pointCollision(env.mousePosition(), setting_pos, setting_size))
	{
		if (env.isPushButton(Mouse::LEFT))
		{
			if (is_setting_hide == false) {
				settingwindow_pos = Vec2f(WIDTH / 2 - 150, HEIGHT / 2 - 100);
				settingwindow_size = Vec2f::Zero();
				modeselectfont.size(modeselectfont_size);

			}
			// モードの初期化
			is_savesetting = false;
			is_fileopensetting = false;
			is_statusdraw = false;
			is_modechange = false;

			// 逆を代入する
			is_setting = !is_setting;
		}
	}

	Vec2f settingwindowspeed = Vec2f(150, 100);

	Vec2f modeselect_pos[mode];
	Vec2f modeselect_size = Vec2f(600, modeselectfont_size / 1.5);
	Color modeselect_color[mode];
	filesave_mode = false;
	fileopen_mode = false;
	now_status = false;

	for (int i = 0; i < mode; i++)
	{
		modeselect_pos[i] = Vec2f(settingwindow_pos.x() + settingwindow_size_max.x() + 30,
								  settingwindow_pos.y() - modeselectfont_size * (i + 1));
		modeselect_color[i] = Color::white;
	}

	// Setting開く描画
	if (is_setting == true)
	{
		if (settingwindow_size.x() >= settingwindow_size_max.x() &&
			settingwindow_size.y() >= settingwindow_size_max.y())
		{
			settingwindow_size.x() -= settingwindowspeed.x();
			settingwindow_size.y() -= settingwindowspeed.y();
			is_setting_hide = true;

			// Easing失敗
			//easing_time += 0.1;
			//settingwindow_size.x() = EasingCircIn(easing_time, settingwindow_pos.x(), settingwindow_pos.x() + settingwindow_size_max.x());
		}
		else
		{
			//fileopenDraw();
			if (is_modechange == false)
			{
				for (int i = 0; i < mode; i++)
				{
					if (pointCollision(env.mousePosition(), modeselect_pos[i], modeselect_size))
					{
						modeselect_color[i] = Color::red;
						if (env.isPushButton(Mouse::LEFT))
						{
							if (i == 0) { filesave_mode = true; }
							if (i == 1) { fileopen_mode = true; }
							if (i == 2) { now_status = true; }
						}
					}
					checkHitFillBox(modeselect_pos[i], modeselect_size, modeselect_color[i]);
					checkHitBox(modeselect_pos[i], modeselect_size, Color::black);
				}
				modeselectfont.draw("filesave", modeselect_pos[0], Color::black);
				modeselectfont.draw("fileopen", modeselect_pos[1], Color::black);
				modeselectfont.draw("help", modeselect_pos[2], Color::black);
			}

		}
	}
	else
	{
		if (settingwindow_size.x() < 0 &&
			settingwindow_size.y() < 0)
		{
			settingwindow_size.x() += settingwindowspeed.x();
			settingwindow_size.y() += settingwindowspeed.y();
		}
	}

	// Settingボタン表示
	Vec2f settingbutton_pos = Vec2f(setting_pos.x(), setting_pos.y() - 10);
	Vec2f settingbutton_size = Vec2f(fontsize * 4, fontsize);
	drawFillBox(settingbutton_pos.x(), settingbutton_pos.y(), settingbutton_size.x(), settingbutton_size.y(), buttoncolor);
	drawBox(settingbutton_pos.x(), settingbutton_pos.y(), settingbutton_size.x(), settingbutton_size.y(), 4, Color::black);
	font.draw("Setting", setting_pos, fontcolor);



}

bool MapEditor::isSettingMode() {
	return (is_setting == false && is_modechange == false);
}

// txtファイル一覧描画
void MapEditor::fileView() {
	std::vector<std::string>  filelist;
	std::vector<Vec2f> filepos;
	Vec2f filenamestart_pos = Vec2f(settingwindow_pos.x() + settingwindow_size_max.x() + 20,
									settingwindow_pos.y());
	int filecount = 0;
	Vec2f filesize = Vec2f(800, fontsize);
	int file_interval = fontsize + 10;

	for (std::tr2::sys::directory_iterator it("res/StageText"), end; it != end; it++)
	{
		filelist.push_back(it->path());
		filepos.push_back(Vec2f::Zero());
	}

	for (auto it : filelist)
	{
		filepos[filecount] = Vec2f(filenamestart_pos.x(),
								   filenamestart_pos.y() - file_interval * (filecount + 1));
		it.erase(0, 14);	//"res/StageText/"だけ消去
		font.draw(it, filepos[filecount], Color::black);

		checkHitBox(filepos[filecount], filesize);

		filecount++;
	}
}

// セーブするときの処理
void MapEditor::saveUpdate() {
	if (filesave_mode == true)
	{
		is_savesetting = true;
		is_modechange = true;
		savefont.size(100);
	}

	if (is_savesetting == true)
	{
		if (env.isPushKey(GLFW_KEY_UP))
		{
			filesavecount++;
		}
		else if (env.isPushKey(GLFW_KEY_DOWN))
		{
			if (filesavecount > 0)
				filesavecount--;
		}
		if (env.isPushKey(GLFW_KEY_BACKSPACE))
		{
			is_savesetting = false;
			is_modechange = false;
			env.flushInput();
		}
	}

}

// セーブ描画
void MapEditor::saveDraw() {
	if (is_savesetting == true)
	{
		fileView();

		Vec2f numberwindow_pos = Vec2f(-100, 0);
		Vec2f numberwindow_size = Vec2f(fontsize * 9, fontsize * 2);
		Vec2f stageleselectfont_pos = Vec2f(numberwindow_pos.x(), numberwindow_pos.y() + 20);
		Vec2f button_pos = Vec2f(stageleselectfont_pos.x() + numberwindow_size.x() - 20,
								 numberwindow_pos.y());
		Vec2f button_size = Vec2f(200, 100);
		Color color = Color::black;

		if (pointCollision(env.mousePosition(), button_pos, button_size))
		{
			color = Color::red;
			if (env.isPushButton(Mouse::LEFT))
			{
				saveExecution();
			}
		}
		if (env.isPushKey(GLFW_KEY_ENTER))
		{
			saveExecution();
		}

		drawFillBox(numberwindow_pos.x(), numberwindow_pos.y(),
					numberwindow_size.x(), numberwindow_size.y(),
					Color::white);

		drawTextureBox(button_pos.x(), button_pos.y(), 200, 100,
					   0, 0, 256, 256,
					   button, Color::white);

		savefont.draw("stage  ", stageleselectfont_pos, Color::black);
		savefont.draw("       " + std::to_string(filesavecount), stageleselectfont_pos, Color::black);
		savefont.draw("         OK", stageleselectfont_pos, color);
	}


	// セーブフォント
	if (is_savedraw == true)
	{
		savefont.draw("Save stage" + std::to_string(filesavecount) + ".txt", Vec2f(-WIDTH / 2 + 250, 0), Color::yellow);
		okbutton_button_delay--;
		is_modechange = true;
		if (okbutton_button_delay < 0)
		{
			okbutton_button_delay = okbutton_delay_max;
			is_savedraw = false;
			is_modechange = false;
		}
	}

}

// セーブ処理　(セーブする場所と名前)
void MapEditor::save(std::string filename) {

	std::string file_name = std::string(filename);
	std::ofstream ofstr(file_name);

	// 最初に今のブロックの要素数を書き出して改行（俺ら流要素数決め方）
	ofstr << blocknum.y() << " " << blocknum.x() << " " << std::endl;

	// y逆回し
	for (int y = block.size() - 1; y >= 0; y--)
	{
		for (unsigned x = 0; x < block[y].size(); x++)
		{
			// 一列回す
			ofstr << block[y][x]->getBlockType() << " ";
		}
		// 一列回し終わったら改行
		ofstr << std::endl;
	}

	return;
}

void MapEditor::saveExecution() {
	is_savedraw = true;
	save("res/StageText/stage" + std::to_string(filesavecount) + ".txt");
	is_savesetting = false;
	is_setting = false;
	is_modechange = false;
}

// ファイル開くときの処理
void MapEditor::fileopenUpdate() {

	if (fileopen_mode == true)
	{
		is_fileopensetting = true;
		is_modechange = true;
		openfont.size(100);
	}

	if (is_fileopensetting == true)
	{

		if (env.isPushKey(GLFW_KEY_UP))
		{
			fileopencount++;
		}
		else if (env.isPushKey(GLFW_KEY_DOWN))
		{
			if (fileopencount > 0)
				fileopencount--;
		}
		if (env.isPushKey(GLFW_KEY_BACKSPACE))
		{
			is_fileopensetting = false;
			is_modechange = false;
			env.flushInput();
		}
	}

}

// ファイルの種類見る処理
void MapEditor::fileopenDraw() {

	if (is_fileopensetting == true)
	{
		fileView();

		Vec2f numberwindow_pos = Vec2f(-100, 0);
		Vec2f numberwindow_size = Vec2f(fontsize * 9, fontsize * 2);
		Vec2f stageleselectfont_pos = Vec2f(numberwindow_pos.x(), numberwindow_pos.y() + 20);
		Vec2f button_pos = Vec2f(stageleselectfont_pos.x() + numberwindow_size.x() - 20,
								 numberwindow_pos.y());
		Vec2f button_size = Vec2f(200, 100);
		Color color = Color::black;

		if (pointCollision(env.mousePosition(), button_pos, button_size))
		{
			color = Color::red;
			if (env.isPushButton(Mouse::LEFT))
			{
				fileopenExecution();
				env.flushInput();
			}
		}
		if (env.isPushKey(GLFW_KEY_ENTER))
		{
			fileopenExecution();
		}

		drawFillBox(numberwindow_pos.x(), numberwindow_pos.y(),
					numberwindow_size.x(), numberwindow_size.y(),
					Color::white);

		drawTextureBox(button_pos.x(), button_pos.y(), 200, 100,
					   0, 0, 256, 256,
					   button, Color::white);

		openfont.draw("stage  ", stageleselectfont_pos, Color::black);
		openfont.draw("       " + std::to_string(fileopencount), stageleselectfont_pos, Color::black);
		openfont.draw("         OK", stageleselectfont_pos, color);
	}
	// セーブフォント
	if (is_fileopendraw == true)
	{
		openfont.draw("Open stage" + std::to_string(fileopencount) + ".txt", Vec2f(-WIDTH / 2 + 250, 0), Color::yellow);
		okbutton_button_delay--;
		is_modechange = true;
		if (okbutton_button_delay < 0)
		{
			okbutton_button_delay = okbutton_delay_max;
			is_fileopendraw = false;
			is_modechange = false;
		}
	}

}

// ファイル開く処理
void MapEditor::fileopen(std::string filename) {

	// ここはファイルが存在しているかどうかの判定
	if (tryFileopen(filename) == false)
	{
		return;
	}

	std::ifstream ifstr(filename);
	Vec2i mapsize;
	int input;

	// ブロックの要素数を決める
	ifstr >> mapsize.y() >> mapsize.x();

	// 上で決まった要素でブロックの要素数を初期化する
	setup(mapsize.x(), mapsize.y());

	// yだけ逆回し
	for (int y = block.size() - 1; y >= 0; y--)
	{
		for (unsigned x = 0; x < block[y].size(); x++)
		{
			ifstr >> input;
			block[y][x]->setBlockType(input);
			block[y][x]->setTexture(&tex[input], Vec2f(0, 0));
		}
	}

	return;
}

void MapEditor::fileopenExecution() {
	is_fileopendraw = true;
	is_fileopensetting = false;
	is_setting = false;
	is_modechange = false;
	fileopen("res/StageText/stage" + std::to_string(fileopencount) + ".txt");
}

bool MapEditor::tryFileopen(std::string filename) {
	std::vector<std::string>  filelist;
	for (std::tr2::sys::directory_iterator it("res/StageText"), end; it != end; it++)
	{
		filelist.push_back(it->path());
	}
	filename.erase(0, 14);
	for (auto it : filelist)
	{
		it.erase(0, 14);
		std::string temp = it;

		if (filename == temp)
		{
			return true;
		}
	}


	return false;
}

void MapEditor::statusUpdate() {
	if (now_status == true)
	{
		is_statusdraw = true;
		is_modechange = true;
		font.size(fontsize);
	}
	if (is_statusdraw == true)
	{
		if (env.isPushKey(GLFW_KEY_BACKSPACE))
		{
			is_statusdraw = false;
			is_modechange = false;
			env.flushInput();
		}
	}
}

// エディターのステータスみる処理
void MapEditor::statusDraw() {
	if (is_statusdraw == true)
	{
		Vec2f status_pos = Vec2f(settingwindow_pos.x() + settingwindow_size_max.x() + 20,
								 settingwindow_pos.y() - fontsize);
		int interval = 0;
		font.draw("Key('1') x++, Key('2')  x--",
				  Vec2f(status_pos.x(), status_pos.y() - fontsize * interval++), Color::black);
		font.draw("Key('3') y++, Key('4')  y--",
				  Vec2f(status_pos.x(), status_pos.y() - fontsize * interval++), Color::black);
		interval++;
		font.draw("Key('W') Up,   Key('S') Down",
				  Vec2f(status_pos.x(), status_pos.y() - fontsize * interval++), Color::black);
		font.draw("Key('A') Left, Key('D') Right",
				  Vec2f(status_pos.x(), status_pos.y() - fontsize * interval++), Color::black);
		interval++;
		font.draw("Key('UP')   zoom in",
				  Vec2f(status_pos.x(), status_pos.y() - fontsize * interval++), Color::black);
		font.draw("Key('DOWN') zoom out",
				  Vec2f(status_pos.x(), status_pos.y() - fontsize * interval++), Color::black);

		interval += 2;
		font.draw("block x:" + std::to_string((int)blocknum.x()) + "y:" + std::to_string((int)blocknum.y()),
				  Vec2f(status_pos.x(), status_pos.y() - fontsize * interval++), Color::black);
	}
}

// 当り判定調べる四角形描画
void MapEditor::checkHitBox(Vec2f pos, Vec2f size, Color color) {
	drawBox(pos.x(), pos.y(), size.x(), size.y(), 2, color);
}

// 当り判定調べる塗りつぶし四角形描画
void MapEditor::checkHitFillBox(Vec2f pos, Vec2f size, Color color) {
	drawFillBox(pos.x(), pos.y(), size.x(), size.y(), color);
}