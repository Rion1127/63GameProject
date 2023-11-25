# REngine
### DirectX 12を使用したゲームエンジン
* 使用している外部ライブラリ
    * Imgui
    * DirectXTex
    * assimp

# 作品紹介
この作品は攻撃やガードの際に適切にキャラを動かすことでプレイヤーの感じるストレスを無くすことをテーマに制作している個人制作作品です。  
* 地上攻撃
![地上攻撃](ReadMeTexture/groundAttack.gif)
* 空中攻撃
![空中攻撃](ReadMeTexture/airAttack.gif)
この攻撃を制作するためにエディターを制作しています。  
## エディター
エディターでは、攻撃に関することを編集し出力・読み込みができます。
* 攻撃の軌道編集  
攻撃の剣と当たり判定の動きはスプライン曲線で制御しています。  
![攻撃の軌道編集](ReadMeTexture/spline_after.gif)
* 多段ヒット  
一回の攻撃で剣を複数回振れるようにできます。
![多段ヒット](ReadMeTexture/swing.gif)
* 姿勢制御・プレビュー表示  
攻撃中の姿勢はクォータニオンで制御しています。  
指定フレームまでに指定クォータニオンの姿勢に線形補間で変化します。  
キーフレームのように複数の姿勢を指定できます。
![姿勢制御・プレビュー表示](ReadMeTexture/preview.gif)