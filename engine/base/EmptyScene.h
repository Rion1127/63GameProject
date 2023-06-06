#pragma once

class EmptyScene
{
public:
	//‹ó‚ÌŠÖ”‚ğéŒ¾‚µ‚Ä‚¨‚­
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

