#include "UIOperation.h"
#include "Util.h"
#include "Texture.h"

/**
 * @file UIOperation.cpp
 * @brief 操作UIを描画するクラス
 */

UIOperation::UIOperation()
{
	operationUI_.resize(4);
	textUI_.resize(4);
	for (uint32_t i = 0; i < operationUI_.size(); i++) {
		operationUI_[i] = MakeUnique<Sprite>();
		operationUI_[i]->Ini();
		operationUI_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("OperationUI"));
		operationUI_[i]->SetTex_Size({ 128,128 });

		Vector2 pos = { 225.f + i * 200.f,690 };
		operationUI_[i]->SetPos(pos);
		operationUI_[i]->SetScale(Vector2(0.3f, 0.6f));

		textUI_[i] = MakeUnique<Sprite>();
		textUI_[i]->Ini();
		textUI_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("UItex"));
		textUI_[i]->SetTex_Size({ 128,64 });

		pos = { operationUI_[i]->GetPos().x + 100.f,650.f };
		textUI_[i]->SetPos(pos);
		textUI_[i]->SetScale(Vector2(0.4f, 0.4f));
	}

	operationUI_[0]->SetTex_LeftTop({ 128,32 });
	textUI_[0]->SetTex_LeftTop({ 0,0 });

	operationUI_[1]->SetTex_LeftTop({ 256,32 });
	textUI_[1]->SetTex_LeftTop({ 128,0 });

	operationUI_[2]->SetTex_LeftTop({ 384,155 });
	textUI_[2]->SetTex_LeftTop({ 0,64 });

	operationUI_[3]->SetTex_LeftTop({ 384,32 });
	textUI_[3]->SetTex_LeftTop({ 128,64 });
	textUI_[3]->SetTex_Size({ 160,64 });
}

void UIOperation::Update()
{
	for (auto& ui : operationUI_) {
		ui->Update();
	}
	for (auto& ui : textUI_) {
		ui->Update();
	}
}

void UIOperation::Draw()
{
	for (auto& ui : operationUI_) {
		ui->Draw();
		//ui->DrawImGui();
	}
	for (auto& ui : textUI_) {
		ui->Draw();
		//ui->DrawImGui();
	}
}
