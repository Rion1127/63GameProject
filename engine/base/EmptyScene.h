#pragma once

class EmptyScene
{
public:
	//��̊֐���錾���Ă���
	EmptyScene() {
	};
	virtual void Ini() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	int GetIsNext() { return isNext_; }
	void SetIsNext(int isnext) { isNext_ = isnext; }

	int isNext_ = false;

	virtual ~EmptyScene() {
	};
};

