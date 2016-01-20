#pragma once
#include "EnvSingleton.h"
#include "Model_MapEditor.h"




class Block {
private:
	Vec2i chip_num;
	BlockType blocktype;
	Vec2f pos;
	Vec2f size;

	bool is_slide_left;
	bool is_slide_right;

	int interval_size;

	Vec2f offset_pos;
	Vec2f offset_size;

	Texture* tex;

	Color color;
	float angle;
	Vec2f zoom;
	Vec2f centering_pos;

	bool mousehit;
	Vec2f vec;
	Vec2f speed;
	float gravity;
	int direciton;

public:

	void setPosInterval(Vec2f getpos, Vec2f getsize, int x, int y) {
		size = getsize;
		pos = Vec2f(pos.x() + size.x() * x, pos.y() + size.y() * y) + getpos;
		chip_num = Vec2i(x, y);
	}

	void setBlockType(int getblocktype) {
		blocktype = static_cast<BlockType>(getblocktype);
	}

	void setColor(Color getcolor) {
		color = getcolor;
	}

	void setTexture(Texture* gettexture, Vec2f getoffset_pos) {
		offset_pos = getoffset_pos;
		offset_size = Vec2f(gettexture->width(), gettexture->height());
		tex = gettexture;
	}

	void setMouseHit(bool getmousehit) {
		mousehit = getmousehit;
	}

	void setExElement(float getangle, Vec2f getzoom, Vec2f getcentering_pos) {
		angle = getangle;
		zoom = getzoom;
		centering_pos = getcentering_pos;
	}

	void setSize(Vec2f getsize) {
		size += getsize;
	}

	void setSelectPos(Vec2f getpos, Vec2f getsize, int i) {
		int interval_size = 150;
		pos.x() = interval_size * i + getpos.x();
		pos.y() = getpos.y();
		size = getsize;
	}

	void setNowSelectPos(Vec2f getpos, Vec2f getsize) {
		pos = getpos;
		size = getsize;
	}


	void setIsSlideLeft(bool getmove) {
		is_slide_left = getmove;
	}

	void setIsSlideRight(bool getmove) {
		is_slide_right = getmove;

	}

	void setSlideLeft(int speed) {
		if (is_slide_left == true) {
			interval_size -= speed;
			pos.x() += speed;

			if (interval_size < 20) {
				is_slide_left = false;
				interval_size = 200;
			}
		}
	}

	void setSlideRight(int speed) {
		if (is_slide_right == true) {
			interval_size -= speed;
			pos.x() -= speed;

			if (interval_size < 20) {
				is_slide_right = false;
				interval_size = 200;
			}
		}
	}

	Vec2f getPos() {
		return pos;
	}
	Vec2f getSize() {
		return size;
	}
	Vec2i getChipNum() {
		return chip_num;
	}
	BlockType getBlockType() {
		return blocktype;
	}



	Block() {
		chip_num = Vec2i(0, 0);
		blocktype = BlockType::AIR_BLOCK;

		pos = Vec2f(0.0f, 0.0f);
		size = Vec2f(0.0f, 0.0f);
		offset_pos = Vec2f(0.0f, 0.0f);
		offset_size = Vec2f(256.0f, 256.0f);
		color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		angle = 0.0f;
		zoom = Vec2f(1.0f, 1.0f);
		centering_pos = Vec2f(0.0f, 0.0f);
		interval_size = 200;

		is_slide_left = false;
		is_slide_right = false;

	};

	Block(Vec2f _pos, Vec2f _size, Vec2f _offset_pos, BlockType _block_type) :
		pos(_pos),
		size(_size),
		offset_pos(_offset_pos),
		blocktype(_block_type)

	{
		chip_num = Vec2i(0, 0);

		offset_pos = Vec2f(0.0f, 0.0f);
		offset_size = Vec2f(256.0f, 256.0f);
		color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		angle = 0.0f;
		zoom = Vec2f(1.0f, 1.0f);
		centering_pos = Vec2f(0.0f, 0.0f);
		interval_size = 200;

		is_slide_left = false;
		is_slide_right = false;
	}

	void draw() {

		switch (blocktype)
		{
		case BlockType::AIR_BLOCK:
			drawBox(pos.x(), pos.y(), size.x(), size.y(), 2, color, angle, zoom, centering_pos);
			break;
		case BlockType::BLOCK_1:
			drawFillBox(pos.x(), pos.y(), size.x(), size.y(), color, angle, zoom, centering_pos);
			break;
		case BlockType::BLOCK_2:
			drawFillBox(pos.x(), pos.y(), size.x(), size.y(), color, angle, zoom, centering_pos);
			break;
		case BlockType::BLOCK_3:
			drawFillBox(pos.x(), pos.y(), size.x(), size.y(), color, angle, zoom, centering_pos);
			break;
		case BlockType::BLOCK_4:
			drawFillBox(pos.x(), pos.y(), size.x(), size.y(), color, angle, zoom, centering_pos);
			break;
		case BlockType::BLOCK_5:
			drawFillBox(pos.x(), pos.y(), size.x(), size.y(), color, angle, zoom, centering_pos);
			break;
		case BlockType::BLOCK_6:
			drawFillBox(pos.x(), pos.y(), size.x(), size.y(), color, angle, zoom, centering_pos);
			break;
		case BlockType::BLOCK_7:
			drawFillBox(pos.x(), pos.y(), size.x(), size.y(), color, angle, zoom, centering_pos);
			break;
		case BlockType::BLOCK_8:
			drawFillBox(pos.x(), pos.y(), size.x(), size.y(), color, angle, zoom, centering_pos);
			break;
		case BlockType::BLOCK_9:
			drawFillBox(pos.x(), pos.y(), size.x(), size.y(), color, angle, zoom, centering_pos);
			break;
		}

		if (mousehit == true)
		{
			drawFillBox(pos.x(), pos.y(), size.x(), size.y(), Color(0.0f, 1.0f, 1.0f, 0.5f), angle, zoom, centering_pos);
		}
	}

	// 空ブロックを選んでいないときにテクスチャ表示
	void textureDraw() {
		if (blocktype == AIR_BLOCK)
		{
			drawBox(pos.x(), pos.y(), size.x(), size.y(), 2, color, angle, zoom, centering_pos);
		}
		else {
			if (blocktype == PLAYER_START_POS || blocktype == GOAL)
			{
				drawTextureBox(pos.x(), pos.y(),
					size.x(), size.y() * 2,
					offset_pos.x(), offset_pos.y(),
					offset_size.x(), offset_size.y(),
					*tex,
					color,
					angle,
					zoom, centering_pos);
			}
			else
			{
				drawTextureBox(pos.x(), pos.y(),
					size.x(), size.y(),
					offset_pos.x(), offset_pos.y(),
					offset_size.x(), offset_size.y(),
					*tex,
					color,
					angle,
					zoom, centering_pos);
			}
		}
	}

};