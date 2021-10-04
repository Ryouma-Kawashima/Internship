# 方針
後々、抽象化レイヤーを学ぶ際に改造しやすいように設計する
- プロジェクトはできるだけコンパクト
- クラスはなるべく柔軟に差し替え可能な設計
- 余計な機能を加えない(アロケーターや音声など)
- フォルダ分け、モジュール分けをきっちりする
- 機能と結果の保証をする

# 構想図(未完成)
* Main
| + Window
| | + OS / Windows
| | + OS / Linux
| | + OS / etc...
|
| + Render
| | + Engine / DirectX11
| | + Engine / DirectX12
| | + Engine / etc..
| 
| + System
| | + Manager
| | + 
|
| + Utility
| | + Timer
| | | + std::chrono (any platform)
| |
| | + Static Polymorphism
| | | + SP_CAST(T) : static_cast(&T)
| | 
| | + Random
| | | + 


* Window
| + Base
| | + virtual bool Create(uint32_t width, uint32_t, height, void** handle)
| | + virutal void Destroy()
| | + virtual void Show(bool isShow)
| | + virtual bool Update() /* 戻り値がtrueだったら終了処理 */

* Render
| + Base
| | + virtual void CreateDevice(void* handle)
| | + virtual void DestroyDevice()
| 
| + DirectX11
| | + void Buffer()
| | + void Draw()
| 

* Timer
| + void Start()
| + void Stop()

※ Rawでキレイに表示されます



| + uint32_t GetElapsedTime()
