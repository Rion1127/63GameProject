#pragma once
#include <unordered_map>
#include <string>
#include "BaseAttack.h"

class AttackDataPool {
private:
	//攻撃のデータ・プール
	std::unordered_map<std::string, BaseAttack::AttackData> attacks_;
	//データプールから攻撃を指定する文字列を指定する
	std::unordered_map<std::string, std::string> attackKeys_;

	//ディレクトリ名
	std::string attackInfoDir_;
	std::string attackKeyDir_;
public:
	AttackDataPool();

	void LoadAttackFile(std::string fileName);
	void LoadattackKeys(std::string fileName);
public:
	std::unordered_map<std::string, BaseAttack::AttackData> GetAttacks() { return attacks_; }
	std::unordered_map<std::string, std::string> GetKeyName() { return attackKeys_; }
};