#pragma once
#include "GameNode.h"

class GameObject : public GameNode
{
public:
	//������
	enum class Occupation : UINT
	{
		Assassin = 0,		//�ܰ�				--						���	A
		Swordsman = 10,		//���				--						���	A
		Myrmidon = 32,		//�ذ�				����:SwordMaster			�߱�	B				
		Mercenary = 42,		//����+��			����:Hero				�߱�	B
		SwordMaster = 52,	//�ְ�				--						���	A
		Archer = 62,		//���� �ü�			����:Sniper				�ϱ�	C
		Sniper = 72,		//ūȰ �ü�			����:BowNight			�߱�	B
		GreatLord = 82,		//����				--						�ֻ�	S
		Knight = 92,		//���尩				����:General(�̱���)		�߱�	B
		WarCleric = 102,	//�ٹ��Ͽ� ����		--						���	A
		Hero = 114,			//ū���� + ��			--						���	A
		Sage = 124,			//�κ�				--						���	A
		Mage = 136,			//���� + �κ�			����:Sage				�߱�	B
		Cleric = 146,		//�Ϲ� ����			����:War Cleric			�߱�	B
		Grandmaster = 158,	//���������			--						���	A
		Tactician = 168,	//������				����:GrandMaster			�߱�	B
		BowKnight = 178,	//��ź �ü�			--						���	A
		DarkKnight = 188,	//�� ���			--						���	A
		GreatKnight = 198,	//���� ���			--						���	A
		Cavalier = 208,		//â ���			����:Paladin				�߱�	B
		Paladin = 218,		//â ��� ����		--						���	A
		Troubadour = 228,	//�⸶����			����:Valkyrie			�߱�	B
		Valkyrie = 240,		//�⸶��������		--						���	A
		Dancer = 252,		//����				����						�ֻ�	S
		DarkMage = 263,		//���渶��			����:Sorcerer			�߱�	B
		Sorcerer = 273,		//���渶������		--						���	A
		Theif = 283,		//����				����:Assassin			�ϱ�	C
	};

	//��ĳ���� �巡��,�̵� ����
	typedef enum class DraggingDirection : INT
	{
		IDLE = 0, LEFT = 2, RIGHT, BOTTOM, TOP, LEFTBOTTOM, RIGHTBOTTOM, LEFTTOP, RIGHTTOP,
	} MovingDirection, AttackingDirection;

protected:
	RECT position;  //��� ���ӻ��� ��ü�� �������� ������
	POINT index;	//���� �������� �ƴ� �ε����� �̵��ϱ⵵ �Ѵ�
	class Cursor* cursor; //Ŀ���� �ʹ� Ŀ��
public:

	void Init() override;
	void Release()override;
	void Update()override;
	void Render()override;

	//�����. �����ڷ� ��ȯ�ϵ� ��������ʰ� ���ȭ.
	const RECT & GetPosition()const { return this->position; }
	const POINT& GetIndex()const { return this->index; }

	//�����. ���ȭ���Ѽ� ���������ʰ� �� �����ڷ� �޾� ������ ��������.
	void SetPosition(const RECT &_position) { this->position = _position; }
	void SetIndex(const POINT& _index) { this->index = _index; }
	void SetPositionViaIndex() { position = RectMake(index.x*48, index.y* 48,48,48); }
	GameObject();
	~GameObject() {};
};

